#include "ModeRunner.h"

#include "esp_log.h"
static const char *TAG = "ModeRunner";

void ModeRunner::init()
{
    this->modeStackDepth = -1;
}

void ModeRunner::loop()
{
    if (this->modeStackDepth < 0) {
        ESP_LOGE(TAG, "Cannot run app mode. None activated (depth %d).", this->modeStackDepth);
        return;
    }
    this->modeStack[this->modeStackDepth]->loop();
}

void ModeRunner::startMainMode(AppMode* mode)
{
    this->finishMode(NULL);
    this->modeStackDepth = -1;
    this->startInnerMode(mode);
}

void ModeRunner::startInnerMode(AppMode* mode)
{
    if (this->modeStackDepth + 1 >= MODE_STACK_SIZE) {
        ESP_LOGE(TAG, "Cannot start inner mode. Stack overflow.");
        return;
    }
    this->modeStackDepth++;
    this->modeStack[this->modeStackDepth] = mode;
    ESP_LOGI(TAG, "Starting new mode (depth %d).", this->modeStackDepth);
    mode->start();
}

void ModeRunner::finishMode(AppMode* mode)
{
    for (int i = this->modeStackDepth; i >= 0; i--) {
        ESP_LOGI(TAG, "Finishing mode (depth %d).", this->modeStackDepth);
        this->modeStack[i]->finish();
        this->modeStackDepth--;
        if (mode == this->modeStack[i]) {
            break;
        }
    }
}

ModeRunner modeRunner;
ModeRunner* getModeRunner()
{
    return &modeRunner;
}
