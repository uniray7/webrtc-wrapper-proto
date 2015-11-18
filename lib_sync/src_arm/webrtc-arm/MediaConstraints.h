#ifndef _MEDIA_CONSTRAINTS_H_
#define _MEDIA_CONSTRAINTS_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	// http://tools.ietf.org/html/draft-burnett-rtcweb-constraints-registry-05
	typedef struct __MediaConstraintsObj {
		void* optional;
		void*  mandatory;
	} MediaConstraintsObj;

	MediaConstraintsObj* MediaConstraintsObj_Init();
	int MediaConstraintsObj_Destroy(MediaConstraintsObj* obj);

	bool MediaConstraintsObj_UseAudio(MediaConstraintsObj* obj);
	bool MediaConstraintsObj_UseVideo(MediaConstraintsObj* obj);

	const char* MediaConstraintsObj_AudioId(MediaConstraintsObj* obj);
	const char* MediaConstraintsObj_VideoId(MediaConstraintsObj* obj);

	
	bool MediaConstraintsObj_IsMandatory(MediaConstraintsObj* obj, const char* key);
	bool MediaConstraintsObj_GetMandatory(MediaConstraintsObj* obj, const char* key);
	void MediaConstraintsObj_RemoveMandatory(MediaConstraintsObj* obj, const char* key);
	void MediaConstraintsObj_AddMandatory(MediaConstraintsObj* obj, const char* key, const char* value);
	void MediaConstraintsObj_SetMandatory(MediaConstraintsObj* obj, const char* key, const char* value);

	bool MediaConstraintsObj_IsOptional(MediaConstraintsObj* obj, const char *key);
	bool MediaConstraintsObj_GetOptional(MediaConstraintsObj* obj, const char *key);
	void MediaConstraintsObj_RemoveOptional(MediaConstraintsObj* obj, const char *key);
	void MediaConstraintsObj_AddOptional(MediaConstraintsObj* obj, const char *key, const char *value);
	void MediaConstraintsObj_SetOptional(MediaConstraintsObj* obj, const char *key, const char *value);

#ifdef __cplusplus
}
#endif
#endif // _MEDIA_CONSTRAINTS_H_