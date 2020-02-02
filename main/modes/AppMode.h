#ifndef _APPMODE_H_
#define _APPMODE_H_

class AppMode
{
public:
    virtual void start();
    virtual void loop();
    virtual void finish();
    bool isFinished() {
        return this->finished;
    }

protected:
    void setFinished() {
        this->finished = true;
    }

private:
    bool finished = false;
};

#endif
