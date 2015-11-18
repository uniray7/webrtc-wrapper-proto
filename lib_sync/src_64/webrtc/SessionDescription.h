#ifndef _SESSION_DESCRIPTION_H_
#define _SESSION_DESCRIPTION_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	enum RTCSdpType {
		offer,
		pranswer,
		answer
	};

	typedef struct __SessionDescription{
		char* sdp;
		// http://www.w3.org/TR/webrtc/#idl-def-RTCSdpType
		// #define kRTCSdpType_offer		"offer"
		// #define kRTCSdpType_pranswer	"pranswer"
		// #define kRTCSdpType_answer		"answer"
		char* type;

		char* json;
	} SessionDescription;

#ifdef __cplusplus
}
#endif
#endif // _SESSION_DESCRIPTION_H_
