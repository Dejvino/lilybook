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
    Page* pageLast = NULL;
    Page* pageCurrent = NULL;

    long bookmark = 0;
    //bool displaySleeping = false;

    while (1) {
		char text[1024];
        if (textReader != NULL) {
            if (pageCurrent == NULL) {
                size_t read = textReader->read(bookmark, text, sizeof(text));
                pageCurrent = typesetter.preparePage(text, sizeof(text));
            }
            if (pageLast == NULL) {
                size_t read = textReader->read(bookmark - sizeof(text), text, sizeof(text));
                pageLast = typesetter.preparePreviousPage(text, sizeof(text));
            }
        } else {
            typesetter.destroyPage(pageCurrent);
            strcpy(text, "File could not be opened.");
            pageCurrent = typesetter.preparePage(text, sizeof(text));
        }

        display_clear();
        pagePrinter.print(pageCurrent);
        display_update();

        //time_t idleStart = clock();
        while (1) {
            sleep(10);
            if (buttons_pressed_ok()) {
                ESP_LOGI(TAG, "Clear page.");
                display_refresh();
                break;
            }
            if (buttons_pressed_plus()) {
                ESP_LOGI(TAG, "Turn page PLUS.");
                if (pageCurrent != NULL) {
                    bookmark += pageCurrent->len;
                    typesetter.destroyPage(pageLast);
                    pageLast = pageCurrent;
                    pageCurrent = NULL;
                } else {
                    ESP_LOGW(TAG, "No current page.");
                }
                break;
            }
            if (buttons_pressed_minus()) {
                ESP_LOGI(TAG, "Turn page MINUS.");
                if (pageLast != NULL) {
                    bookmark -= pageLast->len;
                    typesetter.destroyPage(pageCurrent);
                    pageCurrent = pageLast;
                    pageLast = NULL;
                } else {
                    ESP_LOGW(TAG, "No last page.");
                }
                break;
            }
            /*if (!displaySleeping && (clock() - idleStart > DISPLAY_SLEEP_TIMEOUT)) {
                displaySleeping = true;
                ESP_LOGI(TAG, "Display going to sleep after %d ms.", DISPLAY_SLEEP_TIMEOUT);
                display_sleep();
            }*/
        }
        /*if (displaySleeping) {
            displaySleeping = false;
            ESP_LOGI(TAG, "Display waking up.");
            display_wake();
        }*/
    }

}
