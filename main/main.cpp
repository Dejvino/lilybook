#include <time.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_heap_alloc_caps.h"

#include "esp_log.h"
static const char *TAG = "main";

extern "C" {
#include "config.h"
#include "buttons.h"
#include "storage.h"
#include "display.h"
}

#include "Typesetter.h"
#include "TextStorage.h"
#include "PagePrinter.h"

static struct tm* tm_info;
static char tmp_buff[128];
static time_t time_now, time_last = 0;
static const char *file_fonts[3] = {"/spiffs/fonts/DotMatrix_M.fon", "/spiffs/fonts/Ubuntu.fon", "/spiffs/fonts/Grotesk24x48.fon"};
static const char tag[] = "[LilyBook]";
esp_err_t ret;

void sleep(unsigned int ms)
{
    vTaskDelay(ms / portTICK_RATE_MS);
}

extern "C" void app_main()
{
    printf("\n   LilyBook v%s\n\n", APP_VERSION);

    storage_init();
    buttons_init();
    display_init();
    spi_init();

	sleep(500);

	display_connect();
    display_splash_screen();

    sleep(500);

	printf("==== START ====\r\n\n");

    Typesetter typesetter;
    PagePrinter pagePrinter;
    TextStorage textStorage;
    TextReader* textReader = textStorage.open("/sdcard/book.txt");
    Page page;

    long bookmark = 0;

    while (1) {
		char text[1024];
        if (textReader != NULL) {
            size_t read = textReader->read(bookmark, text, sizeof(text));
        } else {
            strcpy(text, "File could not be opened.");
        }
        typesetter.preparePage(&page, text, sizeof(text));
        display_clear();
        pagePrinter.print(&page);
        display_update();

        while (1) {
            sleep(10);
            if (buttons_pressed_ok()) {
                ESP_LOGI(TAG, "Clear page.");
                display_refresh();
                break;
            }
            if (buttons_pressed_plus()) {
                ESP_LOGI(TAG, "Turn page PLUS.");
                bookmark += sizeof(text);
                break;
            }
            if (buttons_pressed_minus()) {
                ESP_LOGI(TAG, "Turn page MINUS.");
                bookmark -= sizeof(text);
                break;
            }
        }
    }

}
