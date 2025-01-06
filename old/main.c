#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int pos;
int pre;
int suf;
int sesso;
int stoire_month;
int stoire_day;
char selection[100];


char* prefixes[]= {"Porco ","Bastardo ","Boia ", "Porca ","Bastarda ","Boia ", NULL};
char* prefixes_2[]= {"Porco ","Bastardi ","Boia ", "Porco ","Bastardi ","Boia ", NULL};
char* suffixes[]= {" pompinaro", " gay", " bestia", " elettricista fuori fase", " epilettico", " leghista convinto", " monco",
                   " pedofilo", " troglodita", " x-men", " sei volte porco per ogni atomo", ".EXE", " impiccato al contrario",
                   " fascista", " nazista", " comunista", " negro", " cinese", " indiano", "troia",
                   //femminile
                   " pompinara", " gay", " bestia", " elettricista fuori fase", " epilettica", " leghista convinta", " monca",
                   " pedofila", " troglodita", " x-men", " sei volte porca per ogni atomo", ".EXE", " impiccata al contrario",
                   " fascista", " nazista", " comunista", " negra", " cinese", " indiana", " troia", NULL};
char* suffixes_2[] = {"emerito coglione", "stronzo", "bastardo"
                      "emerita puttana", "stronza", "bastarda", NULL}    ;
