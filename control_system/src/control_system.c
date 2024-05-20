#include <stdio.h>
#include <stdlib.h>
#include <kos/thread.h>
#include <assert.h>
#include <mode.h>

int connector_thread(void *context);
int gpio_thread(void *context);

/* Control system entity entry point. */
int main(int argc, const char *argv[])
{
    struct Mode *mode = create_mode();
    Tid connector_tid;
    Retcode rc = KosThreadCreate(&connector_tid, ThreadPriorityNormal, ThreadStackSizeDefault, connector_thread, mode, 1);
    KosThreadResume(connector_tid);
    rtl_assert(rc == rcOk);

    Tid gpio_tid;
    rc = KosThreadCreate(&gpio_tid, ThreadPriorityNormal, ThreadStackSizeDefault, gpio_thread, mode, 0);
    rtl_assert(rc == rcOk);
    KosThreadWait(gpio_tid, INFINITE_TIMEOUT);

    return EXIT_SUCCESS;
}
