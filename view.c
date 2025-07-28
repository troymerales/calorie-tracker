#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "view.h"

//HEADER FILE FOR VIEWING STUFF

static FoodItem* foodarray = NULL;
static int foodCount = 0;
static int foodCapacity = 10;

void initializeFoodArray() {
    foodarray = (FoodItem*)malloc(foodCapacity * sizeof(FoodItem));
    if (!foodarray) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void addFoodItem(FoodItem item) {
    if (foodCount >= foodCapacity) {
        foodCapacity *= 2;
        foodarray = (FoodItem*)realloc(foodarray, foodCapacity * sizeof(FoodItem));
        if (!foodarray) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
    foodarray[foodCount++] = item;
}

void freeFoodarray() {
    free(foodarray);
    foodarray = NULL;
    foodCount = 0;
    foodCapacity = 10;
}

/*void viewWholeDiary(FILE *fDiary) {
    system("cls");
    if (fDiary == NULL) {
        printf("Diary is empty.\n");
        return;
    }
    int go2=1,cou=0;
    char exitba;
    while(go2) {
        FoodItem item;
        char line[256];

        initializeFoodArray();

        while (fgets(line, sizeof(line), fDiary)) {
            sscanf(line, "%d,%d,%99[^,],%f,%f,%f,%f,%f",&item.day, &item.meal, item.food,&item.grams, &item.calories,&item.carbs, &item.fats, &item.protein);
            addFoodItem(item);
            cou++;
        }
        fclose(fDiary);

        printf("\n\n\t\033[1;36m[ESC] - Back\n\n\t\033[1;33mDAY   MEAL   FOOD             GRAMS (g)     CALORIES      CARBS (g)     FATS (g)      PROTEIN (g)\n\n");
        printf("\033[0m");

        // Process and display foodArray as needed
        for (int i = 0; i < foodCount; i++) {
            if (foodArray[i].meal == 0) {
                // Print the total for the day
                printf("\t%-4d  %-5s  %-15s  %-12s  %-12.2f  %-12.2f  %-12.2f  %-10.2f\n", foodArray[i].day, "", "T  O  T  A  L", "", foodArray[i].calories, foodArray[i].carbs, foodArray[i].fats,foodArray[i].protein);
            } else {
                // Print the regular meal entry
                printf("\t%-4d  %-5d  %-15s  %-12.2f  %-12.2f  %-12.2f  %-12.2f  %-10.2f\n", foodArray[i].day, foodArray[i].meal, foodArray[i].food, foodArray[i].grams, foodArray[i].calories, foodArray[i].carbs, foodArray[i].fats, foodArray[i].protein);
            }
            printf("%d %d %s %.2f %.2f %.2f %.2f %.2f\n",
                   foodArray[i].day, foodArray[i].meal, foodArray[i].food,
                   foodArray[i].grams, foodArray[i].calories,
                   foodArray[i].carbs, foodArray[i].fats, foodArray[i].protein);
        }

        freeFoodArray();

        if(cou<26)
            cou=21;
        printBorder(107,cou+5,1);

        exitba = _getch();
        if (exitba == 27)
            go2 = 0;
    }
}
*/

//PRINTING THE BORDER
void printBorder(int l, int h,int header) {
    printf("\033[0;34m");
    for(int ctr=1; ctr<=h; ctr++) {
        for(int ctr2=4;ctr2<=l;ctr2++) {
            if(ctr==1) {
                if(ctr2==4)
                    aprintf(201,ctr2,ctr);
                else if(ctr2==l)
                    aprintf(187,ctr2,ctr);
                else
                    aprintf(205,ctr2,ctr);
            }
            else if(ctr==h) {
                if(ctr2==4)
                    aprintf(200,ctr2,ctr);
                else if(ctr2==l)
                    aprintf(188,ctr2,ctr);
                else
                    aprintf(205,ctr2,ctr);
            }
            else {
                if(ctr2==4)
                    aprintf(186,ctr2,ctr);
                else if(ctr2==l)
                    aprintf(186,ctr2,ctr);
            }
        }
    }
    if(header) {
        for(int a=4;a<=107;a++) {
            if(a==4)
                aprintf(204,a,5);
            else if(a==107)
                aprintf(185,a,5);
            else
                aprintf(196,a,5);
        }
        for(int a=4;a<=107;a++) {
            if(a==4)
                aprintf(204,a,3);
            else if(a==107)
                aprintf(185,a,3);
            else
                aprintf(205,a,3);
        }
    }
    printf("\033[0m");
}

//SPECIAL FUNCTION TO NAVIGATE AROUND THE TERMINAL
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//A QUICK WAY OF PRINTING AN ASCII WITH COORDINATES
void aprintf(int a, int x, int y) {
    gotoxy(x, y);
    printf("%c", a);
    gotoxy(0,0);
}

//VIEW DIARY FOR TODAY'S ENTRIES
void viewDiary (FILE *fDiary, int *dayNumber) {
    int go2 = 1, exitba,cou=0;

    while (go2) {
        float tCa=0,tC=0,tF=0,tP=0;
        system("cls");
        char line[256];

        fDiary = fopen("Diary.txt", "r");
        if (fDiary == NULL) {
            printf("Diary is empty.\n");
            return;
        }
        printf("\n\n\t\033[1;36m[1] Whole Diary %-64c[ESC] Menu\n\n\t\033[1;33mMEAL   FOOD             GRAMS         CALORIES (g)  CARBS (g)     FATS (g)      PROTEIN (g)\n\n",' ');
        printf("\033[0m");
        while (fgets(line, sizeof(line), fDiary)) {
            cou++;
            int day, meal;
            float grams, calories, carbs, fats, protein;
            char food[100];
            sscanf(line, "%d,%d,%99[^,],%f,%f,%f,%f,%f", &day, &meal, food, &grams, &calories, &carbs, &fats, &protein);
            if (day == *dayNumber) {
                tCa+=calories;
                tC+=carbs;
                tF+=fats;
                tP+=protein;
                printf("\t%-5d  %-15s  %-12.2f  %-12.2f  %-12.2f  %-12.2f  %-10.2f\n", meal, food, grams, calories, carbs, fats, protein);
            }
        }
        if(cou<20)
            cou=26;
        else cou+10;

        gotoxy(8,cou-6);
        printf("T O T A L   F O R   D A Y   %d %-6.2c  %-12.2f  %-12.2f  %-12.2f  %-10.2f\n",*dayNumber,' ', tCa, tC, tF, tP);
        fclose(fDiary);

        FILE *fRem= fopen("goal.txt","r");
        float calGoal, carbGoal, fatGoal, proteinGoal;
        fscanf(fRem, "%f,%f,%f,%f", &calGoal, &carbGoal, &fatGoal, &proteinGoal);

        gotoxy(8,cou-4);
        printf("D A I L Y   G O A L                   %-12.2f  %-12.2f  %-12.2f  %-10.2f\n",calGoal, carbGoal, fatGoal, proteinGoal);

        gotoxy(8,cou-2);

        printf("R E M A I N I N G                     ");

        if(0>calGoal-tCa)
            printf("\033[0;31m");
        else
            printf("\033[0;32m");
        printf("%-14.2f",calGoal-tCa);


        if(0>carbGoal-tC)
            printf("\033[0;31m");
        else
            printf("\033[0;32m");
        printf("%-14.2f",carbGoal-tC);


        if(0>fatGoal-tF)
            printf("\033[0;31m");
        else
            printf("\033[0;32m");
        printf("%-14.2f",fatGoal-tF);


        if(0>proteinGoal-tP)
            printf("\033[0;31m");
        else
            printf("\033[0;32m");
        printf("%-10.2f",proteinGoal-tP);

        printf("\033[0m");

        fclose(fRem);
        printBorder(107,cou,1);
        exitba = _getch();
        if (exitba == 27)
            go2 = 0;
        else if (exitba == 49)
            viewWholeDiary (fDiary);
    }
}

//VIEW DATABASE FILE
void viewDatabase() {
    system("cls");
    FILE *fDatabase = fopen("database.txt", "r");
    if (fDatabase == NULL) {
        printf("Database is empty.\n");
        return;
    }

    typedef struct {
        char food[100];
        float calories;
        float carbs;
        float fats;
        float protein;
    } FoodItem;

    FoodItem foodArray[1000];
    int count = 0;
    char line[256];

    // Read the database into the array
    while (fgets(line, sizeof(line), fDatabase) && count < 1000) {
        sscanf(line, "%99[^,],%f,%f,%f,%f", foodArray[count].food, &foodArray[count].calories, &foodArray[count].carbs, &foodArray[count].fats, &foodArray[count].protein);
        count++;
    }
    fclose(fDatabase);

    // Default sorting by food name (alphabetically)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(foodArray[i].food, foodArray[j].food) > 0) {
                FoodItem temp = foodArray[i];
                foodArray[i] = foodArray[j];
                foodArray[j] = temp;
            }
        }
    }

    int choice, go2 = 1;

    while (go2) {
        system("cls");
        printf("\n\n\t\033[1;36mSORT BY -> \t [1]\t    [2]\t\t    [3]\t\t    [4]\t\t    [5]%-14c[ESC]", ' ');
        printf("\n\n\t\033[1;33mFOOD (per 100 grams)        CALORIES        CARBS (g)       FATS (g)        PROTEIN (g)  \n\n");
        printf("\033[0m");

        for (int i = 0; i < count; i++) {
            printf("\t%-27s %-15.2f %-15.2f %-15.2f %-10.2f\n", foodArray[i].food, foodArray[i].calories, foodArray[i].carbs, foodArray[i].fats, foodArray[i].protein);
        }

        choice = _getch();

        // Sort the array based on the user's choice
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                int shouldSwap = 0;
                switch (choice) {
                    case '2':
                        shouldSwap = foodArray[i].calories < foodArray[j].calories;
                        break;
                    case '3':
                        shouldSwap = foodArray[i].carbs < foodArray[j].carbs;
                        break;
                    case '4':
                        shouldSwap = foodArray[i].fats < foodArray[j].fats;
                        break;
                    case '5':
                        shouldSwap = foodArray[i].protein < foodArray[j].protein;
                        break;
                    case '1':
                        shouldSwap = strcmp(foodArray[i].food, foodArray[j].food) > 0;
                        break;
                    case 27:
                        go2 = 0;
                        break;
                    default:
                        break;
                }
                if (shouldSwap) {
                    FoodItem temp = foodArray[i];
                    foodArray[i] = foodArray[j];
                    foodArray[j] = temp;
                }
            }
        }
    }
}

