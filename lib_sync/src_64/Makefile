all: client file_server 

build/sy_lwst.o: sy_lwst.c sy_lwst.h common.h
	gcc -c sy_lwst.c -o build/sy_lwst.o

build/client.o:cli/client.c common.h
	gcc -DDEBUG_SY -Iwebrtc/ -c cli/client.c -o build/client.o

client: build/client.o build/sy_lwst.o
	g++ -DDEBUG_SY -o client build/client.o build/sy_lwst.o webrtc/build/RTCDataChannel.o webrtc/build/RTCPeerConnection.o webrtc/build/Utils.o \
		-lX11 -ldl -lpthread -L/usr/local/lib -luv -lwebsockets -ljansson -fno-rtti -funwind-tables -gsplit-dwarf -Wno-undefined-bool-conversion -Wno-tautological-undefined-compare -fno-exceptions -fno-rtti -fvisibility-inlines-hidden -std=gnu++11 -Wno-deprecated \
    -Wl,-z,now -Wl,-z,relro -Wl,--fatal-warnings -Wl,-z,defs -pthread -Wl,-z,noexecstack -fPIC -fuse-ld=gold \
    -Wl,--disable-new-dtags -m64 -Wl,--detect-odr-violations -Wl,--icf=all \
    -Wl,--start-group \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/jsoncpp/libjsoncpp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/talk/libjingle_peerconnection.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/talk/libjingle_media.a \
    /home/uniray/Build/webrtc-checkout/src/chromium/src/third_party/perl/c/lib/libexpat.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/base/librtc_base.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libwebrtc_common.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/base/librtc_base_approved.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/boringssl/libboringssl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/libyuv.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libjpeg_turbo/libjpeg_turbo.a \
    /home/uniray/Build/webrtc-checkout/src/chromium/src/third_party/perl/c/lib/libjpeg.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/usrsctp/libusrsctplib.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_render_module.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_utility.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_coding_module.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/common_audio/libcommon_audio.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libsystem_wrappers.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/openmax_dl/dl/libopenmax_dl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/common_audio/libcommon_audio_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_encoder_interface.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libg711.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libg722.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libilbc.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libisac.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libpcm16b.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libcng.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_decoder_interface.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libred.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_opus.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/opus/libopus.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libneteq.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libmedia_file.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/common_video/libcommon_video.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libwebrtc.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/librtp_rtcp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libpaced_sender.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libremote_bitrate_estimator.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libbitrate_controller.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_capture_module.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_video_coding.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_i420.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/video_coding/utility/libvideo_coding_utility.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/video_coding/codecs/vp8/libwebrtc_vp8.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_mmx.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_ssse3.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_sse4_1.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_avx2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/video_coding/codecs/vp9/libwebrtc_vp9.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_processing.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_processing_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/voice_engine/libvoice_engine.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_conference_mixer.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_processing.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudioproc_debug_proto.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/protobuf/libprotobuf_lite.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_processing_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_device.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libsystem_wrappers.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/sound/librtc_sound.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libfield_trial_default.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libmetrics_default.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libjingle/xmllite/librtc_xmllite.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libjingle/xmpp/librtc_xmpp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/p2p/librtc_p2p.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_capture_module_internal_impl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_render_module_internal_impl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/talk/libjingle_p2p.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libsrtp/libsrtp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_h264.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libdesktop_capture.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libdesktop_capture_differ_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libisac_common.a \
    -Wl,--end-group 


build/file_server.o: fs/file_server.c 
	gcc -DDEBUG_FS -Iwebrtc/ -c fs/file_server.c -o build/file_server.o

file_server: build/file_server.o build/sy_lwst.o
	g++ -DDEBUG_SY -o file_server build/file_server.o build/sy_lwst.o webrtc/build/RTCDataChannel.o webrtc/build/RTCPeerConnection.o webrtc/build/Utils.o \
		-lX11 -ldl -lpthread -L/usr/local/lib -luv -lwebsockets -ljansson -fno-rtti -funwind-tables -gsplit-dwarf -Wno-undefined-bool-conversion -Wno-tautological-undefined-compare -fno-exceptions -fno-rtti -fvisibility-inlines-hidden -std=gnu++11 -Wno-deprecated \
    -Wl,-z,now -Wl,-z,relro -Wl,--fatal-warnings -Wl,-z,defs -pthread -Wl,-z,noexecstack -fPIC -fuse-ld=gold \
    -Wl,--disable-new-dtags -m64 -Wl,--detect-odr-violations -Wl,--icf=all \
    -Wl,--start-group \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/jsoncpp/libjsoncpp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/talk/libjingle_peerconnection.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/talk/libjingle_media.a \
    /home/uniray/Build/webrtc-checkout/src/chromium/src/third_party/perl/c/lib/libexpat.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/base/librtc_base.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libwebrtc_common.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/base/librtc_base_approved.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/boringssl/libboringssl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/libyuv.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libjpeg_turbo/libjpeg_turbo.a \
    /home/uniray/Build/webrtc-checkout/src/chromium/src/third_party/perl/c/lib/libjpeg.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/usrsctp/libusrsctplib.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_render_module.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_utility.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_coding_module.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/common_audio/libcommon_audio.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libsystem_wrappers.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/openmax_dl/dl/libopenmax_dl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/common_audio/libcommon_audio_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_encoder_interface.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libg711.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libg722.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libilbc.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libisac.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libpcm16b.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libcng.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_decoder_interface.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libred.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_opus.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/opus/libopus.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libneteq.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libmedia_file.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/common_video/libcommon_video.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libwebrtc.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/librtp_rtcp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libpaced_sender.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libremote_bitrate_estimator.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libbitrate_controller.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_capture_module.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_video_coding.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_i420.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/video_coding/utility/libvideo_coding_utility.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/video_coding/codecs/vp8/libwebrtc_vp8.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_mmx.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_ssse3.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_sse4_1.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_avx2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/video_coding/codecs/vp9/libwebrtc_vp9.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_processing.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_processing_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/voice_engine/libvoice_engine.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_conference_mixer.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_processing.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudioproc_debug_proto.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/protobuf/libprotobuf_lite.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_processing_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libaudio_device.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libsystem_wrappers.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/sound/librtc_sound.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libfield_trial_default.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/system_wrappers/libmetrics_default.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libjingle/xmllite/librtc_xmllite.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/libjingle/xmpp/librtc_xmpp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/p2p/librtc_p2p.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_capture_module_internal_impl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libvideo_render_module_internal_impl.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/talk/libjingle_p2p.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/chromium/src/third_party/libsrtp/libsrtp.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libwebrtc_h264.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libdesktop_capture.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libdesktop_capture_differ_sse2.a \
    /home/uniray/Build/webrtc-checkout/src/out/Release/obj/webrtc/modules/libisac_common.a \
    -Wl,--end-group 

clean:
	rm file_server client metadata_server
