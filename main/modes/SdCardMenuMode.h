#include "AppMode.h"
#include "AbstractMenuMode.h"

class SdCardMenuMode : public AbstractMenuMode
{
public:
    virtual void start();
    virtual void finish();

protected:
    virtual char* getTitle();
    virtual char** getOptions();
    virtual int getOptionsSize();
    virtual void onOptionSelected(int option);

private:
};
