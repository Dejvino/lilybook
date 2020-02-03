#include <time.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
#include "driver/gpio.h"

#include "esp_log.h"
static const char *TAG = "main";

#include "core/common.h"
#include "config.h"
#include "core/buttons.h"
#include "core/storage.h"
#include "core/display.h"

#include "modes/ModeRunner.h"
#include "modes/BootMode.h"
#include "modes/MainMenuMode.h"


static struct tm* tm_info;
static char tmp_buff[128];
static time_t time_now, time_last = 0;
static const char *file_fonts[3] = {"/spiffs/fonts/DotMatrix_M.fon", "/spiffs/fonts/Ubuntu.fon", "/spiffs/fonts/Grotesk24x48.fon"};
static const char tag[] = "[LilyBook]";
esp_err_t ret;

extern "C" void app_main()
{
    printf("\n   LilyBook v%s\n\n", APP_VERSION);

    storage_init();
    buttons_init();
    display_init();
    spi_init();

	delay(500);

	display_connect();

	printf("==== START ====\r\n\n");

    getModeRunner()->init();
    getModeRunner()->startMainMode(new BootMode());

    while (1) {
        getModeRunner()->loop();
    }
}
