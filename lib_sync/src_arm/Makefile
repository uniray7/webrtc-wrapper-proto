CC=arm-linux-gnueabihf-gcc
CXX=arm-linux-gnueabihf-g++
all: client 

build/arm_RTCPeerConnection.o: webrtc-arm/RTCPeerConnection.cpp webrtc-arm/RTCPeerConnection.h
	$(CXX) -march=armv7-a -I/home/uniray/Build/webrtc-arm/src -I/home/uniray/Build/webrtc-arm/src/webrtc \
    	-I/home/uniray/Build/webrtc-arm/src/third_party/jsoncpp/overrides/include \
    	-I/home/uniray/Build/webrtc-arm/src/third_party/jsoncpp/source/include \
    	-std=c++11 -fno-rtti -c webrtc-arm/RTCPeerConnection.cpp -o build/arm_RTCPeerConnection.o

build/arm_RTCDataChannel.o: webrtc-arm/RTCDataChannel.cpp webrtc-arm/RTCDataChannel.h
	$(CXX) -march=armv7-a -I/home/uniray/Build/webrtc-arm/src -I/home/uniray/Build/webrtc-arm/src/webrtc \
    	-I/home/uniray/Build/webrtc-arm/src/third_party/jsoncpp/overrides/include \
    	-I/home/uniray/Build/webrtc-arm/src/third_party/jsoncpp/source/include \
    	-std=c++11 -fno-rtti -c webrtc-arm/RTCDataChannel.cpp -o build/arm_RTCDataChannel.o

build/arm_Utils.o: webrtc-arm/Utils.cpp webrtc-arm/Utils.h
	$(CXX) -march=armv7-a -I/home/uniray/Build/webrtc-arm/src -I/home/uniray/Build/webrtc-arm/src/webrtc \
    	-I/home/uniray/Build/webrtc-arm/src/third_party/jsoncpp/overrides/include \
    	-I/home/uniray/Build/webrtc-arm/src/third_party/jsoncpp/source/include \
    	-std=c++11 -fno-rtti -Wno-unused-result -c webrtc-arm/Utils.cpp -o build/arm_Utils.o

build/sy_lwst.o: sy_lwst.c sy_lwst.h common.h
	$(CC) -march=armv7-a -I/usr/local/include -I/home/uniray/Build/webrtc-arm/rootfs/usr/include -c sy_lwst.c -o build/sy_lwst.o

build/client.o:cli/client.c common.h
	$(CC) -march=armv7-a -DDEBUG_SY -Iwebrtc-arm/ -I/usr/local/include \
		-I/home/uniray/Build/webrtc-arm/rootfs/usr/include -c cli/client.c -o build/client.o

client: build/arm_RTCPeerConnection.o build/arm_RTCDataChannel.o build/arm_Utils.o build/client.o build/sy_lwst.o
	$(CXX) -march=armv7-a -DDEBUG_SY -o client build/client.o build/sy_lwst.o build/arm_RTCDataChannel.o build/arm_RTCPeerConnection.o build/arm_Utils.o \
	-ldl -lpthread -lrt -fno-rtti -funwind-tables -gsplit-dwarf -Wno-undefined-bool-conversion -Wno-tautological-undefined-compare -fno-exceptions -fvisibility-inlines-hidden -std=gnu++11 -Wno-deprecated \
    -Wl,-z,now -Wl,-z,relro -Wl,--fatal-warnings -Wl,-z,noexecstack -fPIC \
	-Wl,--disable-new-dtags \
    -Wl,--start-group \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/jsoncpp/libjsoncpp.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/talk/libjingle_peerconnection.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/talk/libjingle_media.a \
    /home/uniray/Build/webrtc-arm/src/chromium/src/build/linux/debian_wheezy_arm-sysroot/usr/lib/arm-linux-gnueabihf/libexpat.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/base/librtc_base.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/libwebrtc_common.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/base/librtc_base_approved.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/boringssl/libboringssl.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/libyuv.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/libjpeg_turbo/libjpeg_turbo.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/usrsctp/libusrsctplib.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libvideo_render_module.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libwebrtc_utility.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudio_coding_module.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/common_audio/libcommon_audio.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/system_wrappers/libsystem_wrappers.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/openmax_dl/dl/libopenmax_dl.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudio_encoder_interface.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libg711.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libg722.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libilbc.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libisac.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libpcm16b.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libcng.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudio_decoder_interface.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libred.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libwebrtc_opus.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/opus/libopus.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libneteq.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libmedia_file.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/common_video/libcommon_video.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/libwebrtc.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/librtp_rtcp.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libpaced_sender.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libremote_bitrate_estimator.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libbitrate_controller.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libvideo_capture_module.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libwebrtc_video_coding.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libwebrtc_i420.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/video_coding/utility/libvideo_coding_utility.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/video_coding/codecs/vp8/libwebrtc_vp8.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/libvpx_new/libvpx_new.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/video_coding/codecs/vp9/libwebrtc_vp9.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libvideo_processing.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/voice_engine/libvoice_engine.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudio_conference_mixer.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudio_processing.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudioproc_debug_proto.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/protobuf/libprotobuf_lite.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudio_device.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/system_wrappers/libsystem_wrappers.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/sound/librtc_sound.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/system_wrappers/libfield_trial_default.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/system_wrappers/libmetrics_default.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/libjingle/xmllite/librtc_xmllite.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/libjingle/xmpp/librtc_xmpp.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/p2p/librtc_p2p.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libvideo_capture_module_internal_impl.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libvideo_render_module_internal_impl.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/talk/libjingle_p2p.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/libsrtp/libsrtp.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libwebrtc_h264.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libdesktop_capture.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libisac_common.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libisac_neon.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/common_audio/libcommon_audio_neon.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libaudio_processing_neon.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/chromium/src/third_party/openmax_dl/dl/libopenmax_dl_neon.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/modules/libisac_fix.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/librtc_event_log.a \
    /home/uniray/Build/webrtc-arm/src/arm/out/Release/obj/webrtc/librtc_event_log_proto.a \
    /home/uniray/Build/webrtc-arm/rootfs/usr/lib/arm-linux-gnueabihf/libX11.a \
    /home/uniray/Build/webrtc-arm/rootfs/usr/lib/arm-linux-gnueabihf/libxcb.a \
    /home/uniray/Build/webrtc-arm/rootfs/usr/lib/arm-linux-gnueabihf/libXau.a \
    /home/uniray/Build/webrtc-arm/rootfs/usr/lib/arm-linux-gnueabihf/libXdmcp.a \
	-Wl,--end-group \
	-L/home/uniray/Build/webrtc-arm/rootfs/usr/local/lib -luv -lwebsockets -ljansson -lz \
	-L/home/uniray/Build/webrtc-arm/rootfs/usr/local/ssl/lib -lssl -lcrypto 
clean:
	rm file_server client metadata_server
