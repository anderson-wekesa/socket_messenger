/*-----------------------------------------Contains the Logic that Loads and Manages the User Interface-----------------------------------*/

#include "Socket Messenger.h"

BOOL CALLBACK ChildWndProc(HWND hChildWnd, LPARAM lChildParam);

HWND hAddressText, hDisplay, hMessage;
HMENU hMainMenu;
HWND hBtnAudioCall, hBtnVideoCall;

char* pszTargetAddress = (char*)malloc(20);

void InitializeUI(HWND hWnd)
{
	//--------------------------------------------------Create Menu----------------------------------------------------------------------//
	hMainMenu = CreateMenu();
		HMENU hFileMenu = CreateMenu();
		HMENU hMediaMenu = CreateMenu();
		HMENU hModeMenu = CreateMenu();

	//File Menu
	AppendMenuA(hMainMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
		AppendMenuA(hFileMenu, MF_STRING, (UINT_PTR)FILE_MENU_EXIT, "Exit");

	//Media Menu
	AppendMenuA(hMainMenu, MF_POPUP, (UINT_PTR)hMediaMenu, "Media");
		AppendMenuA(hMediaMenu, MF_STRING, (UINT_PTR)MEDIA_MENU_SELECT, "Select Media File");

	//Mode Menu
	AppendMenuA(hMainMenu, MF_POPUP, (UINT_PTR)hModeMenu, "Mode");
		AppendMenuA(hModeMenu, MF_STRING, (UINT_PTR)MODE_MENU_TEXT, "Text Message");
		AppendMenuA(hModeMenu, MF_STRING, (UINT_PTR)MODE_MENU_AUDIO, "Audio Message");
		//AppendMenuA(hModeMenu, MF_STRING, (UINT_PTR)MODE_MENU_CALL, "Audio Call");
		AppendMenuA(hModeMenu, MF_STRING, (UINT_PTR)MODE_MENU_VIDEO, "Video Call");

	SetMenu(hWnd, hMainMenu);

	//--------------------------------------------------Load Text Message Mode------------------------------------------------------------//

	TextMessageMode(hWnd);

}


void TextMessageMode(HWND hWnd)
{
	EnumChildWindows(hWnd, (WNDENUMPROC)ChildWndProc, NULL);

	CreateWindowExA(0, "Static", "Message To: ", WS_VISIBLE | WS_CHILD, 40, 20, 100, 20, hWnd, NULL, NULL, NULL);
	hAddressText = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD, 150, 20, 230, 20, hWnd, NULL, NULL, NULL);

	LoadLibrary(L"C:\\Windows\\System32\\riched20.dll");  //Manually? Really?

	hDisplay = CreateWindowEx(ES_SUNKEN, RICHEDIT_CLASS, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL, 
		150, 50, 800, 280, hWnd, NULL, NULL, NULL);
	CreateWindowExA(0, "Button", "Send Message", WS_VISIBLE | WS_BORDER | WS_CHILD, 820, 410, 135, 33, hWnd, (HMENU)BTN_SEND,
									NULL, NULL);

	hMessage = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD, 150, 350, 800, 40, hWnd, NULL, NULL, NULL);
}

void AudioCaptureMode(HWND hWnd)
{
	EnumChildWindows(hWnd, (WNDENUMPROC)ChildWndProc, NULL);

	CreateWindowExA(0, "Static", "Call To: ", WS_VISIBLE | WS_CHILD, 40, 20, 100, 20, hWnd, NULL, NULL, NULL);
	hAddressText = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD, 150, 20, 800, 20, hWnd, NULL, NULL, NULL);

	hBtnAudioCall = CreateWindowExA(0, "Button", "Call", WS_VISIBLE | WS_BORDER | WS_CHILD, 150, 410, 135, 33, hWnd, 
		(HMENU) START_AUDIO_CALL, NULL, NULL);
	CreateWindowExA(0, "Button", "End Call", WS_VISIBLE | WS_BORDER | WS_CHILD, 820, 410, 135, 33, hWnd, (HMENU) END_AUDIO_CALL,
									NULL, NULL);
}

void VideoCaptureMode(HWND hWnd)
{
	EnumChildWindows(hWnd, (WNDENUMPROC)ChildWndProc, NULL);

	CreateWindowExA(0, "Static", "Video Call To: ", WS_VISIBLE | WS_CHILD, 40, 20, 100, 20, hWnd, NULL, NULL, NULL);
	hAddressText = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD, 150, 20, 800, 20, hWnd, NULL, NULL, NULL);

	hBtnVideoCall = CreateWindowExA(0, "Button", "Call", WS_VISIBLE | WS_BORDER | WS_CHILD, 150, 410, 135, 33, hWnd, (HMENU) START_VIDEO_CALL,
									NULL, NULL);
	CreateWindowExA(0, "Button", "End Call", WS_VISIBLE | WS_BORDER | WS_CHILD, 820, 410, 135, 33, hWnd, (HMENU) END_VIDEO_CALL,
									NULL, NULL);

}


BOOL CALLBACK ChildWndProc(HWND hChildWnd, LPARAM lChildParam)
{
	if(hChildWnd != NULL)
	{
		DestroyWindow(hChildWnd);
		return true;
	}

	return false;
}

char* GetTargetAddress()
{
	GetWindowTextA(hAddressText, pszTargetAddress, 20);
	return pszTargetAddress;

}

//Greys Out Controls when Appropriate
void ToggleControlsState(ULONG uMsg)
{
	switch (uMsg)
	{
		case START_AUDIO_CALL:
		{
			EnableWindow(hAddressText, FALSE);
			EnableWindow(hBtnAudioCall, FALSE);
		}
		break;

		case END_AUDIO_CALL:
		{
			EnableWindow(hAddressText, TRUE);
			EnableWindow(hBtnAudioCall, TRUE);
		}
		break;

		case START_VIDEO_CALL:
		{
			EnableWindow(hAddressText, FALSE);
			EnableWindow(hBtnVideoCall, FALSE);
		}
		break;

		case END_VIDEO_CALL:
		{
			EnableWindow(hAddressText, TRUE);
			EnableWindow(hBtnVideoCall, TRUE);
		}
		break;
	}

}



void LoadTabContents()
{
	/*
	int nCurrTab = TabCtrl_GetCurSel(hTab);

		//Destroy Previous Window's UI. 
	EnumChildWindows(hTab, (WNDENUMPROC) ChildWndProc, NULL);

		//Load Current Window's UI
		switch (nCurrTab)	
		{
			case 0:		//Inbox
			{

			}
			break;

			case 1:		//New
			{
				CreateWindowExA(0, "Static", "Message To: ", WS_VISIBLE | WS_CHILD, 40, 20, 100, 20, hTab, NULL, NULL, NULL);
				hAddressText = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD, 150, 20, 230, 20, hTab, NULL, NULL, NULL);
				hDisplay = CreateWindowExA(0, "Edit", NULL, WS_VISIBLE | WS_BORDER | WS_CHILD | ES_MULTILINE, 150, 50, 800, 280, hTab,
                               NULL, NULL, NULL);
				CreateWindowExA(0, "Button", "Send Message", WS_VISIBLE | WS_BORDER | WS_CHILD, 820, 350, 135, 33, hTab, (HMENU)BTN_SEND,
                    NULL, NULL);

			}
			break;

			case 2:		//Sent
			{

			}
			break;

			case 3:		//Draft
			{

			}
			break;
		
		*/
}