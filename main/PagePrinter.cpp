#include "display.h"
#include "EPD.h"
#include "PagePrinter.h"

#include "esp_log.h"
static const char *TAG = "PagePrinter";

int font = DEJAVU18_FONT;//DEFAULT_FONT;

PagePrinter::PagePrinter()
{}

void PagePrinter::print(Page* page)
{
    if (page->text == NULL) {
        ESP_LOGE(TAG, "Page text is NULL");
        return;
    }
    if (page->len == 0) {
        return;
    }
    EPD_setFont(font, NULL);
    text_wrap = 1;
    EPD_print(page->text, 0, 0);
    //EPD_UpdateScreen();

}