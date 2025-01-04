#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "stdbestemms.h"
#include "stdsaint.h"
#include "file_manager.h"
#include "error_messages.h"

bestemmia bestemmie[MAX_TYPES][MAX_TERMS];
int prefixCounter = 0;
int suffixCounter = 0;

int parseAndStoreBestemms(const char *filename)
{
#ifdef DEBUG_SAINTS_PARSING
    printf("Parsing file: %s\n", filename);
#endif

    //prefixCounter = 0;
    //suffixCounter = 0;

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        return ERR_BESTEMMS_FILE_NOT_FOUND;
    }

    char line[512]; // Memory section for storing the read line
    char term[100]; // Memory section for storing the read name
    int gender = 0; // Memory section for storing the gende
    int offense_level = 0;

    int type = 0; // 0 = prefix, 1 = suffix
#ifdef DEBUG_BESTEMMS_PARSING
    int counter = 0;
#endif

    while (fgets(line, sizeof(line), file) && (prefixCounter < MAX_TERMS) && (suffixCounter < MAX_TERMS))
    {

#ifdef DEBUG_BESTEMMS_PARSING
        printf("prefixCounter is: %d\n", prefixCounter);
        printf("suffixCounter is: %d\n", suffixCounter);
#endif

        // Skip empty lines
        if (line[0] == '\n')
        {
#ifdef DEBUG_BESTEMMS_PARSING
            printf("Continuing after skipping /n, line %d: %s\n", counter, line);
#endif
            continue;
        }

        // Assing a value to type if a new type is found
        if (sscanf(line, " \"%d\": [", &type) == 1)
        {            // if sscanf succesfully reads '\"%d\": [', (" symbol is written as \")
#ifdef DEBUG_BESTEMMS_PARSING
            printf("Continuing after resetting day, line %d: %s\n", counter, line);
#endif
            continue;
        }

        // Assing data to the saint struct, if any is found
        int parsedCount = sscanf(line, " {\"term\": \"%[^\"]\", \"gender\": %d, \"offense\": %d}", term, &gender, &offense_level);
        if (parsedCount == 3 && type >= 0 && type < MAX_TYPES)
        {
            switch (type)
            {
                case PREFIX:
                    strcpy(bestemmie[type][prefixCounter].bestemmia, term);
                    bestemmie[type][prefixCounter].gender = gender;
                    bestemmie[type][prefixCounter].offensiveness = offense_level;

#ifdef DEBUG_BESTEMMS_PARSING
                    printf("Found valid data on line %d: %s", counter, line);
                    printf("Conteggio: %d, type: %d, term: %s, gender: %d, offensive: %d\n", prefixCounter, type, term, gender, offense_level);
#endif
                    prefixCounter++;
                    break;

                case SUFFIX:
                    strcpy(bestemmie[type][suffixCounter].bestemmia, term);
                    bestemmie[type][suffixCounter].gender = gender;
                    bestemmie[type][suffixCounter].offensiveness = offense_level;

#ifdef DEBUG_BESTEMMS_PARSING
                    printf("Found valid data on line %d: %s", counter, line);
                    printf("Conteggio: %d, type: %d, term: %s, gender: %d, offensive: %d\n", prefixCounter, type, term, gender, offense_level);
#endif
                    suffixCounter++;
                    break;

                // aggiungere altri tipi di bestemmie qui
                
                default:
                    break;
            }
        }
        else
        {
#ifdef DEBUG_BESTEMMS_PARSING
            printf("Could not find any data on line %d: %s\n", counter, line);
#endif
        }
    }



    if (prefixCounter >= MAX_TERMS || suffixCounter >= MAX_TERMS)
    {
        return ERR_BESTEMMS_MEMORY_ALLOCATION;
    }
    
    fclose(file);
    return EXIT_SUCCESS;
}

void initBestemms()
{
    for (int type = 0; type < MAX_TYPES; type++)
    {
        for (int term = 0; term < MAX_TERMS; term++)
        {
            bestemmie[type][term].bestemmia[0] = '\0';
            bestemmie[type][term].gender = 0;
        }
    }
    return;
}

