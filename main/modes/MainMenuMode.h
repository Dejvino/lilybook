#include "AppMode.h"

class MainMenuMode : public AppMode
{
public:
    virtual void start();
    virtual void loop();
    virtual void finish();

private:
    int cursor = 0;
};
