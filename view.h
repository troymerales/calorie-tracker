#ifndef VIEW_H
#define VIEW_H

typedef struct {
    int day;
    int meal;
    char food[100];
    float grams;
    float calories;
    float carbs;
    float fats;
    float protein;
} FoodItem;

void initializeFoodArray();
void addFoodItem(FoodItem item);
void freeFoodArray();
void viewDiary(FILE *fDiary, int *dayNumber);
void viewWholeDiary(FILE *fDiary);
void viewDatabase();
void gotoxy(int x, int y);
void printBorder(int l, int h, int header);
void aprintf(int a, int x, int y);

#endif // VIEW_H
