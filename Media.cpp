#include "Socket Messenger.h"
#include <dshow.h>
#include <dshowasf.h>
//#include "netsink.h"

ICaptureGraphBuilder2* pCaptureBuilder = nullptr;
IGraphBuilder* pFilterGraph = nullptr;
IMediaControl* pMediaControl = nullptr;
ICreateDevEnum* pDeviceEnum = nullptr;
IEnumMoniker* pEnumMoniker = nullptr;
IMoniker* pMoniker = nullptr;
IBaseFilter* pMicrophone = nullptr, *pNetworkSink = nullptr;
//INetworkSink* pSinkInterface = nullptr;

struct addrinfo* pAudioAddrInfo = NULL;
SOCKET sAudioSocket;

void StartAudioCall()
{

	char* szAddress = GetTargetAddress();
	//----------------------------------------------Initiate Call Protocol------------------------------------------------------//

	/*---Call Protocol---*/
	// 1. Send RQSTCALL Message
	// 2. Wait for ACKCALL Message from Target (Timeout: <Choose a timeout>)
	// 3. Proceed with Call if Acknowledgement is Returned; "<Target> Unavailable" if not.
	/*------------------*/

	
	struct addrinfo* pAddress = NULL;
	
	if (getaddrinfo(szAddress, CONTROL_PORT, NULL, &pAddress)) //&hints //192.168.0.15 szTargetAddress Hello Kebby!
    {
        MessageBoxA(NULL, "Control Address Conversion Failed!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    SOCKET sProtoSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sProtoSock)
    {
        MessageBoxA(NULL, "Failed to Create Control Socket!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

	for(int nTries = 0; nTries < 30; nTries++)
	{
		if (connect(sProtoSock, pAddress->ai_addr, (int)pAddress->ai_addrlen))
		{
			continue;
			//MessageBoxA(NULL, "Failed to Connect to Target!", "Error", MB_OK | MB_ICONERROR);
			//return;
		}
		else
		{
			break;
		}
		Sleep(1000);
	}

	

	//----------------------------------------------Initiate Audio Session------------------------------------------------------//

	HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, 
		(LPVOID*)&pCaptureBuilder);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Capture Graph Builder!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&pFilterGraph);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Filter Graph Manager!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	pCaptureBuilder -> SetFiltergraph(pFilterGraph);



	hr = pFilterGraph -> QueryInterface(IID_IMediaControl, (LPVOID*)&pMediaControl);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Query for Media Controls!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}



	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (LPVOID*)&pDeviceEnum);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Device Enumerator!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	//Load Microphone Filter
	pDeviceEnum -> CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEnumMoniker, NULL);
	pEnumMoniker -> Next(1, &pMoniker, NULL);
	hr = pMoniker -> BindToObject(NULL, NULL, IID_IBaseFilter, (LPVOID*)&pMicrophone);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Start Microphone!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pFilterGraph -> AddFilter(pMicrophone, L"Microphone");
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Add Microphone to Filter Graph!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	/*
	//Load Network Sink
	CoCreateInstance(CLSID_NetworkSink, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)&pNetworkSink);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Instantiate Network Sink!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	hr = pNetworkSink -> QueryInterface(IID_INetworkSink, (LPVOID*)&pSinkInterface);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Query for Network Sink Interface!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	pSinkInterface -> TargetAddress("192.168.0.14");
	pSinkInterface -> TargetPort(_itoa_s());

	hr = pFilterGraph -> AddFilter(pNetworkSink, L"Network Sink");
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Add Network Sink to Filter Graph!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	//Connect Filters
	hr = pCaptureBuilder -> RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Audio, pMicrophone, NULL, pNetworkSink);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Connect Filters!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	*/

	//Run Graph
	hr = pMediaControl -> Run();
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Initiate Call!", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}




}



void StartVideoCall()
{

}
