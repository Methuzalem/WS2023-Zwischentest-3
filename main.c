#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 32

//definition for struct and enum----------------------------------------------------------------------------------------------------
enum genre {
    Horror,
    Abenteuer,
    Romantik,
    Sachbuch,
};

typedef struct book{
    char title[MAX];
    enum genre genreBook;
    int year;
    int amount;
} book;

typedef struct booknode{
    book content;
    struct booknode* next;
}booknode;

typedef struct borrownode{
    char borrowTitle[MAX];
    char borrowName[MAX];
    struct borrownode* next;
}borrownode;


//functions-------------------------------------------------------------------------------------------------------------------------
struct book createBook();
struct booknode* insertBook(book newBook, booknode* inventory);
void printList(booknode* inventory);
struct borrownode* borrowBook(booknode* inventory, borrownode* borrowtory);
struct borrownode* returnBook(booknode* inventory, borrownode* borrowtory);
void printListBorrow(borrownode* borrowtory);
void printListInOrder(booknode* inventory);
void freeInventory(booknode* inventory);
void freeBorrow(borrownode* borrowtory);


//main function---------------------------------------------------------------------------------------------------------------------
int main()
{
    char input = '0';
    booknode* inventory = NULL;
    book newBook;
    borrownode* borrowtory = NULL;



    do{
        printf("\nNeues Buch eingeben (n), Buch ausleihen (b), Buch zurueckgeben (r), Buecher auflisten (l), Buecher sortieren (s), Programm beenden (x)");
        printf("\nAuswahl: ");;
        scanf(" %c", &input);

        switch(input){

            case 'n':   newBook = createBook();
                        inventory = insertBook(newBook, inventory);
                        break;

            case 'b':   borrowtory = borrowBook(inventory, borrowtory);
                        break;

            case 'l':   printList(inventory);
                        break;

            case 'r':   borrowtory = returnBook(inventory, borrowtory);
                        break;

            case 's':   printListInOrder(inventory);
                        break;

            case 'x':   freeInventory(inventory);
                        freeBorrow(borrowtory);
                        break;

            default:    printf("\nUngueltige Eingabe!");
        }


    } while(input != 'x');

    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------
struct book createBook(){

    book newBook;
    int genre = 0;
    int errorCount = 0;

    printf("\nGeben Sie den Titel ein:");
    scanf("%s", newBook.title);

    while(errorCount == 0){
    printf("\nGeben Sie das Genre ein. Horror (1), Abenteuer (2), Romantik (3), Sachbuch (4):");
    scanf("%d", &genre);

        if(genre == 1){
            newBook.genreBook = Horror;
            errorCount++;
        }else if(genre == 2){
            newBook.genreBook = Abenteuer;
            errorCount++;
        }else if(genre == 3){
            errorCount++;
            newBook.genreBook = Romantik;
        }else if(genre == 4){
            errorCount++;
            newBook.genreBook = Sachbuch;
        }else{
            printf("\nUngueltige Eingabe!");
        }
    }

    errorCount = 0;

    while(errorCount == 0){
    printf("\nGeben Sie das Erscheinungsjahr ein:");
    scanf("%d", &newBook.year);
        if(newBook.year <= 0){
            printf("\nUngueltige Eingabe!");
        }else{
            errorCount++;
        }
    }

    errorCount = 0;

    while(errorCount == 0){
    printf("\nGeben Sie ein wieviele Exemplare vorhanden sind:");
    scanf("%d", &newBook.amount);
        if(newBook.amount <= 0){
            printf("\nUngueltige Eingabe!");
        }else{
            errorCount++;
        }
    }

    return newBook;
};


//----------------------------------------------------------------------------------------------------------------------------------
struct booknode* insertBook(book newBook, booknode* inventory){

    booknode* newNode = (booknode*)malloc(sizeof(booknode));
    newNode->content = newBook;
    newNode->next = inventory;

    return newNode;
};


//----------------------------------------------------------------------------------------------------------------------------------
struct borrownode* borrowBook(booknode* inventory, borrownode* borrowtory){

    int amountOfBooks = 0;
    int chosenIndex = 0;
    int errorCount = 0;
    booknode* tempindex = inventory;
    borrownode* tempborrow = borrowtory;
    char name[MAX];

    if(inventory == NULL){
        printf("\n\nEs sind keine Buecher im Inventar vorhanden.");
        return 0;
    }


    printList(inventory);


    while(tempindex != NULL){
    amountOfBooks++;
    tempindex = tempindex->next;
    }

    while(errorCount == 0){
    printf("\nWelchen Titel moechten Sie leihen? (1-%d):", amountOfBooks);
    scanf("%d", &chosenIndex);

        if(chosenIndex > amountOfBooks || chosenIndex < 1){
            printf("\nUngueltige Eingabe!");
        }else{
            errorCount++;
        }
    }

    printf("\nGeben Sie Ihren Namen ein: ");
    scanf("%s", name);

    for(int i = 1; i < chosenIndex; i++){
        inventory = inventory->next;
    }

    if(inventory->content.amount == 0){
        printf("\nBereits alle Exemplare ausgeliehen!");
    }else{
        while(tempborrow != NULL){
            if(strcmp(tempborrow->borrowName, name) == 0 && strcmp(tempborrow->borrowTitle, inventory->content.title) == 0){
                printf("\nSie haben diesen Titel bereits ausgeliehen!");
                return borrowtory;
            }
            tempborrow = tempborrow->next;
        }

        inventory->content.amount = inventory->content.amount - 1;

        borrownode* newBorrow = (borrownode*)malloc(sizeof(borrownode));

        strcpy(newBorrow->borrowTitle, inventory->content.title);
        strcpy(newBorrow->borrowName, name);
        newBorrow->next = borrowtory;

        return newBorrow;
    }
        return borrowtory;
};


//----------------------------------------------------------------------------------------------------------------------------------
struct borrownode* returnBook(booknode* inventory, borrownode* borrowtory){

    int index = 0;
    int chosenIndex = 0;
    int errorCount = 0;
    borrownode* tempindex = borrowtory;

    while(tempindex != NULL){
        index++;
        tempindex = tempindex->next;
    }

    if(borrowtory == NULL){
        printf("\n\nEs sind keine Titel ausgeliehen!");
        return borrowtory;
    }


    printListBorrow(borrowtory);



    while(errorCount == 0){
        printf("\n\nWelchen Titel moechten Sie retournieren? (1-%d): ", index);
        scanf("%d", &chosenIndex);
            if(chosenIndex < 1 || chosenIndex > index){
                printf("\nUngueltige Eingabe!");
            }else{
                errorCount++;
            }
    }

//delete first node
    if(chosenIndex == 1){
        while(strcmp(borrowtory->borrowTitle, inventory->content.title) != 0){
        inventory = inventory->next;
        }
        inventory->content.amount = inventory->content.amount + 1;


        borrownode* temp = borrowtory;
        borrowtory = borrowtory->next;

        free(temp);

//delete second node +
   }else{
        borrownode* tempPrev = borrowtory;
        borrownode* tempCurr = borrowtory->next;

        for (int i = 1; i < chosenIndex - 1; i++) {
            tempPrev = tempPrev->next;
            tempCurr = tempCurr->next;
        }

        while(strcmp(tempCurr->borrowTitle, inventory->content.title) != 0){
        inventory = inventory->next;
        }

        inventory->content.amount = inventory->content.amount + 1;

        tempPrev->next = tempCurr->next;
        free(tempCurr);
    }

    return borrowtory;
};


//----------------------------------------------------------------------------------------------------------------------------------
void printList(booknode* inventory){

    int index = 1;

    while(inventory != NULL){

        if(inventory->content.genreBook == 0){
            printf("\n%d: %s, Horror (%d)", index, inventory->content.title, inventory->content.year);
            inventory = inventory->next;
            index++;
        }else if(inventory->content.genreBook == 1){
            printf("\n%d: %s, Abenteuer (%d)", index, inventory->content.title, inventory->content.year);
            inventory = inventory->next;
            index++;
        }else if(inventory->content.genreBook == 2){
            printf("\n%d: %s, Romantik (%d)", index, inventory->content.title, inventory->content.year);
            inventory = inventory->next;
            index++;
        }else if(inventory->content.genreBook == 3){
            printf("\n%d: %s, Sachbuch (%d)", index, inventory->content.title, inventory->content.year);
            inventory = inventory->next;
            index++;
        }
    }
    printf("\n");
}


//----------------------------------------------------------------------------------------------------------------------------------
void printListBorrow(borrownode* borrowtory){

    int index = 1;

    while(borrowtory != NULL){
        printf("\n%d: %s geliehen von %s", index, borrowtory->borrowTitle, borrowtory->borrowName);
        borrowtory = borrowtory->next;
        index++;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------
void printListInOrder(booknode* inventory){

    book container[MAX];
    int containerSize = 0;
    int tempSortYear = 0;
    int tempSortGenre = 0;
    char tempSortTitle[MAX];
    int index = 1;
    booknode* elemCount = inventory;
    booknode* transfer = inventory;

    while(elemCount != NULL){
        containerSize++;
        elemCount = elemCount->next;
    }

    for(int i = 0; i < containerSize; i++){
        container[i].genreBook = transfer->content.genreBook;
        strcpy(container[i].title, transfer->content.title);
        container[i].year = transfer->content.year;
        transfer = transfer->next;
    }


//BubbleSort
    for(int i = 0; i < containerSize; i++){
        for(int z = 0; z < containerSize-1; z++){
            if(container[z].year > container[z+1].year){
                tempSortYear = container[z].year;
                tempSortGenre = container[z].genreBook;
                strcpy(tempSortTitle, container[z].title);
                container[z].year = container[z+1].year;
                container[z].genreBook = container[z+1].genreBook;;
                strcpy(container[z].title, container[z+1].title);
                container[z+1].year = tempSortYear;
                container[z+1].genreBook = tempSortGenre;
                strcpy(container[z+1].title, tempSortTitle);
                }
            }
    }

//Print
    for(int i = 0; i < containerSize; i++){
        if(container[i].genreBook == 0){
        printf("\n%d: %s, Horror (%d)", index, container[i].title, container[i].year);
        index++;
        }else if(container[i].genreBook == 1){
        printf("\n%d: %s, Abenteuer (%d)", index, container[i].title, container[i].year);
        index++;
        }else if(container[i].genreBook == 2){
        printf("\n%d: %s, Romantik (%d)", index, container[i].title, container[i].year);
        index++;
        }else if(container[i].genreBook == 3){
        printf("\n%d: %s, Sachbuch (%d)", index, container[i].title, container[i].year);
        index++;
        }
    }
    printf("\n");
}


//----------------------------------------------------------------------------------------------------------------------------------
void freeInventory(booknode* inventory){

    while(inventory != NULL){
        booknode* temp = inventory;
        inventory = inventory->next;
        free(temp);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------
void freeBorrow(borrownode* borrowtory){

    while(borrowtory != NULL){
        borrownode* temp = borrowtory;
        borrowtory = borrowtory->next;
        free(temp);
    }
}
