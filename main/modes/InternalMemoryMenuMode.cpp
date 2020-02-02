#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <EPD.h>
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
    EPD_print("Current File:", 5, 30);
    EPD_print("/spiffs/book.txt", 20, 50);
    
    EPD_print("Size:", 5, 80);
    EPD_print("1234 kB", 100, 80);

    EPD_print("Position:", 5, 100);
    EPD_print("45%", 100, 100);
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
