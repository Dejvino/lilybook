#include "PageSettingsProvider.h"
#include "EPD.h"
#include "EPDspi.h" // TODO: remove after display config is extracted

int PageSettingsProvider::getWidth()
{
    return EPD_DISPLAY_WIDTH;
}

int PageSettingsProvider::getHeight()
{
    return EPD_DISPLAY_HEIGHT;
}

int PageSettingsProvider::getCharWidth(char c)
{
    char txt[2] = { c, 0x00 };
    return this->getStringWidth(txt);
}

extern int pageFont;
void activatePageFont()
{
    EPD_setFont(pageFont, NULL); // TODO: hack to get the same font as the printer
}

int PageSettingsProvider::getStringWidth(char* string)
{
    activatePageFont();
    int ret = EPD_getStringWidth(string);
    return ret;
}

int PageSettingsProvider::getCharSpace()
{
    return font_x_space;
}

int PageSettingsProvider::getLineHeight()
{
    activatePageFont();
    return EPD_getfontheight();
}

int PageSettingsProvider::getLineSpace()
{
    return font_line_space;
}
