#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "view.c"

// Function declarations
void inputMeal(FILE *fDiary, int *mealNumber, int *dayNumber);
void clearDiary(FILE *fDiary, int *mealNumber, int *dayNumber);
int choice(int go, FILE *fDiary, int *mealNumber, int *dayNumber);
void addToDatabase(const char *foodName);
int isFoodInDatabase(const char *foodName, float *calories, float *carbohydrates, float *fats, float *protein);
void saveState(int dayNumber, int mealNumber);
void loadState(int *dayNumber, int *mealNumber);
void editGoal(FILE *fDiary, int *mealNumber, int *dayNumber);

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inputMeal(FILE *fDiary, int *mealNumber, int *dayNumber) {
    system("cls");
    char foodName[100];
    float grams;
    float calories = 0, carbohydrates = 0, fats = 0, protein = 0;
    printBorder(107,26,0);
    gotoxy(15,8);
    printf("I N P U T   F O O D   F O R   D A Y   %d   M E A L   %d :", *dayNumber, *mealNumber);

    // Prompt for grams
    gotoxy(15,13);
    printf("E N T E R   N O .   O F   G R A M S: ");

    gotoxy(71,8);
    fgets(foodName, sizeof(foodName), stdin);
    foodName[strcspn(foodName, "\n")] = 0; // Remove the newline character

    gotoxy(52,13);
    scanf("%f", &grams);

    // Clear input buffer after taking numerical input
    clearInputBuffer();

    // Check if the food is in the database
    if (!isFoodInDatabase(foodName, &calories, &carbohydrates, &fats, &protein)) {
        // If not in database, prompt user for nutritional info and add to database
        addToDatabase(foodName);
        // Re-check to get the new values
        isFoodInDatabase(foodName, &calories, &carbohydrates, &fats, &protein);
    }

    // Calculate the values based on grams
    float totalCalories = (calories * grams) / 100;
    float totalCarbohydrates = (carbohydrates * grams) / 100;
    float totalFats = (fats * grams) / 100;
    float totalProtein = (protein * grams) / 100;

    // Write meal information to diary
    fDiary = fopen("Diary.txt", "a");
    fprintf(fDiary, "%d,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f\n", *dayNumber, *mealNumber, foodName, grams, totalCalories, totalCarbohydrates, totalFats, totalProtein);
    fclose(fDiary);

    // Ask if this is the last meal of the day

    char isLastMeal;
    gotoxy(15,20);
    printf("I S   T H I S   T H E   L A S T   M E A L   O F   T H E   D A Y ?   ( Y / N )");
    int go2 = 1;
    while (go2) {
        isLastMeal = _getch();
        if (isLastMeal == 'Y' || isLastMeal == 'y') {
            // Open the file for reading
            fDiary = fopen("Diary.txt", "r");
            if (fDiary == NULL) {
                printf("Error opening diary file.\n");
                return;
            }

            int today, meal;
            float totCal = 0, totCarb = 0, totFat = 0, totPro = 0;
            float G, CA, C, F, P;
            char food[100], line[256];

            while (fgets(line, sizeof(line), fDiary)) {
                sscanf(line, "%d,%d,%99[^,],%f,%f,%f,%f,%f", &today, &meal, food, &G, &CA, &C, &F, &P);
                if (today == *dayNumber) {
                    totCal += CA;
                    totCarb += C;
                    totFat += F;
                    totPro += P;
                }
            }
            fclose(fDiary);

            // Open the file for appending the totals
            fDiary = fopen("Diary.txt", "a");
            fprintf(fDiary, "%d,0, ,0,%.2f,%.2f,%.2f,%.2f\n", *dayNumber, totCal, totCarb, totFat, totPro);
            fclose(fDiary);

            (*dayNumber)++;
            *mealNumber = 1;
            go2 = 0;
        } else if (isLastMeal == 'N' || isLastMeal == 'n') {
            (*mealNumber)++;
            go2 = 0;
        }
    }
    saveState(*dayNumber, *mealNumber);
}

