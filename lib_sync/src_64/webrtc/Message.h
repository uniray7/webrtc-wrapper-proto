#ifndef _MESSAGE_EVENT_H_
#define _MESSAGE_EVENT_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	struct __RTCDataChannel;
	struct __RTCIceCandidate;
	struct __MediaStream;
	// http://www.w3.org/TR/webrtc/#idl-def-RTCDTMFToneChangeEvent
	typedef struct __RTCDTMFToneChangeEvent {
		const char* tone; // readonly attribute DOMString tone;		
	} RTCDTMFToneChangeEvent;

	// http://www.w3.org/TR/webrtc/#idl-def-RTCDataChannelEvent
	typedef struct __RTCDataChannelEvent {
		struct __RTCDataChannel* channel; // readonly    attribute RTCDataChannel channel;
	} RTCDataChannelEvent;

	// http://www.w3.org/TR/webrtc/#event-datachannel-message
	typedef struct __MessageEvent {
		void* data;
		int binary;
	} MessageEvent;

	// http://www.w3.org/TR/webrtc/#idl-def-RTCPeerConnectionIceEvent
	typedef struct __RTCPeerConnectionIceEvent {
		struct __RTCIceCandidate* candidate;
	} RTCPeerConnectionIceEvent;

	// http://www.w3.org/TR/webrtc/#idl-def-MediaStreamEvent
	typedef struct __MediaStreamEvent {
		struct __MediaStream* stream;
	} MediaStreamEvent;

#ifdef __cplusplus
}
#endif

#endif
