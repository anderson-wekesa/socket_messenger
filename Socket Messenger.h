#pragma once

/*-----------------------------------Contains all Global Header Files, Functions, Constants, etc.------------------------------*/


#include <WinSock2.h>
#include <WS2tcpip.h>
#include <WinInet.h>
#include <Windows.h>
#include "resource.h"
//#include <Windows.h>
#include <CommCtrl.h>
#include <Richedit.h>



//Port Numbers Used by the Application
#define CONTROL_PORT "3007"
#define DATA_PORT "3773"

#define BTN_SEND 1

#define FILE_MENU_EXIT 2
#define MEDIA_MENU_SELECT 3
#define MODE_MENU_TEXT 4
#define MODE_MENU_AUDIO 5
#define MODE_MENU_VIDEO 6

#define START_AUDIO_CALL 7
#define END_AUDIO_CALL 8
#define START_VIDEO_CALL 9
#define END_VIDEO_CALL 10

extern HWND hAddressText, hDisplay, hMessage; // hBtnSend;


void Initialize(HWND hWnd);
void InitializeUI(HWND hWnd);
void SendTextMessage();
void RecieveTextMessage();

void ToggleControlsState(ULONG uMsg);
void TextMessageMode(HWND hWnd);
void AudioCaptureMode(HWND hWnd);
void VideoCaptureMode(HWND hWnd);
void CleanUp();

char* GetTargetAddress();
void Calling();
void StartAudioCall();
void StartVideoCall();



//------------------------TODO---------------------------//

//Make Target Text Blue in the Chat Log Control

