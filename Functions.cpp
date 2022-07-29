#define _WIN32_WINNT 0x501
#define _CRT_SECURE_NO_WARNINGS
//---------------------------------------------------------Main Functions Module----------------------------------------------------------//

#include "Socket Messenger.h"

/*
    char Buffer[10];
    DWORD err = WSAGetLastError();            //Error-Checking Code
    itoa(err, Buffer, 10);
    SetWindowTextA(hDisplay, Buffer);
    return;
*/

SOCKET sInSock, sOutSock, sIncoming;
char* szTargetAddress = nullptr;
//char Target[15] = {}; // Maximum length of an IP Address


void Initialize(HWND hWnd)
{
    //-----------------------------------Initialize User Interface-------------------------------------------------//

	InitializeUI(hWnd);	
	
    //-------------------------------------Initialize WinSock---------------------------------------------------//

    WSADATA wData;

    if (WSAStartup(MAKEWORD(2, 2), &wData))
    {
        MessageBoxA(NULL, "Failed To Initialize WinSock!", "Error", MB_OK | MB_ICONERROR);
		SendMessageA(hWnd, WM_QUIT, NULL, NULL);
    }

    //-----------------------------------Create Server Thread---------------------------------------------------//
	
    if (!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecieveTextMessage, NULL, 0, 0))
    {
        MessageBoxA(NULL, "Failed To Start Server Thread!", "Error", MB_OK | MB_ICONERROR);
        SendMessageA(hWnd, WM_QUIT, NULL, NULL);
    }
	

	//---------------------------------------Initialize COM----------------------------------------------------//
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if(FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to Initialize COM!", "Error", MB_OK | MB_ICONERROR);
		SendMessageA(hWnd, WM_QUIT, NULL, NULL);
	}


}

