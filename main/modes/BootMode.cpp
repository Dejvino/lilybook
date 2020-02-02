#include "core/common.h"
#include "core/display.h"
#include "ModeRunner.h"
#include "MainMenuMode.h"
#include "BootMode.h"

void BootMode::start()
{}

void BootMode::loop()
{
    display_splash_screen();
    delay(200);

    getModeRunner()->startMainMode(new MainMenuMode());
}

void BootMode::finish()
{}
