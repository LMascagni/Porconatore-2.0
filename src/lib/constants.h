#ifndef CONSTANTS_H
#define CONSTANTS_H

// definizione costanti
#define MAX_LUNGHEZZA 50
#define START_WINDOW_WIDTH  500
#define START_WINDOW_HEIGHT 300

// definizioni delle stringhe
#if defined(DEBUG) || defined(DEBUG_SAINTS_PARSING)
#define STRING_WINDOW_TITLE "(DEBUG MODE) - Porconatore v2.0"
#else
#define STRING_WINDOW_TITLE "Porconatore v2.0"
#endif
#define STRING_MENU_RANDOM_SAINT "Santo random"
#define STRING_MENU_SAINT_OF_THE_DAY "Santo del giorno"
#define STRING_BUTTON_RANDOM_SAINT "Santo random"
#define STRING_BUTTON_CLEAR "CLEAR"
#define STRING_SANTO_RANDOM_LABEL "Premi il pulsante per un santo casuale"

// definizioni per gli ID dei pulsanti
#define ID_MENU_RANDOM_SAINT 1
#define ID_MENU_SAINT_OF_THE_DAY 2
#define ID_MENU_EXIT 9
#define ID_BUTTON_RANDOM_SAINT 101
#define ID_BUTTON_CLEAR 102

#endif // CONSTANTS_H