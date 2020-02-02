#ifndef _PAGE_H_
#define _PAGE_H_
#include <stdlib.h>

class Page
{
public:
    Page();

    size_t start;
    char* text;
    size_t len;

private:
};

#endif
