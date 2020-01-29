#include <time.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_heap_alloc_caps.h"
#include "spiffs_vfs.h"
#include "esp_log.h"
#include "spi_master_lobo.h"
#include "img_hacking.c"
#include "EPD.h"
#include "storage.h"

#define APP_VERSION "0.1"

#define BTN_PIN_PLUS ((gpio_num_t)39)
#define BTN_PIN_OK ((gpio_num_t)37)
#define BTN_PIN_MINUS ((gpio_num_t)38)

static const char *TAG = "main";

static struct tm* tm_info;
static char tmp_buff[128];
static time_t time_now, time_last = 0;
static const char *file_fonts[3] = {"/spiffs/fonts/DotMatrix_M.fon", "/spiffs/fonts/Ubuntu.fon", "/spiffs/fonts/Grotesk24x48.fon"};
static const char tag[] = "[LilyBook]";
esp_err_t ret;

void display_init()
{
	disp_buffer = (uint8_t*)pvPortMallocCaps(EPD_DISPLAY_WIDTH * (EPD_DISPLAY_HEIGHT/8), MALLOC_CAP_DMA);
	assert(disp_buffer);
	drawBuff = disp_buffer;

	gs_disp_buffer = (uint8_t*)pvPortMallocCaps(EPD_DISPLAY_WIDTH * EPD_DISPLAY_HEIGHT, MALLOC_CAP_DMA);
	assert(gs_disp_buffer);
	gs_drawBuff = gs_disp_buffer;
}


spi_lobo_device_interface_config_t devcfg;
spi_lobo_bus_config_t buscfg;
void spi_init()
{
    gpio_set_direction(DC_Pin, GPIO_MODE_OUTPUT);
	gpio_set_level(DC_Pin, 1);
	gpio_set_direction(RST_Pin, GPIO_MODE_OUTPUT);
	gpio_set_level(RST_Pin, 0);
	gpio_set_direction(BUSY_Pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUSY_Pin, GPIO_PULLUP_ONLY);

#ifdef POWER_Pin
	gpio_set_direction(POWER_Pin, GPIO_MODE_OUTPUT);
	gpio_set_level(POWER_Pin, 1);
#endif

    buscfg.miso_io_num = -1;    			// set SPI MISO pin
    buscfg.mosi_io_num = MOSI_Pin;  		// set SPI MOSI pin
    buscfg.sclk_io_num = SCK_Pin;			// set SPI CLK pin
    buscfg.quadwp_io_num=-1;
    buscfg.quadhd_io_num=-1;
	buscfg.max_transfer_sz = 5*1024;		// max transfer size is 4736 bytes

    devcfg.clock_speed_hz=40000000;		// SPI clock is 40 MHz
    devcfg.mode=0;						// SPI mode 0
    devcfg.spics_io_num=-1;				// we will use external CS pin
	devcfg.spics_ext_io_num = CS_Pin;		// external CS pin
	devcfg.flags=SPI_DEVICE_HALFDUPLEX;	// ALWAYS SET  to HALF DUPLEX MODE for display spi !!
}

void display_connect()
{
    ret=spi_lobo_bus_add_device(SPI_BUS, &buscfg, &devcfg, &disp_spi);
    assert(ret==ESP_OK);
	printf("SPI: display device added to spi bus\r\n");

	ret = spi_lobo_device_select(disp_spi, 1);
    assert(ret==ESP_OK);
	ret = spi_lobo_device_deselect(disp_spi);
    assert(ret==ESP_OK);

    printf("SPI: attached display device, speed=%u\r\n", spi_lobo_get_speed(disp_spi));
	printf("SPI: bus uses native pins: %s\r\n", spi_lobo_uses_native_pins(disp_spi) ? "true" : "false");
}

void fs_init()
{
    vfs_spiffs_register();
    if (spiffs_is_mounted) {
        ESP_LOGI(tag, "File system mounted.");
    } else {
        ESP_LOGE(tag, "Error mounting file system.");
    }

    if (storage_init() == 0) {
        ESP_LOGI(tag, "SD card mounted.");
    } else {
        ESP_LOGE(tag, "Error mounting SD card.");
    }
}

extern "C" void app_main()
{
    gpio_config_t buttons_config;
    buttons_config.intr_type = (gpio_int_type_t)GPIO_PIN_INTR_DISABLE;
    buttons_config.mode = GPIO_MODE_INPUT;
    buttons_config.pull_up_en = (gpio_pullup_t)1;
    buttons_config.pull_down_en = (gpio_pulldown_t)0;
    buttons_config.pin_bit_mask = (1LL << BTN_PIN_MINUS);
    gpio_config(&buttons_config);
    buttons_config.pin_bit_mask = (1LL << BTN_PIN_OK);
    gpio_config(&buttons_config);
    buttons_config.pin_bit_mask = (1LL << BTN_PIN_PLUS);
    gpio_config(&buttons_config);

    display_init();
    spi_init();

	vTaskDelay(500 / portTICK_RATE_MS);

    printf("\n   LilyBook v%s\n\n", APP_VERSION);

    fs_init();

	display_connect();

	EPD_DisplayClearFull();
    
	printf("==== START ====\r\n\n");

    FILE* f = fopen("/sdcard/book.txt", "r");

	_gs = 1;
	uint32_t tstart;
	int pass = 0;
    int font = DEFAULT_FONT;
    EPD_setFont(font, NULL);

    EPD_wait(1000);

    while (1) {
		EPD_fillScreen(_bg);
		_fg = 15;
		_bg = 0;

        char text[128];
        
        if (f == NULL) {
            ESP_LOGE(TAG, "Failed to open file for reading");
            sprintf(text, "Could not open SD card.");
            f = fopen("/sdcard/book.txt", "r");
        } else {
            if (fgets(text, sizeof(text), f) == NULL) {
                ESP_LOGI(TAG, "End of file, closing.");
                fclose(f);
                f = NULL;
            } else {
                ESP_LOGI(TAG, "Read content: %s", text);
            }
        }

        text_wrap = 1;
        EPD_print(text, 10, 10);
        EPD_UpdateScreen();

        while (1) {
            EPD_wait(100);
            if (gpio_get_level(BTN_PIN_OK) == 0) {
                ESP_LOGI(TAG, "Clear page.");
                EPD_DisplayClearPart();
                EPD_fillScreen(_bg);
                EPD_print(text, 10, 10);
                EPD_UpdateScreen();
            }
            if (gpio_get_level(BTN_PIN_PLUS) == 0) {
                ESP_LOGI(TAG, "Turn page PLUS.");
                break;
            }
            if (gpio_get_level(BTN_PIN_MINUS) == 0) {
                ESP_LOGI(TAG, "Turn page MINUS.");
                fseek(f, 2 * sizeof(text), SEEK_CUR);
                break;
            }
        }
    }

}
