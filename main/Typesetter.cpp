#include <string.h>
#include "Typesetter.h"

#include "esp_log.h"
static const char *TAG = "Typesetter";

Typesetter::Typesetter()
{
    this->pageSettingsProvider = new PageSettingsProvider(); // TODO: make it a param
}

Typesetter::~Typesetter()
{
    delete this->pageSettingsProvider;
}

static Page* extractPage(char* text, size_t len)
{
    Page* page = new Page;
    page->text = new char[len+1];
    memcpy(page->text, text, len);
    page->text[len] = 0;
    page->len = len;
    ESP_LOGI(TAG, "Extracted page (%d bytes):\n%s\n----", page->len, page->text);
    return page;
}

Page* Typesetter::preparePage(char* text, size_t len)
{
    return this->preparePageInternal(text, len, 0);
}

Page* Typesetter::preparePreviousPage(char* text, size_t len)
{
    return this->preparePageInternal(text, len, 1);
}

void Typesetter::destroyPage(Page* page)
{
    if (page == NULL) {
        return;
    }
    delete page->text;
    delete page;
}

Page* Typesetter::preparePageInternal(char* text, size_t len, int reverse)
{
    int page_width = this->pageSettingsProvider->getWidth();
    int page_height = this->pageSettingsProvider->getHeight();
    int line_height = this->pageSettingsProvider->getLineHeight()
        + this->pageSettingsProvider->getLineSpace();
    int line_count = page_height / line_height;

    char buf[len+1];
    memset(buf, 0, len+1);

    int line_start = 0;
    ESP_LOGI(TAG, " === Typesetting%s page === ", reverse ? " reversed" : "");
    for (int line = 0; line < line_count && line_start < len; line++) {
        int i;
        for (i = line_start; i < len; i++) {
            buf[i] = text[reverse ? (len - i) : (i)];
            if (buf[i] == '\n') {
                break;
            }
            int line_width = this->pageSettingsProvider->getStringWidth(&buf[line_start]);
            if (line_width > page_width) {
                // backtrack
                buf[i] = 0;
                i--;
                break;
            }
        }
        ESP_LOGI(TAG, "Line %d | %s", line, &buf[line_start]);
        //ESP_LOGI(TAG, "Line %d [%04d:%04d] | %s | %03d / %03d", line, line_start, i, &buf[line_start],
        //    this->pageSettingsProvider->getStringWidth(&buf[line_start]), page_width);
        line_start = i + 1;
    }

    return extractPage(text, line_start);
}