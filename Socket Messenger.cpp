// Socket Messenger.cpp : Defines the entry point for the application.
//

#pragma comment (lib, "wininet.Lib")
#pragma comment (lib, "WSock32.Lib")
#pragma comment (lib, "WS2_32.Lib")
#pragma comment (lib, "ComCtl32.Lib")
//#pragma comment (lib, "comsuppw.lib")
#pragma comment (lib, "strmiids.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "Socket Messenger.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hMainWindow;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    const wchar_t CLASS_NAME[] = L"SockClass";

    WNDCLASS wc = {};
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;
	//wc.hIcon = LoadIconA(hInstance, "C:\\Users\\opiyo\\Documents\\Dev Stash\\Win32 Stash\\Socket Messenger\\Socket Messenger\\Socket Messenger.ico");

    if (!RegisterClass(&wc))
    {
        MessageBoxW(NULL, L"Failed to Register Class!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }


    hMainWindow = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Socket Messenger",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, //WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hMainWindow)
    {
        MessageBoxW(NULL, L"Failed to Create Window!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

	InitializeUI(hMainWindow);

    ShowWindow(hMainWindow, nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, hMainWindow, NULL, NULL) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
		case WM_PAINT:
		{
			PAINTSTRUCT ps = {};
			HDC hdc = BeginPaint(hWnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hWnd, &ps);
		}
		break;

		case WM_CREATE:
		{
		  // Initialize(hWnd);
		}
		break;

		case WM_COMMAND:
		{
			switch (wParam)
			{
				case BTN_SEND:
				{
					//SendTextMessage();
				}
				break;

				case FILE_MENU_EXIT:
				{
					if(MessageBoxA(NULL, "Are you Sure you Want to Close?", "Confirm", MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						SendMessageA(hWnd, WM_CLOSE, NULL, NULL);
					}
				}
				break;

				case MEDIA_MENU_SELECT:
				{
					MessageBoxW(NULL, L"Select Media!", L"Confirmation", MB_OK | MB_ICONINFORMATION);
				}
				break;

				case MODE_MENU_TEXT:
				{
					TextMessageMode(hWnd);
				}
				break;

				case MODE_MENU_AUDIO:
				{
					AudioCaptureMode(hWnd);
				}
				break;

				case MODE_MENU_VIDEO:
				{
					VideoCaptureMode(hWnd);
				}
				break;


				case START_AUDIO_CALL:
				{
					StartAudioCall();
					ToggleControlsState(START_AUDIO_CALL);
				}
				break;

				case END_AUDIO_CALL:
				{
					//EndAudioCall();
					ToggleControlsState(END_AUDIO_CALL);
				}
				break;

				case START_VIDEO_CALL:
				{
					//StartVideoCall();
					ToggleControlsState(START_VIDEO_CALL);
				}
				break;

				case END_VIDEO_CALL:
				{
					//EndVideoCall();
					ToggleControlsState(END_VIDEO_CALL);
				}
				break;

			}
		}
		break;

		case WM_CLOSE:
		{
			//CleanUp();
			DestroyWindow(hWnd);
		}
		break;

    }



    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

