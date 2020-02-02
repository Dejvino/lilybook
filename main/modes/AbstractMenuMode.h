#ifndef _ABSTRACTMENUMODE_H_
#define _ABSTRACTMENUMODE_H_
#include "AppMode.h"

class AbstractMenuMode : public AppMode
{
public:
    virtual void start();
    virtual void loop();
    virtual void finish();

protected:
    virtual char* getTitle();
    virtual char** getOptions();
    virtual int getOptionsSize();
    virtual void onOptionSelected(int option);
    virtual int getOptionsX();
    virtual int getOptionsFont();

private:
    int cursor = 0;
};

#endif
