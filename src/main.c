#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "lib/stdbestemms.h"
#include "lib/controls.h"
#include "lib/resource_manager.h"
#include "lib/file_manager.h"
#include "lib/error_messages.h"

// percorso del file dei percorsi
#define FILE_PATHS "data\\file_paths.txt"

// prototipi delle funzioni
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void getBestemmsByDateAux();
void InitializeWindowControls();
void InitializeResources();
HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
void HandleCommand(WPARAM wParam, HWND hwnd);
void SetControlsVisibility(int *controlsToShow, int showCount, int *controlsToHide, int hideCount);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // inizializza le risorse
    InitializeResources();

    // crea la finestra principale
    HWND hwnd = CreateMainWindow(hInstance, nCmdShow);
    if (hwnd == NULL)
    {
        MessageBox(NULL, GetResourceString("ERR_MAIN_WINDOW_CREATION"), GetResourceString("ERR_MSGBOX_TITLE"), MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // ciclo dei messaggi
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void InitializeResources()
{
    int result;

    // inizializza il generatore di numeri casuali
    srand(time(NULL));

    // carica i percorsi dei file
    result = LoadFilePaths(FILE_PATHS);

    if (result != EXIT_SUCCESS)
    {
        // messagebox con errore e codice errore
        char errorMessage[256];
        sprintf(errorMessage, "%s\nCOD: %d", GetResourceString("ERR_FILE_PATH_OPEN"), result);
        MessageBox(NULL, errorMessage, GetResourceString("ERR_MSGBOX_TITLE"), MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    // carica le risorse
    result = LoadResources(filePaths, filePathCount);

    if (result |= EXIT_SUCCESS)
    {
        char errorMessage[256];
        switch (result)
        {
        case ERR_RESOURCE_STRING_OPEN:
            // messagebox con errore e codice errore
            // Non posso prendere le stringhe dalle risorse perchè non sono state caricate
            sprintf(errorMessage, "%s\nCOD: %d", "Errore nell'apertura del file delle risorse stringhe", result);
            MessageBox(NULL, errorMessage, "Errore", MB_ICONERROR);
            break;

        case ERR_RESOURCE_NUMERIC_OPEN:
            // messagebox con errore e codice errore
            sprintf(errorMessage, "%s\nCOD: %d", GetResourceString("ERR_RESOURCE_NUMERIC_OPEN"), result);
            MessageBox(NULL, errorMessage, GetResourceString("ERR_MSGBOX_TITLE"), MB_ICONERROR);
            break;

        default:
            break;
        }
        exit(EXIT_FAILURE);
    }

    // inizializza il Bestemms_Engine
    result = intiBestemmsEngine();

    if (result != EXIT_SUCCESS)
    {
        // messagebox con errore e codice errore
        char errorMessage[256];
        sprintf(errorMessage, "%s\nCOD: %d", GetResourceString("ERR_BESTEMMS_ENGINE_INIT"), result);
        MessageBox(NULL, errorMessage, GetResourceString("ERR_MSGBOX_TITLE"), MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG_RESOURCE_PARSING
    // stampa le risorse
    PrintResources();
#endif

#ifdef DEBUG_FILE_PATHS
    // stampa i percorsi dei file
    printFilePaths();
#endif

#ifdef DEBUG_SAINTS_PARSING
    // stampa tutti i santi
    printAllSaints(santi);
#endif

#ifdef DEBUG_BESTEMMS_PARSING
    // stampa tutte le bestemmie
    printAllBestemms();
#endif
}

HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "MyWindowClass",
        GetResourceString("STRING_WINDOW_TITLE"),
        WS_OVERLAPPEDWINDOW, // per rendere le dimensioni fisse & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, GetResourceNumeric("START_WINDOW_WIDTH"), GetResourceNumeric("START_WINDOW_HEIGHT"),
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hwnd != NULL)
    {
        ShowWindow(hwnd, nCmdShow);
    }

    return hwnd;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Inizializza i controlli se non sono stati inizializzati
    if (!controlsInitialized)
    {
        InitializeWindowControls(controls);
        controlsInitialized = 1;
    }

    switch (uMsg)
    {
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *minMaxInfo = (MINMAXINFO *)lParam;
        minMaxInfo->ptMinTrackSize.x = GetResourceNumeric("MIN_WINDOW_WIDTH");
        minMaxInfo->ptMinTrackSize.y = GetResourceNumeric("MIN_WINDOW_HEIGHT");
        return 0;
    }

    case WM_CREATE:
    {
        //menu a tendina
        HMENU hMenu = CreateMenu();
        HMENU hSubMenu = CreatePopupMenu();
        HMENU hSubMenuMode = CreatePopupMenu();

        AppendMenu(hSubMenuMode, MF_STRING, ID_MENU_RANDOM_SAINT, GetResourceString("STRING_SUBMENU_RANDOM_SAINT"));
        AppendMenu(hSubMenuMode, MF_STRING, ID_MENU_SAINT_OF_THE_DAY, GetResourceString("STRING_SUBMENU_SAINT_OF_THE_DAY"));
        AppendMenu(hSubMenuMode, MF_STRING, ID_MENU_INSERT_DATA, GetResourceString("STRING_SUBMENU_INSERT_DATE"));

        AppendMenu(hSubMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenuMode, GetResourceString("STRING_MENU_MODE"));
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_SETTINGS, GetResourceString("STRING_MENU_SETTINGS"));
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_INFO, GetResourceString("STRING_MENU_INFO"));
        AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hSubMenu, MF_STRING, ID_MENU_EXIT, GetResourceString("STRING_MENU_EXIT"));

        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, GetResourceString("STRING_MENU_TITLE"));

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
        HandleCommand(wParam, hwnd);
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

void HandleCommand(WPARAM wParam, HWND hwnd)
{
    switch (LOWORD(wParam))
    {
    case ID_MENU_EXIT:
        if (MessageBox(hwnd, GetResourceString("STRING_MSGBOX_EXIT_MESSAGE"), GetResourceString("STRING_MSG_BOX_EXIT_TITLE"), MB_YESNO | MB_ICONQUESTION) == IDYES)
            PostQuitMessage(0);
        break;

    case ID_MENU_RANDOM_SAINT:
    {
        // Mostra i controlli per il santo random
        int controlsToShow[] = {CONTROL_BUTTON_RANDOM_SAINT_GENERATE, CONTROL_BUTTON_RANDOM_SAINT_CLEAR};
        // Nasconde gli altri controlli
        int controlsToHide[] = {CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE, CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR, CONTROL_EDIT_INSERT_DATE, CONTROL_BUTTON_INSERT_DATE_CLEAR};

        SetControlsVisibility(controlsToShow, sizeof(controlsToShow) / sizeof(controlsToShow[0]), controlsToHide, sizeof(controlsToHide) / sizeof(controlsToHide[0]));

        SetWindowText(controls[CONTROL_LABEL].hwnd, GetResourceString("STRING_RANDOM_SAINT_LABEL"));
        break;
    }
    case ID_MENU_SAINT_OF_THE_DAY:
    {
        // Mostra i controlli per il santo del giorno
        int controlsToShow[] = {CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE, CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR};
        // Nasconde gli altri controlli
        int controlsToHide[] = {CONTROL_BUTTON_RANDOM_SAINT_GENERATE, CONTROL_BUTTON_RANDOM_SAINT_CLEAR, CONTROL_EDIT_INSERT_DATE, CONTROL_BUTTON_INSERT_DATE_CLEAR};

        SetControlsVisibility(controlsToShow, sizeof(controlsToShow) / sizeof(controlsToShow[0]), controlsToHide, sizeof(controlsToHide) / sizeof(controlsToHide[0]));

        SetWindowText(controls[CONTROL_LABEL].hwnd, GetResourceString("STRING_SAINT_OF_THE_DAY_LABEL"));
        break;
    }
    case ID_MENU_INSERT_DATA:
    {
        // Mostra i controlli per l'inserimento della data
        int controlsToShow[] = {CONTROL_EDIT_INSERT_DATE, CONTROL_BUTTON_INSERT_DATE_CLEAR};
        // Nasconde gli altri controlli
        int controlsToHide[] = {CONTROL_BUTTON_RANDOM_SAINT_GENERATE, CONTROL_BUTTON_RANDOM_SAINT_CLEAR, CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE, CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR};

        SetControlsVisibility(controlsToShow, sizeof(controlsToShow) / sizeof(controlsToShow[0]), controlsToHide, sizeof(controlsToHide) / sizeof(controlsToHide[0]));

        SetWindowText(controls[CONTROL_LABEL].hwnd, GetResourceString("STRING_INSERT_DATE_LABEL"));
        break;
    }

    case ID_MENU_INFO:
        MessageBox(hwnd, GetResourceString("STRING_MSGBOX_INFO_MESSAGE"), GetResourceString("STRING_MSGBOX_INFO_TITLE"), MB_OK | MB_ICONINFORMATION);
        break;

    case ID_BUTTON_RANDOM_SAINT_GENERATE:
        SetWindowText(controls[CONTROL_LABEL].hwnd, getRandomBestemms());
        break;

    case ID_BUTTON_RANDOM_SAINT_CLEAR:
        SetWindowText(controls[CONTROL_LABEL].hwnd, GetResourceString("STRING_RANDOM_SAINT_LABEL"));
        break;

    case ID_BUTTON_SAINT_OF_THE_DAY_GENERATE:
        SetWindowText(controls[CONTROL_LABEL].hwnd, getTodayBestemms());
        break;

    case ID_BUTTON_SAINT_OF_THE_DAY_CLEAR:
        SetWindowText(controls[CONTROL_LABEL].hwnd, GetResourceString("STRING_SAINT_OF_THE_DAY_LABEL"));
        break;

    case ID_EDIT_INSERT_DATE:
        getBestemmsByDateAux();
        break;

    case ID_BUTTON_INSERT_DATE_CLEAR:
        SetWindowText(controls[CONTROL_EDIT_INSERT_DATE].hwnd, GetResourceString("STRING_EDIT_INSERT_DATE"));
        SetWindowText(controls[CONTROL_LABEL].hwnd, GetResourceString("STRING_INSERT_DATE_LABEL"));
        break;

    default:
        break;
    }
}

void getBestemmsByDateAux()
{
    // Get the length of the text
    int length = GetWindowTextLength(controls[CONTROL_EDIT_INSERT_DATE].hwnd);

    // Allocate memory for the text
    char *buffer = (char *)GlobalAlloc(GPTR, length + 1);

    // Get the text from the edit control
    GetWindowText(controls[CONTROL_EDIT_INSERT_DATE].hwnd, buffer, length + 1);

    // se il testo è uguale a STRING_EDIT_INSERT_DATE, cancella il testo
    if (strcmp(buffer, GetResourceString("STRING_EDIT_INSERT_DATE")) == 0)
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
        const char *saint = getBestemmsByDate(month, day);

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

// Funzione per inizializzare i controlli
void InitializeWindowControls()
{
    // Definizione dei controlli
    // Nome della classe, testo, stile, posizione e dimensioni, ID, visibilità
    // controllo per il label
    controls[CONTROL_LABEL] = (WindowControl){NULL, "STATIC", GetResourceString("STRING_RANDOM_SAINT_LABEL"), WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 0, 0, NULL, TRUE};
    // controlli per il santo random
    controls[CONTROL_BUTTON_RANDOM_SAINT_GENERATE] = (WindowControl){NULL, "BUTTON", GetResourceString("STRING_BUTTON_RANDOM_SAINT_GENERATE"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_RANDOM_SAINT_GENERATE, TRUE};
    controls[CONTROL_BUTTON_RANDOM_SAINT_CLEAR] = (WindowControl){NULL, "BUTTON", GetResourceString("STRING_BUTTON_RANDOM_SAINT_CLEAR"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_RANDOM_SAINT_CLEAR, TRUE};
    // controlli per il santo del giorno
    controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE] = (WindowControl){NULL, "BUTTON", GetResourceString("STRING_BUTTON_SAINT_OF_THE_DAY_GENERATE"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_SAINT_OF_THE_DAY_GENERATE, FALSE};
    controls[CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR] = (WindowControl){NULL, "BUTTON", GetResourceString("STRING_BUTTON_SAINT_OF_THE_DAY_CLEAR"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_SAINT_OF_THE_DAY_CLEAR, FALSE};
    // controlli per l'inserimento della data
    controls[CONTROL_EDIT_INSERT_DATE] = (WindowControl){NULL, "EDIT", GetResourceString("STRING_EDIT_INSERT_DATE"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 0, 0, (HMENU)ID_EDIT_INSERT_DATE, FALSE};
    controls[CONTROL_BUTTON_INSERT_DATE_CLEAR] = (WindowControl){NULL, "BUTTON", GetResourceString("STRING_BUTTON_INSERT_DATE_CLEAR"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, (HMENU)ID_BUTTON_INSERT_DATE_CLEAR, FALSE};

    // Aggiungi altri controlli qui
}

void SetControlsVisibility(int *controlsToShow, int showCount, int *controlsToHide, int hideCount)
{
    for (int i = 0; i < showCount; i++)
    {
        controls[controlsToShow[i]].visible = TRUE;
    }

    for (int i = 0; i < hideCount; i++)
    {
        controls[controlsToHide[i]].visible = FALSE;
    }

    // Aggiorna l'interfaccia
    UpdateControlVisibility(controls, CONTROL_COUNT);
}