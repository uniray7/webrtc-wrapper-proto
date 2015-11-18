#include <string.h>
#include "sy_lwst.h"

#ifndef WIN32
#include <syslog.h>
#endif
struct lwst_conn_t* lwst_initial(const char *address, 
                                    int port, 
                                    struct libwebsocket_protocols protocols[], 
                                    char *protocol_name, 
                                    void* user_data)
{
    struct libwebsocket *wsi;
    int use_ssl = 0;
    const char *iface = NULL;
    int ietf_version = -1; 
    int opts = 0;

    /*syslog setting*/
#ifndef WIN32
    int syslog_options = LOG_PID | LOG_PERROR; 
    setlogmask(LOG_UPTO (LOG_DEBUG));
    openlog("lwsts", syslog_options, LOG_DAEMON);
#endif

	int debug_level = 7;
    lws_set_log_level(debug_level, lwsl_emit_syslog);
    /*end syslog setting*/
    
    struct libwebsocket_context *context; 
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.iface = iface;
    info.protocols = protocols;
    info.extensions = libwebsocket_get_internal_extensions();

    info.gid = -1;
    info.uid = -1;
    info.options = opts;

    struct lwst_conn_t* SDP_conn = (struct lwst_conn_t *)calloc(1, sizeof(struct lwst_conn_t)); 
    context = libwebsocket_create_context(&info);
    if (context == NULL) {
        lwsl_err("libwebsocket_create_context failed\n");
        goto bail;
    }
    SDP_conn->context = context;

    wsi = libwebsocket_client_connect_extended(context, address, port, use_ssl, "/", 
            address, address, protocol_name, ietf_version, user_data);
    if (wsi == NULL)
    {
        fprintf(stderr, "libwebsocket_client_connect failed\n");
        goto bail;
    }
    SDP_conn->wsi = wsi;
    return SDP_conn;
bail:
    fprintf(stderr, "Exit\n");
    libwebsocket_context_destroy(context); 
#ifndef WIN32
    closelog();
#endif
    return 0;

}


void lwst_uv_connect(uv_work_t *work)
{
    fprintf(stderr, "SIGNALING: waiting for connect...\n");
    int n = 0;
    struct lwst_conn_t* conn_info = (struct lwst_conn_t *)work->data;
    struct libwebsocket_context *context = conn_info->context;
    volatile uint8_t *exit = conn_info->exit;
    while( n >= 0 && !(*exit)){
        n = libwebsocket_service(context, 20);
    }
    fprintf(stderr, "SIGNALING: close connect\n\n");
    libwebsocket_context_destroy(context);
}


void lwst_connect(struct libwebsocket_context *context, volatile int *exit)
{
    fprintf(stderr, "SIGNALING: waiting for connect...\n");
    int n = 0;
    while( n >= 0 && !(*exit)){
        n = libwebsocket_service(context, 20);
    }
    fprintf(stderr, "SIGNALING: close connect\n");
}
