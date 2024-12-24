#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "lib/stdsaint.h"

//#define MAX_NOMI 400
#define MAX_LUNGHEZZA 50
#define WINDOW_WIDTH  300
#define WINDOW_HEIGHT 300

santo santi[MAX_MONTHS][MAX_DAYS];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hLabel;

    switch (uMsg) {
        case WM_CREATE:
            CreateWindow(
                "BUTTON", "Santo random",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 100, 200, 30,
                hwnd, (HMENU) 1, NULL, NULL);
            CreateWindow(
                "BUTTON", "CLEAR",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 150, 200, 30,
                hwnd, (HMENU) 2, NULL, NULL);
            hLabel = CreateWindow(
                "STATIC", "Premi il pulsante per un santo casuale",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                50, 50, 200, 30,
                hwnd, NULL, NULL, NULL);
            break;

        case WM_COMMAND:
            
            long long unsigned int param;
            param = LOWORD(wParam);

            if (param == 1) 
            {
                int month = rand() % 12 + 1;
                int day = rand() % 31 + 1;
                char santo[MAX_LUNGHEZZA];
                strcpy(santo, getSanto(santi, month, day));
                char buffer[MAX_LUNGHEZZA + 50];
                sprintf(buffer, "Santo del %d/%d: %s", day, month, santo);
                SetWindowText(hLabel, buffer);
            }
            else if (param == 2)
            {
                SetWindowText(hLabel, "Premi il pulsante per un santo casuale");
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
    srand(time(NULL));

    //inizializza la matrice dei santi
    initSaints(santi);

    //carica i santi da file
    if(parseAndStoreSaints("data\\santi.txt", santi) < 0)
    {
        fprintf(stderr, "Errore durante il caricamento dei santi.\n");
        exit(EXIT_FAILURE);
    }

    //stampa tutti i santi
#ifdef DEBUG
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
        "Porconatore v2.0",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        fprintf(stderr, "Errore durante la creazione della finestra.\n");
        exit(EXIT_FAILURE);
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
