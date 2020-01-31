
#include "spi_master_lobo.h"
#include "EPD.h"
#include <stdlib.h>
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_heap_alloc_caps.h"
#include "esp_log.h"
#include "config.h"
#include "display.h"

void display_init()
{
    // drawing buffer
	disp_buffer = (uint8_t*)pvPortMallocCaps(EPD_DISPLAY_WIDTH * (EPD_DISPLAY_HEIGHT/8), MALLOC_CAP_DMA);
	assert(disp_buffer);
	drawBuff = disp_buffer;

    // gray-scale drawing buffer
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
    int ret;
    
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

void display_splash_screen()
{
    EPD_DisplayClearPart();
    EPD_fillScreen(0);
    EPD_UpdateScreen();

    EPD_setFont(COMIC24_FONT, NULL);
    EPD_print("LilyBook", 30, 30);
    EPD_setFont(DEFAULT_FONT, NULL);
    EPD_print("Version:", 30, 70);
    EPD_print(APP_VERSION, 100, 70);
    EPD_UpdateScreen();
}

void display_clear()
{
    EPD_fillScreen(_bg);
}

void display_refresh()
{
    EPD_DisplayClearPart();
    EPD_fillScreen(_bg);
}

void display_update()
{
    EPD_UpdateScreen();
}

extern void EPD_wake();
void display_wake()
{
    EPD_wake();
}

void display_sleep()
{
    EPD_PowerOff();
}
