#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

//errori per il programma principale
#define ERR_MAIN_WINDOW_CREATION 10 //"Errore durante la creazione della finestra"

//errori per il file_manager
#define ERR_FILE_PATH_OPEN 20 //"Errore nell'apertura del file dei percorsi"

//errori per il resource_manager
#define ERR_RESOURCE_STRING_OPEN 30 //"Errore nell'apertura del file delle risorse stringhe"
#define ERR_RESOURCE_NUMERIC_OPEN 31 //"Errore nell'apertura del file delle risorse numeriche"

//errori per il bestemms_engine
#define ERR_BESTEMMS_FILE_NOT_FOUND 40//"Errore nel caricamento del file delle bestemmie"
#define ERR_BESTEMMS_PARSING 41 //"Errore durante il parsing del file delle bestemmie"
#define ERR_BESTEMMS_MEMORY_ALLOCATION 42 //"Errore nell'allocazione di memoria per le bestemmie"

#define ERR_BESTEMMS_ENGINE_PARSE_SAINTS 43 //"Errore durante il caricamento dei santi"
#define ERR_BESTEMMS_ENGINE_PARSE_BESTEMMS 44 //"Errore durante il caricamento delle bestemmie"

//errori per il saint_engine
#define ERR_SAINT_FILE_NOT_FOUND 50 //"Errore nel caricamento del file dei santi"
#define ERR_SAINT_PARSING 51 //"Errore durante il parsing del file dei santi"
#define ERR_SAINT_MEMORY_ALLOCATION 52 //"Errore nell'allocazione di memoria per i santi"

#endif // ERROR_MESSAGES