#include <string.h>
#include "core/common.h"
#include "core/buttons.h"
#include "core/display.h"
#include <epaper/EPD.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "reader/reader_storage.h"
#include "SdCardMenuMode.h"

#include "esp_log.h"
static char* TAG = "SdCardMenuMode";

#define SDCARD_BASEDIR "/sdcard/"

// TODO: make use of bytes

void SdCardMenuMode::start()
{
    display_refresh();

    dir_entry_t* dir_chain_start = new dir_entry_t;
    strcpy(dir_chain_start->name, "[Back]");
    dir_entry_t* last_entry = dir_chain_start;
    int entries = 1;

    struct stat stats;
    struct dirent *dir;
    DIR* d = opendir(this->basedir);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            ESP_LOGI(TAG, "* %s", dir->d_name);
            dir_entry_t* new_entry = new dir_entry_t;
            strcpy(new_entry->name, dir->d_name);
            char path[64];
            strcpy(path, this->basedir);
            strcat(path, dir->d_name);
            if (stat(path, &stats) == 0) {
                last_entry->bytes = stats.st_size;
            }
            new_entry->next = NULL;
            if (dir_chain_start == NULL) {
                dir_chain_start = new_entry;
            }
            if (last_entry == NULL) {
                last_entry = new_entry;
            } else {
                last_entry->next = new_entry;
                last_entry = new_entry;
            }
            entries++;
        }
        closedir(d);
    } else {
        ESP_LOGE(TAG, "Could not open dir.");
    }

    this->optionsNames = new char[entries*DIR_ENTRY_NAME_SIZE];
    this->options = new char*[entries];
    this->optionsBytes = new unsigned long[entries];
    this->optionsSize = entries;
    last_entry = dir_chain_start;
    int i = 0;
    while (last_entry != NULL) {
        this->options[i] = &this->optionsNames[i * DIR_ENTRY_NAME_SIZE];
        memcpy(this->options[i], last_entry->name, DIR_ENTRY_NAME_SIZE);
        this->optionsBytes[i] = last_entry->bytes;

        dir_entry_t* next = (dir_entry_t*)last_entry->next;
        delete last_entry;
        last_entry = next;
        i++;
    }

}

void SdCardMenuMode::finish()
{
    delete this->options;
    delete this->optionsNames;
    delete this->optionsBytes;
}

char* SdCardMenuMode::getTitle()
{
    return "SD Card";
}

char** SdCardMenuMode::getOptions()
{
    return this->options;
}
    
int SdCardMenuMode::getOptionsSize()
{
    return this->optionsSize;
}

void SdCardMenuMode::onOptionSelected(int option)
{
    if (option == 0) {
        this->setFinished();
        return;
    }

    // TODO: spawn a copy AppMode
    display_alert("Copying into internal memory...");
    char source[64];
    strcpy(source, SDCARD_BASEDIR);
    strcat(source, this->options[option]);
    reader_storage_store_file(source);
    reader_storage_set_position(0);
    this->setFinished();
}

int SdCardMenuMode::getOptionsX()
{
    return 35;
}

int SdCardMenuMode::getOptionsFont()
{
    return SMALL_FONT;
}
