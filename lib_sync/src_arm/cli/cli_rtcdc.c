#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli_rtcdc.h"
#include "sy.h"


/* upload_client part */
void uv_client_upload(uv_work_t *work)
{
    struct sy_rtcdc_info_t *rtcdc_info = (struct sy_rtcdc_info_t *)work->data;
    struct rtcdc_data_channel *data_channel = rtcdc_info->data_channel; 
    const struct sy_diff_t *sy_session_diff = rtcdc_info->sy_session_diff;
    struct diff_info_t *file = sy_session_diff->files_diff;
    char local_file_path[LOCAL_PATH_SIZE];
    int files_num = sy_session_diff->num;
    int i, packet_index;
    struct sctp_packet_t packet;
    for(i = 0; i<files_num; i++)
    {
        memset(local_file_path, 0, LOCAL_PATH_SIZE);
        strncpy(local_file_path, rtcdc_info->local_repo_path, strlen(rtcdc_info->local_repo_path));
        strcat(local_file_path, file[i].filename);
        if(file[i].dirty == FILE_FS_LACK || file[i].dirty == FILE_DIRTY)
        {
            FILE *pFile;
            pFile = fopen(local_file_path, "r");
            if(pFile == NULL)
                fprintf(stderr, "fail open file: %s\n", local_file_path);
            else{
                packet_index = 0;
                memset(&packet, 0, sizeof(packet));
                packet.index = packet_index;
                strcpy(packet.buf, file[i].filename);

                printf("file_path: %s\n", local_file_path);
                rtcdc_send_message(data_channel, RTCDC_DATATYPE_STRING, (void *)&packet, sizeof(packet));

                while(!feof(pFile))
                {
                    packet_index++;
                    memset(&packet, 0, sizeof(packet));
                    if(fgets(packet.buf, SCTP_BUF_SIZE, pFile) == NULL)
                        break;
                    packet.index = packet_index;
                    packet.buf_len = strlen(packet.buf);
                    rtcdc_send_message(data_channel, RTCDC_DATATYPE_STRING, (void *)&packet, sizeof(packet));
                    usleep(100);
                }
                memset(&packet, 0, sizeof(packet));
                packet.index = -1;
                packet.buf_len = 0;
                rtcdc_send_message(data_channel, RTCDC_DATATYPE_STRING, (void *)&packet, sizeof(packet));
                fclose(pFile);
            }
        }
    }
    /* TODO: uniray: i want to stop rtcdc loop, 
     * but the next assignment will result in core dump or 
     * "Inferior 1 (process 19439) exited with code 01" in gdb */
    rtcdc_info->peer->exit_thread = 1;
}



void upload_client_on_message(struct rtcdc_data_channel *channel,
                              int datatype, void *data, 
                              size_t len, void *user_data)
{
//    printf("receive from fs\n");
}


void upload_client_on_open(struct rtcdc_data_channel *channel, void *user_data)
{
#ifdef DEBUG_RTCDC
    fprintf(stderr, "signaling_rtcdc: upload client on_open\n");
#endif
    /* allocate a uv work to upload files */
    struct sy_rtcdc_info_t *rtcdc_info = (struct sy_rtcdc_info_t *)user_data; 
    rtcdc_info->data_channel = channel;
    uv_loop_t *uv_loop = (uv_loop_t *)rtcdc_info->uv_loop;
    
    /* TODO: uniray: i dont know why it will segamentation fault in uv_upload if without declaring tt */
    struct sy_rtcdc_info_t tt;

    uv_work_t upload_work;
    upload_work.data = (void *)rtcdc_info;
    uv_queue_work(uv_loop, &upload_work, uv_client_upload, NULL);
}


void upload_client_on_channel(struct rtcdc_peer_connection *peer,
                struct rtcdc_data_channel *dc,
                void *user_data)
{
    dc->on_message = upload_client_on_message;
#ifdef DEBUG_RTCDC
    fprintf(stderr, "signaling_rtcdc: upload client on_channel\n");
#endif
}


void upload_client_on_connect(struct rtcdc_peer_connection *peer
                        ,void *user_data)
{
#ifdef DEBUG_RTCDC
    fprintf(stderr, "signaling_rtcdc: upload client on_connect\n");
    fprintf(stderr, "signaling_rtcdc: start create data channel\n");
#endif
    struct rtcdc_data_channel *upload_dc = rtcdc_create_data_channel(peer, "Upload Channel",
                                                                    "", upload_client_on_open,
                                                                    NULL, NULL, user_data);
    if(upload_dc == NULL)
        fprintf(stderr, "signaling_rtcdc: fail create data channel\n");
}



/* download_client part */

void download_client_on_message(struct rtcdc_data_channel *channel,
                              int datatype, void *data, 
                              size_t len, void *user_data)
{
    char *local_repo_path = (char *)user_data;
    struct sctp_packet_t *recvd_packet = (struct sctp_packet_t *)data;

    static FILE *pFILE;
    if(recvd_packet->index == 0)
    {
        char local_file_path[LOCAL_PATH_SIZE];
        memset(local_file_path, 0, LOCAL_PATH_SIZE);
        strcpy(local_file_path, local_repo_path);
        strcat(local_file_path, recvd_packet->buf);
        pFILE = fopen(local_file_path, "w");
        printf("filename:%s\n", local_file_path);
    }
    else if(recvd_packet->index == -1)
    {
        /*receive last buf*/
        printf("fininsh!!!!!!!!:%d\n", len);
        fclose(pFILE);
    }
    else if(recvd_packet->index > 0)
    {
        /*receive buf*/
        fwrite(recvd_packet->buf, sizeof(char), recvd_packet->buf_len, pFILE);
    }
 
}


void download_client_on_open(struct rtcdc_data_channel *channel, void *user_data)
{
#ifdef DEBUG_RTCDC
    fprintf(stderr, "signaling_rtcdc: download client on_open\n");
#endif
}


void download_client_on_channel(struct rtcdc_peer_connection *peer,
                struct rtcdc_data_channel *dc,
                void *user_data)
{
    dc->on_message = download_client_on_message;
#ifdef DEBUG_RTCDC
    fprintf(stderr, "signaling_rtcdc: download client on_channel\n");
#endif
    /* TODO: here is weird, u can observe the pointer addr of user_data, channel->user_data and channel 
     * the channel ptr addr is same with on_message's, but different with on_connect and on_open*/
    struct sy_rtcdc_info_t *rtcdc_info = (struct sy_rtcdc_info_t *)user_data;
    char *local_repo_path = rtcdc_info->local_repo_path;
    dc->user_data = (void *)local_repo_path;

}


void download_client_on_connect(struct rtcdc_peer_connection *peer
                        ,void *user_data)
{
#ifdef DEBUG_RTCDC
    fprintf(stderr, "signaling_rtcdc: download client on_connect\n");
#endif
/*    
 *  struct rtcdc_data_channel *download_dc = rtcdc_create_data_channel(peer, "Download Channel",
                                                                    "", download_client_on_open,
                                                                    NULL, NULL, user_data);
    if(download_dc == NULL)
        fprintf(stderr, "signaling_rtcdc: fail create data channel\n");
*/
}