void clearDiary(FILE *fDiary, int *mealNumber, int *dayNumber) {
    char sure;
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n");
    printf("\033[0;33m");
    printf("%-20cA R E   Y O U   S U R E   Y O U   W A N T   T O   C L E A R   D I A R Y ?\n\n\n",' ');
    printf("\033[0m");
    printf("%-50c( Y / N )",' ');

    printBorder(107,26,0);
    int go2 = 1;
    while (go2) {
        sure = _getch();
        if (sure == 'Y' || sure == 'y') {
            FILE *fDiary = fopen("diary.txt", "w");
            saveState(1, 1);
            loadState(dayNumber, mealNumber);
            fclose(fDiary);
            go2 = 0;

            FILE *fgoal = fopen("goal.txt","w");
            fprintf(fgoal,"2130,186,83,160");
            fclose(fgoal);
        } else if (sure == 'N' || sure == 'n') {
            go2 = 0;
        }
    }
}

int choice(int go, FILE *fDiary, int *mealNumber, int *dayNumber) {
    char c;
    c = _getch();
    switch (c) {
        case '1':
            inputMeal(fDiary, mealNumber, dayNumber);
            break;
        case '2':
            viewDiary(fDiary,dayNumber);
            break;
        case '3':
            viewDatabase();
            break;
        case '4':
            clearDiary(fDiary, mealNumber, dayNumber);
            break;
        case '5':
            editGoal(fDiary, mealNumber, dayNumber);
            break;
        case 27:
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\n\n");
            printf("\033[0;33m");
            printf("%-27cA R E   Y O U   S U R E   Y O U   W A N T   T O   E X I T ?\n\n\n",' ');
            printf("\033[0m");
            printf("%-50c( Y / N )",' ');
            printBorder(107,26,0);
            int go2 = 1;
            while (go2) {
                int exitba = _getch();
                if (exitba == 'Y' || exitba == 'y' || exitba == 27  ) {
                    saveState(*dayNumber, *mealNumber);
                    go = 0;
                    go2 = 0;
                    system("cls");
                    printf("\n\n\n\n\n\n\n\n\n\n\n");
                    printf("\033[0;33m");
                    printf("%-33cC L O S E D   C A L O R I E   T R A C K E R . . .\n\n\n",' ');
                    printf("\033[0m");
                    printf("%-50c( Y / N )",' ');
                    printBorder(107,26,0);

                    gotoxy(0,30);
                } else if (exitba == 'N' || exitba == 'n') {
                    go = 1;
                    go2 = 0;

                }
            }
            break;
        default:
            break;
    }
    return go;
}

void addToDatabase(const char *foodName) {
    char choice;
    int calories, carbohydrates, fats, protein;
    system("cls");
    printBorder(102,26,0);
    gotoxy(24,5);
    printf("T H E   F O O D   I S   N O T   I N   T H E   D A T A B A S E ");
    gotoxy(15,9);
    printf("Please enter nutritional information for %s (per 100 grams):\n", foodName);
    gotoxy(20,11);
    printf("Calories      : ");
    gotoxy(20,12);
    printf("Carbohydrates : ");
    gotoxy(20,13);
    printf("Fats          : ");
    gotoxy(20,14);
    printf("Protein       : ");

    gotoxy(36,11);
    scanf("%d", &calories);
    gotoxy(36,12);
    scanf("%d", &carbohydrates);
    gotoxy(36,13);
    scanf("%d", &fats);
    gotoxy(36,14);
    scanf("%d", &protein);

    gotoxy(15,17);
    printf("A D D   %s   T O   D A T A B A S E ?   ( Y / N )", foodName);
    choice = _getch();
    if (choice == 'Y' || choice == 'y') {
        FILE *fDatabase = fopen("database.txt", "a");
        fprintf(fDatabase, "%s,%d,%d,%d,%d\n", foodName, calories, carbohydrates, fats, protein);
        fclose(fDatabase);
    }
    clearInputBuffer();
}

