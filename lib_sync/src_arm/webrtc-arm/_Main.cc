#include <stdlib.h>
#include <stdio.h>
//#include <tchar.h>
#include <assert.h>
#include <string.h>
#include "linux_preprocessor.h"
#include "Common_.hpp"


#include "Utils.h"
#include "RTCPeerConnection.h"
#include <iostream>
#include <fstream>
#include <string>

#ifdef WIN32
#include <Windows.h>
#endif
void MyRTCSessionDescriptionCallback(PeerConnectionCtx* ctx, SessionDescription* description)
{
	std::cout << "CallBack: MyRTCSessionDescriptionCallback" << std::endl;
}

void MyRTCPeerConnectionErrorCallback(PeerConnectionCtx* ctx)
{
	std::cerr << "Callback: MyRTCPeerConnectionErrorCallback" << std::endl;
}

static unsigned long CreatePeerConnection(void* lpParam)
{
	Libwebrtc_Init();

    #ifndef LOG
    std::cout<<"shit!!!"<<std::endl;
    #endif
	//WORK: WebRtcConnectionManager manager;
	PeerConnection* offerer = RTCPeerConnection_Init(NULL, NULL);
	PeerConnection* answerer = RTCPeerConnection_Init(NULL, NULL);

	while (1) {
		std::string input;
		std::cout << "Enter command:" << std::endl;
		std::cin >> input;

		if (input.compare("quit") == 0) 
			break;

		if (input.compare("o") == 0) {
			offerer->CreateOffer(offerer->ctx_, NULL, NULL, NULL);
		}
		else if (input.compare("a") == 0) {
			std::ofstream offerer_sdp("offer1.sdp");
			const SessionDescription* desc1 = offerer->LocalDescription(offerer->ctx_);
			std::cout << "offerer:" << std::endl;
			std::cout << "\tSDP[type]: " << desc1->type << std::endl;
			std::cout << "\tSDP[sdp]: " << desc1->sdp << std::endl;
			offerer_sdp << desc1->sdp;

			answerer->SetRemoteDescription(answerer->ctx_, desc1, NULL, NULL);
			answerer->CreateAnswer(answerer->ctx_, NULL, NULL, NULL);
		}
		else if (input.compare("pr") == 0) {
			//WORK: manager.CreateOffer();
			std::string sdp = "", line;
			std::ifstream answerer_sdp("offer1.sdp");
			while (getline(answerer_sdp, line)) {
				std::cout << line << std::endl;
				sdp += line + "\r\n";
			}

			const SessionDescription* description = answerer->LocalDescription(answerer->ctx_);
			offerer->SetRemoteDescription(offerer->ctx_, description, NULL, NULL);
		} 
		else if (input.compare("send") == 0) {
			RTCDataChannel* dc = offerer->CreateDataChannel(offerer->ctx_, "demo", NULL);
			Buffer* raw_ = (Buffer*)calloc(1, sizeof(Buffer));
			unsigned char b[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			raw_->raw = b;
			raw_->size = strlen((const char*)b);
			dc->Send(dc->ctx_, raw_, false); // 
		}

	};

	RTCPeerConnection_Destroy(offerer);
	RTCPeerConnection_Destroy(answerer);

	Libwebrtc_Dispose();

	exit(0);
}

int main(int argc, char* argv[])
{
	CreatePeerConnection(0);

	// getchar();

/*	
	MSG msg;
	BOOL gm;
	while ((gm = ::GetMessage(&msg, NULL, 0, 0)) != 0 && gm != -1) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
*/
	return 0;

bail:
	assert(0);
	return -1;
}


