#ifndef _RTC_CONFIGURATION_H_
#define _RTC_CONFIGURATION_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct __RTCIceServer {
		char* uri;
		char* username;
		char* password;
	} RTCIceServer;

	typedef struct __RTCConfiguration {
		RTCIceServer iceServers[20];
	} RTCConfiguration;

#ifdef __cplusplus
}
#endif
#endif // _RTC_CONFIGURATION_H_