int isFoodInDatabase(const char *foodName, float *calories, float *carbohydrates, float *fats, float *protein) {
    FILE *fDatabase = fopen("database.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), fDatabase)) {
        char name[100];
        sscanf(line, "%99[^,],%f,%f,%f,%f", name, calories, carbohydrates, fats, protein);
        if (stricmp(name, foodName) == 0) {
            fclose(fDatabase);
            return 1;
        }
    }
    fclose(fDatabase);
    return 0;
}

void saveState(int dayNumber, int mealNumber) {
    FILE *fState = fopen("state.txt", "w");
    if (fState != NULL) {
        fprintf(fState, "%d %d", dayNumber, mealNumber);
        fclose(fState);
    }
}

void loadState(int *dayNumber, int *mealNumber) {
    FILE *fState = fopen("state.txt", "r");
    if (fState != NULL) {
        fscanf(fState, "%d %d", dayNumber, mealNumber);
        fclose(fState);
    }
}

void editGoal(FILE *fDiary, int *mealNumber, int *dayNumber) {
    system("cls");
    float height,weight,bmr,tdee,age,goal,carbGoal,fatGoal,proteinGoal,cM,fM,pM;
    char sex;
    int level,diet,exit=1;
    printf("\n\n\n\tE N T E R   T H E   F O L L O W I N G   I N F O R M A T I O N\n\n");
    printf("\t\tWeight (kgs)      : \n");
    printf("\t\tHeight (cms)      : \n");
    printf("\t\tAge               : \n");
    printf("\t\tSex (M/F)         : \n");
    printf("\t\tGoal weight (kgs) : \n");

    printf("\n\tE N T E R   Y O U R   A C T I V I T Y   L E V E L : \n\n\t\tSedentary (office job) - 1\n");
    printf("\t\tLightly Active (1-2 days per week) - 2\n");
    printf("\t\tModerately Active (3-5 days per week) - 3\n");
    printf("\t\tVery Active (5-7 days per week) - 4\n");
    printf("\t\tSuper Active (2 times per day) - 5\n");

    printf("\n\tS E L E C T   Y O U R   D E S I R E D   D I E T   (Carbs/Fats/Protein) :\n\n\t\tModerate Carb (35/35/30) - 1\n\t\tLower Carb (20/40/40) - 2\n\t\tHigher Carb (50/20/30) - 3");

    printBorder(107,26,0);

    gotoxy(36,5);
    scanf("%f",&weight);
    gotoxy(36,6);
    scanf("%f",&height);
    gotoxy(36,7);
    scanf("%f",&age);


    gotoxy(36,8);
    scanf("% c",&sex);
    clearInputBuffer();

    gotoxy(36,9);
    scanf("%f",&goal);

    bmr=(10*weight)+(6.25*height)-(5*age)+(5);

    if(sex=='M'||sex=='m')
        bmr+=100;
    else if(sex=='F'||sex=='f')
        bmr-=100;

    gotoxy(60,11);
    scanf("%d",&level);

    switch(level) {
        case 1:
            tdee=bmr*1.2;
            break;
        case 2:
            tdee=bmr*1.375;
            break;
        case 3:
            tdee=bmr*1.55;
            break;
        case 4:
            tdee=bmr*1.725;
            break;
        case 5:
            tdee=bmr*1.9;
            break;
        default:
            break;
    }
    gotoxy(81,19);
    scanf("%d",&diet);

    if(diet==1) {
        pM=0.30/4;
        fM=0.35/9;
        cM=0.35/4;
    }
    else if(diet==2) {
        pM=0.40/4;
        fM=0.40/9;
        cM=0.20/4;
    }
    else if(diet==3) {
        pM=0.30/4;
        fM=0.20/9;
        cM=0.50/4;
    }

    proteinGoal=tdee*pM;
    fatGoal=tdee*fM;
    carbGoal=tdee*cM;

    system("cls");

    printBorder(107,26,0);

    if(goal<weight){
        tdee-=500;
        gotoxy(8,6);
        printf("Y O U   N E E D   T O   G O   O N   A   C U T ! ");
        gotoxy(8,9);
        printf("You have to eat at a DEFICIT of 500 calories to lose weight.\n");
        gotoxy(8,11);
        printf("You would need a total of %.2f calories of food per day!",tdee);
    }
    else if(goal>weight){
        tdee+=500;
        gotoxy(8,6);
        printf("Y O U   N E E D   T O   G O   O N   A   B U L K ! ");
        gotoxy(8,9);
        printf("You have to eat at a SURPLUS of 500 calories to gain weight.\n");
        gotoxy(8,11);
        printf("You would need a total of %.2f calories of food per day!",tdee);
    }
    else {
        gotoxy(8,6);
        printf("Y O U   J U S T   N E E D   T O   E A T   O N   A   M A I N T E N A N C E !");
        gotoxy(8,9);
        printf("You have to eat at equal to your TDEE to maintain weight.\n");
        gotoxy(8,11);
        printf("You would need a total of %.2f calories of food per day!",tdee);
    }
    gotoxy(8,13);
    printf("You would need %.2fg of carbs, %.2fg of fats, and %.2fg of protein.",carbGoal,fatGoal,proteinGoal);

    //printf("h=%.2f, w=%.2f, a=%.2f, s=%c, g=%.2f, b=%.2f, t=%.2f, l=%d,d=%d", weight, height,age,sex,goal,bmr,tdee,level,diet);
    FILE *fGoal = fopen("goal.txt", "w");
    if(fGoal != NULL) {
        fprintf(fGoal, "%f,%f,%f,%f", tdee,carbGoal,fatGoal,proteinGoal);
    }
    fclose(fGoal);
    gotoxy(8,20);
    printf("Press [ESC] to exit.");

    char c;
    while(exit) {
        c=_getch();
        if(c==27)
            exit=0;
    }
}

