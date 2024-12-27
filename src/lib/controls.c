#include "controls.h"
#include "error_messages.h"

// Definizione delle variabili globali
WindowControl controls[CONTROL_COUNT];
int controlsInitialized = 0;

HWND CreateWindowControl(HWND parent, WindowControl *control)
{
    // Implementazione della funzione per creare un controllo
    control->hwnd = CreateWindowEx(
        0,
        control->className,
        control->text,
        control->style,
        control->x,
        control->y,
        control->width,
        control->height,
        parent,
        control->id,
        GetModuleHandle(NULL),
        NULL
    );

    if (control->hwnd && control->visible)
    {
        ShowWindow(control->hwnd, SW_SHOW);
    }

    return control->hwnd;
}

void PositionWindowControl(HWND hwnd, int x, int y, int width, int height)
{
    SetWindowPos(hwnd, NULL, x, y, width, height, SWP_NOZORDER);
}

void UpdateControlVisibility(WindowControl controls[], int count)
{
    for (int i = 0; i < count; i++)
    {
        ShowWindow(controls[i].hwnd, controls[i].visible ? SW_SHOW : SW_HIDE);
    }
}