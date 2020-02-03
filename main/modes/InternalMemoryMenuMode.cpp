#include <string.h>
#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <epaper/EPD.h>
#include "reader/reader_storage.h"
#include "InternalMemoryMenuMode.h"

void InternalMemoryMenuMode::start()
{
    display_refresh();
}

void InternalMemoryMenuMode::loop()
{
    char text[1024];
    
    //display_clear();
    EPD_setFont(COMIC24_FONT, NULL);
    EPD_print("Internal Memory", CENTER, 00);

    EPD_setFont(DEJAVU18_FONT, NULL);
    // TODO: free memory?

    EPD_print("Size:", 5, 40);
    long length = reader_storage_get_length();
    if (length < 0) {
        strcpy(text, "unknown");
    } else {
        sprintf(text, "%ld kB", length / 1024);
    }
    EPD_print(text, 100, 40);

    EPD_print("Position:", 5, 70);
    long position = reader_storage_get_position();
    if (position < 0) {
        strcpy(text, "unknown");
    } else {
        sprintf(text, "%d%%", (int)(position * 100 / length));
    }
    EPD_print(text, 100, 70);
    display_update();

    while (1) {
        delay(10);
        if (buttons_pressed_ok()) {
            this->setFinished();
            break;
        }
    }
}
void InternalMemoryMenuMode::finish()
{}
