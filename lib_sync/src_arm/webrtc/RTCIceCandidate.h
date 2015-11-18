#ifndef _RTC_ICE_CANDIDATE_H_
#define _RTC_ICE_CANDIDATE_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct __RTCIceCandidate{
		char* candidate;
		char* sdpMid;
		unsigned short sdpMLineIndex;
	} RTCIceCandidate;
#ifdef __cplusplus
}
#endif
#endif // _RTC_ICE_CANDIDATE_H_