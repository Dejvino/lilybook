#include <stdlib.h>
#include "Page.h"

class Typesetter
{
public:
    Typesetter();

    Page* preparePage(char* text, size_t len);
    Page* preparePreviousPage(char* text, size_t len);
    void destroyPage(Page* page);
};
