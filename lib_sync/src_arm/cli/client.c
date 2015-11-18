#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <jansson.h>

#include <sys/types.h>
#include <stdlib.h>
#include "cli_struct.h"
#include "../sy_lwst.h"

#include <sys/time.h>

typedef unsigned int bool;
#include "Utils.h"
#include "RTCPeerConnection.h"
#include "RTCDataChannel.h"

#define MD5_LINE_SIZE 32
#define LOCAL_REPO_PATH "/home/uniray/local_repo/"
#define METADATA_SERVER_IP "192.168.0.103"




static uv_loop_t *main_loop;
/* this design may be a problem */
static struct libwebsocket_protocols client_protocols[];

static void on_message(Buffer *buf)
{
    fprintf(stderr, "\n\nhello: %s\n\n", (unsigned char *)buf->raw);
}

static void on_datachannel(PeerConnectionCtx* ctx, RTCDataChannelEvent* e){
    printf("on channel hahaha\n");
    SetCallback_RTCDataChannel_onmessage(e->channel->ctx_, &on_message);
}

static void on_open(RTCDataChannel *dc, void *user_data)
{
    fprintf(stderr, "on open!!!\n");
    volatile uint8_t *client_exit = (volatile uint8_t *)user_data;
    
    int size = 16384;
    unsigned char *test_str = (char *)calloc(1, size+1);
    memset(test_str, 'a', size);
    Buffer* raw_ = (Buffer*)calloc(1, sizeof(Buffer));
    raw_->raw = strdup(test_str);
    raw_->size = strlen((const char*)test_str);
    int count = 0;

    struct timeval start_time, end_time;
    gettimeofday( &start_time, NULL );

    while (count <= 1250)
    {
        printf("index: %i\n", count);
        count++;
        dc->Send(dc->ctx_, raw_, 0);
        usleep(10000);
    }
    gettimeofday( &end_time, NULL );
    float total_time = 1000000*(end_time.tv_sec-start_time.tv_sec)+end_time.tv_usec-start_time.tv_usec;
    printf("send %iMB in %f secs\n", (size*count)/1000000, total_time/1000000);
}