int main() {
    FILE *fDiary;
    int go = 1,ctr=0;
    int mealNumber;
    int dayNumber;

    loadState(&dayNumber, &mealNumber);

    while (go) {
        system("cls");
        printf("\n\n\n\n");

        printf("%-47cW E L C O M E   TO\n",' ');


        printf("\033[33m");
        printf("  %-28c____      _      _        ___    ____    ___   _____  \n",' ');
        printf("%-28c / ___|    / \\    | |      / _ \\  |  _ \\  |_ _| | ____| \n",' ');
        printf("%-28c| |       / _ \\   | |     | | | | | |_) |  | |  |  _|   \n",' ');
        printf("%-28c| |___   / ___ \\  | |___  | |_| | |  _ <   | |  | |___  \n",' ');
        printf("%-28c \\____| /_/   \\_\\ |_____|  \\___/  |_| \\_\\ |___| |_____| \n",' ');
        printf(" %-28c_____   ____       _       ____   _  __  _____   ____  \n",' ');
        printf("%-28c|_   _| |  _ \\     / \\     / ___| | |/ / | ____| |  _ \\ \n",' ');
        printf("  %-28c| |   | |_) |   / _ \\   | |     | ' /  |  _|   | |_) |\n",' ');
        printf("  %-28c| |   |  _ <   / ___ \\  | |___  | . \\  | |___  |  _ < \n",' ');
        printf("  %-28c|_|   |_| \\_\\ /_/   \\_\\  \\____| |_|\\_\\ |_____| |_| \\_\\\n",' ');
        printf("\033[0m");

        printf("\n\n%-45cWhat do you want to do?\n\n",' ');
        printf("%-47c1] Input Meal\n",' ');
        printf("%-47c2] View Diary\n",' ');
        printf("%-47c3] Access Food Database\n",' ');
        printf("%-47c4] Clear Diary\n",' ');
        printf("%-47c5] Edit Goal\n",' ');
        printf("%-47cESC] Exit\n\n",' ');

        printBorder(107,26,0);

        go = choice(go, fDiary, &mealNumber, &dayNumber);
    }
    
    return 0;
}
