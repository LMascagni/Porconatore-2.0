#ifndef STDSAINT_H
#define STDSAINT_H

#include <stdbool.h>


//parametri di configurazione
#define MAX_MONTHS 12
#define MAX_DAYS 31
#define MAX_LUNGHEZZA 50
#define MAX_ITER 400

//definizione costanti
#define F false
#define M true

//messaggi di errore
#define ERROR_SAINT_FILE_NOT_FOUND -1
#define ERROR_MEMORY_ALLOCATION -2

typedef struct {
    char nome[MAX_LUNGHEZZA];
    bool sesso;
} santo;

int parseAndStoreSaints(const char *filename, santo santi[MAX_MONTHS][MAX_DAYS]);
int initSaints(santo santi[MAX_MONTHS][MAX_DAYS]);
void printAllSaints(santo santi[MAX_MONTHS][MAX_DAYS]);
const char *getSanto(santo santi[MAX_MONTHS][MAX_DAYS], int mese, int giorno);
const char *getRandomSaint(santo santi[MAX_MONTHS][MAX_DAYS]);

#endif // STDSAINT_H