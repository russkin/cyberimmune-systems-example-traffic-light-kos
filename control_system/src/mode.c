#include <mode.h>

struct Mode {
    KosMutex lock;
    unsigned char value;
};

struct Mode* create_mode()
{
    static struct Mode mode;
    KosMutexInit(&mode.lock);
    return &mode;
}

void set_mode(struct Mode* mode, const unsigned char newval)
{
    KosMutexLock(&mode->lock);
    mode->value = newval;
    KosMutexUnlock(&mode->lock);
}

unsigned char get_mode(struct Mode* mode)
{
    unsigned char val;
    KosMutexLock(&mode->lock);
    val = mode->value;
    KosMutexUnlock(&mode->lock);
    return val;
}