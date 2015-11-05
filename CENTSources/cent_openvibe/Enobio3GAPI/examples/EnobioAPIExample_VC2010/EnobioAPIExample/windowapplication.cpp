#include <windows.h>
#include <string.h>
#include <stdio.h>

#include "Enobio3G.h"
#include "channeldata.h"
#include "StatusData.h"

#define ID_OPEN_CLOSE_ENOBIO_BUTTON 1
#define ID_START_STOP_EEG_BUTTON 2
#define ID_ENOBIO_DATA_EDIT 3
#define ID_ENOBIO_STATUS_EDIT 4

//
// User defined messages
//
#define WM_UPDATE_ENOBIODATA	(WM_USER + 0)
#define WM_UPDATE_STATUSDATA	(WM_USER + 1)

//
// Definition of the consumers to receive both data and status from Enobio
//
class EnobioDataConsumer : public IDataConsumer
{
public:
    void receiveData (const PData& data);

	void setWindowHandler(HWND hWnd) {_hWnd = hWnd;}
private:
	HWND _hWnd;
};

class EnobioStatusConsumer : public IDataConsumer
{
public:
    void receiveData (const PData& data);
	
	void setWindowHandler(HWND hWnd) {_hWnd = hWnd;}
private:
	HWND _hWnd;
};

//
// Implementation of the receiveData for both Data and Status consumers
// The execution of these methods happens in a thread created by the Enobio
// instance so accesing GUI resources might lead to a program crash
//
void EnobioDataConsumer::receiveData(const PData &data)
{
    // The EnobioData is destroyed after the execution of receiveData by
    // the caller
    ChannelData * pData = (ChannelData *)data.getData();
    // This memory should be deallocated when the message is attended
    TCHAR * strMsg = new TCHAR[200]; 
    sprintf_s(strMsg, 200, "%d %d %d %d %d %d %d %d %lld",
                                                   pData->data()[0],
                                                   pData->data()[1],
                                                   pData->data()[2],
                                                   pData->data()[3],
                                                   pData->data()[4],
                                                   pData->data()[5],
                                                   pData->data()[6],
                                                   pData->data()[7],
                                                   pData->timestamp());
	PostMessage(_hWnd, WM_UPDATE_ENOBIODATA, 0, (LPARAM)strMsg);
}

void EnobioStatusConsumer::receiveData(const PData &data)
{
    StatusData * pStatus = (StatusData *)data.getData();
    PostMessage(_hWnd, WM_UPDATE_STATUSDATA, (WPARAM)pStatus->getCode(), 0);
}


//
// Declaration of Enobio and consumers
//
Enobio3G enobio;
EnobioDataConsumer enobioDataConsumer;
EnobioStatusConsumer enobioStatusConsumer;


