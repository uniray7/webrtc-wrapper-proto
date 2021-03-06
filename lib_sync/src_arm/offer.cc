#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
// #include "Common_.hpp"

#include "Utils.h"
#include "RTCPeerConnection.h"


#ifdef WIN32
#include <Windows.h>
#endif



static void on_message(Buffer *buf)
{
    fprintf(stderr, "\n\nhello: %s\n\n", (unsigned char *)buf->raw);
}

static void on_datachannel(PeerConnectionCtx* ctx, RTCDataChannelEvent* e){
    printf("on channel hahaha\n");
    SetCallback_RTCDataChannel_onmessage(e->channel->ctx_, &on_message);
}

static unsigned long CreatePeerConnection(void* lpParam)
{
    Libwebrtc_Init();

    PeerConnection* offerer = RTCPeerConnection_Init(NULL, NULL);
    SetCallback_RTCPeerConnection_ondatachannel(offerer->ctx_, &on_datachannel);

    offerer->CreateOffer(offerer->ctx_, NULL, NULL, NULL);

    RTCDataChannel* dc = offerer->GetDataChannel(offerer->ctx_, "demo");
    SetCallback_RTCDataChannel_onmessage(dc->ctx_, &on_message);

    sleep(1);

    const SessionDescription* desc1 = offerer->LocalDescription(offerer->ctx_);
    fprintf(stderr, "offerer:\n");
    fprintf(stderr, "\tSDP[type]: %s\n", desc1->type);
    fprintf(stderr, "\tSDP[sdp]: %s\n", desc1->sdp);

    /* FIXME: signaling */
    FILE *offer_file;
    offer_file = fopen("offerSDP", "wb");
    fwrite(desc1->sdp, 1, strlen(desc1->sdp), offer_file);
    fclose(offer_file);

    getchar();

    /* FIXME: signaling */
    FILE *answer_file;
    answer_file = fopen("answerSDP", "rb");
    char answerSDP_str[2048];
    memset(answerSDP_str, 0, 2048);
    fread(answerSDP_str, 1, 2048, offer_file);
    fclose(answer_file);

    SessionDescription* answer_desc = (SessionDescription *)calloc(1, sizeof(SessionDescription));
    answer_desc->type = strdup("answer\0");
    answer_desc->sdp = strdup(answerSDP_str);
    offerer->SetRemoteDescription(offerer->ctx_, answer_desc, NULL, NULL);

    unsigned char input[128];
    memset(input, 0, 128);

    while (fscanf(stdin, "%s", input))
    {
        if (!strcmp((const char *)input, "quit"))
            break;
        else
        {
            dc = offerer->GetDataChannel(offerer->ctx_, "demo");
            Buffer* raw_ = (Buffer*)calloc(1, sizeof(Buffer));
            raw_->raw = (unsigned char *)strdup((char *)input);
            raw_->size = strlen((const char*)input);
            dc->Send(dc->ctx_, raw_, 0);

        }
        memset(input, 0, 128);
    }



    RTCPeerConnection_Destroy(offerer);

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


