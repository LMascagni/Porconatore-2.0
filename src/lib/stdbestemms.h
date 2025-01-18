#ifndef STDBESTEMMS_H
#define STDBESTEMMS_H

#include "stdsaint.h"

// parametri di configurazione
#define MAX_LEN 100
#define MAX_TERMS 100
#define MAX_TYPES 2

// definizione costanti
#define PREFIX 0
#define SUFFIX 1

#define F 0
#define M 1
#define MF 2

typedef struct
{
    char bestemmia[50];
    int gender;
    int offensiveness;
} bestemmia;

extern bestemmia bestemmie[MAX_TYPES][MAX_TERMS];
extern int prefixCounter;
extern int suffixCounter;

int initBestemmsEngine();
void initBestemms();
int parseAndStoreBestemms(const char *filename);
const char *getRandomBestemms(int offenseLevel);
const char *getTodayBestemms(int offenseLevel);
const char *getBestemmsByDate(int month, int day, int offenseLevel);
bestemmia getRandomTerm(int type, int gender, int offenseLevel);
const char *buildSimpleBestemms(santo saint, int offenseLevel);

void printAllBestemms();

int isVowel(char c);

#endif // STDBESTEMMIETOR_H