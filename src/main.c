#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NOMI 400
#define MAX_LUNGHEZZA 50

char nomi[MAX_NOMI][MAX_LUNGHEZZA];
int count = 0;

void carica_nomi() {
    FILE *file = fopen("../data/nomi.txt", "r");
    if (file == NULL) {
        MessageBox(NULL, "Errore nell'apertura del file.", "Errore", MB_OK);
        exit(1);
    }

    while (fgets(nomi[count], MAX_LUNGHEZZA, file) != NULL && count < MAX_NOMI) {
        nomi[count][strcspn(nomi[count], "\n")] = '\0';
        count++;
    }

    fclose(file);

    if (count == 0) {
        MessageBox(NULL, "Il file è vuoto.", "Errore", MB_OK);
        exit(1);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hLabel;

    switch (uMsg) {
        case WM_CREATE:
            CreateWindow(
                "BUTTON", "Santo random",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 100, 200, 30,
                hwnd, (HMENU) 1, NULL, NULL);
            hLabel = CreateWindow(
                "STATIC", "Premi il pulsante per un santo casuale",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                50, 50, 200, 30,
                hwnd, NULL, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                srand(time(NULL));  // Usa il tempo attuale come seed per rand()
                for (int i = 0; i < 5; i++) { // Migliora la casualità con più chiamate a rand()
                    rand();
                    printf("%d:%d\n", i, rand());
                }
                int randomIndex = rand() % count; // Genera l'indice casuale dei nomi
                printf("%d", randomIndex);
                SetWindowText(hLabel, nomi[randomIndex]);
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
    carica_nomi();

    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Porconatore v2.0",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL, NULL, hInstance, NULL
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
