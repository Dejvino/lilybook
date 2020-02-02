#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <EPD.h>
#include "SettingsMenuMode.h"

static char* options[] = {
        "[Back]",
        "Reader Option X",
        "Reader Option Y",
        "Reader Option Z",
    };

void SettingsMenuMode::start()
{
    display_refresh();
}

void SettingsMenuMode::finish()
{}

char* SettingsMenuMode::getTitle()
{
    return "Settings";
}

char** SettingsMenuMode::getOptions()
{
    return options;
}
    
int SettingsMenuMode::getOptionsSize()
{
    return 4;
}

void SettingsMenuMode::onOptionSelected(int option)
{
    switch (option) {
        case 0:
            // TODO
            return;
        case 1:
            // TODO
            break;
        case 2:
            // TODO
            break;
        case 3:
            this->setFinished();
            break;
    }
}
