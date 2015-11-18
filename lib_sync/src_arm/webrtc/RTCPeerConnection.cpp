#include "linux_preprocessor.h"

#include "RTCPeerConnection.h"
#include "RTCDataChannel.h"
#include "Message.h"
#include <cassert>
#include <string>

#include "Common_.hpp"

#include "talk/app/webrtc/peerconnectionfactoryproxy.h"
#include "talk/app/webrtc/datachannelinterface.h"
#include "talk/app/webrtc/proxy.h"

extern webrtc::PeerConnectionFactoryInterface* GetFactory();
extern rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> CreateFactory();
extern RTCDataChannel* RTCDataChannel_Init(webrtc::DataChannelInterface *channel);
extern void RTCDataChannel_Destroy(RTCDataChannel* pc);

#if 0
static void _successCallback(PeerConnectionCtx* ctx)
{
    LOG(INFO) << __FUNCTION__;

    return;
}

static void _failureCallback(PeerConnectionCtx* ctx)
{
    LOG(INFO) << __FUNCTION__ << " ";

    return;
}
#endif

class DummySetSessionDescriptionObserver
: public webrtc::SetSessionDescriptionObserver {
    public:
        static DummySetSessionDescriptionObserver* Create(PeerConnectionCtx* ctx) {
            return
                new rtc::RefCountedObject<DummySetSessionDescriptionObserver>(ctx);
        }

        virtual void OnSuccess() {
            LOG(INFO) << __FUNCTION__;
        }
        virtual void OnFailure(const std::string& error) {
            LOG(INFO) << __FUNCTION__ << " " << error;
        }

    protected:
        DummySetSessionDescriptionObserver(PeerConnectionCtx* ctx) : ctx_(ctx) {}
        ~DummySetSessionDescriptionObserver() {}

        PeerConnectionCtx *ctx_;
};

class WebRtcPeerConnection
: public webrtc::PeerConnectionObserver,
    public webrtc::CreateSessionDescriptionObserver {
        public:
            WebRtcPeerConnection();
            ~WebRtcPeerConnection();

            webrtc::PeerConnectionInterface* GetSocket();
            RTCDataChannel* GetDataChannel();

            bool InitConnection();
            bool CreateDataChannel(const char* label);
            void Shutdown();

            void CreateOffer();
            void CreateAnswer();

            void AddPrAnswer(const std::string& type, const std::string& sdp);
            void AddPrAnswer(webrtc::SessionDescriptionInterface* desc);
            void AddCandidate(std::string& sdp,std::string& mid, int line);

#if 0
            static webrtc::SessionDescriptionInterface* StringToDescription(
                    const std::string string_desc);
            static std::string DescriptionToString(
                    const webrtc::SessionDescriptionInterface* desc);
#endif

            virtual const webrtc::SessionDescriptionInterface*
                local_description() const {
                    return peer_connection_->local_description();
                }

            virtual const webrtc::SessionDescriptionInterface*
                remote_description() const {
                    return peer_connection_->remote_description();
                }

        protected:
            // inherited from PeerConnectionObserver
            virtual void OnError();
            virtual void OnStateChange(
                    webrtc::PeerConnectionObserver::StateType state_changed);
            virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
            virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
            virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
            virtual void OnDataChannel(webrtc::DataChannelInterface* data_channel);
            virtual void OnRenegotiationNeeded(void);

            // inherited from CreateSessionDescriptionObserver
            virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
            virtual void OnFailure(const std::string &error);
            virtual int AddRef() { return 1; }
            virtual int Release() { return 0; }

#if 0
            // inherited from SetSessionDescriptionObserver
            virtual void OnSuccess();
#endif
        private:
#if 0
            rtc::Thread* GetWorkerThread();
#endif
            rtc::Thread* _worker_thread, *_signal_thread;

            rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
                peer_connection_factory_;
            static rtc::scoped_refptr<webrtc::PortAllocatorFactoryInterface> _port_allocator_factory;
            webrtc::PeerConnectionInterface::IceServers servers_;
            webrtc::PeerConnectionInterface::IceServer server_;
            webrtc::FakeConstraints constraints_;
            webrtc::DataChannelInit dcConfig_;
            rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
#if 0
            rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track_;
            rtc::scoped_refptr<webrtc::MediaStreamInterface> stream_;
#endif
            // rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel_;
            RTCDataChannel* data_channel_;

        public:
            onsignalingstatechangeCallback _onsignalingstatechange;
            oniceconnectionstatechangeCallback _oniceconnectionstatechange;
            onicecandidateCallback _onicecandidate;
            ondatachannelCallback _ondatachannel;
            onnegotiationneededCallback _onnegotiationneeded;
            onaddstreamCallback _onaddstream;
            onremovestreamCallback _onremovestream;

            RTCSessionDescriptionCallback _offerCallback;
            RTCPeerConnectionErrorCallback _offerErrorCallback;

            RTCSessionDescriptionCallback _answerCallback;
            RTCPeerConnectionErrorCallback _answerErrorCallback;

            VoidFunctionCallback _localCallback;
            RTCPeerConnectionErrorCallback _localErrorCallback;

            VoidFunctionCallback _remoteCallback;
            RTCPeerConnectionErrorCallback _remoteErrorCallback;
            struct __PeerConnection_Obj* obj_;

    };

