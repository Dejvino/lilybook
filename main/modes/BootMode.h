#include "AppMode.h"

class BootMode : public AppMode
{
public:
    virtual void start();
    virtual void loop();
    virtual void finish();
};
