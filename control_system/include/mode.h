#ifndef CONTROL_SYSTEM_MODE
#define CONTROL_SYSTEM_MODE

#include <kos/mutex.h>

#define LogPrefix "[ControlSystem]"

struct Mode;
struct Mode* create_mode();
void set_mode(struct Mode* mode, const unsigned char newval);
unsigned char get_mode(struct Mode* mode);


#endif//CONTROL_SYSTEM_MODE