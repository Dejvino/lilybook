#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "esp_system.h"
#include "spiffs/spiffs_vfs.h"

#include "esp_log.h"
static const char *TAG = "reader_storage";

static const char* filename_stored = "stored.dat";
static const char* filename_position = "position.dat";

static int check_storage_ready()
{
    if (spiffs_is_mounted == 0) {
        ESP_LOGE(TAG, "Reader storage is not mounted.");
    }
    return spiffs_is_mounted;
}

void reader_storage_get_filename(char* buf, int len)
{
    if (check_storage_ready() == 0) {
        return;
    }

    strcpy(buf, SPIFFS_BASE_PATH);
    strcat(buf, "/");
    strcat(buf, filename_stored);
}

long reader_storage_get_length()
{
    if (check_storage_ready() == 0) {
        return -1;
    }

    char path[64];
    reader_storage_get_filename(path, sizeof(path));

    long length = -1;
    struct stat stats;
    if (stat(path, &stats) == 0) {
        length = stats.st_size;
    }

    ESP_LOGI(TAG, "Getting length: %ld", length);
    return length;
}

void reader_storage_store_file(char* source_path)
{
    if (check_storage_ready() == 0) {
        return;
    }

    char dest_path[64];
    reader_storage_get_filename(dest_path, sizeof(dest_path));

    ESP_LOGI(TAG, "Storing file %s into %s", source_path, dest_path);
    
    FILE* src = fopen(source_path, "r");
    if (src == NULL) {
        ESP_LOGE(TAG, "Failed opening source file.");
        return;
    }
    FILE* dst = fopen(dest_path, "w");
    if (dst == NULL) {
        ESP_LOGE(TAG, "Failed opening destination file.");
        return;
    }
    
    char buf[32];
    int len = 0;
    do {
        len = fread(buf, 1, sizeof(buf), src);
        if (len > 0) {
            len = fwrite(buf, 1, sizeof(buf), dst);
        }
    } while (len > 0);
    fclose(dst);
    fclose(src);
    ESP_LOGI(TAG, "File stored.");
}

long reader_storage_get_position()
{
    if (check_storage_ready() == 0) {
        return -1;
    }

    long position = 0;
    char* path[64];
    strcpy(path, SPIFFS_BASE_PATH);
    strcat(path, "/");
    strcat(path, filename_position);
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed opening position file for reading.");
        return;
    }
    if (fread(&position, sizeof(position), 1, f) != 1) {
        ESP_LOGE(TAG, "Failed reading position.");
        position = -1;
    }
    fclose(f);
    ESP_LOGI(TAG, "Getting position: %ld", position);
    return position;
}

void reader_storage_set_position(long position)
{
    if (check_storage_ready() == 0) {
        return;
    }

    ESP_LOGI(TAG, "Setting position: %ld", position);

    char* path[64];
    strcpy(path, SPIFFS_BASE_PATH);
    strcat(path, "/");
    strcat(path, filename_position);
    FILE* f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed opening position file for writing.");
        return;
    }
    if (fwrite(&position, sizeof(position), 1, f) != 1) {
        ESP_LOGE(TAG, "Failed writing position.");
    }
    fclose(f);
}