char* gender[]= {"San ","Santa ","Sant'"};
char* mesi[] = {"gennaio","febbraio","marzo","aprile", "maggio", "giugno", "luglio", "agosto", "settembre", "ottobre", "novembre", "dicembre"};
char* santi[]=
{
    //gennaio
    "Telemaco","Adelardo","xGenoveffa","xBenedetta", "Astolfo", "Gaspare", "Crispino", "Erardo", "xAlessia", "Aldo", "xOnorata", "Arcadio",
    "xLiana","xBianca","Abaco","Accursio", "xAlba", "xBeatrice", "Mario", "Sebastiano",
    "xAgnese", "Domenico", "Durante", "xBabila", "Agape", "Alberico", "xDevota", "Amedeo", "Cosatnzo", "xBatilda", "Ciro",
    //febbraio
    "xBrigida","Sabatino","Biagio","Gilberto", "xAgata", "xAmanda", "xGiuliana", "Gerolamo", "Rinaldo", "Guglielmo", "Dante",
    "Costante", "xFosca","Cirillo","xGiorgia","Samuele", "Flaviano", "xArtemisia", "Alvaro", "Eleuterio", "xEleonora", "Ariosto", "xRomina",
    "xFiorentina", "Romeo", "Cornelio", "Antigone", "xAntonia", "Augusto", "", "",
    //marzo
    "Silvio","xDelinda","xCamelia","Nestore", "Adriano", "Ezio", "xPerpetua", "xBeata", "xFrancesca", "Crescente", "Ramiro", "Aureliano",
    "xEufrasia","xMatilde","xLucrezia","Abramio", "Patrizio", "Salvatore", "Giuseppe", "xAlessandra", "Serapione", "xLea", "Turibio",
    "xAttilia", "xAnnunziata", "Emanuele", "Arcibaldo", "Doroteo", "Secondo", "Decimo", "xCornelia",
    //aprile
    "xDora","Abbondio","Riccardo","Isidoro", "Diogene", "Celestino", "Cristiano", "Dionigi", "Cleo", "Ezechiele", "Angelo", "Alfiero",
    "Marzio","Ermenegildo","xAnastasia","xGrazia", "Aniceto", "Galdino", "xEmma", "xAdalgisa", "Caio", "Leonida", "Adalberto", "xMelissa",
    "Baldassarre", "xAlida", "Antimo", "xValeria", "xCaterina", "xRosamunda", "",
    //maggio
    "Achille","Celeste","xViola", "Fulvio", "Gottardo", "xPrudenzia", "Augusto", "Bonifacio", "xLuminosa", "Cataldo", "xStella", "Danio",
    "xFatima","Ampelio","xDionisia","Adamo", "xBasilia", "xErica", "xGisella", "Anastasio", "Adalrico", "xGiulia", "Desiderio", "Albano",
    "Adelmo", "Filippo", "Oliviero", "Emilio", "Massimino", "xGiovanna", "Battista",
    //giugno
    "xErmenegilda","Alcibiade","xClotilde","xIsabella", "Fernando", "Norberto", "Landolfo", "Dolcelino", "Efrem", "xDiana", "Barnaba", "Guido",
    "Antonio","xRosmunda","xGermana","Ferruccio", "Adolfo", "Calogero", "Gervasio", "xConsolata", "Luigi", "Eberardo", "xAgrippina", "Gianbattista",
    "Eraldo", "Deodato", "Ladislao", "xEraclide", "Pietro", "Ottone", "",
    //luglio
    "Arsenio","Santo","Tommaso","xBetta", "xFilomena", "Isaia", "Villibaldo", "Edgardo", "xAnatolia", "Michelangelo", "xAmabile", "Ermagora",
    "Enrico","Camillo","Amerigo","Carmelo", "xGenerosa", "Federico", "xGiusta", "Elia", "Daniele", "Amore", "Apollinare", "xCristina",
    "Cristoforo", "xAnna", "Arnaldo", "Nazario", "xMarta", "Abdone", "xDaniela",
    //agosto
    "Alfonso","xAlfreda","Dalmazio","Agabio", "xAbele", "Donato", "Benedetto Xi", "Ciriaco", "Romano", "Lorenzo", "xChiara", "Ercole",
    "xConcordia","xAtanasia","xAchiropita","Rocco", "Giacinto", "Aimone", "xItalia", "Bernardo", "Baldovino", "Aldobrando", "Benito", "xAnita",
    "Clodoveo", "Oronzo", "xMonica", "Agostino", "Adelfo", "xGaudenzia", "xAida",
    //settembre
    "Egidio","Ottaviano","xClelia","xDina", "Berto", "xEva", "Guido", "xAddolorata", "Omero", "xCandida", "Almiro", "xMaria", "Amato","Crescenzio",
    "Baldo","Cipriano", "xColomba", "xArianna", "Elio", "xFilippa", "Matteo", "Maurizio", "Libero", "xAmata", "xAurelia", "xCosma", "Fidenzio",
    "Venceslao", "Alarico", "Dino", "",
    //ottobre
    "Remigio","Berengario","Candido","Francesco", "Attila", "Alberta", "Adelchi", "Ivano", "Abramo", "Alderico", "Emanuela", "Amico", "Edoardo",
    "Callisto","xEdda","Edvige", "Rodolfo", "Luca", "xLaura", "xAurora", "Bertoldo", "Apollo", "Manlio", "Raffaele", "Gavino", "Evaristo", "Fiorenzo",
    "Simone", "xErmelinda", "xBenvenuta", "Volfango",
    //novembre
    "yTutti I Santi","Tobia","Tobia","Amauri", "xElisa", "Leonardo", "xCarina", "Goffredo", "Teodoro", "xFiorenza", "Martino", "Aurelio", "xAgostina",
    "Venerando","Alberto","xGeltrude", "Eugenio", "Noe'", "Fausto", "Benigno", "Celso", "xCecilia", "xAdelinda", "xFlora", "xCaterina", "Delfino",
    "Gustavo", "xTeodora", "Fedro", "Andrea", "",
    //dicembre
    "Eligio","xViviana","Cassiano","xAda", "xCrispina", "xAngelica", "Ambrogio", "xConcetta", "Amleto", "Amanzio", "Damaso", "xLena", "Antioco",
    "Pompeo","xCristiana","xAdelaide", "Lazzaro", "Graziano", "Dario", "Liberato", "Temistocle", "Demetrio", "Dagoberto", "xAdele", "Nerea",
    "Stefano", "xDelfina", "xDonna", "Davide", "Raniero", "xIlaria"

};

int length_array (char *includes[]) {
    size_t count = 0;
    while (includes[count] != NULL) count++;
    return count;
}