void AppendWindowText(HWND hDisplay, LPCSTR Text)
{
	int idx = GetWindowTextLength(hMessage);
    SendMessage(hDisplay, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
    SendMessage(hDisplay, EM_REPLACESEL, 0, (LPARAM)Text);
}


void SendTextMessage()
{
    if (!InternetGetConnectedState(NULL, 0))
    {
        MessageBoxA(NULL, "No Internet!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
	
	int nAddrSize = GetWindowTextLengthA(hAddressText);
	nAddrSize++;
	GetWindowTextA(hAddressText, szTargetAddress, nAddrSize);
	

    struct addrinfo* pAddress = NULL;
	/*
    struct addrinfo hints;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
	*/
	
	if (getaddrinfo("192.168.0.14", "6007", NULL, &pAddress)) //&hints //192.168.0.15 szTargetAddress Hello Kebby!
    {
        MessageBoxA(NULL, "Address Conversion Failed!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    sOutSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sOutSock)
    {
        MessageBoxA(NULL, "Failed to Create Socket!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    
			/*
            The bind function can be called here to specify the outgoing port number. This can be useful
            in preventing the firewall from blocking the outgoing connection from the port randomly assigned
            by the operating system.
            To do this, create and fill a sockaddr_in struct, including the desired port number, and use that
            to bind the socket.

            struct sockaddr_in sLocalPort;
            sLocalPort.sin_addr.S_addr = inet_addr("192.168.0.12");
            sLocalPort.sin_family = AF_INET;
            sLocalPort.sin_port = htons(5007);

            if (bind(sOutSock, sLocalPort.sin_addr, sizeof(sLocalPort.sin_addr)))
            {
                MessageBoxA(NULL, "Failed to Establish Outgoing Port!", "Error", MB_OK | MB_ICONERROR);
                return;
            }
			*/

    

    if (connect(sOutSock, pAddress->ai_addr, (int)pAddress->ai_addrlen))
    {
        MessageBoxA(NULL, "Failed to Connect to Target!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
	

	/*
    int nBufferSize = GetWindowTextLengthA(hMessage);
    nBufferSize++;
    char* pbuffer = new char[nBufferSize];
	GetWindowTextA(hMessage, pbuffer, nBufferSize);
	*/

	//SetWindowTextA(hDisplay, ">>>");
	//AppendWindowText(hDisplay, ">>>");
	//AppendWindowText(hDisplay, pbuffer);
	//SetWindowTextA(hDisplay, pbuffer);

	int nBufferSize = GetWindowTextLengthA(hMessage);
	int nCarriage = sizeof("\r\n");
	nBufferSize += nCarriage;
    nBufferSize++;
    char* pbuffer = new char[nBufferSize];
    GetWindowTextA(hMessage, pbuffer, nBufferSize);
	strcat(pbuffer, "\r\n");

	int nTempBufferSize = GetWindowTextLengthA(hDisplay);
    nTempBufferSize++;
    char* ptempbuffer = new char[nTempBufferSize];
    GetWindowTextA(hDisplay, ptempbuffer, nTempBufferSize);

	strcat(ptempbuffer, pbuffer);

	//(hDisplay, ">>>");
	SetWindowTextA(hDisplay, ptempbuffer); //pbuffer);
	

	
    int nBytesSent = 0;
    int nBufferIndex = 0;
    while (nBytesSent < (nBufferSize - 1))
    {
        nBytesSent = send(sOutSock, &pbuffer[nBufferIndex], nBufferSize, NULL);
        nBufferIndex += nBytesSent;
    }
    closesocket(sOutSock);
	

	//Cleanup

	//delete pbuffer;
	//delete ptempbuffer;

}


void RecieveTextMessage()
{
    struct addrinfo* pLocalInfo = NULL;
	struct addrinfo hints = {0};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, "6007", &hints, &pLocalInfo)) //&hints
    {
        MessageBoxA(NULL, "Failed to Resolve Local IP Address!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    sInSock = socket(pLocalInfo -> ai_family, pLocalInfo -> ai_socktype, pLocalInfo -> ai_protocol);
    if (sInSock == INVALID_SOCKET)
    {
        MessageBoxA(NULL, "Failed to Create Incoming Socket!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    if(bind(sInSock, pLocalInfo -> ai_addr, (int)pLocalInfo -> ai_addrlen))
    {
        MessageBoxA(NULL, "Failed to Bind Socket!", "Error", MB_OK | MB_ICONERROR);
        closesocket(sInSock);
        return;
    }


    char pInBuffer[1024];
    while(1)
    {

        if(listen(sInSock, SOMAXCONN))
        {
            MessageBoxA(NULL, "Cannot Listen to Socket!", "Error", MB_OK | MB_ICONERROR);
            closesocket(sInSock);
            break;
        }
		//MessageBoxA(NULL, "Listening Now!", "Confirmation", MB_OK | MB_ICONINFORMATION);

        sIncoming = accept(sInSock, NULL, NULL);
        if(!sIncoming)
        {
            MessageBoxA(NULL, "Target cannot connect to Socket!", "Error", MB_OK | MB_ICONERROR);
            closesocket(sInSock);
            break;
        }

        while(1)
        {
            ZeroMemory(pInBuffer, sizeof(pInBuffer));
            if( recv(sIncoming, pInBuffer, sizeof(pInBuffer), 0) == 0)
            {
                break;
            }

			int nTempBufferSize = GetWindowTextLengthA(hDisplay);
			nTempBufferSize++;
			char* ptempbuffer = new char[nTempBufferSize];
			GetWindowTextA(hDisplay, ptempbuffer, nTempBufferSize);

			strcat(pInBuffer, "\r\n");
			strcat(ptempbuffer, pInBuffer);

            SetWindowTextA(hDisplay, (LPCSTR)pInBuffer);
			
        }

        closesocket(sIncoming);

    }

    //closesocket(sIncoming);

}



void CleanUp()
{
		closesocket(sOutSock);
		closesocket(sInSock);
		WSACleanup();
}