#ifndef _RTC_DATA_CHANNEL_H_
#define _RTC_DATA_CHANNEL_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    struct __RTCDataChannel;
    struct __RTCDataChannel_Obj;

#if 0
    typedef enum RTCDataChannelEvent {
        kDataChannelMessage = 0x1 << 0, // 1
        kDataChannelError = 0x1 << 1, // 2
        kDataChannelState
    } RTCDataChannelEvent;
#endif

    typedef struct __Buffer {
        unsigned char *raw;
        unsigned int size;
    } Buffer;

    typedef struct __RTCDataChannelCtx {
        struct __RTCDataChannel *o;
        struct __RTCDataChannel_Obj* obj;

        void* UserData;
    } RTCDataChannelCtx;

    typedef struct __RTCDataChannelConfig {
        char* label;
#if 0
        int/*int*/ ordered;  // readonly attribute boolean ordered;
        unsigned short maxRetransmitTime; // readonly attribute unsigned short ? maxRetransmitTime;
        unsigned short maxRetransmits;// readonly attribute unsigned short ? maxRetransmits;
        char* protocol; // readonly attribute DOMString protocol;
        int/*bool*/ negotiated;  // readonly attribute boolean negotiated;
        unsigned short id; // readonly attribute unsigned short ? id;
        char* readyState; // readonly attribute RTCDataChannelState readyState;
        unsigned long bufferedAmount; // readonly attribute unsigned long bufferedAmount;
        char* binaryType; // attribute DOMString binaryType;
#endif
    } RTCDataChannelConfig;

    typedef void(*onopenCallback)(struct __RTCDataChannel* dc, void* user_data);
    typedef void(*onmessageCallback)(Buffer *buf);


    typedef struct __RTCDataChannel {
        int(*Send)(RTCDataChannelCtx* ctx, const Buffer* data, bool binary/* = true*/); // void send(DOMString data); void send(Blob data); void send(ArrayBuffer data); void send(ArrayBufferView data);
        void(*Close)(RTCDataChannelCtx* ctx); // void close();

        // Library set
        const char* (*Label)(RTCDataChannelCtx* ctx);
        int(*BinaryType)(RTCDataChannelCtx* ctx);
        const char* (*ReadyState)(RTCDataChannelCtx* ctx);
        void(*SetBinaryType)(RTCDataChannelCtx* ctx, const char* binaryType);
        int(*ReadOnly)(RTCDataChannelCtx* ctx);

        RTCDataChannelCtx* ctx_;
    } RTCDataChannel;

    void SetCallback_RTCDataChannel_onmessage(RTCDataChannelCtx* ctx, onmessageCallback onmessage);
    void SetCallback_RTCDataChannel_onopen(RTCDataChannelCtx* ctx, onopenCallback onopen, void *user_data);
    
    typedef void(*VoidFunctionCallback)(RTCDataChannelCtx* ctx);
    typedef void(*StringFunctionCallback)(RTCDataChannelCtx* ctx, const char* string);

    // RTCDataChannel* RTCDataChannel_Init();
    // void RTCDataChannel_Destroy(RTCDataChannel* pc);

    int RTCDataChannel_Send(RTCDataChannelCtx* ctx, const Buffer* data, bool binary/* = true*/);
    void RTCDataChannel_Close(RTCDataChannelCtx* ctx);

#ifdef __cplusplus
}
#endif

#endif // _RTC_DATA_CHANNEL_H_
