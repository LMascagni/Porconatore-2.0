#ifndef CONTROLS_H
#define CONTROLS_H

#include <windows.h>

// definizioni per gli ID
// definizioni per le icone
#define IDI_APP_ICON 1001
#define IDI_WINDOW_ICON 1002

// definizioni per i menu a tendina
#define ID_MENU_RANDOM_SAINT 1
#define ID_MENU_SAINT_OF_THE_DAY 2
#define ID_MENU_INSERT_DATA 3
#define ID_MENU_EXIT 9
// definizioni per i pulsanti
#define ID_BUTTON_RANDOM_SAINT_GENERATE 101
#define ID_BUTTON_RANDOM_SAINT_CLEAR 102
#define ID_BUTTON_SAINT_OF_THE_DAY_GENERATE 201
#define ID_BUTTON_SAINT_OF_THE_DAY_CLEAR 202
#define ID_EDIT_INSERT_DATE 301
#define ID_BUTTON_INSERT_DATE_CLEAR 302

// Enum per identificare i controlli
typedef enum
{
    CONTROL_LABEL,
    CONTROL_BUTTON_RANDOM_SAINT_GENERATE,
    CONTROL_BUTTON_RANDOM_SAINT_CLEAR,
    CONTROL_BUTTON_SAINT_OF_THE_DAY_GENERATE,
    CONTROL_BUTTON_SAINT_OF_THE_DAY_CLEAR,
    CONTROL_EDIT_INSERT_DATE,
    CONTROL_BUTTON_INSERT_DATE_CLEAR,
    //$ CONTROL_TEST,
    // Aggiungi altri controlli qui
    CONTROL_COUNT
} ControlID;

// Struttura per rappresentare un controllo della finestra
typedef struct
{
    HWND hwnd;
    const char *className;
    const char *text;
    DWORD style;
    int x, y, width, height;
    HMENU id;
    BOOL visible;
} WindowControl;

// Variabili globali
extern WindowControl controls[CONTROL_COUNT];
extern int controlsInitialized;

// Funzione generica per creare un controllo
HWND CreateWindowControl(HWND parent, WindowControl *control);

// Funzione generica per posizionare un controllo
void PositionWindowControl(HWND hwnd, int x, int y, int width, int height);

// Funzione per mostrare o nascondere i controlli in base al campo `visible`
void UpdateControlVisibility(WindowControl controls[], int count);

#endif // CONTROLS_H