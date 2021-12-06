// Processes.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Processes.h"

#define MAX_LOADSTRING 100
#define CMD_BUTTON_1 1001
#define CMD_BUTTON_2 1002
#define CMD_BUTTON_3 1003
#define CMD_BUTTON_4 1004
#define CMD_BUTTON_5 1005
#define CMD_BUTTON_6 1006



// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];    // the main window class name
HWND list;
STARTUPINFO notepad_sinfo;
PROCESS_INFORMATION notepad_pinfo;
STARTUPINFO mystat_sinfo;
PROCESS_INFORMATION mystat_pinfo;

bool notepad_running = false;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                NotepadStart();
void               NotepadClose();
void               OpenMyStat();
void               CloseMyStat();
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROCESSES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROCESSES));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROCESSES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+22);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROCESSES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        
        list = CreateWindowW(L"listbox", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL,
            100, 10, 300, 400, hWnd, 0, hInst, NULL);
        CreateWindowW(L"Static", L"Notepad", WS_VISIBLE | WS_CHILD | DT_CENTER | WS_BORDER,
            5, 10, 90, 23, hWnd, (HMENU)CMD_BUTTON_1, hInst, NULL);
        CreateWindowW(L"Button", L"Open", WS_VISIBLE | WS_CHILD | WS_BORDER,
            15, 40, 75, 23, hWnd, (HMENU)CMD_BUTTON_2, hInst, NULL);
        CreateWindowW(L"Button", L"Close", WS_VISIBLE | WS_CHILD | WS_BORDER,
            15, 70, 75, 23, hWnd, (HMENU)CMD_BUTTON_3, hInst, NULL);
        CreateWindowW(L"Static", L"MyStat", WS_VISIBLE | WS_CHILD | DT_CENTER | WS_BORDER,
            5, 100, 90, 23, hWnd, (HMENU)CMD_BUTTON_4, hInst, NULL);
        CreateWindowW(L"Button", L"Open", WS_VISIBLE | WS_CHILD | WS_BORDER,
            15, 130, 75, 23, hWnd, (HMENU)CMD_BUTTON_5, hInst, NULL);
        CreateWindowW(L"Button", L"Close", WS_VISIBLE | WS_CHILD | WS_BORDER,
            15, 160, 75, 23, hWnd, (HMENU)CMD_BUTTON_6, hInst, NULL);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case CMD_BUTTON_2:
                NotepadStart();
                break;
            case CMD_BUTTON_3:
                NotepadClose();
                break;
            case CMD_BUTTON_5:
                OpenMyStat();
                break;
            case CMD_BUTTON_6:
                CloseMyStat();
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                TerminateProcess(notepad_pinfo.hProcess, 0);
                DestroyWindow(hWnd);
                notepad_running = false;
                
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        notepad_running = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}







void  NotepadStart() {

   
    if (notepad_running == true)
    {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Notepad is already running!");

    }
    else{
    if (CreateProcessW(
        L"C:\\Windows\\notepad.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        L"C:\\Windows\\",
        &notepad_sinfo,
        &notepad_pinfo
    )) {
        notepad_running = true;
        WaitForSingleObject(notepad_pinfo.hProcess, 0);
      
        
        
        SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)L"Notepad started");
    }
    else {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Error!");
    }
}
}
void NotepadClose()
{
    notepad_running = false;
    WaitForSingleObject(notepad_pinfo.hProcess, 0);
    if (TerminateProcess(notepad_pinfo.hProcess, 0)) {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"Notepad closed");
    };
    CloseHandle(notepad_pinfo.hThread);
    CloseHandle(notepad_pinfo.hProcess);
}


void OpenMyStat()
{
    
    if (CreateProcessW(L"C:\\Users\\rezol\\AppData\\Local\\Programs\\Opera GX\\launcher.exe",
        (LPWSTR)L"\\ https://itstep.org", NULL, NULL, TRUE, IDLE_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &mystat_sinfo, &mystat_pinfo)) {

        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"chrome works");
        CreateThread(NULL, 0, , &mystat_sinfo, 0, NULL);
    }
    else {

        SendMessageW(hListbox, LB_ADDSTRING, 100, (LPARAM)L"chrome error");
    }
}

void CloseMyStat()
{
    WaitForSingleObject(mystat_pinfo.hProcess, 0);
    if (TerminateProcess(mystat_pinfo.hProcess, 0)) {
        SendMessageW(list, LB_ADDSTRING, 100, (LPARAM)L"MyStat closed");
    };
    CloseHandle(mystat_pinfo.hThread);
    CloseHandle(mystat_pinfo.hProcess);
}