void delay(int milli_seconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

int get_saint(int giorno, int mese) {
    int position = 31*mese-31 + giorno - 1;
    return position;
}

int position_get() {
    int pos = get_saint(stoire_day, stoire_month);
    return pos;
}

int genderator() {
    int sex;
    char start = santi[pos][0];
    if (start == 'x')
    {
        sex = 1;
        start = santi[pos][1];
    }
    else if(start == 'A'|| start == 'E'|| start == 'I'|| start == 'O'|| start == 'U')
    {
        sex = 2;
    }
    else
    {
        sex = 0;
    }
    return sex;
}
int random_number(int min, int max) {
    int num = (rand() % (max - min + 1)) + min;
    //printf("%d\n", num);
    return num;

}

int prefixerator() {
    int length = length_array(prefixes);

    int pre = random_number(0, ((length-1)/2));
    //printf("%d", pre);
    if(sesso == 1) {
        pre = pre + (((length-1)/2)+1);
    }
    //printf("%d", pre);
    return pre;
}

int suffixerator() {
    int length = length_array(suffixes);
    suf = random_number(0, ((length-1)/2));
    if(sesso == 1) {
        suf = suf + (((length-1)/2)+1);
    }
    return suf;
}

int suffixerator_2() {
    int length = length_array(suffixes_2);
    suf = random_number(0, ((length-1)/2));
    if(sesso == 1) {
        suf = suf + (((length-1)/2)+1);
    }
    return suf;
}

void make_saint() {
    pos = position_get();
    sesso = genderator();
    pre = prefixerator();
    suf = suffixerator();
    if(sesso == 0||sesso == 2)
        printf("\nSanto selezionato\t\t\t\t| %s%s",prefixes[pre], gender[sesso]);
    else if(sesso == 1)
        printf("\nSanta selezionata\t\t\t\t| %s%s",prefixes[pre], gender[sesso]);
    if (sesso == 1) {
        for(long unsigned int i = 1; i < strlen(santi[pos]); i++) {
            printf("%c", santi[pos][i]);
        }
    } else {
        printf("%s", santi[pos]);
    }
    printf("%s\n\n", suffixes[suf]);
    printf("-------------------------------------------------------------------------------------\n");
}

void date_saint() {
    printf("\nInserire il giorno:\t\t\t\t| ");
    scanf("%d", &stoire_day);
    printf("Inserire il mese:\t\t\t\t| ");
    scanf("%d", &stoire_month);
    make_saint();
    printf("-------------------------------------------------------------------------------------\n");
}

void random_saint() {
    stoire_day = random_number(1, 29);
    stoire_month = random_number(1, 12);
    make_saint();
}

void day_saint() {
    struct tm *newtime;
    time_t ltime;

    time(&ltime);
    newtime = localtime(&ltime);
    char* time_string = asctime(newtime);

    char month[5];
    memcpy(month, &time_string[4], 3 );
    month[3] = '\0';
    //printf("%s", month);
    char day = time_string[9];
    int int_day = day - '0';
    //printf("%d", int_day);
    //printf("Data: %s", time_string);

    int int_month;
    char* ret;

    ret = strstr(month, "Jen");
    if(ret)
        int_month = 1;

    ret = strstr(month, "Feb");
    if(ret)
        int_month = 2;

    ret = strstr(month, "Mar");
    if(ret)
        int_month = 3;

    ret = strstr(month, "Apr");
    if(ret)
        int_month = 4;

    ret = strstr(month, "May");
    if(ret)
        int_month = 5;

    ret = strstr(month, "Jun");
    if(ret)
        int_month = 6;

    ret = strstr(month, "Jul");
    if(ret)
        int_month = 7;

    ret = strstr(month, "Aug");
    if(ret)
        int_month = 8;

    ret = strstr(month, "Sep");
    if(ret)
        int_month = 9;

    ret = strstr(month, "Oct");
    if(ret)
        int_month = 10;

    ret = strstr(month, "Nov");
    if(ret)
        int_month = 11;

    ret = strstr(month, "Dec");
    if(ret)
        int_month = 12;

//printf("%d", int_month);

    stoire_day = int_day;
    stoire_month = int_month;
    make_saint();
}

void interval_saint() {
    int stoire_day1;
    int stoire_day2;
    int stoire_month1;
    int stoire_month2;

    printf("\nInserire il primo giorno:\t\t\t| ");
    scanf("%d", &stoire_day1);
    printf("Inserire il primo mese:\t\t\t\t| ");
    scanf("%d", &stoire_month1);
    printf("\nInserire il secondo giorno:\t\t\t| ");
    scanf("%d", &stoire_day2);
    printf("Inserire il secondo mese:\t\t\t| ");
    scanf("%d", &stoire_month2);
        printf("\n-------------------------------------------------------------------------------------\n");

    int new_month = stoire_month1;
    int new_day = stoire_day1;

    do {
        stoire_day = new_day;
        stoire_month = new_month;
        make_saint();
        if(new_day==29&&new_month==2) {
            new_day = 1;
            new_month = new_month + 1;
        }
        if(new_day==30&&new_month==4||new_day==30&&new_month==4||new_day==30&&new_month==6||new_day==30&&new_month==9||new_day==30&&new_month==11) {
            new_day = 1;
            new_month = new_month + 1;
        } else if(new_day==31) {
            new_day = 1;
            new_month = new_month + 1;
        } else {
            new_day=new_day+1;
        } if(new_month==12&&new_day==31) {
            new_day = 1;
            new_month = 1;
        }
        delay(50);
    }
    while(!(new_month == stoire_month2&&new_day == stoire_day2));
}

void sainterator(){
    int n_sainterator;
    printf("\nInserire il un numero di santi desiderato:\t| ");
    scanf("%d", &n_sainterator);
        printf("\n-------------------------------------------------------------------------------------\n");
    for(int i = 0; i < n_sainterator; i++){
        random_saint();
    }
}

void porconatore(int ln){
    sesso = 0;
    pre = prefixerator();
    suf = suffixerator();
    if(ln==0)
    printf("\nBestemmia generata\t\t\t\t| %sDio%s", prefixes[pre], suffixes[suf]);
    else
    printf("\nBestemmia generata:\t\t\t\t| \n%sDio%s", prefixes[pre], suffixes[suf]);
}

void complex_porconatore(){

    sesso = 0;
    int length = length_array(prefixes_2);
    int pref = random_number(0, (length-1));

    stoire_day = random_number(1, 29);
    stoire_month = random_number(1, 12);
    pos = position_get();
    sesso = genderator();
    suf = suffixerator_2();

    printf("\nBestemmia generata:\n\n%sDio", prefixes_2[pref] );
    if(sesso == 0||sesso == 2){
        if(suffixes_2[suf][0]=='a'||suffixes_2[suf][0]=='e'||suffixes_2[suf][0]=='i'||
            suffixes_2[suf][0]=='o'||suffixes_2[suf][0]=='u'||suffixes_2[suf][0]=='x')

                printf(" e quell'%s di %s", suffixes_2[suf], gender[sesso]);
        else if(suffixes_2[suf][0]=='s')
                printf(" e quello %s di %s", suffixes_2[suf], gender[sesso]);
              else
                printf(" e quel %s di %s", suffixes_2[suf], gender[sesso]);
    }
    else if(sesso == 1)
        printf(" e quella %s di %s", suffixes_2[suf], gender[sesso]);
    if (sesso == 1) {
        for(long unsigned int i = 1; i < strlen(santi[pos]); i++) {
            printf("%c", santi[pos][i]);
        }
    } else {
        printf("%s", santi[pos]);
        }
printf("\n\n-------------------------------------------------------------------------------------\n");
}

void insult(){
    char nome[20];
    printf("\nInserire nome:\t\t\t\t\t| ");
    scanf("%s", nome);
    sesso = 0;
    pre = prefixerator();
    suf = suffixerator();

    printf("Insulto selezionato\t\t\t\t| %s%s",prefixes[pre], nome);
    printf("%s\n\n", suffixes[suf]);
    printf("-------------------------------------------------------------------------------------\n");
    memset(nome, 0, 20);
}

int main() {
    printf("-------------------------------------------------------------------------------------\n");
    printf("                                      PORCONATORE\n");
    printf("-------------------------------------------------------------------------------------\n");
start:
    printf("\nPer visualizzare la lista comandi inserire '?'\n\n");
    printf("SELEZIONE:\t\t\t\t\t| ");
    selection[0] = '\0';
    scanf("%s", selection);
        printf("\n-------------------------------------------------------------------------------------\n");
    if(selection[0] == '1') {
        date_saint();
    } else if(selection[0]=='2') {
        random_saint();
    } else if(selection[0]=='3') {
        day_saint();
    } else if(selection[0]=='4') {
        interval_saint();
    } else if(selection[0]=='5'){
        sainterator();
    } else if(selection[0]=='6') {
        porconatore(0);
    } else if(selection[0]=='7') {
        complex_porconatore();
    } else if(selection[0]=='8') {
        insult();
    } else if(selection[0]=='?' || selection[0]=='h' || selection[0]=='H') {
    //printf("-------------------------------------------------------------------------------------\n");
    printf("                                   LISTA DI COMANDI:\n");
    printf("-------------------------------------------------------------------------------------\n");
    printf("-1: Genera un santo inserendo la data\n");
    printf("-2: Genera un santo casuale\n");
    printf("-3: Genera il santo del giorno\n");
    printf("-4: Genera un intervallo di Santi\n");
    printf("-5: Genera dei santi in maniera casuale\n");
    printf("-6: Genera una bestemmia\n");
    printf("-7: Genera una bestemmia complessa\n");
    printf("-8: Genera un insulto\n\n");
    } else{
        printf("\n-------------------------------------------------------------------------------------\n");
        printf("Selezione non valida\n");
        printf("-------------------------------------------------------------------------------------\n");
    }
    goto start;
}