typedef struct __PeerConnection_Obj {
    WebRtcPeerConnection rtc_;
    PeerConnectionCtx *ctx_;
} PeerConnection_Obj;

static webrtc::SessionDescriptionInterface* StringToDescription(const std::string string_desc);
static std::string DescriptionToString(const webrtc::SessionDescriptionInterface* desc);
static void _onnegotiationneededCallback(PeerConnectionCtx* ctx);
static void _onsignalingstatechangeCallback(PeerConnectionCtx* ctx);
static void _onaddstreamCallback(PeerConnectionCtx* ctx, struct __MediaStreamEvent* stream);
static void _onremovestreamCallback(PeerConnectionCtx* ctx, struct __MediaStreamEvent* stream);
static void _oniceconnectionstatechangeCallback(PeerConnectionCtx* ctx);
static void _onicecandidateCallback(PeerConnectionCtx* ctx, struct __RTCPeerConnectionIceEvent* e);
// static void _RTCStatsCallback(PeerConnectionCtx* ctx, struct __RTCStatsReport* report);
static void _ontonechangeCallback(PeerConnectionCtx* ctx, struct __RTCDTMFToneChangeEvent* e);
static void _ondatachannelCallback(PeerConnectionCtx* ctx, struct __RTCDataChannelEvent* e);
//static void _onmessageCallback(PeerConnectionCtx* ctx, struct __MessageEvent* e);


PeerConnection* RTCPeerConnection_Init(const RTCConfiguration* configuration, const MediaConstraintsObj* constraints)
{
    // Allocation
    PeerConnection* o = new PeerConnection();
    PeerConnection_Obj* obj = new PeerConnection_Obj();
    PeerConnectionCtx* ctx = new PeerConnectionCtx();

    if (0==o || 0==obj || 0==ctx)
        goto bail; // FIXME: 

    // Cyclic pointers
    ctx->o = o;
    ctx->obj = obj;
    o->ctx_ = ctx;
    obj->ctx_ = ctx;
    obj->rtc_.obj_ = obj;

    o->CreateOffer = RTCPeerConnection_CreateOffer;
    o->CreateAnswer = RTCPeerConnection_CreateAnswer;
    o->SetLocalDescription = RTCPeerConnection_SetLocalDescription;
    o->LocalDescription = RTCPeerConnection_LocalDescription;
    o->SetRemoteDescription = RTCPeerConnection_SetRemoteDescription;
    o->RemoteDescription = RTCPeerConnection_RemoteDescription;
    o->SignalingState = RTCPeerConnection_SignalingState;
    o->UpdateIce = RTCPeerConnection_UpdateIce;
    o->AddIceCandidate = RTCPeerConnection_AddIceCandidate;
    o->IceGatheringState = RTCPeerConnection_IceGatheringState;
    o->IceConnectionState = RTCPeerConnection_IceConnectionState;
    o->Close = RTCPeerConnection_Close;
    o->GetDataChannel = RTCPeerConnection_GetDataChannel;
    o->CreateDataChannel = RTCPeerConnection_CreateDataChannel;

    assert((void*)&obj->rtc_ == (void*)obj); 

    obj->rtc_.GetSocket();

    return o;

bail:
    if (0 != o)
        delete o;
    if (0 != obj)
        delete obj;
    if (0 != ctx)
        delete ctx;

    return 0;
}