//VIEW EVERY ENTRY
void viewWholeDiary(FILE *fDiary) {
    int go2 = 1, exitba,cou=0;
    while (go2) {
        system("cls");
        char line[256];

        fDiary = fopen("Diary.txt", "r");
        if (fDiary == NULL) {
            printf("Diary is empty.\n");
            return;
        }

        printf("\n\n\t\033[1;36m[ESC] - Back\n\n\t\033[1;33mDAY   MEAL   FOOD             GRAMS (g)     CALORIES      CARBS (g)     FATS (g)      PROTEIN (g)\n\n");
        printf("\033[0m");

        while (fgets(line, sizeof(line), fDiary)) {
            cou++;
            int day, meal;
            float grams, calories, carbs, fats, protein;
            char food[100];
            sscanf(line, "%d,%d,%99[^,],%f,%f,%f,%f,%f", &day, &meal, food, &grams, &calories, &carbs, &fats, &protein);

            if (meal == 0) {
                // Print the total for the day
                printf("\t%-4d  %-5s  %-15s  %-12s  %-12.2f  %-12.2f  %-12.2f  %-10.2f\n", day, "", "T  O  T  A  L", "", calories, carbs, fats, protein);
            } else {
                // Print the regular meal entry
                printf("\t%-4d  %-5d  %-15s  %-12.2f  %-12.2f  %-12.2f  %-12.2f  %-10.2f\n", day, meal, food, grams, calories, carbs, fats, protein);
            }
        }
        fclose(fDiary);
        if(cou<26)
            cou=21;
        printBorder(107,cou+5,1);

        exitba = _getch();
        if (exitba == 27)
            go2 = 0;
    }
}

