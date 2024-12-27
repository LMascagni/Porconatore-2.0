#ifndef STDSAINT_H
#define STDSAINT_H

#include <stdbool.h>


//parametri di configurazione
#define MAX_MONTHS 12
#define MAX_DAYS 31
#define MAX_LEN 50
#define MAX_ITER 400

//definizione costanti
#define F 0
#define M 1

//messaggi di errore
#define ERROR_SAINT_FILE_NOT_FOUND -1
#define ERROR_MEMORY_ALLOCATION -2

// definizioni messaggi di errore
#define STRING_ERROR_DATE_FORMAT "Formato data non valido. Inserisci la data nel formato gg/mm"


typedef struct {
    char name[MAX_LEN];
    int gender;
} santo;

extern santo santi[MAX_MONTHS][MAX_DAYS];

int parseAndStoreSaints(const char *filename);
int initSaints();
void printAllSaints();
const char *chkDate(int mese, int giorno);
santo getSaint(int mese, int giorno);
santo getTodaySaint();
santo getRandomSaint();

#endif // STDSAINT_H