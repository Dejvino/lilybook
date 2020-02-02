#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <EPD.h>
#include "ModeRunner.h"
#include "ReaderMode.h"
#include "MainMenuMode.h"

static int baseY = 35;
static int menuSkip = 20;
static int x = 40;
static int menu_options_size = 4;

void draw_menu_option(char* text, int line, bool selected)
{
    EPD_print(text, CENTER, baseY + line * menuSkip); 
}

void draw_menu_cursor(int cursor)
{
    EPD_drawRect(x, baseY + cursor * menuSkip,
        EPD_DISPLAY_WIDTH - 2*x, menuSkip + 1, 0x0F);
}

void MainMenuMode::start()
{
    display_refresh();
}

void MainMenuMode::loop()
{
    display_clear();
    EPD_setFont(COMIC24_FONT, NULL);
    EPD_print("Main Menu", CENTER, 0);

    int line = 0;
    EPD_setFont(DEJAVU18_FONT, NULL);
    draw_menu_option("Continue Reading", line, this->cursor == line); line++;
    draw_menu_option("Internal Memory", line, this->cursor == line); line++;
    draw_menu_option("SD Card", line, this->cursor == line); line++;
    draw_menu_option("Settings", line, this->cursor == line); line++;
    draw_menu_cursor(this->cursor);
    display_update();

    while(1) {
        delay(5);
        if (buttons_pressed_ok()) {
            switch (this->cursor) {
                case 0: // reading
                    display_refresh();
                    getModeRunner()->startInnerMode(new ReaderMode());
                    return;
                case 1: // memory
                    // TODO
                    break;
                case 2: // sd card
                    // TODO
                    break;
                case 3: // settings
                    // TODO
                    break;
            }
            break;
        }
        if (buttons_pressed_minus()) {
            this->cursor = (this->cursor + menu_options_size - 1) % menu_options_size;
            break;
        }
        if (buttons_pressed_plus()) {
            this->cursor = (this->cursor + 1) % menu_options_size;
            break;
        }
    }
}

void MainMenuMode::finish()
{}
