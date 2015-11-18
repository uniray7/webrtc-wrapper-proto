#include <libwebsockets.h>
#include <stdio.h>
#include <uv.h>
#include <stdint.h>
#include "common.h"

struct lwst_writedata_t{
    struct libwebsocket *target_wsi;
    METADATAtype type;
    char data[WRITE_DATA_SIZE];
};


struct lwst_conn_t{
    struct libwebsocket *wsi;
    struct libwebsocket_context* context;
    volatile uint8_t *exit;
};


struct lwst_conn_t* lwst_initial(const char *address, 
                                    int port, 
                                    struct libwebsocket_protocols protocols[], 
                                    char *protocol_name, 
                                    void *user_data);

void lwst_uv_connect(uv_work_t *work);

void lwst_connect(struct libwebsocket_context *context, volatile int *exit);

