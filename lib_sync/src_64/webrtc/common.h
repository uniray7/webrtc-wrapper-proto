#ifndef _COMMON_H_
#define _COMMON_H_
#pragma once

#ifdef _MSC_VER
#	if !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#	endif
#	define LIBWEBRTC_INLINE	_inline
#	define LIBWEBRTC_stricmp	_stricmp
#	ifndef __STDC_LIMIT_MACROS
#		define __STDC_LIMIT_MACROS
#	endif
#else
#	define LIBWEBRTC_INLINE	inline
#	define LIBWEBRTC_stricmp	stricmp
#endif

#if !defined(__GNUC__) && defined(LIBWEBRTC_EXPORTS)
# 	define LIBWEBRTC_API		__declspec(dllexport)
#	define LIBWEBRTC_EXTERN	extern __declspec(dllexport)
#elif !defined(__GNUC__) && !defined(LIBWEBRTC_IMPORTS_IGNORE)
# 	define LIBWEBRTC_API		__declspec(dllimport)
#	define LIBWEBRTC_EXTERN	__declspec(dllimport)
#else
#	define LIBWEBRTC_API
#	define LIBWEBRTC_EXTERN	extern
#endif

// http://www.w3.org/TR/webrtc/#idl-def-RTCSdpType
#define kRTCSdpType_offer		webrtc::SessionDescriptionInterface::kOffer
#define kRTCSdpType_pranswer	webrtc::SessionDescriptionInterface::kPrAnswer
#define kRTCSdpType_answer		webrtc::SessionDescriptionInterface::kAnswer

static const char kStunServerUri[] = "stun:stun.l.google.com:19302";
static const char kAudioLabel[] = "audio_label";
static const char kVideoLabel[] = "video_label";
static const char kStreamLabel[] = "stream_label";

// http://www.w3.org/TR/webrtc/#idl-def-RTCSignalingState
static const char kSignalingStateStable[] = "stable";
static const char kSignalingStateHaveLocalOffer[] = "have-local-offer";
static const char kSignalingStateHaveRemoteOffer[] = "have-remote-offer";
static const char kSignalingStateHaveLocalPrAnswer[] = "have-local-pranswer";
static const char kSignalingStateHaveRemotePrAnswer[] = "have-remote-pranswer";
static const char kSignalingStateClosed[] = "closed";

// http://www.w3.org/TR/webrtc/#idl-def-RTCIceGatheringState
static const char kIceGatheringStateNew[] = "new";
static const char kIceGatheringStateGathering[] = "gathering";
static const char kIceGatheringStateComplete[] = "complete";

// http://www.w3.org/TR/webrtc/#idl-def-RTCIceConnectionState
static const char kIceConnectionStateNew[] = "new";
static const char kIceConnectionStatChecking[] = "checking";
static const char kIceConnectionStatConnected[] = "connected";
static const char kIceConnectionStatCompleted[] = "completed";
static const char kIceConnectionStatFailed[] = "failed";
static const char kIceConnectionStatDisconnected[] = "disconnected";
static const char kIceConnectionStatClosed[] = "closed";

// http://www.w3.org/TR/mediacapture-streams/#idl-def-MediaStreamTrackState
static const char kMediaStreamTrackStateNew[] = "new";
static const char kMediaStreamTrackStateLive[] = "live";
static const char kMediaStreamTrackStateEnded[] = "ended";
static const char kMediaStreamTrackStateFailed[] = "failed";

// http://www.w3.org/TR/mediacapture-streams/#idl-def-SourceTypeEnum
static const char kSourceTypeEnumNone[] = "none";
static const char kSourceTypeEnumCamera[] = "camera";
static const char kSourceTypeEnumMicrophone[] = "microphone";

// http://www.w3.org/TR/mediacapture-streams/#idl-def-VideoFacingModeEnum
static const char kVideoFacingModeEnumUser[] = "user";
static const char kVideoFacingModeEnumEnvironment[] = "environment";
static const char kVideoFacingModeEnumLeft[] = "left";
static const char kVideoFacingModeEnumRight[] = "right";

// http://www.w3.org/TR/webrtc/#rtcstats-dictionary
static const char kRTCStatsTypeInboundRtp[] = "inbound-rtp";
static const char kRTCStatsTypeOutboundRtp[] = "outbound-rtp";

// http://www.w3.org/TR/webrtc/#idl-def-RTCDataChannelState
static const char kRTCDataChannelStateConnecting[] = "connecting";
static const char kRTCDataChannelStateOpen[] = "open";
static const char kRTCDataChannelStateClosing[] = "closing";
static const char kRTCDataChannelStateClosed[] = "closed";

#ifdef __cplusplus
#if __clang__
#   include <tr1/memory>
#   include <tr1/functional>

namespace cpp11 = std::tr1;
class {
public:
	template<typename T>
	operator cpp11::shared_ptr<T>() { return cpp11::shared_ptr<T>(); }
} nullPtr;
#elif _MSC_VER || __llvm__ || 1
#   include <mutex>
namespace cpp11 = std;
#   define nullPtr nullptr
#else
#	error "Not implemented"
#endif
#endif

#endif