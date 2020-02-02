#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <EPD.h>
#include "MainMenuMode.h"

void MainMenuMode::start()
{}

void MainMenuMode::loop()
{
    display_clear();
    EPD_print("Main Menu", CENTER, 0);
    display_update();

    while(1) {
        delay(5);
        if (buttons_pressed_ok()) {
            break;
        }
    }
}

void MainMenuMode::finish()
{}
