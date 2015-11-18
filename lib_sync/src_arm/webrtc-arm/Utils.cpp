#include "linux_preprocessor.h"
#include "Common_.hpp"
#include "Utils.h"

#include "talk/app/webrtc/peerconnectionfactoryproxy.h"
#include "talk/app/webrtc/proxy.h"

/*
* The MIT License (MIT)
*
* Copyright (c) 2015 vmolsa <ville.molsa@gmail.com> (http://github.com/vmolsa)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

//FIXME: missing cpumonitor.h/cc
#if 0
#include "webrtc/base/cpumonitor.h"
#endif

#ifdef LIBUV
using namespace v8;
#endif

rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> CreateFactory();
webrtc::PeerConnectionFactoryInterface* GetFactory();
cricket::DeviceManagerInterface* GetManager();

class BlockingThread : public rtc::Thread {
public:
	virtual void Run() {
		LOG(LS_INFO) << __PRETTY_FUNCTION__;

		rtc::Thread::SetAllowBlockingCalls(true);
		rtc::Thread::Run();
	}
};

static rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _factory;
static rtc::scoped_ptr<cricket::DeviceManagerInterface> _manager;
BlockingThread* _signal;
BlockingThread* _worker;
#ifdef _WIN32
rtc::Win32Thread worker;
#endif

void Libwebrtc_Init() {
#ifdef _WIN32
	rtc::EnsureWinsockInit();
	worker.Start();
#endif

	// WebRTC::Core::Init
	LOG(LS_INFO) << __PRETTY_FUNCTION__;
	rtc::InitializeSSL();

	_signal = new BlockingThread();
	_signal->Start();
    _worker = new BlockingThread();
    _worker->Start();
	// rtc::ThreadManager::Instance()->SetCurrentThread(_signal);

	_factory = ::CreateFactory();
	_manager.reset(cricket::DeviceManagerFactory::Create());

	if (!_manager->Init()) {
		_manager.release();
	}

}

void Libwebrtc_Dispose() {
	LOG(LS_INFO) << __PRETTY_FUNCTION__;

#ifdef _WIN32
	worker.Quit();
	worker.Stop();
#endif

	// WebRTC::Core::Dispose(); 
#ifdef LIBUV
	NanLowMemoryNotification();
#endif

	_factory.release();
	if (_manager.get()) {
		_manager->Terminate();
	}

	_manager.release();

	_signal->Stop();
	delete _signal;
}

rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> CreateFactory()
{
	//TODO: works ==> peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(_worker_thread, _worker_thread, NULL, NULL, NULL);
	return webrtc::CreatePeerConnectionFactory(_signal, _signal, NULL, NULL, NULL); 
}

webrtc::PeerConnectionFactoryInterface* GetFactory() {
	LOG(LS_INFO) << __PRETTY_FUNCTION__;

	return _factory.get();
}

cricket::DeviceManagerInterface* GetManager() {
	LOG(LS_INFO) << __PRETTY_FUNCTION__;

	return _manager.get();
}
