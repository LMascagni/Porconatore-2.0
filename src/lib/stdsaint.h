#ifndef STDSAINT_H
#define STDSAINT_H

#include <stdbool.h>


//parametri di configurazione
#define MAX_MONTHS 12
#define MAX_DAYS 31
#define MAX_LEN 50
#define MAX_ITER 400

//definizione costanti
#define F false
#define M true

//messaggi di errore
#define ERROR_SAINT_FILE_NOT_FOUND -1
#define ERROR_MEMORY_ALLOCATION -2

// definizioni messaggi di errore
#define STRING_ERROR_DATE_FORMAT "Formato data non valido. Inserisci la data nel formato gg/mm"


typedef struct {
    char nome[MAX_LEN];
    bool sesso;
} santo;

int parseAndStoreSaints(const char *filename, santo santi[MAX_MONTHS][MAX_DAYS]);
int initSaints(santo santi[MAX_MONTHS][MAX_DAYS]);
void printAllSaints(santo santi[MAX_MONTHS][MAX_DAYS]);
const char *getSaint(santo santi[MAX_MONTHS][MAX_DAYS], int mese, int giorno);
const char *getTodaySaint(santo santi[MAX_MONTHS][MAX_DAYS]);
const char *getRandomSaint(santo santi[MAX_MONTHS][MAX_DAYS]);

#endif // STDSAINT_H