void RTCPeerConnection_Destroy(PeerConnection* pc)
{
    if (0 == pc || 0 == pc->ctx_)
        return;

    PeerConnectionCtx* ctx = pc->ctx_;
    PeerConnection_Obj* obj = ctx->obj;

    if (obj) {
        WebRtcPeerConnection* pc = (WebRtcPeerConnection*)obj;
        pc->Shutdown();

        delete obj;
        obj = 0;
    }

    pc->CreateOffer = 0;
    pc->CreateAnswer = 0;
    pc->SetLocalDescription = 0;
    pc->LocalDescription = 0;
    pc->SetRemoteDescription = 0;
    pc->RemoteDescription = 0;
    pc->SignalingState = 0;
    pc->UpdateIce = 0;
    pc->AddIceCandidate = 0;
    pc->IceGatheringState = 0;
    pc->IceConnectionState = 0;
    pc->Close = 0;
    pc->CreateDataChannel = 0;

    delete ctx;
    ctx = 0;

    delete pc;
    pc = 0;
}

// PeerConnectionInterface
void RTCPeerConnection_CreateOffer(PeerConnectionCtx* ctx, RTCSessionDescriptionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback, const MediaConstraintsObj* constraints)
{
    static webrtc::FakeConstraints constraints_;
    constraints_.SetMandatoryReceiveAudio(false);
    constraints_.SetMandatoryReceiveVideo(false);
    constraints_.SetAllowRtpDataChannels();

    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *pc = obj->rtc_.GetSocket();

    rtc->_offerCallback = successCallback;
    rtc->_offerErrorCallback = failureCallback;

    if (constraints == NULL)
        pc->CreateOffer(&ctx->obj->rtc_, &constraints_);
}

void RTCPeerConnection_CreateAnswer(PeerConnectionCtx* ctx, RTCSessionDescriptionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback, const MediaConstraintsObj* constraints)
{
    static webrtc::FakeConstraints constraints_;
#if 0
    constraints_.SetMandatoryReceiveAudio(false);
    constraints_.SetMandatoryReceiveVideo(false);
    constraints_.SetAllowRtpDataChannels();
#endif

    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *pc = obj->rtc_.GetSocket();

    rtc->_answerCallback = successCallback;
    rtc->_answerErrorCallback = failureCallback;

    if (constraints == NULL)
        pc->CreateAnswer(rtc, &constraints_);
}

void RTCPeerConnection_SetLocalDescription(PeerConnectionCtx* ctx, const SessionDescription* description, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    webrtc::SessionDescriptionInterface* local = webrtc::CreateSessionDescription(description->type, description->sdp, NULL);
    p->SetLocalDescription(DummySetSessionDescriptionObserver::Create(ctx), local);
}

void RTCPeerConnection_SetRemoteDescription(PeerConnectionCtx* ctx, const SessionDescription* description, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    if (description == NULL)
        return;

    std::string sdp = description->sdp;
    std::string type = description->type;

#if 1
    rtc->AddPrAnswer(type, sdp);
#else
    webrtc::SessionDescriptionInterface* remote = webrtc::CreateSessionDescription(kRTCSdpType_answer, sdp.c_str(), NULL);
    p->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(ctx), remote);
#endif
}

const SessionDescription* RTCPeerConnection_LocalDescription(PeerConnectionCtx* ctx)
{
    if (0 == ctx || 0 == ctx->obj)
        return NULL;

    static char type[20], sdp[400]; // will memory management cheat work? XD
    WebRtcPeerConnection* w = (WebRtcPeerConnection*)ctx->obj;
    const webrtc::SessionDescriptionInterface* desc = w->GetSocket()->local_description();
    if (desc) {
        SessionDescription* c_desc = (SessionDescription*)malloc(sizeof(SessionDescription));
        std::string type = desc->type(), sdp, json = ::DescriptionToString(desc);
        desc->ToString(&sdp);
        c_desc->type = (char*)calloc(1, type.size() + 1);
        type.copy(c_desc->type, type.size());
        c_desc->sdp = (char*)calloc(1, sdp.size()+1);
        sdp.copy(c_desc->sdp, sdp.size());
        c_desc->json = NULL;

        return c_desc;
    }
    else
        return NULL;
}

