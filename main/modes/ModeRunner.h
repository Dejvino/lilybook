#include "AppMode.h"
#define MODE_STACK_SIZE 5

class ModeRunner
{
public:
    void init();
    void loop();

    void startMainMode(AppMode* mode);
    void startInnerMode(AppMode* mode);

    void finishMode(AppMode* mode);

private:
    AppMode* modeStack[MODE_STACK_SIZE];
    int modeStackDepth = -1;
};

ModeRunner* getModeRunner();
