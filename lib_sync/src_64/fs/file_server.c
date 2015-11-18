#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <uv.h>

#include <sys/types.h>
#include <stdlib.h>

#include <errno.h>
#include <jansson.h>
#include <sys/stat.h>
#include "../sy_lwst.h"

typedef unsigned int bool;
#include "Utils.h"
#include "RTCPeerConnection.h"
#include "RTCDataChannel.h"

#define fs_port 7681
#define fs_protocol "fileserver-protocols"

struct lwst_conn_t *signal_conn;
static volatile int sync_exit;
//static uv_loop_t *main_loop;
static struct libwebsocket_protocols sync_protocols[];
static struct libwebsocket_protocols fileserver_protocols[];


static void on_message(Buffer *buf)
{
        fprintf(stderr, "\n\nhello: %s\n\n", (unsigned char *)buf->raw);
}

static void on_datachannel(PeerConnectionCtx* ctx, RTCDataChannelEvent* e){
        printf("on channel hahaha\n");
            SetCallback_RTCDataChannel_onmessage(e->channel->ctx_, &on_message);
}



static int callback_http(struct libwebsocket_context *context,
        struct libwebsocket *wsi,
        enum libwebsocket_callback_reasons reason, void *user,
        void *in, size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_HTTP:
            break;
        default:
            break;
    }
    return 0;
}


static int callback_fileserver(struct libwebsocket_context *context,
        struct libwebsocket *wsi,
        enum libwebsocket_callback_reasons reason, void *user,
        void *in, size_t len)
{
    struct lwst_writedata_t **write_data_ptr = (struct lwst_writedata_t **)user;
    switch (reason) {
        case LWS_CALLBACK_PROTOCOL_INIT:
            {
                fprintf(stderr, "FILE_SERVER: LWS_CALLBACK_PROTOCOL_INIT\n");
                // send FS_REGISTER to MS
                const char *address = "192.168.0.103";
                int port = 8080;
                signal_conn = lwst_initial(address, port, sync_protocols, "sync-protocol", NULL);
                struct libwebsocket_context *context = signal_conn->context;
                sync_exit = 0;
                lwst_connect(context, &sync_exit);
                break;
            }
        case LWS_CALLBACK_ESTABLISHED:
            {
                fprintf(stderr, "FILE_SERVER: LWS_CALLBACK_ESTABLISHED\n");
                break;
            }
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, "FILE_SERVER: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
            break;
        case LWS_CALLBACK_CLOSED:
            fprintf(stderr, "FILE_SERVER: LWS_CALLBACK_CLOSED\n");
            break;
        case LWS_CALLBACK_SERVER_WRITEABLE:
            {
                int lws_err;
                size_t data_len;
                struct lwst_writedata_t *write_data = *write_data_ptr;
                if((data_len = strlen(write_data->data))>0)
                {

                    lws_err = libwebsocket_write(write_data->target_wsi, (void *) write_data->data, strlen(write_data->data), LWS_WRITE_TEXT);
                    switch(write_data->type){
                        case FS_REGISTER:
                            {
#ifdef DEBUG_FS
                                if(lws_err<0)
                                    fprintf(stderr, "FILE_SERVER: send FS_REGISTER fail\n");
                                else 
                                    fprintf(stderr, "FILE_SERVER: send FS_REGISTER\n");
#endif
                                break;
                            }
                        default:
                            break;
                    }                
                    memset(write_data->data, 0, 100);
                }
                break;
            }
        case LWS_CALLBACK_RECEIVE:
            {
                int metadata_type;
                char *recvd_data_str = (char *)in;
                json_t *recvd_json = NULL;
                json_error_t *err = NULL;
                recvd_json = json_loads((const char *)recvd_data_str, JSON_DECODE_ANY, err);
                json_unpack(recvd_json, "{s:i}", "metadata_type", &metadata_type);
                switch(metadata_type){
                    case UPLOAD_REQUEST:
                        {
#ifdef DEBUG_FS
                            fprintf(stderr, "FILE_SERVER: receive UPLOAD_REQUEST %s\n", recvd_data_str);
#endif
                            // create answer             
                            PeerConnection* answerer = RTCPeerConnection_Init(NULL, NULL);
                            SetCallback_RTCPeerConnection_ondatachannel(answerer->ctx_, &on_datachannel);
                            json_t *offer_json = json_object_get(recvd_json, "offer");
                            char *offer_sdp = (char *)json_string_value(json_object_get(offer_json, "sdp"));
                            SessionDescription* offer_desc = (SessionDescription *)calloc(1, sizeof(SessionDescription));
                            offer_desc->type = strdup("offer\0");
                            offer_desc->sdp = strdup(offer_sdp);
                            answerer->SetRemoteDescription(answerer->ctx_, offer_desc, NULL, NULL);
                            answerer->CreateAnswer(answerer->ctx_, NULL, NULL, NULL);

                            const SessionDescription* answer_desc = answerer->LocalDescription(answerer->ctx_);
                            json_t *sent_json = json_object();
                            json_t *answer_json = json_object();
                            json_object_set_new(sent_json, "metadata_type", json_integer(UPLOAD_READY));
                            json_object_set_new(sent_json, "userID", json_string("fakeRay0"));
                            json_object_set_new(offer_json, "type", json_string("answer"));
                            json_object_set_new(offer_json, "sdp", json_string(answer_desc->sdp));
                            json_object_set_new(sent_json, "answer", answer_json);
                            char *sent_str = json_dumps(sent_json, 0);

                            struct lwst_writedata_t *write_data = *write_data_ptr;
                            strcpy(write_data->data, sent_str);
                            write_data->target_wsi = wsi;
                            write_data->type = UPLOAD_READY;
                            libwebsocket_callback_on_writable(context, wsi);
                            printf("%s\n", sent_str);
                            break;
                        }
                    default:
                        break;
                }
            }
        default:
            break;
    }
    return 0;
}


