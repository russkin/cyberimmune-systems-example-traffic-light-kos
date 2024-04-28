
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* EDL description of the server entity. */
#include <traffic_light/Diagnostics.edl.h>

#include <assert.h>

/* Type of interface implementing object. */
typedef struct IReportImpl {
    struct traffic_light_IReport base;     /* Base interface of object */
    rtl_uint32_t step;         /* Extra parameters */
} IReportImpl;

/* Report method implementation. */
static nk_err_t Report_impl(struct traffic_light_IReport *self,
                          const struct traffic_light_IReport_report_req *req,
                          const struct nk_arena *req_arena,
                          struct traffic_light_IReport_report_res *res,
                          struct nk_arena *res_arena)
{
    IReportImpl *impl = (IReportImpl *)self;
    /**
     * Increment value in client request by
     * one step and include into result argument that will be
     * sent to the client in the server response.
     */
    res->result = req->value + impl->step;
    return NK_EOK;
}

/**
 * Diagnostics object constructor.
 * step is the number by which the input value is increased.
 */
static struct traffic_light_IReport *CreateIReportImpl(rtl_uint32_t step)
{
    /* Table of implementations of IReport interface methods. */
    static const struct traffic_light_IReport_ops ops = {
        .report = Report_impl
    };

    /* Interface implementing object. */
    static struct IReportImpl impl = {
        .base = {&ops}
    };

    impl.step = step;

    return &impl.base;
}

/* Server entry point. */
int main(void)
{
    NkKosTransport transport;
    ServiceId iid;

    /* Get server IPC handle of "server_connection". */
    Handle handle = ServiceLocatorRegister("diagnostics_connection", NULL, 0, &iid);
    assert(handle != INVALID_HANDLE);

    /* Initialize transport to client. */
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    /**
     * Prepare the structures of the request to the server entity: constant
     * part and arena. Because none of the methods of the server entity has
     * sequence type arguments, only constant parts of the
     * request and response are used. Arenas are effectively unused. However,
     * valid arenas of the request and response must be passed to
     * server transport methods (nk_transport_recv, nk_transport_reply) and
     * to the server_entity_dispatch method.
     */
    traffic_light_Diagnostics_entity_req req;
    char req_buffer[traffic_light_Diagnostics_entity_req_arena_size];
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer,
                                        req_buffer + sizeof(req_buffer));

    /* Prepare response structures: constant part and arena. */
    traffic_light_Diagnostics_entity_res res;
    char res_buffer[traffic_light_Diagnostics_entity_res_arena_size];
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer,
                                        res_buffer + sizeof(res_buffer));

    /**
     * Initialize report component dispatcher. 0x2000000 is the value of the step,
     * which is the number by which the input value is increased.
     */
    traffic_light_CReport_component component;
    traffic_light_CReport_component_init(&component, CreateIReportImpl(0x2000000));

    /* Initialize server entity dispatcher. */
    traffic_light_Diagnostics_entity entity;
    traffic_light_Diagnostics_entity_init(&entity, &component);

    fprintf(stderr, "Hello I'm Diagnostics\n");

    /* Dispatch loop implementation. */
    do
    {
        /* Flush request/response buffers. */
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        /* Wait for request to server entity. */
        if (nk_transport_recv(&transport.base,
                              &req.base_,
                              &req_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_recv error\n");
        } else {
            /**
             * Handle received request by calling implementation Report_impl
             * of the requested IReport interface method.
             */
            traffic_light_Diagnostics_entity_dispatch(&entity, &req.base_, &req_arena,
                                        &res.base_, &res_arena);
        }

        /* Send response. */
        if (nk_transport_reply(&transport.base,
                               &res.base_,
                               &res_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_reply error\n");
        }
    }
    while (true);

    return EXIT_SUCCESS;
}
