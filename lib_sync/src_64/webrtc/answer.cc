#include "linux_preprocessor.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "Utils.h"
#include "RTCPeerConnection.h"

static int count = 0;
static void on_message(Buffer *buf)
{
    fprintf(stderr, "receive data: %d, size: %d\n\n", count, buf->size);
    count++;
}

static void on_datachannel(PeerConnectionCtx* ctx, RTCDataChannelEvent* e){
    printf("on channel hahaha\n");
    SetCallback_RTCDataChannel_onmessage(e->channel->ctx_, &on_message);
}


static unsigned long CreatePeerConnection(void* lpParam)
{
    Libwebrtc_Init();
    PeerConnection* answerer = RTCPeerConnection_Init(NULL, NULL);
    SetCallback_RTCPeerConnection_ondatachannel(answerer->ctx_, &on_datachannel);
    /* FIXME: signaling */
    FILE *offer_file;
    offer_file = fopen("offerSDP", "rb");
    char offerSDP_str[2048];
    memset(offerSDP_str, 0, 2048);
    fread(offerSDP_str, 1, 2048, offer_file);
    fclose(offer_file);

    SessionDescription* offer_desc = (SessionDescription *)calloc(1, sizeof(SessionDescription));
    offer_desc->type = strdup("offer\0");
    offer_desc->sdp = strdup(offerSDP_str);

    answerer->SetRemoteDescription(answerer->ctx_, offer_desc, NULL, NULL);
    answerer->CreateAnswer(answerer->ctx_, NULL, NULL, NULL);

    RTCDataChannel* dc = answerer->GetDataChannel(answerer->ctx_, "demo");
    SetCallback_RTCDataChannel_onmessage(dc->ctx_, &on_message);

    sleep(3);
    const SessionDescription* answer_desc = answerer->LocalDescription(answerer->ctx_);
    fprintf(stderr, "offerer:\n");
    fprintf(stderr, "\tSDP[type]: %s\n", answer_desc->type);
    fprintf(stderr, "\tSDP[sdp]: %s\n", answer_desc->sdp);

    /* FIXME: signaling */
    FILE *answer_file;
    answer_file = fopen("answerSDP", "wb");
    fwrite(answer_desc->sdp, 1, strlen(answer_desc->sdp), answer_file);
    fclose(answer_file);

    unsigned char input[128];

    memset(input, 0, 128);

    while (fscanf(stdin, "%s", input))
    {
        
        if (!strcmp((const char*)input, "quit"))
            break;
        else
        {
            dc = answerer->GetDataChannel(answerer->ctx_, "demo");
            Buffer* raw_ = (Buffer*)calloc(1, sizeof(Buffer));
            raw_->raw = (unsigned char *)strdup((char *)input);
            raw_->size = strlen((const char*)input);
            dc->Send(dc->ctx_, raw_, 0);

        }
        memset(input, 0, 128);
    }


    RTCPeerConnection_Destroy(answerer);

    Libwebrtc_Dispose();

    exit(0);
}

int main(int argc, char* argv[])
{

    CreatePeerConnection(0);
    return 0;

bail:
    assert(0);
    return -1;
}

