#ifndef STDBESTEMMS_H
#define STDBESTEMMS_H

#include "stdsaint.h"

// parametri di configurazione
#define MAX_LEN 50
#define MAX_TERMS 100
#define MAX_TYPES 2

// definizione costanti
#define PREFIX 0
#define SUFFIX 1

#define F 0
#define M 1
#define MF 2

// messaggi di errore
#define ERROR_BESTEMMIA_FILE_NOT_FOUND -1
#define ERROR_MEMORY_ALLOCATION -2

typedef struct
{
    char bestemmia[50];
    int gender;
} bestemmia;

extern bestemmia bestemmie[MAX_TYPES][MAX_TERMS];
extern int prefixCounter;
extern int suffixCounter;

int intiBestemmsEngine();
int initBestemms();
int parseAndStoreBestemms(const char *filename);
const char *getRandomBestemms();
const char *getTodayBestemms();
const char *getBestemmsByDate(int month, int day);
bestemmia getRandomTerm(int type, int gender);
const char *buildSimpleBestemms(santo saint);

#endif // STDBESTEMMIETOR_H