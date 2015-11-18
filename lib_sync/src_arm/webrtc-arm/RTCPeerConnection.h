#ifndef _RTC_PEER_CONNECTION_H_
#define _RTC_PEER_CONNECTION_H_
#pragma once

#include "RTCIceCandidate.h"
#include "RTCConfiguration.h"
#include "MediaConstraints.h"
#include "SessionDescription.h"
#include "RTCDataChannel.h"
#include "Message.h"

#ifdef __cplusplus
extern "C" {
#endif
    struct __PeerConnection;
    struct __PeerConnection_Obj;
    struct __RTCStatsReport;
    struct __RTCDTMFToneChangeEvent;
    struct __MessageEvent;

    typedef struct __PeerConnectionCtx { 
        struct __PeerConnection* o;
        struct __PeerConnection_Obj* obj;

        void* UserData;
    } PeerConnectionCtx;

#if 0
    typedef void(*VoidFunctionCallback)(PeerConnectionCtx* ctx);
    typedef void(*StringFunctionCallback)(PeerConnectionCtx* ctx, const char* string);
#endif

    // http://www.w3.org/TR/webrtc/#idl-def-RTCSessionDescriptionCallback
    typedef void(*RTCSessionDescriptionCallback)(PeerConnectionCtx* ctx, struct __SessionDescription* description);
    // http://www.w3.org/TR/webrtc/#idl-def-RTCPeerConnectionErrorCallback
    typedef void(*RTCPeerConnectionErrorCallback)(PeerConnectionCtx* ctx);

    // http://www.w3.org/TR/webrtc/#event-negotiation
    //WE: typedef cpp11::function<void()> _onnegotiationneededCallback;
    typedef void(*onnegotiationneededCallback)(PeerConnectionCtx* ctx);
    // http://www.w3.org/TR/webrtc/#event-icecandidate
    //WE: typedef cpp11::function<void(cpp11::shared_ptr<_RTCPeerConnectionIceEvent> e)> _onicecandidateCallback;
    typedef void(*onicecandidateCallback)(PeerConnectionCtx* ctx, struct __RTCPeerConnectionIceEvent* e);
    // http://www.w3.org/TR/webrtc/#event-signalingstatechange
    //WE: typedef cpp11::function<void()> _onsignalingstatechangeCallback;
    typedef void(*onsignalingstatechangeCallback)(PeerConnectionCtx* ctx);
    // http://www.w3.org/TR/webrtc/#event-mediastream-addstream
    //WE: typedef cpp11::function<void(cpp11::shared_ptr<_MediaStreamEvent> stream)> _onaddstreamCallback;
    typedef void(*onaddstreamCallback)(PeerConnectionCtx* ctx, struct __MediaStreamEvent* stream);
    // http://www.w3.org/TR/webrtc/#event-mediastream-removestream
    //WE: typedef cpp11::function<void(cpp11::shared_ptr<_MediaStreamEvent> stream)> _onremovestreamCallback;
    typedef void(*onremovestreamCallback)(PeerConnectionCtx* ctx, struct __MediaStreamEvent* stream);
    // http://www.w3.org/TR/webrtc/#event-iceconnectionstatechange
    //WE: typedef cpp11::function<void()> _oniceconnectionstatechangeCallback;
    typedef void(*oniceconnectionstatechangeCallback)(PeerConnectionCtx* ctx);
    // http://www.w3.org/TR/webrtc/#idl-def-RTCStatsCallback
    //WE: typedef cpp11::function<void(cpp11::shared_ptr<_RTCStatsReport> report)> _RTCStatsCallback;
    typedef void(*RTCStatsCallback)(PeerConnectionCtx* ctx, struct __RTCStatsReport* report);
    // http://www.w3.org/TR/webrtc/#idl-def-RTCDTMFToneChangeEvent
    //WE: typedef cpp11::function<void(cpp11::shared_ptr<_RTCDTMFToneChangeEvent> e)> _ontonechangeCallback;
    typedef void(*ontonechangeCallback)(PeerConnectionCtx* ctx, struct __RTCDTMFToneChangeEvent* e);
    // http://www.w3.org/TR/webrtc/#idl-def-RTCDataChannelEvent
    //WE: typedef cpp11::function<void(cpp11::shared_ptr<_RTCDataChannelEvent> e)> _ondatachannelCallback;
    typedef void(*ondatachannelCallback)(PeerConnectionCtx* ctx, struct __RTCDataChannelEvent* e);
    // http://www.w3.org/TR/webrtc/#event-datachannel-message
    //WE: typedef cpp11::function<void(cpp11::shared_ptr<_MessageEvent> e)> _onmessageCallback;
    //typedef void(*onmessageCallback)(PeerConnectionCtx* ctx, struct __MessageEvent* e);

    enum RTCPeerConnectionEvent {
        kPeerConnectionCreateClosed = 1,
        kPeerConnectionCreateOffer,
        kPeerConnectionCreateOfferError,
        kPeerConnectionCreateAnswer,
        kPeerConnectionCreateAnswerError,
        kPeerConnectionSetLocalDescription,
        kPeerConnectionSetLocalDescriptionError,
        kPeerConnectionSetRemoteDescription,
        kPeerConnectionSetRemoteDescriptionError,
        kPeerConnectionIceCandidate,
        kPeerConnectionSignalChange,
        kPeerConnectionIceChange,
        kPeerConnectionIceGathering,
        kPeerConnectionDataChannel,
        kPeerConnectionAddStream,
        kPeerConnectionRemoveStream,
        kPeerConnectionRenegotiation,
        kPeerConnectionStats
    };

    typedef struct __PeerConnection {
        void(*CreateOffer)(PeerConnectionCtx* ctx, RTCSessionDescriptionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback, const MediaConstraintsObj* constraints);
        void(*CreateAnswer)(PeerConnectionCtx* ctx, RTCSessionDescriptionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback, const MediaConstraintsObj* constraints);
        void(*SetLocalDescription)(PeerConnectionCtx* ctx, const SessionDescription* description, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback);
        const SessionDescription* (*LocalDescription)(PeerConnectionCtx* ctx);
        void(*SetRemoteDescription)(PeerConnectionCtx* ctx, const SessionDescription* description, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback);
        const SessionDescription* (*RemoteDescription)(PeerConnectionCtx* ctx);
        const char* (*SignalingState)(PeerConnectionCtx* ctx);
        int(*UpdateIce)(PeerConnectionCtx* ctx, const RTCConfiguration* configuration, const MediaConstraintsObj* constraints);
        int(*AddIceCandidate)(PeerConnectionCtx* ctx, const RTCIceCandidate* candidate, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback);
        const char* (*IceGatheringState)(PeerConnectionCtx* ctx);
        const char* (*IceConnectionState)(PeerConnectionCtx* ctx);

#if 0 //FIXME: impl
        cpp11::shared_ptr<_Sequence<_MediaStream> > (*GetLocalStreams)(PeerConnectionCtx* ctx);
        cpp11::shared_ptr<_Sequence<_MediaStream> > (*GetRemoteStreams)(PeerConnectionCtx* ctx);
        cpp11::shared_ptr<_MediaStream> (*GetStreamById)(PeerConnectionCtx* ctx, const char* streamId);
        int (*AddStream)(PeerConnectionCtx* ctx, _MediaStream* stream, const _MediaConstraintsObj* constraints = NULL);
        int (*RemoveStream)(PeerConnectionCtx* ctx, _MediaStream* stream);
#endif
        void(*Close)(PeerConnectionCtx* ctx);
#if 0 //FIXME: impl
        int (*GetStats)(PeerConnectionCtx* ctx, _MediaStreamTrackBase* selector = NULL, _RTCStatsCallback successCallback = nullPtr, RTCPeerConnectionErrorCallback failureCallback = nullPtr);
        cpp11::shared_ptr<_RTCDTMFSender> (*CreateDtmfSender)(PeerConnectionCtx* ctx, _MediaStreamTrack *track);
#endif
        RTCDataChannel* (*GetDataChannel)(PeerConnectionCtx* ctx, const char* label);
        RTCDataChannel* (*CreateDataChannel)(PeerConnectionCtx* ctx, const char* label, const RTCDataChannelConfig* dataChannelDict);

        PeerConnectionCtx* ctx_;
    } PeerConnection;

    PeerConnection* RTCPeerConnection_Init(const RTCConfiguration* configuration, const MediaConstraintsObj* constraints);
    void RTCPeerConnection_Destroy(PeerConnection* pc);

    void RTCPeerConnection_CreateOffer(PeerConnectionCtx* ctx, RTCSessionDescriptionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback, const MediaConstraintsObj* constraints);
    void RTCPeerConnection_CreateAnswer(PeerConnectionCtx* ctx, RTCSessionDescriptionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback, const MediaConstraintsObj* constraints);
    void RTCPeerConnection_SetLocalDescription(PeerConnectionCtx* ctx, const SessionDescription* description, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback);
    const SessionDescription* RTCPeerConnection_LocalDescription(PeerConnectionCtx* ctx);
    void RTCPeerConnection_SetRemoteDescription(PeerConnectionCtx* ctx, const SessionDescription* description, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback);
    const SessionDescription* RTCPeerConnection_RemoteDescription(PeerConnectionCtx* ctx);
    const char* RTCPeerConnection_SignalingState(PeerConnectionCtx* ctx);
    int RTCPeerConnection_UpdateIce(PeerConnectionCtx* ctx, const RTCConfiguration* configuration, const MediaConstraintsObj* constraints);
    int RTCPeerConnection_AddIceCandidate(PeerConnectionCtx* ctx, const RTCIceCandidate* candidate, VoidFunctionCallback successCallback, RTCPeerConnectionErrorCallback failureCallback);
    const char* RTCPeerConnection_IceGatheringState(PeerConnectionCtx* ctx);
    const char* RTCPeerConnection_IceConnectionState(PeerConnectionCtx* ctx);
#if 0 //FIXME: impl
    cpp11::shared_ptr<_Sequence<_MediaStream> > RTCPeerConnection_GetLocalStreams(PeerConnectionCtx* ctx);
    cpp11::shared_ptr<_Sequence<_MediaStream> > RTCPeerConnection_GetRemoteStreams(PeerConnectionCtx* ctx);
    cpp11::shared_ptr<_MediaStream> RTCPeerConnection_GetStreamById(PeerConnectionCtx* ctx, const char* streamId);
    int RTCPeerConnection_AddStream(PeerConnectionCtx* ctx, _MediaStream* stream, const _MediaConstraintsObj* constraints = NULL);
    int RTCPeerConnection_RemoveStream(PeerConnectionCtx* ctx, _MediaStream* stream);
#endif
    void RTCPeerConnection_Close(PeerConnectionCtx* ctx);
#if 0 //FIXME: impl
    int RTCPeerConnection_GetStats(PeerConnectionCtx* ctx, _MediaStreamTrackBase* selector = NULL, _RTCStatsCallback successCallback = nullPtr, RTCPeerConnectionErrorCallback failureCallback = nullPtr);
    cpp11::shared_ptr<_RTCDTMFSender> RTCPeerConnection_CreateDtmfSender(PeerConnectionCtx* ctx, _MediaStreamTrack *track);
#endif
    RTCDataChannel* RTCPeerConnection_GetDataChannel(PeerConnectionCtx* ctx, const char* label);
    RTCDataChannel* RTCPeerConnection_CreateDataChannel(PeerConnectionCtx* ctx, const char* label, const RTCDataChannelConfig* dict);

    void SetCallback_RTCPeerConnection_onnegotiationneeded(PeerConnectionCtx* ctx, onnegotiationneededCallback onnegotiationneeded);
    void SetCallback_RTCPeerConnection_onicecandidate(PeerConnectionCtx* ctx, onicecandidateCallback onicecandidate);
    void SetCallback_RTCPeerConnection_onsignalingstatechange(PeerConnectionCtx* ctx, onsignalingstatechangeCallback onsignalingstatechange);
    void SetCallback_RTCPeerConnection_onaddstream(PeerConnectionCtx* ctx, onaddstreamCallback onaddstream);
    void SetCallback_RTCPeerConnection_onremovestream(PeerConnectionCtx* ctx, onremovestreamCallback onremovestream);
    void SetCallback_RTCPeerConnection_oniceconnectionstatechange(PeerConnectionCtx* ctx, oniceconnectionstatechangeCallback oniceconnectionstatechange);
    void SetCallback_RTCPeerConnection_ondatachannel(PeerConnectionCtx* ctx, ondatachannelCallback ondatachannel);

#ifdef __cplusplus
}
#endif

#endif // _RTC_PEER_CONNECTION_H_