void appendToWindow (HWND hWnd, const char * message)
{
    int length;
    length = GetWindowTextLength(hWnd) + strlen(message) + 3;
    char * newMessage = new char[length];
    GetWindowText(hWnd, newMessage, length);
    strcat_s(newMessage, length, message);
    strcat_s(newMessage, length, "\r\n");
    SetWindowText(hWnd, newMessage);
    delete newMessage;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndOpenCloseButton;
    static HWND hwndStartStopButton;
    static HWND hwndDataEdit;
    static HWND hwndStatusEdit;
    static int len;
    static char text[30];
    char * message;

    switch(msg)
    {
    case WM_CREATE:
        hwndOpenCloseButton = CreateWindow(TEXT("button"), TEXT("Open Enobio"),
                                           WS_VISIBLE | WS_CHILD, 10, 10, 100, 25,
                                           hwnd, (HMENU) ID_OPEN_CLOSE_ENOBIO_BUTTON, NULL, NULL);
        CreateWindow(TEXT("static"), TEXT("Data"), WS_CHILD | WS_VISIBLE, 10, 40, 50, 25,
                     hwnd, (HMENU) 1, NULL, NULL);
        hwndStartStopButton = CreateWindow(TEXT("button"), TEXT("Start EEG"),
                                           WS_VISIBLE | WS_CHILD, 140, 10, 100, 25,
                                           hwnd, (HMENU) ID_START_STOP_EEG_BUTTON, NULL, NULL);
        hwndDataEdit = CreateWindow(TEXT("Edit"), NULL,
                                    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_LEFT |
                                    ES_MULTILINE | ES_AUTOVSCROLL,
                                    30, 60, 700, 30, hwnd, (HMENU) ID_ENOBIO_DATA_EDIT,
                                    NULL, NULL);
        CreateWindow(TEXT("static"), TEXT("Status"), WS_CHILD | WS_VISIBLE, 10, 90, 50, 25,
                     hwnd, (HMENU) 1, NULL, NULL);
        hwndStatusEdit = CreateWindow(TEXT("Edit"), NULL,
                                      WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_LEFT |
                                      ES_MULTILINE | ES_AUTOVSCROLL,
                                      30, 110, 700, 90, hwnd, (HMENU) ID_ENOBIO_STATUS_EDIT,
                                      NULL, NULL);
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            switch(LOWORD(wParam))
            {
            case 1: // open / close Enobio
                GetWindowText(hwndOpenCloseButton, text, 30);
                if (strcmp(text, "Open Enobio") == 0)
                {
                  unsigned char mac[6] = {0x6a, 0xfb, 0x4b, 0x80, 0x07, 0x00};
                  if (!enobio.openDevice(mac))
                  {
                    MessageBox(NULL, TEXT("Error opening Enobio. Check it is plugged to the computer"),
                      TEXT("EnobioAPI demo"), MB_OK);
                  }
                  else
                  {
                    SetWindowText(hwndOpenCloseButton, TEXT("Close Enobio"));
                  }
                }
                else
                {
                  enobio.closeDevice();
                  SetWindowText(hwndOpenCloseButton, TEXT("Open Enobio"));
                }
                break;
            case 2: // start / stop EEG streaming
              GetWindowText(hwndStartStopButton, text, 30);
              if (strcmp(text, "Start EEG") == 0)
              {
                enobio.startStreaming();
                SetWindowText(hwndStartStopButton, TEXT("Stop EEG"));
              }
              else
              {
                enobio.stopStreaming();
                SetWindowText(hwndStartStopButton, TEXT("Start EEG"));
              }
              break;
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
	break;

	case WM_UPDATE_ENOBIODATA: // new Enobio data received
        message = (char*)lParam;
        SetWindowText(hwndDataEdit, message);
        // The message shall be deallocated since it was dinamically created
        // when the data was received
        delete message;
		break;

	case WM_UPDATE_STATUSDATA: // new Status data received
        appendToWindow(hwndStatusEdit,
                       StatusData::getStringFromCode((StatusData::statusCode)wParam));
		break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow )
{
    MSG  msg ;    
    WNDCLASS wc = {0};
    wc.lpszClassName = TEXT( "Enobio API example" );
    wc.hInstance     = hInstance ;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc ;
    wc.hCursor       = LoadCursor(0,IDC_ARROW);
    
    RegisterClass(&wc);
    HWND hWnd = CreateWindow( wc.lpszClassName, TEXT("Enobio API example"),
                              WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                              220, 220, 750, 260, 0, 0, hInstance, 0);
    
    // The consumer shall knwo the handle of the main window in order to
    // notify that new data or status has been received
    enobioDataConsumer.setWindowHandler(hWnd);
    enobioStatusConsumer.setWindowHandler(hWnd);
    // The consumers are registereds into the Enobio processor in order to
    // receive both data and status
    enobio.registerConsumer(Enobio3G::ENOBIO_DATA, enobioDataConsumer);
    enobio.registerConsumer(Enobio3G::STATUS, enobioStatusConsumer);
    
    //enobio.setDemoMode(true, "demosignals.txt");

    while( GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}
