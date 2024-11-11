#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_NAME_LENGTH 100
#define MAX_NAMES 1000

void getRandomName(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char names[MAX_NAMES][MAX_NAME_LENGTH];
    int count = 0;

    while (fgets(names[count], MAX_NAME_LENGTH, file) != NULL && count < MAX_NAMES) {
        // Remove newline character if present
        names[count][strcspn(names[count], "\n")] = '\0';
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("No names found in the file.\n");
        return;
    }

    srand(time(NULL));
    int randomIndex = rand() % count;
    printf("Random name: %s\n", names[randomIndex]);
}

int main() {
    printf("Press any key to get a random name from santi.txt...\n");
    getch(); // Wait for a key press
    getRandomName("santi.txt");
    return 0;
}