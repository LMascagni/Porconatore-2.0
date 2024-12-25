#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "lib/stdsaint.h"
#include "lib/constants.h"
#include "lib/controls.h"

santo santi[MAX_MONTHS][MAX_DAYS];

// prototipi delle funzioni
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void getSaintByDate(WindowControl controls[CONTROL_COUNT], santo santi[MAX_MONTHS][MAX_DAYS]);

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
        WS_OVERLAPPEDWINDOW, // per rendere le dimensioni fisse & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static WindowControl controls[CONTROL_COUNT] = {
        // Definizione dei controlli
        // Nome della classe, testo, stile, posizione e dimensioni, ID, visibilità
        // controllo per il label
        {NULL, "STATIC", STRING_RANDOM_SAINT_LABEL, WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 0, 0, NULL, TRUE},
        // controlli per il santo random
        {NULL, "BUTTON", STRING_BUTTON_RANDOM_SAINT_GENERATE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_RANDOM_SAINT_GENERATE, TRUE},
        {NULL, "BUTTON", STRING_BUTTON_RANDOM_SAINT_CLEAR, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_RANDOM_SAINT_CLEAR, TRUE},
        // controlli per il santo del giorno
        {NULL, "BUTTON", STRING_BUTTON_SAINT_OF_THE_DAY_GENERATE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_SAINT_OF_THE_DAY_GENERATE, FALSE},
        {NULL, "BUTTON", STRING_BUTTON_SAINT_OF_THE_DAY_CLEAR, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_SAINT_OF_THE_DAY_CLEAR, FALSE},
        // controlli per l'inserimento della data
        {NULL, "EDIT", STRING_EDIT_INSERT_DATE, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 0, 0, (HMENU)ID_EDIT_INSERT_DATE, FALSE},
        {NULL, "BUTTON", STRING_BUTTON_INSERT_DATE_CLEAR, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_INSERT_DATE_CLEAR, FALSE},

        // Aggiungi altri controlli qui
    };

    switch (uMsg)
    {
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *minMaxInfo = (MINMAXINFO *)lParam;
        minMaxInfo->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
        minMaxInfo->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;
        return 0;
    }

    case WM_CREATE:
    {
        // Menu a tendina per le opzioni
        HMENU hMenu = CreateMenu();
        HMENU hSubMenu = CreatePopupMenu();

        AppendMenu(hSubMenu, MF_STRING, ID_MENU_RANDOM_SAINT, STRING_MENU_RANDOM_SAINT);
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_SAINT_OF_THE_DAY, STRING_MENU_SAINT_OF_THE_DAY);
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_INSERT_DATA, STRING_MENU_INSERT_DATE);
        AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_EXIT, STRING_MENU_EXIT);
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, STRING_MENU_TITLE);

        SetMenu(hwnd, hMenu);

        // Crea i controlli
        for (int i = 0; i < CONTROL_COUNT; i++)
        {
            controls[i].hwnd = CreateWindowControl(hwnd, &controls[i]);
        }

        UpdateControlVisibility(controls, CONTROL_COUNT);
        break;
    }

    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hwnd, &rect);

        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        int labelHeight = height / 5;
        int buttonWidth = width / 2;
        int buttonHeight = height / 5;

        // Leggi il testo corrente del label
        char labelText[256];
        GetWindowText(controls[CONTROL_LABEL].hwnd, labelText, sizeof(labelText));

        // Ridisegna il label con le nuove dimensioni
        PositionWindowControl(controls[CONTROL_LABEL].hwnd, 0, 0, width, labelHeight);

        // Reimposta il testo letto nel label
        SetWindowText(controls[CONTROL_LABEL].hwnd, labelText);

        // schermata per il santo random
        //  Calcola le posizioni dei controlli
        int buttonY = labelHeight + 20;          // Posiziona il primo pulsante sotto il label con un margine di 10 pixel
        int buttonX = (width - buttonWidth) / 2; // Centra i pulsanti orizzontalmente

        PositionWindowControl(controls[CONTROL_BUTTON_RANDOM_SAINT_GENERATE].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
        buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

        PositionWindowControl(controls[CONTROL_BUTTON_RANDOM_SAINT_CLEAR].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
        buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

        // schermata per il santo del giorno
        //  Calcola le posizioni dei controlli
        buttonY = labelHeight + 20;          // Posiziona il primo pulsante sotto il label con un margine di 10 pixel
        buttonX = (width - buttonWidth) / 2; // Centra i pulsanti orizzontalmente

        PositionWindowControl(controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
        buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

        PositionWindowControl(controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
        buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

        // schermata per l'inserimento della data
        //  Calcola le posizioni dei controlli
        buttonY = labelHeight + 20;          // Posiziona il primo pulsante sotto il label con un margine di 10 pixel
        buttonX = (width - buttonWidth) / 2; // Centra i pulsanti orizzontalmente

        PositionWindowControl(controls[CONTROL_EDIT_INSERT_DATE].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
        buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

        PositionWindowControl(controls[CONTROL_BUTTON_INSERT_DATE_CLEAR].hwnd, buttonX, buttonY, buttonWidth, buttonHeight);
        buttonY += buttonHeight + 10; // Aggiorna la posizione Y per il prossimo pulsante

        // Posiziona altri controlli qui

        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_MENU_EXIT:
            if (MessageBox(hwnd, STRING_MSGBOX_EXIT_MESSAGE, STRING_MSG_BOX_EXIT_TITLE, MB_YESNO | MB_ICONQUESTION) == IDYES)
                PostQuitMessage(0);
            break;

        case ID_MENU_RANDOM_SAINT:
            // Mostra i controlli per il santo random
            controls[CONTROL_BUTTON_RANDOM_SAINT_GENERATE].visible = TRUE;
            controls[CONTROL_BUTTON_RANDOM_SAINT_CLEAR].visible = TRUE;

            // Nasconde gli altri controlli
            controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE].visible = FALSE;
            controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR].visible = FALSE;
            controls[CONTROL_EDIT_INSERT_DATE].visible = FALSE;
            controls[CONTROL_BUTTON_INSERT_DATE_CLEAR].visible = FALSE;

            // Aggiorna l'interfaccia
            UpdateControlVisibility(controls, CONTROL_COUNT);
            SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_RANDOM_SAINT_LABEL);
            break;

        case ID_MENU_SAINT_OF_THE_DAY:
            // Mostra i controlli per il santo del giorno
            controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE].visible = TRUE;
            controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR].visible = TRUE;

            // Nasconde gli altri controlli
            controls[CONTROL_BUTTON_RANDOM_SAINT_GENERATE].visible = FALSE;
            controls[CONTROL_BUTTON_RANDOM_SAINT_CLEAR].visible = FALSE;
            controls[CONTROL_EDIT_INSERT_DATE].visible = FALSE;
            controls[CONTROL_BUTTON_INSERT_DATE_CLEAR].visible = FALSE;

            // Aggiorna l'interfaccia
            UpdateControlVisibility(controls, CONTROL_COUNT);
            SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_SAINT_OF_THE_DAY_LABEL);
            break;

        case ID_MENU_INSERT_DATA:
            // Mostra i controlli per l'inserimento della data
            controls[CONTROL_EDIT_INSERT_DATE].visible = TRUE;
            controls[CONTROL_BUTTON_INSERT_DATE_CLEAR].visible = TRUE;

            // Nasconde gli altri controlli
            controls[CONTROL_BUTTON_RANDOM_SAINT_GENERATE].visible = FALSE;
            controls[CONTROL_BUTTON_RANDOM_SAINT_CLEAR].visible = FALSE;
            controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE].visible = FALSE;
            controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR].visible = FALSE;

            // Aggiorna l'interfaccia
            UpdateControlVisibility(controls, CONTROL_COUNT);
            SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_INSERT_DATE_LABEL);
            break;

        case ID_BUTTON_RANDOM_SAINT_GENERATE:
            SetWindowText(controls[CONTROL_LABEL].hwnd, getRandomSaint(santi));
            break;

        case ID_BUTTON_RANDOM_SAINT_CLEAR:
            SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_RANDOM_SAINT_LABEL);
            break;

        case ID_BUTTON_SAINT_OF_THE_DAY_GENERATE:
            SetWindowText(controls[CONTROL_LABEL].hwnd, getTodaySaint(santi));
            break;

        case ID_BUTTON_SAINT_OF_THE_DAY_CLEAR:
            SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_SAINT_OF_THE_DAY_LABEL);
            break;

        case ID_EDIT_INSERT_DATE:
            getSaintByDate(controls, santi);
            break;

        case ID_BUTTON_INSERT_DATE_CLEAR:
            SetWindowText(controls[CONTROL_EDIT_INSERT_DATE].hwnd, STRING_EDIT_INSERT_DATE);
            SetWindowText(controls[CONTROL_LABEL].hwnd, STRING_INSERT_DATE_LABEL);
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