const SessionDescription* RTCPeerConnection_RemoteDescription(PeerConnectionCtx* ctx)
{
    if (0 == ctx || 0 == ctx->obj)
        return NULL;

    static char type[20], sdp[400]; // will memory management cheat work? XD
    WebRtcPeerConnection* w = (WebRtcPeerConnection*)ctx->obj;
    const webrtc::SessionDescriptionInterface* desc = w->GetSocket()->remote_description();
    if (desc) {
        SessionDescription* c_desc = (SessionDescription*)malloc(sizeof(SessionDescription));
        std::string type = desc->type(), sdp, json = ::DescriptionToString(desc);
        desc->ToString(&sdp);
        c_desc->type = (char*)calloc(1, type.size() + 1);
        type.copy(c_desc->type, type.size());
        c_desc->sdp = (char*)calloc(1, sdp.size() + 1);
        sdp.copy(c_desc->sdp, sdp.size());
        c_desc->json = NULL;


        return c_desc;
    }
    else
        return NULL;
}
const char* RTCPeerConnection_SignalingState(PeerConnectionCtx* ctx)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    return "unknown";
}

int RTCPeerConnection_UpdateIce(PeerConnectionCtx* ctx, const RTCConfiguration* configuration, const MediaConstraintsObj* constraints)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    return -1;
}

int RTCPeerConnection_AddIceCandidate(PeerConnectionCtx* ctx, const RTCIceCandidate* candidate, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    return -1;
}

const char* RTCPeerConnection_IceGatheringState(PeerConnectionCtx* ctx)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    return "unknown";
}

const char* RTCPeerConnection_IceConnectionState(PeerConnectionCtx* ctx)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    return "unknown";
}

void RTCPeerConnection_Close(PeerConnectionCtx* ctx)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = ctx->obj->rtc_.GetSocket();

    rtc->Shutdown();
}

RTCDataChannel* RTCPeerConnection_GetDataChannel(PeerConnectionCtx* ctx, const char* label)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << label << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = rtc->GetSocket();

    if (rtc->GetDataChannel() == NULL)
        rtc->CreateDataChannel(label);

    return rtc->GetDataChannel();
}

RTCDataChannel* RTCPeerConnection_CreateDataChannel(PeerConnectionCtx* ctx, const char* label, const RTCDataChannelConfig* dict)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << label << std::endl;
    PeerConnection_Obj* obj = ctx->obj;
    WebRtcPeerConnection* rtc = (WebRtcPeerConnection*)obj;
    webrtc::PeerConnectionInterface *p = rtc->GetSocket();

    if (rtc->GetDataChannel() == NULL)
        rtc->CreateDataChannel(label);

    return rtc->GetDataChannel();
}

// Callback
void SetCallback_RTCPeerConnection_onnegotiationneeded(PeerConnectionCtx* ctx, onnegotiationneededCallback onnegotiationneeded){ ctx->obj->rtc_._onnegotiationneeded = onnegotiationneeded; }
void SetCallback_RTCPeerConnection_onsignalingstatechange(PeerConnectionCtx* ctx, onsignalingstatechangeCallback onsignalingstatechange) { ctx->obj->rtc_._onsignalingstatechange = onsignalingstatechange; }
void SetCallback_RTCPeerConnection_onaddstream(PeerConnectionCtx* ctx, onaddstreamCallback onaddstream) { ctx->obj->rtc_._onaddstream = onaddstream; }
void SetCallback_RTCPeerConnection_onremovestream(PeerConnectionCtx* ctx, onremovestreamCallback onremovestream) { ctx->obj->rtc_._onremovestream = onremovestream; }
void SetCallback_RTCPeerConnection_oniceconnectionstatechange(PeerConnectionCtx* ctx, oniceconnectionstatechangeCallback oniceconnectionstatechange) { ctx->obj->rtc_._oniceconnectionstatechange = oniceconnectionstatechange; }
void SetCallback_RTCPeerConnection_onicecandidate(PeerConnectionCtx* ctx, onicecandidateCallback onicecandidate) { ctx->obj->rtc_._onicecandidate = onicecandidate;  };
void SetCallback_RTCPeerConnection_ondatachannel(PeerConnectionCtx* ctx, ondatachannelCallback ondatachannel){ ctx->obj->rtc_._ondatachannel = ondatachannel;  };

