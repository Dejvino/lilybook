#ifndef _APPMODE_H_
#define _APPMODE_H_

class AppMode
{
public:
    virtual void start();
    virtual void loop();
    virtual void finish();
};

#endif
