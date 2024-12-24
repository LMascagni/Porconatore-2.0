#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "lib/stdsaint.h"
#include "lib/constants.h"

santo santi[MAX_MONTHS][MAX_DAYS];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hLabel;
    static HWND hButtonRandomSaint;
    static HWND hButtonClear;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        // Menu a tendina per le opzioni
        HMENU hMenu = CreateMenu();
        HMENU hSubMenu = CreatePopupMenu();

        AppendMenu(hSubMenu, MF_STRING, ID_MENU_RANDOM_SAINT, STRING_MENU_RANDOM_SAINT);
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_SAINT_OF_THE_DAY, STRING_MENU_SAINT_OF_THE_DAY);
        AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_EXIT, "Esci");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "Opzioni");

        SetMenu(hwnd, hMenu);

        hLabel = CreateWindow(
            "STATIC", STRING_SANTO_RANDOM_LABEL,
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            0, 0, 0, 0,
            hwnd, NULL, NULL, NULL);

        hButtonRandomSaint = CreateWindow(
            "BUTTON", STRING_BUTTON_RANDOM_SAINT,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            0, 0, 0, 0,
            hwnd, (HMENU)ID_BUTTON_RANDOM_SAINT, NULL, NULL);

        hButtonClear = CreateWindow(
            "BUTTON", STRING_BUTTON_CLEAR,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            0, 0, 0, 0,
            hwnd, (HMENU)ID_BUTTON_CLEAR, NULL, NULL);
        break;
    }

    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hwnd, &rect);

        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        int labelHeight = height / 10;
        int buttonWidth = width / 2;
        int buttonHeight = height / 10;
        int buttonY = height / 2 - buttonHeight / 2;

        // Leggi il testo corrente del label
        char labelText[256];
        GetWindowText(hLabel, labelText, sizeof(labelText));

        // Ridisegna il label con le nuove dimensioni
        SetWindowPos(hLabel, NULL, 0, 0, width, labelHeight, SWP_NOZORDER);

        // Reimposta il testo letto nel label
        SetWindowText(hLabel, labelText);

        SetWindowPos(hButtonRandomSaint, NULL, width / 4, buttonY, buttonWidth, buttonHeight, SWP_NOZORDER);
        SetWindowPos(hButtonClear, NULL, width / 4, buttonY + buttonHeight + 10, buttonWidth, buttonHeight, SWP_NOZORDER);
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_MENU_EXIT:
            PostQuitMessage(0);
            break;

        case ID_MENU_RANDOM_SAINT:
            // Menu a tendina
            break;

        case ID_MENU_SAINT_OF_THE_DAY:
            break;

        case ID_BUTTON_RANDOM_SAINT:
            SetWindowText(hLabel, getRandomSaint(santi));
            break;

        case ID_BUTTON_CLEAR:
            SetWindowText(hLabel, STRING_SANTO_RANDOM_LABEL);
            break;

        default:
            break;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int result;

    srand(time(NULL));

    // inizializza la matrice dei santi
    initSaints(santi);

    // carica i santi da file
    if ((result = parseAndStoreSaints("data\\santi.txt", santi)) != EXIT_SUCCESS)
    {
        fprintf(stderr, "Errore durante il caricamento dei santi. Codice errore: %d\n", result);
        exit(EXIT_FAILURE);
    }

    // stampa tutti i santi
#ifdef DEBUG_SAINTS_PARSING
    printAllSaints(santi);
#endif

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "MyWindowClass",
        STRING_WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW, //per rendere le dimensioni fisse & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, START_WINDOW_WIDTH, START_WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hwnd == NULL)
    {
        fprintf(stderr, "Errore durante la creazione della finestra.\n");
        exit(EXIT_FAILURE);
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