static void _onnegotiationneededCallback(PeerConnectionCtx* ctx)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}
static void _onsignalingstatechangeCallback(PeerConnectionCtx* ctx)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}
static void _onaddstreamCallback(PeerConnectionCtx* ctx, struct __MediaStreamEvent* stream)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}
static void _onremovestreamCallback(PeerConnectionCtx* ctx, struct __MediaStreamEvent* stream)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}
static void _oniceconnectionstatechangeCallback(PeerConnectionCtx* ctx)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}
static void _onicecandidateCallback(PeerConnectionCtx* ctx, struct __RTCPeerConnectionIceEvent* e)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}
// static void _RTCStatsCallback(PeerConnectionCtx* ctx, struct __RTCStatsReport* report);
static void _ontonechangeCallback(PeerConnectionCtx* ctx, struct __RTCDTMFToneChangeEvent* e)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}
static void _ondatachannelCallback(PeerConnectionCtx* ctx, struct __RTCDataChannelEvent* e)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
    __RTCDataChannel *dc = e->channel;
}
//static void _onmessageCallback(PeerConnectionCtx* ctx, struct __MessageEvent* e);

// https://gist.github.com/sukinull/9328109
void WebRtcPeerConnection::OnDataChannel(webrtc::DataChannelInterface *data_channel)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
    RTCDataChannel *rtc_data_channel = RTCDataChannel_Init(data_channel);
    RTCDataChannelEvent* e = (RTCDataChannelEvent *)calloc(1, sizeof(RTCDataChannelEvent));
    e->channel = rtc_data_channel;
    if (_ondatachannel){
        _ondatachannel(obj_->ctx_, e);
    }
    printf("\n\non_datachannel\n\n");
}

void WebRtcPeerConnection::OnRenegotiationNeeded(void)
{
    LOG(LS_INFO) << __PRETTY_FUNCTION__;
}

RTCDataChannel* WebRtcPeerConnection::GetDataChannel() {
    LOG(LS_INFO) << __PRETTY_FUNCTION__;

    if (peer_connection_.get()) {
        return data_channel_;
    }

    return NULL;
}

webrtc::PeerConnectionInterface *WebRtcPeerConnection::GetSocket() {
    LOG(LS_INFO) << __PRETTY_FUNCTION__;

    if (!peer_connection_.get()) {
        InitConnection();
    }

    return peer_connection_.get();
}

class BlockingSignalingThread : public rtc::Thread {
    public:
        virtual void Run() {
            LOG(LS_INFO) << __PRETTY_FUNCTION__;

            rtc::Thread::SetAllowBlockingCalls(true);
            rtc::Thread::Run();
        }
};

WebRtcPeerConnection::WebRtcPeerConnection() :
    data_channel_(NULL),
    _worker_thread(NULL),
    _onsignalingstatechange(_onsignalingstatechangeCallback),
    _oniceconnectionstatechange(_oniceconnectionstatechangeCallback),
    _onicecandidate(_onicecandidateCallback),
    _ondatachannel(_ondatachannelCallback),
    _onnegotiationneeded(_onnegotiationneededCallback),
    _onaddstream(_onaddstreamCallback),
    _onremovestream(_onremovestreamCallback) {
        _signal_thread = new BlockingSignalingThread;//? rtc::ThreadManager::Instance()->CurrentThread();
        //? rtc::ThreadManager::Instance()->WrapCurrentThread();
        _worker_thread = new rtc::Thread;
        if (_worker_thread) {
            _worker_thread->Start();
        }

        if (_signal_thread) {
            _signal_thread->Start();
        }

        //TODO: works ==> peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(_worker_thread, _worker_thread, NULL, NULL, NULL);
        peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(_worker_thread, _signal_thread, NULL, NULL, NULL);
        //FIXME: Failed ==> peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();
        server_.uri = kStunServerUri;
        servers_.push_back(server_);
        constraints_.SetMandatoryReceiveAudio(false);
        constraints_.SetMandatoryReceiveVideo(false);
        //constraints_.SetAllowRtpDataChannels();
    }

