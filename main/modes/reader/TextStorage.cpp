#include "TextStorage.h"

#include "esp_log.h"
static const char *TAG = "TextStorage";

TextStorage::TextStorage()
{}

TextReader* TextStorage::open(char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "File could not be opened");
        return NULL;
    }
    ESP_LOGI(TAG, "File opened for reading.");
    return new TextReader(f);
}

void TextStorage::close(TextReader* reader)
{
    reader->close();
    delete reader;
    ESP_LOGI(TAG, "File closed.");
}
