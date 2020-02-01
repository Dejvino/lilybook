#include <stdlib.h>
#include "Page.h"
#include "PageSettingsProvider.h"

class Typesetter
{
public:
    Typesetter();
    ~Typesetter();

    Page* preparePage(char* text, size_t len);
    Page* preparePreviousPage(char* text, size_t len);
    void destroyPage(Page* page);

private:
    Page* preparePageInternal(char* text, size_t len, int direction);

    PageSettingsProvider* pageSettingsProvider; // TODO: expose so it may be changed
};