WebRtcPeerConnection::~WebRtcPeerConnection()
{
    if (_worker_thread) {
        _worker_thread->Quit();
        _worker_thread->Stop();

        _worker_thread = NULL;
    }

    if (_signal_thread) {
        _signal_thread->Quit();
        _signal_thread->Stop();

        _signal_thread = NULL;
    }

    peer_connection_.release();
    peer_connection_factory_.release();
}

#if 0
rtc::Thread* WebRtcPeerConnection::GetWorkerThread()
{
    return _worker_thread;
}
#endif

bool WebRtcPeerConnection::InitConnection() {
    peer_connection_ = peer_connection_factory_->CreatePeerConnection(servers_, &constraints_, NULL, NULL, this);
#if 0
    // audio_track_ = peer_connection_factory_->CreateAudioTrack(kAudioLabel, NULL);
    stream_ = peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);
    //stream_->AddTrack(audio_track_);
    peer_connection_->AddStream(stream_);
#endif

    // SHOULD NOT DO THIS HERE, BUT FOR INTERNAL USAGE, WHY NOT? XD
    /* create datachannel */
    CreateDataChannel("demo");
#if 0
    RTCDataChannel *channel = GetDataChannel();
    data_channel_ = RTCDataChannel_Init(channel);
#endif

    //FIXME: data_channel_->onopenSet(onOpenCallback);
    return false; //AWARE: double check, remove CreateDataChannel
}

bool WebRtcPeerConnection::CreateDataChannel(const char* label) {
    rtc::scoped_refptr<webrtc::DataChannelInterface> dc = peer_connection_->CreateDataChannel(label, &dcConfig_);
    data_channel_ = RTCDataChannel_Init(dc.get());

    return false; // AWARE: double check
}


void WebRtcPeerConnection::Shutdown() {
    if (peer_connection_.get())
        peer_connection_->Close();

#if 0
    if (stream_.get()) {
        stream_->Release();
    }
#endif
    // Check Data
    if (data_channel_ != NULL /*&& recording_data_*/)
    {
        data_channel_->Close(data_channel_->ctx_);
        RTCDataChannel_Destroy(data_channel_);

        data_channel_ = NULL;
    }
}

void WebRtcPeerConnection::CreateOffer() {
    PeerConnection_Obj* obj = (PeerConnection_Obj*)this;

    peer_connection_->CreateOffer(this, &constraints_);
}

void WebRtcPeerConnection::CreateAnswer() {
    PeerConnection_Obj* obj = (PeerConnection_Obj*)this;

    peer_connection_->CreateAnswer(this, &constraints_);
}

void WebRtcPeerConnection::AddPrAnswer(const std::string& type, const std::string& sdp) {
    webrtc::SessionDescriptionInterface* session_description(webrtc::CreateSessionDescription(type, sdp, NULL));

    AddPrAnswer(session_description);
}

void WebRtcPeerConnection::AddPrAnswer(
        webrtc::SessionDescriptionInterface* desc) {
    PeerConnection_Obj* obj = (PeerConnection_Obj*)this;

    if (!desc)
        return;

    peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(obj->ctx_), desc);

    // NEXT_SHOULD: Execute following
    // this->CreateAnswer();
}

void WebRtcPeerConnection::AddCandidate(std::string& sdp, std::string& mid, int line) {
    rtc::scoped_ptr<webrtc::IceCandidateInterface> candidate(webrtc::CreateIceCandidate(mid, line, sdp, NULL));
    if (!candidate.get()) {
        LOG(LS_INFO) << __PRETTY_FUNCTION__ << "Failed to parse candidate" << std::endl;
        return;
    }
    if (!peer_connection_->AddIceCandidate(candidate.get())) {
        LOG(LS_INFO) << __PRETTY_FUNCTION__ << "Failed to add candidate" << std::endl;
        return;
    }

    return;
}

