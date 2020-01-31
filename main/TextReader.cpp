#include <string.h>
#include "TextReader.h"

#include "esp_log.h"
static const char *TAG = "TextReader";

TextReader::TextReader(FILE* file)
{
    this->f = file;
}

void TextReader::close()
{
    fclose(this->f);
}

size_t TextReader::read(long pos, char* text, size_t len)
{
    memset(text, 0, len);
    if (pos < 0) {
        len += pos;
        pos = 0;
    }
    if (len <= 0) {
        return 0;
    }
    if (this->f == NULL) {
        ESP_LOGE(TAG, "File not opened.");
        sprintf(text, "File could not be opened.");
        return 0;
    } else {
        fseek(this->f, pos, SEEK_SET);
        size_t read = fread(text, 1, len, this->f);
        if (read > 0) {
            ESP_LOGI(TAG, "Read content: %s", text);
        } else {
            ESP_LOGI(TAG, "End of file. Closing.");
            fclose(this->f);
            this->f = NULL;
        }
        return read;
    }
}
