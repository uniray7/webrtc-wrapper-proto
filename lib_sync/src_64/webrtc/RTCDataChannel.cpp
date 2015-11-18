#include <iostream>
#include "linux_preprocessor.h"

#include "RTCDataChannel.h"

#include "Message.h"
#include <cassert>
#include <string>

#include "Common_.hpp"

#include "talk/app/webrtc/datachannelinterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"

using rtc::scoped_refptr;
using rtc::RefCountedObject;

struct __RTCDataChannel_Obj;

const char* RTCDataChannel_Label(RTCDataChannelCtx* ctx);
int RTCDataChannel_BinaryType(RTCDataChannelCtx* ctx);
const char* RTCDataChannel_ReadyState(RTCDataChannelCtx* ctx);
void RTCDataChannel_SetBinaryType(RTCDataChannelCtx* ctx, const char* binaryType);
int RTCDataChannel_ReadOnly(RTCDataChannelCtx* ctx);

class WebRtcDataChannel : public webrtc::DataChannelObserver {
    public:
        WebRtcDataChannel();
        ~WebRtcDataChannel();

        void SetDataChannel(webrtc::DataChannelInterface* channel) {
            this->data_channel_ = channel;

            if (data_channel_.get() != NULL) {
                data_channel_->RegisterObserver(this);
            }
        };

        inline void SetCallback_onmessage(onmessageCallback onmessage_){ onmessage = onmessage_;}
        inline void SetCallback_onopen(onopenCallback onopen_, void *user_data){ onopen = onopen_;onopen_userdata = user_data;}
        bool Send(const webrtc::DataBuffer& buffer);
        void Close();

        //webrtc::DataChannelObserver implementation.
            virtual void OnStateChange();
        virtual void OnMessage(const webrtc::DataBuffer& buffer);

        const char* GetLabel() { return label_.c_str(); };

        struct __RTCDataChannel_Obj* obj_;
    private:
        rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel_;
        webrtc::DataChannelInterface::DataState state;
        onopenCallback onopen;
        onmessageCallback onmessage;
        void *onopen_userdata;
        std::string label_;
};


typedef struct __RTCDataChannel_Obj {
    WebRtcDataChannel dc;

    RTCDataChannelCtx *ctx_;
} RTCDataChannel_Obj;

WebRtcDataChannel::WebRtcDataChannel() {
    onmessage = NULL;
}

WebRtcDataChannel::~WebRtcDataChannel() {
#if 0
    if (data_channel_.get() != NULL) {
        data_channel_->UnregisterObserver();
    }
#else
    // Do uregister in Close(), user's duty
#endif
}

bool WebRtcDataChannel::Send(const webrtc::DataBuffer& buffer)
{
    if (data_channel_.get() != NULL) {
        return data_channel_->Send(buffer);
    }
}

void WebRtcDataChannel::Close()
{
    data_channel_->Close();

    if (data_channel_.get() != NULL) {
        data_channel_->UnregisterObserver();
    }
}

void WebRtcDataChannel::OnStateChange()
{
    LOG(INFO) << __FUNCTION__;
    switch (data_channel_->state()) {
        case webrtc::DataChannelInterface::kConnecting:
            break;
        case webrtc::DataChannelInterface::kOpen:
        {
            if(onopen)
            {
                onopen(obj_->ctx_->o, onopen_userdata);
            }
            break;
        }
        case webrtc::DataChannelInterface::kClosing:
            break;
        case webrtc::DataChannelInterface::kClosed:
            break;
     }

}

void WebRtcDataChannel::OnMessage(const webrtc::DataBuffer& buffer)
{
    LOG(INFO) << __FUNCTION__;
    std::cout << __FUNCTION__<<std::endl;
    if (onmessage)
    {
        Buffer *buf = new Buffer();
        buf->raw = (unsigned char *)calloc(1, buffer.size()+1);
        memset(buf->raw, 0, buffer.size());
        buf->size = buffer.size();

        memcpy(buf->raw, (unsigned char*)buffer.data.data(), buffer.size());

        onmessage(buf);
    }
}


