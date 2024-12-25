#ifndef CONSTANTS_H
#define CONSTANTS_H

// definizione costanti
#define MAX_LUNGHEZZA 50
#define START_WINDOW_WIDTH  500
#define START_WINDOW_HEIGHT 300

// definizioni delle stringhe
// definizione del titolo della finestra
#if defined(DEBUG) || defined(DEBUG_SAINTS_PARSING)
#define STRING_WINDOW_TITLE "(DEBUG MODE) - Porconatore v2.0"
#else
#define STRING_WINDOW_TITLE "Porconatore v2.0"
#endif
// definizioni delle opzioni del menu a tendina
#define STRING_MENU_TITLE "Opzioni"
#define STRING_MENU_RANDOM_SAINT "Santo random"
#define STRING_MENU_SAINT_OF_THE_DAY "Santo del giorno"
#define STRING_MENU_EXIT "Esci"
// definizioni dei pulsanti
#define STRING_BUTTON_RANDOM_SAINT_GENERATE "Santo random"
#define STRING_BUTTON_RANDOM_SAINT_CLEAR "CLEAR"
#define STRING_BUTTON_SAINT_OF_THE_DAY_GENERATE "Santo del giorno"
#define STRING_BUTTON_SAINT_OF_THE_DAY_CLEAR "CLEAR"
// definizione del label
#define STRING_RANDOM_SAINT_LABEL "Premi il pulsante per un santo casuale"
#define STRING_SAINT_OF_THE_DAY_LABEL "Premi il pulsante per il santo del giorno"
// definizioni dei messaggi nelle finestre di dialogo
#define STRING_MSG_BOX_EXIT_TITLE "Uscita"
#define STRING_MSGBOX_EXIT_MESSAGE "Sei sicuro di voler uscire?"


// definizioni per gli ID dei pulsanti
// definizioni per i menu a tendina
#define ID_MENU_RANDOM_SAINT 1
#define ID_MENU_SAINT_OF_THE_DAY 2
#define ID_MENU_EXIT 9
// definizioni per i pulsanti
#define ID_BUTTON_RANDOM_SAINT_GENERATE 101
#define ID_BUTTON_RANDOM_SAINT_CLEAR 102
#define ID_BUTTON_SAINT_OF_THE_DAY_GENERATE 201
#define ID_BUTTON_SAINT_OF_THE_DAY_CLEAR 202

#endif // CONSTANTS_H