static int callback_sync(struct libwebsocket_context *context,
        struct libwebsocket *wsi,
        enum libwebsocket_callback_reasons reason, void *user,
        void *in, size_t len)
{
    struct lwst_writedata_t **write_data_ptr = (struct lwst_writedata_t **)user;
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            {
                fprintf(stderr, "FILE_SERVER: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
                /* initial write data */
                struct lwst_writedata_t *write_data = (struct lwst_writedata_t *)calloc(1, sizeof(struct lwst_writedata_t));
                *write_data_ptr = write_data;

                /* register to metadata server */
                json_t *sent_session_JData = json_object();
                json_object_set_new(sent_session_JData, "metadata_type", json_integer(FS_REGISTER)); 
                json_object_set_new(sent_session_JData, "fs_port", json_integer(fs_port)); 
                json_object_set_new(sent_session_JData, "fs_protocol", json_string(fs_protocol)); 
                char *sent_data_str = json_dumps(sent_session_JData, 0);
                /* TODO: should allocate memory for write_data pointer? */
                write_data->target_wsi = wsi;
                write_data->type = FS_REGISTER;
                strcpy(write_data->data, sent_data_str);

                free(sent_data_str);
                free(sent_session_JData);

                libwebsocket_callback_on_writable(context, wsi);
                break;
            }
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, "FILE_SERVER: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
            break;
        case LWS_CALLBACK_CLOSED:
            fprintf(stderr, "FILE_SERVER: LWS_CALLBACK_CLOSED\n");
            // TODO when close signaling session, fileserver needs to deregister to signaling server
            sync_exit = -1;
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                int lws_err;
                size_t data_len;
                struct lwst_writedata_t *write_data = *write_data_ptr;
                if((data_len = strlen(write_data->data))>0)
                {
                    lws_err = libwebsocket_write(write_data->target_wsi, (void *) write_data->data, strlen(write_data->data), LWS_WRITE_TEXT);
                    switch(write_data->type){
                        case FS_REGISTER:
                            {
#ifdef DEBUG_FS
                                if(lws_err<0)
                                    fprintf(stderr, "FILE_SERVER: send FS_REGISTER fail\n");
                                else 
                                    fprintf(stderr, "FILE_SERVER: send FS_REGISTER\n");
#endif
                                break;
                            }
                        default:
                            break;
                    }                
                    memset(write_data->data, 0, 100);
                }
                break;
            }
        case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                int metadata_type;
                char *recvd_data_str = (char *)in;
                char *sent_data_str = NULL;
                json_t *recvd_session_JData = NULL;
                json_error_t *err = NULL;
                recvd_session_JData = json_loads((const char *)recvd_data_str, JSON_DECODE_ANY, err);
                json_unpack(recvd_session_JData, "{s:i}", "metadata_type", &metadata_type);
                switch(metadata_type){
                    case FS_REGISTER_OK:
                        {
#ifdef DEBUG_FS
                            fprintf(stderr, "FILE_SERVER: receive FS_REGISTER_OK\n");
#endif
                            sync_exit = 1;
                            return -1;
                            break;
                        }
                    default:
                        break;
                }
            }
        default:
            break;
    }
    return 0;
}





static struct libwebsocket_protocols fileserver_protocols[] = {
    {
        "http-only",
        callback_http,
        0
    },
    {
        fs_protocol,
        callback_fileserver,
        2048,
    },
    { NULL, NULL, 0, 0 }
};


static struct libwebsocket_protocols sync_protocols[] = {
    {
        "sync-protocol",
        callback_sync,
        2048,
    },
    { NULL, NULL, 0, 0 }
};


int main(int argc, char *argv[]){
    Libwebrtc_Init(); 
    
    char cert_path[1024];
    char key_path[1024];
    int use_ssl = 0;
    int debug_level = 7;
    const char *iface = NULL;
    struct lws_context_creation_info info;
    int n;
    int opts = 0;
    int daemonize = 0;
    int syslog_options = LOG_PID | LOG_PERROR; 
    static struct libwebsocket_context *context; 
    static volatile int force_exit = 0;


    memset(&info, 0, sizeof(info));
    info.port = fs_port;


    setlogmask(LOG_UPTO (LOG_DEBUG));
    openlog("lwsts", syslog_options, LOG_DAEMON);
    lws_set_log_level(debug_level, lwsl_emit_syslog);

    info.iface = iface;
    info.protocols = fileserver_protocols;
    info.extensions = libwebsocket_get_internal_extensions();

    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;

    info.gid = -1;
    info.uid = -1;
    info.options = opts;

    context = libwebsocket_create_context(&info);

    if (context == NULL) {
        lwsl_err("create context failed\n");
        return -1;
    }
    n = 0;
    while( n>=0 && !force_exit){
        n = libwebsocket_service(context, 50);
    }

    libwebsocket_context_destroy(context); 
    lwsl_notice("libwebsockets-test-server exited cleanly\n"); 

    closelog();

    Libwebrtc_Dispose();
    return 0;
}

