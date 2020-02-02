#include "AppMode.h"
#include "AbstractMenuMode.h"

#define DIR_ENTRY_NAME_SIZE 16
typedef struct {
    char name[DIR_ENTRY_NAME_SIZE];
    unsigned long bytes;
    void* next;
} dir_entry_t;

class SdCardMenuMode : public AbstractMenuMode
{
public:
    void start();
    void finish();

protected:
    char* getTitle();
    char** getOptions();
    int getOptionsSize();
    void onOptionSelected(int option);
    int getOptionsX();
    int getOptionsFont();

private:
    char* basedir = "/sdcard/";
    char** options;
    char* optionsNames;
    unsigned long* optionsBytes;
    int optionsSize;
};