void getSaintByDate(WindowControl controls[CONTROL_COUNT], santo santi[MAX_MONTHS][MAX_DAYS])
{
    // Get the length of the text
    int length = GetWindowTextLength(controls[CONTROL_EDIT_INSERT_DATE].hwnd);

    // Allocate memory for the text
    char *buffer = (char *)GlobalAlloc(GPTR, length + 1);

    // Get the text from the edit control
    GetWindowText(controls[CONTROL_EDIT_INSERT_DATE].hwnd, buffer, length + 1);

#ifdef DEBUG_INSERT_DATE
    printf("Testo inserito: >%s<\n", buffer);
#endif

    //se il testo è uguale a STRING_EDIT_INSERT_DATE, cancella il testo
    if (strcmp(buffer, STRING_EDIT_INSERT_DATE) == 0)
    {
        buffer[0] = '\0';
        SetWindowText(controls[CONTROL_EDIT_INSERT_DATE].hwnd, buffer);
        return;
    }

    // Check if the last character is a newline
    // If it is, remove it and get the date
    if (buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = '\0';
        char *token = strtok(buffer, "/");
        int day = atoi(token);
        token = strtok(NULL, "/");
        int month = atoi(token);
        // Get the saint
        const char *saint = getSaint(santi, month, day);

        // Set the text of the label
        SetWindowText(controls[CONTROL_LABEL].hwnd, saint);

        // Clear the edit control
        buffer[0] = '\0';

        // Set the text of the edit control
        SetWindowText(controls[CONTROL_EDIT_INSERT_DATE].hwnd, buffer);
    }

    // Free the memory
    GlobalFree(buffer);
}
