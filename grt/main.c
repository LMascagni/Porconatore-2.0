#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// #define DEBUG
// #define DEBUG_SAINTS_PARSING
// #define DEBUG_DIRECTORY

const char *filePath = "santi.txt";
// const char *filePath = "c:\\Users\\cla20\\Documents\\Code\\c\\santi\\santi.txt";      //backup path

#define MAX_DAYS 31
#define MAX_ITER 400

#ifdef _WIN32
    #include <stdlib.h>
    #define clrscr() system("cls") // Clears the console on Windows
    #define flushf() scanf("%*[^\n]");
#else
    #define clrscr() printf("\e[1;1H\e[2J") // Clears the console on Linux/Mac
    #define flushf() scanf("%*[^\n]");
#endif

#ifdef _WIN32
    #include <conio.h>
    #define getCharWithoutEnter() _getch()
#else
    #include <termios.h>
    #include <unistd.h>

char getCharWithoutEnter()
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // Save current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings
    return ch;
}
#endif

typedef enum
{
    GENNAIO = 1,
    FEBBRAIO,
    MARZO,
    APRILE,
    MAGGIO,
    GIUGNO,
    LUGLIO,
    AGOSTO,
    SETTEMBRE,
    OTTOBRE,
    NOVEMBRE,
    DICEMBRE,
    MAX_MONTHS // Maximum value for month (12)
} Month;

typedef enum
{
    DEFAULT = 0,
    MANUAL_SELECT,
    BLANK,
    MAX_CASES
} Case;

int getMonthFromInput(char *input)
{
    const char *months[] = {
        "gennaio", "febbraio", "marzo", "aprile", "maggio", "giugno",
        "luglio", "agosto", "settembre", "ottobre", "novembre", "dicembre"};

    const char *months_uppercase[] = {
        "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno",
        "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"};

    // Attempt to parse as integer
    int monthNum = atoi(input);
    
    if (monthNum >= 1 && monthNum <= MAX_MONTHS)
    {
        return monthNum; 
    }
    else if(monthNum !=0){      //monthum is a number, but not a vlid one
        return -1;
    }

    // If not an int, try matching as string
    for (int i = 0; i < MAX_MONTHS; i++)
    {
        if (strcasecmp(input, months[i]) == 0)
        {
            #ifdef DEBUG
                printf("i e' %d\n", i);
            #endif
            
            return i+1; // Found matching month name
        }
        else if (strcasecmp(input, months_uppercase[i]) == 0)
        {
            #ifdef DEBUG
                printf("i e' %d\n", i);
            #endif
            
            return i+1; // Found matching uppercase month name
        }
    }

    return -1;
}

int daysInMonth[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

typedef struct
{
    char name[100];
    bool feminine;
} Saint;

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Flushes remaining characters
}

int parseAndStoreSaints(const char *filename, Saint saintsData[MAX_MONTHS][MAX_DAYS])
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

            saintsData[month][day].feminine = feminine;
            strcpy(saintsData[month][day].name, name);

#ifdef DEBUG_SAINTS_PARSING
            printf("Found valid data on line %d: %s", counter, line);
            printf("Parsed Count: %d, Month: %d, Day: %d, Name: %s, Feminine: %d\n",
                   parsedCount, month, day, saintsData[month][day].name, saintsData[month][day].feminine);
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

void getCurrentTimeAndDate()
{
    // Get the current time
    time_t now = time(NULL);

    // Check if time retrieval was successful
    if (now == ((time_t)-1))
    {
        perror("Failed to get current time");
        return;
    }

    // Convert time to local time structure
    struct tm *localTime = localtime(&now);
    if (localTime == NULL)
    {
        perror("Failed to convert time");
        return;
    }

    // Format the time and date
    char dateTimeString[100];
    strftime(dateTimeString, sizeof(dateTimeString), "%Y-%m-%d %H:%M:%S", localTime);

    // Output the current date and time
    printf("Current Date and Time: %s\n", dateTimeString);
    printf("Current Year: %d\n", localTime->tm_year + 1900); // tm_year is years since 1900
    printf("Current Day: %d\n", localTime->tm_mday);         // Day of the month
    printf("Current Month: %d\n", localTime->tm_mon + 1);    // Month (0-11)
}

