#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <EPD.h>
#include "SdCardMenuMode.h"

static char* options[] = {
        "[Back]",
        "/file.txt",
        "/book.txt",
        "/asdf.gif",
    };

void SdCardMenuMode::start()
{
    display_refresh();
}

void SdCardMenuMode::finish()
{}

char* SdCardMenuMode::getTitle()
{
    return "SD Card";
}

char** SdCardMenuMode::getOptions()
{
    return options;
}
    
int SdCardMenuMode::getOptionsSize()
{
    return 4;
}

void SdCardMenuMode::onOptionSelected(int option)
{
    if (option == 0) {
        this->setFinished();
        return;
    }
    // TODO files
}
