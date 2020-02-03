#include "core/common.h"
#include "string.h"
#include "core/buttons.h"
#include "core/display.h"
#include "reader/reader_storage.h"
#include "ReaderMode.h"

#include "esp_log.h"
static const char *TAG = "ReaderMode";

//bool displaySleeping = false;

void ReaderMode::start()
{
    char filename[64];
    reader_storage_get_filename(filename, sizeof(filename));
    this->textReader = textStorage.open(filename);
    
    this->bookmark_max = reader_storage_get_length();
    this->bookmark = reader_storage_get_position();
    if (this->bookmark_max < 0) {
        this->bookmark_max = 0;
    }
    if (this->bookmark < 0 || this->bookmark > this->bookmark_max) {
        this->bookmark = 0;
    }
    ESP_LOGI(TAG, "Opening %s for reading at %ld / %ld.",
        filename, bookmark, bookmark_max);
}

void ReaderMode::loop()
{
    char text[1024];
    if (textReader != NULL) {
        if (pageCurrent == NULL) {
            size_t read = textReader->read(bookmark, text, sizeof(text));
            pageCurrent = typesetter.preparePage(text, read);
            pageCurrent->start = bookmark;
        }
        if (pageLast == NULL) {
            // align with the start?
            if (bookmark < sizeof(text)) {
                size_t read = textReader->read(0, text, sizeof(text));
                pageLast = typesetter.preparePage(text, read);
                pageLast->start = 0;
            } else {
                size_t read = textReader->read((bookmark - sizeof(text)), text, sizeof(text));
                pageLast = typesetter.preparePreviousPage(text, read);
                pageLast->start = bookmark - pageLast->len;
            }
        }
    } else {
        typesetter.destroyPage(pageCurrent);
        strcpy(text, "File could not be opened.");
        pageCurrent = typesetter.preparePage(text, sizeof(text));
    }

    display_clear();
    if (bookmark == bookmark_max) {
        display_alert("THE END");
    } else {
        pagePrinter.print(pageCurrent);
    }
    display_update();

    //time_t idleStart = clock();
    while (1) {
        delay(10);
        if (buttons_pressed_ok()) {
            ESP_LOGI(TAG, "Exiting reader.");
            this->setFinished();
            break;
        }
        if (buttons_pressed_plus()) {
            ESP_LOGI(TAG, "Turn page PLUS.");
            if (pageCurrent != NULL) {
                bookmark = pageCurrent->start + pageCurrent->len;
                if (bookmark > bookmark_max) {
                    bookmark = bookmark_max;
                }
                reader_storage_set_position(bookmark);
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
                bookmark = pageLast->start;
                reader_storage_set_position(bookmark);
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

void ReaderMode::finish()
{
    textStorage.close(this->textReader);
}