void printAllBestemms()
{
    for (int type = 0; type < MAX_TYPES; type++)
    {
        for (int term = 0; term < MAX_TERMS; term++)
        {
            if (strlen(bestemmie[type][term].bestemmia) > 0)
            {
                printf("Tipo %d, Termine %d: Bestemmia: %s, Genere: %d, offensivita': %d\n", type, term,
                       bestemmie[type][term].bestemmia, bestemmie[type][term].gender, bestemmie[type][term].offensiveness);
            }
        }
    }
    return;
}

int initBestemmsEngine()
{
    int result;

    //inizializza l'array dei santi
    initSaints();

    //inizializza l'array delle bestemmie
    initBestemms();

    // carica i santi da file
    result = parseAndStoreSaints(GetFilePath("SAINTS_FILE"));
    if (result != EXIT_SUCCESS)
    {
        return result;
    }

    // carica le bestemmie da file
    result = parseAndStoreBestemms(GetFilePath("BESTEMMS_FILE"));
    if (result != EXIT_SUCCESS)
    {
        return result;
    }

    //stampe di debug
#ifdef DEBUG_BESTEMMS_PARSING
    printAllBestemms();
#endif

#ifdef DEBUG_SAINTS_PARSING
    printAllSaints();
#endif

    return EXIT_SUCCESS;
}

const char *getRandomBestemms()
{
    santo saint = getRandomSaint();

#ifdef DEBUG_BESTEMMS
    printf("Santo trovato, nome: %s, genere: %d ->", saint.name, saint.gender);
#endif

    return buildSimpleBestemms(saint);
}

const char *getTodayBestemms()
{
    santo saint = getTodaySaint();

#ifdef DEBUG_BESTEMMS
    printf("Santo trovato, nome: %s, genere: %d ->", saint.name, saint.gender);
#endif

    return buildSimpleBestemms(saint);
}

const char *getBestemmsByDate(int month, int day)
{
    const char *result = chkDate(month, day);
    if (result != NULL)
    {
        return result;
    }
    
    santo saint = getSaint(month, day);

#ifdef DEBUG_BESTEMMS
    printf("Santo trovato, nome: %s, genere: %d ->", saint.name, saint.gender);
#endif

    return buildSimpleBestemms(saint);
}

const char *buildSimpleBestemms(santo saint)
{
    char name[MAX_LEN];

    //cerca un prefisso e un suffisso con lo stesso genere del santo
    bestemmia prefix = getRandomTerm(PREFIX, saint.gender);
    bestemmia suffix = getRandomTerm(SUFFIX, saint.gender);

    //aggiungi San, Santa, Sant' in base al genere del santo
    if (isVowel(saint.name[0]))
    {
        sprintf(name, "Sant'%s", saint.name);
    }
    else
    {
        const char *prefix = (saint.gender == M) ? "San" : "Santa";
        sprintf(name, "%s %s", prefix, saint.name);
    }

    static char bestemmia[MAX_LEN];
    sprintf(bestemmia, "%s %s %s", prefix.bestemmia, name, suffix.bestemmia);

    //se il suffisso è .EXE sostituisci gli spazi con dei punti
    if (strcmp(suffix.bestemmia, "EXE") == 0)
    {
        for (int i = 0; bestemmia[i]; i++)
        {
            if (bestemmia[i] == ' ')
            {
                bestemmia[i] = '.';
            }
        }
    }

#ifdef DEBUG_BESTEMMS
    printf("Bestemmia generata: %s\n", bestemmia);
#endif

    return bestemmia;
}

bestemmia getRandomTerm(int type, int gender)
{
    int term = rand() % (type == PREFIX ? prefixCounter : suffixCounter);
    while (bestemmie[type][term].gender != gender && bestemmie[type][term].gender != MF)
    {
        term = rand() % (type == PREFIX ? prefixCounter : suffixCounter);
    }

    return bestemmie[type][term];
}

int isVowel(char c)
{
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}