void WebRtcPeerConnection::OnError() {
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << "PEERCONNECTION ERROR" << std::endl;
    PeerConnection_Obj *obj = (PeerConnection_Obj*)this;
    PeerConnectionCtx *ctx = obj->ctx_;
}

void WebRtcPeerConnection::OnStateChange(
        webrtc::PeerConnectionObserver::StateType state_changed) {
    PeerConnection_Obj *obj = (PeerConnection_Obj*)this;
    PeerConnectionCtx *ctx = obj->ctx_;

}

void WebRtcPeerConnection::OnAddStream(
        webrtc::MediaStreamInterface* stream) {
    PeerConnection_Obj *obj = (PeerConnection_Obj*)this;
    PeerConnectionCtx *ctx = obj->ctx_;

}

void WebRtcPeerConnection::OnRemoveStream(
        webrtc::MediaStreamInterface* stream) {
    PeerConnection_Obj *obj = (PeerConnection_Obj*)this;
    PeerConnectionCtx *ctx = obj->ctx_;


}

void WebRtcPeerConnection::OnIceCandidate(
        const webrtc::IceCandidateInterface* candidate) {
    PeerConnection_Obj *obj = (PeerConnection_Obj*)this;
    PeerConnectionCtx *ctx = obj->ctx_;
    std::string c;

    candidate->ToString(&c);
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << "ICE ICE baby" << c << std::endl;

    struct __RTCPeerConnectionIceEvent* e = (RTCPeerConnectionIceEvent*)calloc(1, sizeof(RTCPeerConnectionIceEvent));
    //TODO: e->???
    _onicecandidate(ctx, e);
}

void WebRtcPeerConnection::OnSuccess(
        webrtc::SessionDescriptionInterface* desc) {
    PeerConnection_Obj *obj = (PeerConnection_Obj*)this;
    PeerConnectionCtx *ctx = obj->ctx_;

    if (desc->type().compare(kRTCSdpType_offer) == 0) {
#if 0
        if (obj->offer_successCallback) {
            SessionDescription *c_desc = (SessionDescription*)malloc(sizeof(SessionDescription));
            std::string sdp;
            desc->ToString(&sdp);
            c_desc->sdp = (char*)sdp.c_str();
            c_desc->type = (char*)desc->type().c_str();
            obj->offer_successCallback(ctx, c_desc);

            free(c_desc);
        }
#endif
        peer_connection_->SetLocalDescription(
                DummySetSessionDescriptionObserver::Create(ctx), desc);
    }
    else if (desc->type().compare(kRTCSdpType_answer) == 0) {
        peer_connection_->SetLocalDescription(
                DummySetSessionDescriptionObserver::Create(ctx), desc);
    }
    else {
        LOG(LS_INFO) << __PRETTY_FUNCTION__ << "WebRtcPeerConnection::OnSuccess ==> Others?" << std::endl;
    }
}

void WebRtcPeerConnection::OnFailure(const std::string &error) {
    LOG(LS_INFO) << __PRETTY_FUNCTION__ << error << std::endl;

    PeerConnection_Obj *obj = (PeerConnection_Obj*)this;
    PeerConnectionCtx *ctx = obj->ctx_;
}
static webrtc::SessionDescriptionInterface*
StringToDescription(const std::string string_desc) {
    Json::Reader reader;
    Json::Value jdesc;
    if (!reader.parse(string_desc, jdesc)) {
        LOG(WARNING) << "Unknown string desc " << string_desc;
        return NULL;
    }

    // replace with global constants
    std::string type = jdesc["type"].asString();
    std::string sdp = jdesc["sdp"].asString();

    return webrtc::CreateSessionDescription(type, sdp, NULL);
}

static std::string 
DescriptionToString(const webrtc::SessionDescriptionInterface* desc) {
    Json::FastWriter writer;
    Json::Value jdesc;
    jdesc["type"] = desc->type();
    std::string sdp;
    desc->ToString(&sdp);
    jdesc["sdp"] = sdp;

    return writer.write(jdesc);
}
