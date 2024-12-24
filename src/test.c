#include <windows.h>

#define ID_MENU_OPTION1 1
#define ID_MENU_OPTION2 2
#define ID_BUTTON1 3
#define ID_BUTTON2 4

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hButton1 = NULL, hButton2 = NULL;

    switch (uMsg) {
        case WM_CREATE: {
            HMENU hMenu = CreateMenu();
            HMENU hSubMenu = CreatePopupMenu();

            AppendMenu(hSubMenu, MF_STRING, ID_MENU_OPTION1, "Tasto 1");
            AppendMenu(hSubMenu, MF_STRING, ID_MENU_OPTION2, "Tasto 2");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "Opzioni");

            SetMenu(hwnd, hMenu);

            hButton1 = CreateWindow("BUTTON", "Tasto 1", WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
                                    50, 50, 100, 30, hwnd, (HMENU)ID_BUTTON1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
            hButton2 = CreateWindow("BUTTON", "Tasto 2", WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
                                    50, 90, 100, 30, hwnd, (HMENU)ID_BUTTON2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            if (!hButton1 || !hButton2) {
                MessageBox(hwnd, "Failed to create buttons.", "Error", MB_OK | MB_ICONERROR);
                return -1;
            }

            ShowWindow(hButton1, SW_HIDE);
            ShowWindow(hButton2, SW_HIDE);
        }
        break;

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_MENU_OPTION1:
                    ShowWindow(hButton1, SW_SHOW);
                    ShowWindow(hButton2, SW_HIDE);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case ID_MENU_OPTION2:
                    ShowWindow(hButton1, SW_HIDE);
                    ShowWindow(hButton2, SW_SHOW);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
        }
        break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Menu a Tendina",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}