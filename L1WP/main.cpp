#include <windows.h>
#include <ctime>
#include <cstring>
#include <cstdlib>

#define max(a,b) (a > b ? a : b)            //Cause it doesn't work by default when including other libraries

#define IDC_SUBMIT_BUTTON           101             // Button identifier
#define IDC_MAIN_EDIT	            102			    // Edit box identifier
#define IDC_SECOND_BUTTON           103
#define IDC_EXIT_BUTTON             104
#define IDC_OUTPUT_TEXT             105
#define IDC_CHANGE_FONT_BUTTON      106
#define IDC_CHANGE_SIZE_BUTTON      107
#define IDC_CHANGE_COLOR_BUTTON     108

LRESULT CALLBACK WinProc (HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "Parent Window";

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WinProc;
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    if (!RegisterClassEx (&wincl)) return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           "Lab nr. 1",
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           650,
           400,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL);

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    srand(time(NULL));

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}


LRESULT CALLBACK WinProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndExitButton, hwndSubmitButton, hwndInputText, hwndOutputText;
    static HWND hwndFontButton, hwndSizeButton, hwndColorButton;
    int iScreenW, iScreenH, iClientWidth, iClientHeight;
    static HGDIOBJ hfSize, phfSizes[4][3];
    static int cxChar, cyChar, iFont = 2, iSize = 1, iColor = 1;
    const char * szMove = "MOVE";
    static char szBuffer[256], szSubmitBuffer[512], szInputName[10], szOutputName[11], szAdjustments[12];
    PAINTSTRUCT ps;
    HDC         hdc;
    RECT rcWindow, rcClient;
    HBRUSH hbr;

    switch (message)
    {
        case WM_CREATE:
            cxChar = LOWORD (GetDialogBaseUnits ());
            cyChar = HIWORD (GetDialogBaseUnits ());

            iScreenW = GetSystemMetrics(SM_CXSCREEN);
            iScreenH = GetSystemMetrics(SM_CYSCREEN);

            GetClientRect(hwnd, &rcClient);
            iClientWidth = rcClient.right - rcClient.left;
            iClientHeight = rcClient.bottom - rcClient.top;

            hbr = CreateSolidBrush(RGB(0,255,0));

            hwndExitButton = CreateWindow( TEXT("button"),
                TEXT("Exit Button"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                iClientWidth - 130,
                iClientHeight - 80,
                100, 50,
                hwnd,
                (HMENU) IDC_EXIT_BUTTON,
                ((LPCREATESTRUCT) lParam)->hInstance,
                NULL);

            // Create an edit box
			hwndInputText = CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"Edit Box",
				WS_CHILD|WS_VISIBLE|
				ES_MULTILINE|ES_AUTOVSCROLL,
                iClientWidth * 1/10,
				iClientHeight * 1/10,
				iClientWidth * 55/100,
				iClientHeight * 3/10,
				hwnd,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL);

			phfSizes[0][0] = CreateFont(12,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Tahoma"));
			phfSizes[0][1] = CreateFont(14,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Tahoma"));
			phfSizes[0][2] = CreateFont(16,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Tahoma"));
			phfSizes[1][0] = CreateFont(12,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Comic Sans"));
			phfSizes[1][1] = CreateFont(14,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Comic Sans"));
			phfSizes[1][2] = CreateFont(16,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Comic Sans"));
			phfSizes[2][0] = CreateFont(12,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier"));
			phfSizes[2][1] = CreateFont(14,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier"));
			phfSizes[2][2] = CreateFont(16,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier"));
			phfSizes[3][0] = CreateFont(12,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Bookman Old Style"));
			phfSizes[3][1] = CreateFont(14,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Bookman Old Style"));
			phfSizes[3][2] = CreateFont(16,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Bookman Old Style"));

            //Sets the font to be displayed in Edit Box
			SendMessage(hwndInputText,
				WM_SETFONT,
				(WPARAM)phfSizes[iFont][iSize],
				MAKELPARAM(FALSE,0));

            //Sets the default text of the Edit Box
			SendMessage(hwndInputText,
				WM_SETTEXT,
				NULL,
				(LPARAM)"Insert text here...");

            // Create a submit button
			hwndSubmitButton = CreateWindowEx(NULL,
				"BUTTON",
				"Submit",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				50,
				cyChar + 110,
				100,
				24,
				hwnd,
				(HMENU)IDC_SUBMIT_BUTTON,
				GetModuleHandle(NULL),
				NULL);

            //Sets the default font for the OK Button
			SendMessage(hwndSubmitButton,
				WM_SETFONT,
				(WPARAM)phfSizes[iFont][iSize],
				MAKELPARAM(FALSE,0));

            // Create a comic button
			hwndFontButton = CreateWindowEx(NULL,
				"BUTTON",
				"Change Font",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				iClientWidth * 7/10,
				iClientHeight * 1/10,
				100,
				24,
				hwnd,
				(HMENU)IDC_CHANGE_FONT_BUTTON,
				GetModuleHandle(NULL),
				NULL);

            SendMessage(hwndFontButton,
				WM_SETFONT,
				(WPARAM)phfSizes[iFont][iSize],
				MAKELPARAM(FALSE,0));

            //Create Change Size Button
            hwndSizeButton = CreateWindowEx(NULL,
				"BUTTON",
				"Change Size",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				iClientWidth * 7/10,
				iClientHeight * 3/10,
				100,
				24,
				hwnd,
				(HMENU)IDC_CHANGE_SIZE_BUTTON,
				GetModuleHandle(NULL),
				NULL);

            hfSize = CreateFont(18,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Bookman Old Style"));

            SendMessage(hwndSizeButton,
				WM_SETFONT,
				(WPARAM)hfSize,
				MAKELPARAM(FALSE,0));

            //Create Change Color Button
            hwndColorButton = CreateWindowEx(NULL,
				"BUTTON",
				"Change Color",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				iClientWidth * 7/10,
				iClientHeight * 5/10,
				100,
				24,
				hwnd,
				(HMENU)IDC_CHANGE_COLOR_BUTTON,
				GetModuleHandle(NULL),
				NULL);

            SendMessage(hwndColorButton,
				WM_SETFONT,
				(WPARAM)phfSizes[iFont][iSize],
				MAKELPARAM(FALSE,0));

            //Create the Edit Box
            hwndOutputText = CreateWindowEx(
                (DWORD)NULL,
                TEXT("edit"),
                TEXT("Output Box"),
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE,
                iClientWidth * 1/10,
                iClientHeight * 65/100,
                iClientWidth * 55/100,
                iClientHeight * 3/10,
                hwnd,
                (HMENU)IDC_OUTPUT_TEXT,
                GetModuleHandle(NULL),
                NULL);

            SendMessage(hwndOutputText,
                WM_SETFONT,
                (WPARAM)phfSizes[iFont][iSize],
                1);
            break;

        case WM_PAINT:
            strcpy(szInputName, "Input Box");
            strcpy(szOutputName, "Output Box");
            strcpy(szAdjustments, "Adjustments");

            GetClientRect(hwnd, &rcClient);
            iClientWidth = rcClient.right - rcClient.left;
            iClientHeight = rcClient.bottom - rcClient.top;

            hdc = BeginPaint(hwnd, &ps);
            TextOut (hdc, iClientWidth * 3/10, iClientHeight * 2/100, szInputName, 9);
            TextOut (hdc, iClientWidth * 3/10, iClientHeight * 55/100, szOutputName, 10);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(80, 180, 80));
            SelectObject(hdc, phfSizes[3][2]);

            TextOut (hdc, iClientWidth * 73/100, iClientHeight * 2/100, szAdjustments, 11);

            EndPaint(hwnd, &ps);

            break;

        case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO pMMInfo;
                pMMInfo = (LPMINMAXINFO)lParam;

                RECT rct;

                rct.left = 0;
                rct.top = 0;
                rct.right = 400;
                rct.bottom = 200;

                AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

                POINT minSize;

                minSize.x = rct.right - rct.left;
                minSize.y = rct.bottom - rct.top;

                pMMInfo->ptMinTrackSize = minSize;
            }
            break;

        case WM_CTLCOLOREDIT:
        {
            HDC hdc = (HDC)wParam;
            int iColorAux = iColor % 3;

            switch (iColorAux)
            {
                case 0:
                    SetTextColor(hdc, RGB(255, 0, 0));
                    break;

                case 1:
                    SetTextColor(hdc, RGB(0, 255, 0));
                    break;

                case 2:
                    SetTextColor(hdc, RGB(0, 0, 255));
                    break;
            }
            return (LRESULT)hbr;
        }
        break;

        case WM_SIZE:
            GetClientRect(hwnd, &rcClient);
            iClientWidth = rcClient.right - rcClient.left;
            iClientHeight = rcClient.bottom - rcClient.top;

            MoveWindow(hwndExitButton,
                iClientWidth * 7/10,
                iClientHeight * 7/10,
                100, 50,
                TRUE);

            MoveWindow(hwndInputText,
                iClientWidth * 1/10,
                iClientHeight * 1/10,
                iClientWidth * 55/100,
                iClientHeight * 3/10,
                TRUE);

            MoveWindow(hwndSubmitButton,
                iClientWidth * 1/10,
                iClientHeight * 43/100,
                100, 24,
                TRUE);

            MoveWindow(hwndFontButton,
                iClientWidth * 7/10,
                iClientHeight * 1/10,
                100, 24,
                TRUE);

            MoveWindow(hwndSizeButton,
                iClientWidth * 7/10,
                iClientHeight * 3/10,
                100, 24,
                TRUE);

            MoveWindow(hwndColorButton,
                iClientWidth * 7/10,
                iClientHeight * 5/10,
                100, 24,
                TRUE);

            MoveWindow(hwndOutputText,
                iClientWidth * 1/10,
                iClientHeight * 65/100,
                iClientWidth * 55/100,
                iClientHeight * 3/10,
                TRUE);

            break;

        case WM_COMMAND:
			switch(LOWORD(wParam))
            {
				case IDC_SUBMIT_BUTTON:
				{
					SendMessage(hwndInputText,
						WM_GETTEXT,
						sizeof(szSubmitBuffer)/sizeof(szSubmitBuffer[0]),
						reinterpret_cast<LPARAM>(szSubmitBuffer));

                    if (MessageBox(NULL,
                            TEXT("Are you sure you want to submit?"),
                            TEXT("Submit?"),
                            MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING) == IDYES) {
                        SendMessage(hwndOutputText, WM_SETTEXT, 0, (LPARAM)szSubmitBuffer);
                    };

                    if ( strcmp(szMove, szSubmitBuffer) == 0 )
                    {
                        ShowWindow(hwnd, SW_MAXIMIZE);
                    }
				}
				break;

                case IDC_CHANGE_FONT_BUTTON:
                {
                    SendMessage(hwndOutputText,
                        WM_SETFONT,
                        (WPARAM)phfSizes[(++iFont)%4][iSize%3],
                        1);
                }
                break;

                case IDC_CHANGE_SIZE_BUTTON:
                {
                    SendMessage(hwndOutputText,
                        WM_SETFONT,
                        (WPARAM)phfSizes[iFont%4][(++iSize)%3],
                        1);
                }
                break;

                case IDC_CHANGE_COLOR_BUTTON:
                {
                    iColor++;

                    SendMessage(hwndInputText,
                        WM_SETFONT,
                        (WPARAM)phfSizes[iFont%4][iSize%3],
                        1);
                }
                break;

				case IDC_EXIT_BUTTON:
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            DeleteObject(phfSizes[i][j]);
                        }
                    }

                    if (hbr)
                    {
                        DeleteObject(hbr);
                    }

                    PostQuitMessage (0);
                    break;
            }
			break;

        case WM_SYSCOMMAND:
        {
            switch(wParam)
            {
                case SC_MINIMIZE:
                    return MessageBox(NULL,
                                TEXT("What do we say to minimize?\n     Not today!"),
                                TEXT("Minimize Button Clicked!"),
                                MB_OK | MB_ICONWARNING);

                case SC_CLOSE:
                    return MessageBox(NULL,
                                TEXT("This is not what do you think it is... Check Exit Button!"),
                                TEXT("Maximize Button Clicked!"),
                                MB_OK | MB_ICONASTERISK);

                case SC_MAXIMIZE:
                    iScreenW = GetSystemMetrics(SM_CXSCREEN);
                    iScreenH = GetSystemMetrics(SM_CYSCREEN);

                    GetWindowRect(hwnd, &rcWindow);
                    SetWindowPos(
                        hwnd, 0,
                        (iScreenW - rcWindow.right) / 10 * (rand() % 11),
                        (iScreenH - rcWindow.bottom) / 10 * (rand() % 11),
                        0, 0,
                        SWP_NOZORDER|SWP_NOSIZE);

                    return MessageBox(NULL,
                                TEXT("Don't do this again!"),
                                TEXT("Meh"),
                                MB_OK | MB_ICONASTERISK);

                default:
                    return DefWindowProc(hwnd, message, wParam, lParam);
            }
        }

        case WM_DESTROY:
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    DeleteObject(phfSizes[i][j]);
                }
            }

            if(hbr)
            {
                DeleteObject(hbr);
            }

            PostQuitMessage (0);
            break;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