RTCDataChannel* RTCDataChannel_Init(webrtc::DataChannelInterface *channel)
{
    RTCDataChannelCtx *ctx = new RTCDataChannelCtx();
    RTCDataChannel *o = new RTCDataChannel();
    RTCDataChannel_Obj *obj = new RTCDataChannel_Obj();
    WebRtcDataChannel *dc = (WebRtcDataChannel*)obj;

    o->Send = RTCDataChannel_Send;
    o->Close = RTCDataChannel_Close; // void close();

    // Library set
    o->Label = RTCDataChannel_Label;
    o->BinaryType = RTCDataChannel_BinaryType;
    o->ReadyState = RTCDataChannel_ReadyState;
    o->SetBinaryType = RTCDataChannel_SetBinaryType;
    o->ReadOnly = RTCDataChannel_ReadOnly;

    dc->SetDataChannel(channel);

    o->ctx_ = ctx;
    obj->ctx_ = ctx;

    ctx->o = o;
    ctx->obj = obj;

    obj->dc.obj_ = obj;
    return o;
}

void RTCDataChannel_Destroy(RTCDataChannel* o)
{
    RTCDataChannelCtx *ctx = o->ctx_;
    RTCDataChannel_Obj *obj = ctx->obj;
    WebRtcDataChannel *dc = (WebRtcDataChannel*)obj;

    dc->Close();
    o->Send = 0;
    o->Close = 0; // void close();

    // Library set
    o->Label = 0;
    o->BinaryType = 0;
    o->ReadyState = 0;
    o->SetBinaryType = 0;
    o->ReadOnly = 0;

    //delete dc;
    delete o;

    delete obj;
    delete ctx;
}

int RTCDataChannel_Send(RTCDataChannelCtx* ctx, const Buffer* data, bool binary/* = false*/)
{
    RTCDataChannel *o = ctx->o;
    RTCDataChannel_Obj *obj = ctx->obj;
    WebRtcDataChannel *dc = (WebRtcDataChannel*)obj;

    //FIXME: binary type is not supported currently
    binary = false;

    const rtc::Buffer buf(data->raw, data->size);
    const webrtc::DataBuffer buffer(buf, binary);

    return dc->Send(buffer);
}

void RTCDataChannel_Close(RTCDataChannelCtx* ctx)
{
    RTCDataChannel *o = ctx->o;
    RTCDataChannel_Obj *obj = ctx->obj;
    WebRtcDataChannel *dc = (WebRtcDataChannel*)obj;

    dc->Close();
}

const char* RTCDataChannel_Label(RTCDataChannelCtx* ctx)
{
    RTCDataChannel *o = ctx->o;
    RTCDataChannel_Obj *obj = ctx->obj;
    WebRtcDataChannel *dc = (WebRtcDataChannel*)obj;

    return dc->GetLabel();
}

int RTCDataChannel_BinaryType(RTCDataChannelCtx* ctx)
{

    return 1; // Default is YES
}

const char* RTCDataChannel_ReadyState(RTCDataChannelCtx* ctx)
{

    return "unknown";
}

void RTCDataChannel_SetBinaryType(RTCDataChannelCtx* ctx, const char* binaryType)
{
}

int RTCDataChannel_ReadOnly(RTCDataChannelCtx* ctx)
{
    return 0;
}


void SetCallback_RTCDataChannel_onmessage(RTCDataChannelCtx* ctx, onmessageCallback onmessage)
{
    RTCDataChannel *o = ctx->o;
    RTCDataChannel_Obj *obj = ctx->obj;
    WebRtcDataChannel *dc = (WebRtcDataChannel*)obj;
    dc->SetCallback_onmessage(onmessage);
}


void SetCallback_RTCDataChannel_onopen(RTCDataChannelCtx* ctx, onopenCallback onopen, void* user_data)
{
    RTCDataChannel *o = ctx->o;
    RTCDataChannel_Obj *obj = ctx->obj;
    WebRtcDataChannel *dc = (WebRtcDataChannel*)obj;
    dc->SetCallback_onopen(onopen, user_data);
}

