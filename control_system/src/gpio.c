#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <mode.h>

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* Description of the lights gpio interface used by the `ControlSystem` entity. */
#include <traffic_light/IMode.idl.h>

#include <assert.h>

#define MODE_A 0
#define MODE_B 2
#define MODE_A_STEPS 10
#define MODE_B_STEPS 2
const char modes_size[] = {MODE_A_STEPS, MODE_B_STEPS};


struct control_state
{
    unsigned int delay;
    nk_uint32_t value;
};


int gpio_thread(void *context)
{
    NkKosTransport transport;
    struct traffic_light_IMode_proxy proxy;
    static const struct control_state a_steps[MODE_A_STEPS] = {
        {20, traffic_light_IMode_Direction1Green + traffic_light_IMode_Direction2Red},
        {3, traffic_light_IMode_Direction1Green + traffic_light_IMode_Direction1Blink + traffic_light_IMode_Direction2Red},
        {3, traffic_light_IMode_Direction1Yellow + traffic_light_IMode_Direction2Red},
        {10, traffic_light_IMode_Direction1Red + traffic_light_IMode_Direction2Red},
        {3, traffic_light_IMode_Direction1Red + traffic_light_IMode_Direction2Red + traffic_light_IMode_Direction2Yellow},

        {20, traffic_light_IMode_Direction2Green + traffic_light_IMode_Direction1Red},
        {3, traffic_light_IMode_Direction2Green + traffic_light_IMode_Direction2Blink + traffic_light_IMode_Direction1Red},
        {3, traffic_light_IMode_Direction2Yellow + traffic_light_IMode_Direction1Red},
        {10, traffic_light_IMode_Direction2Red + traffic_light_IMode_Direction1Red},
        {3, traffic_light_IMode_Direction2Red + traffic_light_IMode_Direction1Red + traffic_light_IMode_Direction1Yellow},

        // invalid states
        // {1, traffic_light_IMode_Direction1Green + traffic_light_IMode_Direction2Green},
        // {1, 0xFFFF},
    };

    static const struct control_state b_steps[MODE_B_STEPS] = {
        {1, traffic_light_IMode_Direction1Yellow + traffic_light_IMode_Direction2Yellow},
        {1, traffic_light_IMode_Direction1Yellow + traffic_light_IMode_Direction2Yellow + traffic_light_IMode_Direction1Blink + traffic_light_IMode_Direction2Blink},
    };

    /**
     * Get the LightsGPIO IPC handle of the connection named
     * "lights_gpio_connection".
     */
    Handle handle = ServiceLocatorConnect("lights_gpio_connection");
    assert(handle != INVALID_HANDLE);

    /* Initialize IPC transport for interaction with the lights gpio entity. */
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    /**
     * Get Runtime Interface ID (RIID) for interface traffic_light.Mode.mode.
     * Here mode is the name of the traffic_light.Mode component instance,
     * traffic_light.Mode.mode is the name of the Mode interface implementation.
     */
    nk_iid_t riid = ServiceLocatorGetRiid(handle, "lightsGpio.mode");
    assert(riid != INVALID_RIID);

    /**
     * Initialize proxy object by specifying transport (&transport)
     * and lights gpio interface identifier (riid). Each method of the
     * proxy object will be implemented by sending a request to the lights gpio.
     */
    traffic_light_IMode_proxy_init(&proxy, &transport.base, riid);

    /* Request and response structures */
    traffic_light_IMode_FMode_req req;
    traffic_light_IMode_FMode_res res;

    while(1)
    {
        /* Test loop. */
        req.value = 0;
        
        unsigned char mode = get_mode((struct Mode*)context);
        struct control_state *state = mode == MODE_A ? a_steps : b_steps;
        fprintf(stderr, "%s GPIO loop (mode: %d)\n", LogPrefix, (int)mode);
        for(unsigned int i = 0; i < modes_size[mode]; ++i)
        {
            req.value = state->value;
            /**
             * Call Mode interface method.
             * Lights GPIO will be sent a request for calling Mode interface method
             * mode_comp.mode_impl with the value argument. Calling thread is locked
             * until a response is received from the lights gpio.
             */
            if (traffic_light_IMode_FMode(&proxy.base, &req, NULL, &res, NULL) == rcOk)

            {
                /**
                 * Include received result value into value argument
                 * to resend to lights gpio in next iteration.
                 */
                req.value = res.result;

            }
            else
            {
                fprintf(stderr, "Failed to call traffic_light.Mode.Mode()\n");
            }
            ++state;
            // sleep(tl_modes[i].delay); // remove this string for demo
        }
    }

    return EXIT_SUCCESS;
}
