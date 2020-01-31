#include <string.h>
#include "Typesetter.h"

Typesetter::Typesetter()
{}

Page* Typesetter::preparePage(char* text, size_t len)
{
    Page* page = new Page;
    page->text = new char[len+1];
    memcpy(page->text, text, len);
    page->text[len] = 0;
    page->len = len;
    return page;
}

Page* Typesetter::preparePreviousPage(char* text, size_t len)
{
    Page* page = new Page;
    page->text = new char[len+1];
    memcpy(page->text, text, len);
    page->text[len] = 0;
    page->len = len;
    return page;
}

void Typesetter::destroyPage(Page* page)
{
    if (page == NULL) {
        return;
    }
    delete page->text;
    delete page;
}
