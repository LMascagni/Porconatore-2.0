#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "lib/stdsaint.h"
#include "lib/constants.h"

santo santi[MAX_MONTHS][MAX_DAYS];

// Enum per identificare i controlli
typedef enum {
    CONTROL_LABEL,
    CONTROL_BUTTON_RANDOM_SAINT,
    CONTROL_BUTTON_CLEAR,
    //$ CONTROL_TEST,
    // Aggiungi altri controlli qui
    CONTROL_COUNT
} ControlID;

// Struttura per rappresentare un controllo della finestra
typedef struct {
    HWND hwnd;
    const char *className;
    const char *text;
    DWORD style;
    int x, y, width, height;
    HMENU id;
    BOOL visible;
} WindowControl;

// Funzione generica per creare un controllo
HWND CreateWindowControl(HWND parent, WindowControl *control) {
    HWND hwnd = CreateWindow(
        control->className, control->text,
        control->style,
        control->x, control->y, control->width, control->height,
        parent, control->id, NULL, NULL);
    ShowWindow(hwnd, control->visible ? SW_SHOW : SW_HIDE);
    return hwnd;
}

// Funzione generica per posizionare un controllo
void PositionWindowControl(HWND hwnd, int x, int y, int width, int height) {
    SetWindowPos(hwnd, NULL, x, y, width, height, SWP_NOZORDER);
}

// Funzione per mostrare o nascondere i controlli in base al campo `visible`
void UpdateControlVisibility(WindowControl controls[], int count) {
    for (int i = 0; i < count; i++) {
        ShowWindow(controls[i].hwnd, controls[i].visible ? SW_SHOW : SW_HIDE);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static WindowControl controls[CONTROL_COUNT] = {
        {NULL, "STATIC", STRING_SANTO_RANDOM_LABEL, WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 0, 0, NULL, TRUE},
        {NULL, "BUTTON", STRING_BUTTON_RANDOM_SAINT, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_RANDOM_SAINT, TRUE},
        {NULL, "BUTTON", STRING_BUTTON_CLEAR, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_CLEAR, TRUE},
        //$ {NULL, "BUTTON", "aa", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0,(HMENU)ID_BUTTON_CLEAR, TRUE}
        // Aggiungi altri controlli qui
    };

    switch (uMsg) {
        case WM_CREATE: {
            // Menu a tendina per le opzioni
            HMENU hMenu = CreateMenu();
            HMENU hSubMenu = CreatePopupMenu();

            AppendMenu(hSubMenu, MF_STRING, ID_MENU_RANDOM_SAINT, STRING_MENU_RANDOM_SAINT);
            AppendMenu(hSubMenu, MF_STRING, ID_MENU_SAINT_OF_THE_DAY, STRING_MENU_SAINT_OF_THE_DAY);
            AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(hSubMenu, MF_STRING, ID_MENU_EXIT, "Esci");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "Opzioni");

            SetMenu(hwnd, hMenu);

            // Crea i controlli
            for (int i = 0; i < CONTROL_COUNT; i++) {
                controls[i].hwnd = CreateWindowControl(hwnd, &controls[i]);
            }
            break;
        }

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            int labelHeight = height / 10;
            int buttonWidth = width / 2;
            int buttonHeight = height / 10;

            // Leggi il testo corrente del label
            char labelText[256];
            GetWindowText(controls[CONTROL_LABEL].hwnd, labelText, sizeof(labelText));

            // Ridisegna il label con le nuove dimensioni
            PositionWindowControl(controls[CONTROL_LABEL].hwnd, 0, 0, width, labelHeight);

            // Reimposta il testo letto nel label
            SetWindowText(controls[CONTROL_LABEL].hwnd, labelText);

            // Calcola le posizioni dei controlli
            int buttonY = labelHeight + 10; // Posiziona il primo pulsante sotto il label con un margine di 10 pixel
            int buttonX = (width - buttonWidth) / 2; // Centra i pulsanti orizzontalmente

            PositionWindowControl(controls[CONTROL_BUTTON_RANDOM_SAINT].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
            buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

            PositionWindowControl(controls[CONTROL_BUTTON_CLEAR].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
            buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

            //$ PositionWindowControl(controls[CONTROL_TEST].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
            // Posiziona altri controlli qui

            break;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_MENU_EXIT:
                    PostQuitMessage(0);
                    break;

                case ID_MENU_RANDOM_SAINT:
                    controls[CONTROL_BUTTON_RANDOM_SAINT].visible = TRUE;
                    controls[CONTROL_BUTTON_CLEAR].visible = TRUE;
                    UpdateControlVisibility(controls, CONTROL_COUNT);
                    SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_SANTO_RANDOM_LABEL);
                    break;

                case ID_MENU_SAINT_OF_THE_DAY:
                    controls[CONTROL_BUTTON_RANDOM_SAINT].visible = FALSE;
                    controls[CONTROL_BUTTON_CLEAR].visible = FALSE;
                    UpdateControlVisibility(controls, CONTROL_COUNT);
                    SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_SANTO_RANDOM_LABEL);
                    break;

                case ID_BUTTON_RANDOM_SAINT:
                    SetWindowText(controls[CONTROL_LABEL].hwnd, getRandomSaint(santi));
                    break;

                case ID_BUTTON_CLEAR:
                    SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_SANTO_RANDOM_LABEL);
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
