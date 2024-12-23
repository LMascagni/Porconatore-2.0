#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "stdsaint.h"


int daysInMonth[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int parseAndStoreSaints(const char *filename, santo santi[MAX_MONTHS][MAX_DAYS])
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Unable to open file");
        return -1;
    }

    char line[512]; // Memory section for storing the read line
    char name[100]; // Memory section for storing the read name
    int feminine;

    int month = 0;
    int day = 0;
    int counter = 0;

    while (fgets(line, sizeof(line), file) && (counter++ < MAX_ITER))
    {

#ifdef DEBUG_SAINTS_PARSING
        printf("Counter is: %d\n", counter);
#endif

        // Skip empty lines
        if (line[0] == '\n')
        {
#ifdef DEBUG_SAINTS_PARSING
            printf("Continuing after skipping /n, line %d: %s\n", counter, line);
#endif
            continue;
        }

        // Assing a value to month if a new month is found
        if (sscanf(line, " \"%d\": [", &month) == 1)
        {            // if sscanf succesfully reads '\"%d\": [', (" symbol is written as \")
            day = 0; // Reset day for new month
#ifdef DEBUG_SAINTS_PARSING
            printf("Continuing after resetting day, line %d: %s\n", counter, line);
#endif
            continue;
        }

        // Assing data to the saint struct, if any is found
        int parsedCount = sscanf(line, " {\"name\": \"%[^\"]\", \"feminine\": %d}", name, &feminine);
        if (parsedCount == 2 && month >= 0 && month < MAX_MONTHS)
        {

            santi[month][day].sesso = (feminine == 1) ? F : M;
            strcpy(santi[month][day].nome, name);

#ifdef DEBUG_SAINTS_PARSING
            printf("Found valid data on line %d: %s", counter, line);
            printf("Conteggio: %d, Mese: %d, Giorno: %d, Nome: %s, Sesso: %d\n",
                   parsedCount, month, day, santi[month][day].nome, santi[month][day].sesso);
#endif

            day++;
        }
        else
        {
#ifdef DEBUG_SAINTS_PARSING
            printf("Could not find any data on line %d: %s", counter, line);
#endif
        }
    }

    if (counter >= MAX_ITER)
    {
        printf("An error occurred while parsing: reached maximum iterations.\n");
        return -2;
    }
    fclose(file);
    return 1;
}

int initSaints(santo santi[MAX_MONTHS][MAX_DAYS])
{
    for (int month = 0; month < MAX_MONTHS; month++)
    {
        for (int day = 0; day < MAX_DAYS; day++)
        {
            santi[month][day].sesso = F;
            strcpy(santi[month][day].nome, "");
        }
    }
    return 1;
}

void printAllSaints(santo santi[MAX_MONTHS][MAX_DAYS])
{
    for (int month = 0; month < MAX_MONTHS; month++)
    {
        for (int day = 0; day < MAX_DAYS; day++)
        {
            if (strlen(santi[month][day].nome) > 0)
            {
                printf("Mese %d, Giorno %d: Nome: %s, Sesso: %d\n", month + 1, day + 1,
                       santi[month][day].nome, santi[month][day].sesso);
            }
        }
    }
}

const char *getSanto(santo santi[MAX_MONTHS][MAX_DAYS], int mese, int giorno)
{
    // Check month validity
    if (mese < 1 || mese > MAX_MONTHS)
    {
        return "Il mese inserito non e' valido.";
    }

    // Check day validity
    if (giorno < 1 || giorno > daysInMonth[mese])
    {
        return "Il giorno inserito non e' valido";
    }

    return santi[mese - 1][giorno - 1].nome;
}