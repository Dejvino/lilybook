#include "AppMode.h"
#include "reader/Typesetter.h"
#include "reader/TextStorage.h"
#include "reader/PagePrinter.h"

class ReaderMode : public AppMode
{
public:
    virtual void start();
    virtual void loop();
    virtual void finish();

private:
    long bookmark = 0;
    Typesetter typesetter;
    PagePrinter pagePrinter;
    TextStorage textStorage;
    TextReader* textReader = NULL;
    Page* pageLast = NULL;
    Page* pageCurrent = NULL;
};
