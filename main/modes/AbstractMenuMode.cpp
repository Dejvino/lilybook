#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <epaper/EPD.h>
#include "ModeRunner.h"
#include "ReaderMode.h"
#include "AbstractMenuMode.h"

static int baseY = 35;
static int x = 20;

void draw_menu_option(char* text, int textX, int textHeight, int line, bool selected)
{
    EPD_print(text, textX, baseY + line * textHeight); 
}

void draw_menu_cursor(int cursor, int textHeight)
{
    EPD_drawRect(x, baseY + cursor * textHeight,
        EPD_DISPLAY_WIDTH - 2*x, textHeight + 1, 0x0F);
}

void AbstractMenuMode::start()
{}

void AbstractMenuMode::loop()
{
    display_clear();
    EPD_setFont(COMIC24_FONT, NULL);
    EPD_print(this->getTitle(), CENTER, 0);

    int line = 0;
    EPD_setFont(this->getOptionsFont(), NULL);
    int menu_option_height = EPD_getfontheight() + 1;
    int menu_options_limit = (EPD_DISPLAY_HEIGHT - baseY) / (EPD_getfontheight() + 1);
    char** options = this->getOptions();
    int menu_options_size = this->getOptionsSize();
    int start = (this->cursor / menu_options_limit) * menu_options_limit;
    int stop = ((this->cursor / menu_options_limit) + 1) * menu_options_limit;
    for (int line = start; line < menu_options_size && line < stop; line++) {
        draw_menu_option(options[line], this->getOptionsX(), menu_option_height,
        line % menu_options_limit, this->cursor == line);
    }
    draw_menu_cursor(this->cursor % menu_options_limit, menu_option_height);
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

int AbstractMenuMode::getOptionsX()
{
    return CENTER;
}

int AbstractMenuMode::getOptionsFont()
{
    return DEJAVU18_FONT;
}
