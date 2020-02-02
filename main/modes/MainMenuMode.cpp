#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <epaper/EPD.h>
#include "ModeRunner.h"
#include "ReaderMode.h"
#include "InternalMemoryMenuMode.h"
#include "SdCardMenuMode.h"
#include "SettingsMenuMode.h"
#include "MainMenuMode.h"

static char* options[] = {
        "Continue Reading",
        "Internal Memory",
        "SD Card",
        "Settings"
    };

void MainMenuMode::start()
{
    display_refresh();
}

void MainMenuMode::finish()
{}

char* MainMenuMode::getTitle()
{
    return "Main Menu";
}

char** MainMenuMode::getOptions()
{
    return options;
}
    
int MainMenuMode::getOptionsSize()
{
    return 4;
}

void MainMenuMode::onOptionSelected(int option)
{
    switch (option) {
        case 0: // reading
            display_refresh();
            getModeRunner()->startInnerMode(new ReaderMode());
            return;
        case 1: // memory
            getModeRunner()->startInnerMode(new InternalMemoryMenuMode());
            break;
        case 2: // sd card
            getModeRunner()->startInnerMode(new SdCardMenuMode());
            break;
        case 3: // settings
            getModeRunner()->startInnerMode(new SettingsMenuMode());
            break;
    }
}