const char *getSanto(Saint saintsData[MAX_MONTHS][MAX_DAYS], int month, int day)
{
    // Check month validity
    if (month < 1 || month > MAX_MONTHS)
    {
        return "Il mese inserito non e' valido.";
    }

    // Check day validity
    if (day < 1 || day > daysInMonth[month])
    {
        return "Il giorno inserito non e' valido";
    }

    return saintsData[month - 1][day - 1].name;
}

const char *getSantoLeap(Saint saintsData[MAX_MONTHS][MAX_DAYS], int month, int day, int year)
{

    // Leap year calculation and daysInMonth adjustement if true
    bool isLeap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));

    if ((month == 2 && !isLeap) && day == 29)
    {
        return "Il giorno inserito non e' valido.";
    }
    return getSanto(saintsData, month, day);
}

#ifdef DEBUG
void printAllSaints(Saint saintsData[MAX_MONTHS][MAX_DAYS])
{
    for (int month = 0; month < MAX_MONTHS; month++)
    {
        for (int day = 0; day < MAX_DAYS; day++)
        {
            if (strlen(saintsData[month][day].name) > 0)
            {
                printf("Month %d, Day %d: Name: %s, Feminine: %d\n", month + 1, day + 1,
                       saintsData[month][day].name, saintsData[month][day].feminine);
            }
        }
    }
}
#endif

void handlePostSelection(bool *selectionProcessed, int *selection) {

        puts("Premere invio per ripetere, del per uscire o  qualsiasi altro tasto per tornare alla selezione...");
        clearInputBuffer();
        char input = getCharWithoutEnter();

#ifdef DEBUG
        printf("Input code: %d\n", (int)input);
#endif

        if (input == 13) { // Enter key
            *selectionProcessed = false; 
            return; // Return to indicate continue
        }
        if (input == 8 || input == 127) { // Backspace or DEL
            exit(EXIT_SUCCESS); // Exit the program
        }

        // when the loop is required to return to the selection screen
        *selection = 0;
        *selectionProcessed = false;     
}


int main()
{
#ifdef DEBUG_DIRECTORY
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
    }
#endif

    // TXT PARSING
    Saint saintsData[MAX_MONTHS][MAX_DAYS] = {0}; // Declare saintsData to parse from .txt
    if (parseAndStoreSaints(filePath, saintsData) < 0)
    {
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    printAllSaints(saintsData);
#endif

    // INTERFACE LOGIC
    int selection = 0;
    int mainEntered = 0;
    bool selectionProcessed = false;

    while (true && (mainEntered++ >= 0))
    {
 
        if (mainEntered > 1 && selectionProcessed){
            handlePostSelection(&selectionProcessed, &selection);
        }
                
        if (selection == 0 || selection > MAX_CASES){
            clrscr();
            printf("Inserire numero: ");
            scanf("%d", &selection);
        }

        switch (selection)
        {
        case MANUAL_SELECT:
        {
            
            clrscr();
            printf("La tua scelta e': selezione manuale.\n");

            char input[20];
            printf("Inserire mese (numero o nome): ");
            scanf("%19s", input);           
            int month = getMonthFromInput(input);

            if (month == -1)
            {
                clrscr();
                printf("Mese non valido.\n");
                selectionProcessed = 1;
                break;
            }
            
            int day = 0;
            printf("Inserire giorno: ");
            scanf("%d", &day);
            if (day<1 || day>31){
                clrscr();
                printf("Giorno non valido.\n");
                selectionProcessed = 1;
                break;
            }
            clrscr();

            printf("Santo del %d/%d: %s\n", day, month, getSanto(saintsData, month, day));
            selectionProcessed = 1;

            break;
        case BLANK:
            clrscr();
            printf("case 2\n");
            selectionProcessed = 1;
            break;
        default:
            clrscr();
            printf("Selezione non valida.\n");
            selectionProcessed = 1;
            break;
        }
        }
    }

    return EXIT_FAILURE;
}