static int callback_client(struct libwebsocket_context *context,
        struct libwebsocket *wsi,
        enum libwebsocket_callback_reasons reason, void *user,
        void *in, size_t len)
{
    struct lwst_writedata_t *write_data = (struct lwst_writedata_t *)user;
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            fprintf(stderr, "SYCLIENT: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, "SYCLIENT: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
            break;

        case LWS_CALLBACK_CLOSED:
            fprintf(stderr, "SYCLIENT: LWS_CALLBACK_CLOSED\n");
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                switch(write_data->type){
                    case(UPLOAD_REQUEST):
                        {
                            int lws_err = libwebsocket_write(write_data->target_wsi, (void *)write_data->data, strlen(write_data->data), LWS_WRITE_TEXT);
#ifdef DEBUG_SY            
                            if(lws_err < 0)
                                fprintf(stderr, "SYCLIENT: send SY_UPLOAD to metadata_server fail\n");
                            else
                                fprintf(stderr, "SYCLIENT: send SY_UPLOAD to metadata_server\n");
#endif
                            break;
                        }
                        break;
                }
            }
        case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                if(in != NULL)
                { 
                    int metadata_type ,lws_err;
                    char *session_SData = (char *)in;
                    json_error_t *err = NULL;
                    json_t *recvd_session_JData = NULL;
                    recvd_session_JData = json_loads((const char *)session_SData, JSON_DECODE_ANY, err);
                    json_unpack(recvd_session_JData, "{s:i}", "metadata_type", &metadata_type);

                    switch (metadata_type){
                        case UPLOAD_READY:
                        {    
                            fprintf(stderr,"SYCLIENT: receive UPLOAD_READY\n");
                            /*TODO: need mutex? */
                            memset(write_data->data, 0, WRITE_DATA_SIZE);
                            strcpy(write_data->data, session_SData);
                            write_data->target_wsi = NULL;
                            write_data->type = UPLOAD_READY;
                            json_decref(recvd_session_JData);
                            recvd_session_JData = NULL;
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

static struct libwebsocket_protocols client_protocols[] = {
    {
        "sync-protocol",
        callback_client,
        2048, /* TODO: the size may be a trouble */
    },
    { NULL, NULL, 0, 0 }
};


struct sy_session_t *sy_default_session()
{
    struct sy_session_t *sy_session = (struct sy_session_t *)calloc(1, sizeof(struct sy_session_t));
    return sy_session;
}


uint8_t sy_upload()
{
    /* connect to metadata server*/
    const char *MS_IP = METADATA_SERVER_IP;
    uint16_t MS_port = 8080;
    uint8_t metadata_type;
    struct lwst_conn_t *metadata_conn;
    struct lwst_writedata_t *write_data = (struct lwst_writedata_t *)calloc(1, sizeof(struct lwst_writedata_t));
    metadata_conn = lwst_initial(MS_IP, MS_port, client_protocols, "sync-protocol", (void *)write_data);
    struct libwebsocket_context *context = metadata_conn->context;
    struct libwebsocket *wsi = metadata_conn->wsi;
    volatile uint8_t client_exit = 0;
    metadata_conn->exit = &client_exit;

    /* allocate a uv to run lwst_uv_connect() */
    main_loop = uv_default_loop();
    uv_work_t work;
    work.data = (void *)metadata_conn;
    uv_queue_work(main_loop, &work, lwst_uv_connect, NULL);
    /* gen rtcdc SDP and candidates */
    //Libwebrtc_Init();
    PeerConnection *offerer = RTCPeerConnection_Init(NULL, NULL);
    SetCallback_RTCPeerConnection_ondatachannel(offerer->ctx_, &on_datachannel); 
    // TODO: createDataChannel() is in CreateOffer(), it's dirty code
    offerer->CreateOffer(offerer->ctx_, NULL, NULL, NULL);
    RTCDataChannel *dc = offerer->GetDataChannel(offerer->ctx_, "demo");
    SetCallback_RTCDataChannel_onopen(dc->ctx_, on_open, (void *)&client_exit);
    sleep(1);
    const SessionDescription *desc = offerer->LocalDescription(offerer->ctx_);
    //fprintf(stderr, "sdp:%s\n", desc->sdp);
    char *offer_sdp = strdup(desc->sdp);

    /* send request: upload files which the fileserver doesnt have */

    json_t *sent_json = json_object();
    json_t *offer_json = json_object();
    json_object_set_new(sent_json, "metadata_type", json_integer(UPLOAD_REQUEST));
    json_object_set_new(sent_json, "userID", json_string("fakeRay0"));
    json_object_set_new(offer_json, "type", json_string("offer"));
    json_object_set_new(offer_json, "sdp", json_string(offer_sdp));
    json_object_set_new(sent_json, "offer", offer_json);

    char *sent_str = json_dumps(sent_json, 0);
    strcpy(write_data->data, sent_str);
    write_data->target_wsi = wsi;
    write_data->type = UPLOAD_REQUEST;

    libwebsocket_callback_on_writable(context, wsi);


    /* wait for FS_UPLOAD_READY fs SDP and candidate */
    /* TODO: here needs mutex */
    while(1){
        if(write_data->type == UPLOAD_READY){
            //client_exit = 1;
            break;
        }
        usleep(2500);
    }
    /* close the libwebsocket connection and then start rtcdc connection*/
    /* parse fs_SDP and fs_candidates */
    json_t *recvd_json = json_loads(write_data->data, JSON_DECODE_ANY, NULL); 
    json_t *answer_json = json_object_get(recvd_json, "answer");
    char *answer_sdp = (char *)json_string_value(json_object_get(answer_json, "sdp"));

    SessionDescription* answer_desc = (SessionDescription *)calloc(1, sizeof(SessionDescription));
    answer_desc->type = strdup("answer\0");
    answer_desc->sdp = strdup(answer_sdp);

    offerer->SetRemoteDescription(offerer->ctx_, answer_desc, NULL, NULL);
    /* rtcdc connection */
    
    sleep(5);
    client_exit = 1;

    /* TODO: when the rtcdc_loop terminate? */
    uv_run(main_loop, UV_RUN_DEFAULT);

     /* free memory */
    /*free(sent_str);
      sent_str = NULL;
      free(client_SDP);
      libwebsocket_context_destroy(context);
      json_decref(recvd_json);
      json_decref(sent_json); 

      free(write_data);
      free(metadata_conn);
      */
    json_decref(recvd_json);
    json_decref(answer_json);

    RTCPeerConnection_Destroy(offerer);
    //Libwebrtc_Dispose();
    /* return METADATAtype */
    
    printf("bye!!!!!!!\n") ;
    return UPLOAD_OK;
}



int main(int argc, char *argv[]){

    Libwebrtc_Init();
    while (1){
        printf("start upload\n");
        sy_upload();
        printf("bye\n");
        sleep(5);
    }
    Libwebrtc_Dispose();

    printf("bye\n") ;
    return 0;
}

