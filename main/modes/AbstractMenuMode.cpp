#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <EPD.h>
#include "ModeRunner.h"
#include "ReaderMode.h"
#include "AbstractMenuMode.h"

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

void AbstractMenuMode::start()
{}

void AbstractMenuMode::loop()
{
    display_clear();
    EPD_setFont(COMIC24_FONT, NULL);
    EPD_print(this->getTitle(), CENTER, 0);

    int line = 0;
    EPD_setFont(DEJAVU18_FONT, NULL);
    char** options = this->getOptions();
    int menu_options_size = this->getOptionsSize();
    for (int line = 0; line < menu_options_size; line++) {
        draw_menu_option(options[line], line, this->cursor == line);
    }
    draw_menu_cursor(this->cursor);
    display_update();

    while(1) {
        delay(5);
        if (buttons_pressed_ok()) {
            this->onOptionSelected(this->cursor);
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

void AbstractMenuMode::finish()
{}
