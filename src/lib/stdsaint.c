#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "stdsaint.h"
#include "error_messages.h"

santo santi[MAX_MONTHS][MAX_DAYS];

int daysInMonth[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int parseAndStoreSaints(const char *filename)
{
#ifdef DEBUG
    printf("Parsing file: %s\n", filename);
#endif

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        return ERR_SAINT_FILE_NOT_FOUND;
    }
#ifdef DEBUG
    printf("\tFile opened successfully\n");
#endif

    char line[512]; // Memory section for storing the read line
    char name[100]; // Memory section for storing the read name
    int gender = 0; // Memory section for storing the gender

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
        int parsedCount = sscanf(line, " {\"name\": \"%[^\"]\", \"gender\": %d}", name, &gender);
        if (parsedCount == 2 && month >= 0 && month < MAX_MONTHS)
        {

            santi[month][day].gender = (gender == 1) ? F : M;
            strcpy(santi[month][day].name, name);

#ifdef DEBUG_SAINTS_PARSING
            printf("Found valid data on line %d: %s", counter, line);
            printf("Conteggio: %d, Mese: %d, Giorno: %d, Nome: %s, Sesso: %d\n",
                   parsedCount, month, day, santi[month][day].name, santi[month][day].gender);
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
        return ERR_SAINT_MEMORY_ALLOCATION;
    }
    fclose(file);
    return EXIT_SUCCESS;
}

void initSaints()
{
    for (int month = 0; month < MAX_MONTHS; month++)
    {
        for (int day = 0; day < MAX_DAYS; day++)
        {
            santi[month][day].gender = F;
            strcpy(santi[month][day].name, "");
        }
    }
    return;
}

#ifdef DEBUG_SAINTS_PARSING
void printAllSaints()
{
    for (int month = 0; month < MAX_MONTHS; month++)
    {
        for (int day = 0; day < MAX_DAYS; day++)
        {
            if (strlen(santi[month][day].name) > 0)
            {
                printf("Mese %d, Giorno %d: Nome: %s, Sesso: %d\n", month + 1, day + 1,
                       santi[month][day].name, santi[month][day].gender);
            }
        }
    }
}
#endif

const char *chkDate(int mese, int giorno)
{
    // Check month validity
    if (mese < 1 || mese > MAX_MONTHS)
    {
        return STRING_ERROR_DATE_FORMAT;
    }

    // Check day validity
    if (giorno < 1 || giorno > daysInMonth[mese])
    {
        return STRING_ERROR_DATE_FORMAT;
    }

    return NULL;
}

santo getSaint(int mese, int giorno)
{
    return santi[mese - 1][giorno - 1];
}

santo getTodaySaint()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return getSaint(tm.tm_mon + 1, tm.tm_mday);
}

santo getRandomSaint()
{
    int month = rand() % MAX_MONTHS;
    int day = rand() % daysInMonth[month + 1];
    return santi[month][day];
}