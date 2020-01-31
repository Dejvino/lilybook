#include "Typesetter.h"

Typesetter::Typesetter()
{}

void Typesetter::preparePage(Page* page, char* text, size_t len)
{
    page->text = text;
    page->len = len;
}
