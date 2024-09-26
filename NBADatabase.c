#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define HASHTABLESIZE 30

typedef struct NBAPlayer 
{
    char *name;
    float height;
    float weight;
    char position;
    int jersey_number;
    char *team;
    struct NBAPlayer *next_ptr;
} 
NBAPLAYER;

enum MenuChoices
{
    DISPLAY = 1,
    SHOWBYNAME,
    SHOWBYLETTER,
    ADD,
    DELETE,
    COUNT,
    EXIT
};

int MyHashFunction(char *name) 
{
    int hash = 0;
    while (*name) 
    {
        hash += *name++;
    }
    return hash % HASHTABLESIZE;
}

void AddNode(NBAPLAYER *NewNode, NBAPLAYER *HashTable[])
{
    int HashIndex = MyHashFunction(NewNode->name);

    if (HashTable[HashIndex] == NULL) 
    {
        #if PRINTINSERT
        printf("\nInserting %s at index %d\n", NewNode->name, HashIndex);
        #endif
        HashTable[HashIndex] = NewNode;
    }
    else 
    {
        NBAPLAYER *TempPtr = HashTable[HashIndex];

        while (TempPtr->next_ptr != NULL) 
        {
            TempPtr = TempPtr->next_ptr;
        }
        TempPtr->next_ptr = NewNode;
        #if PRINTINSERT
        printf("\nInserting %s at index %d\n", NewNode->name, HashIndex);
        #endif
    }
}

void FreeDynamicMemory(NBAPLAYER *HashTable[])
{
    NBAPLAYER *TempPtr = NULL, *NextPtr = NULL;

    for (int i = 0; i < HASHTABLESIZE; i++)
    {
        TempPtr = HashTable[i];

        if (TempPtr != NULL) 
        {
            while (TempPtr != NULL) 
            {
                free(TempPtr->name);
                free(TempPtr->team);
                NextPtr = TempPtr->next_ptr;
                free(TempPtr);
                TempPtr = NextPtr;
            }    
        }
    }
}

int DeleteNode(NBAPLAYER *HashTable[])
{
    char LookupName[100] = {};
    int result = 0;

    printf("Enter the name of the NBA player to delete from the Hash Table: ");
    fgets(LookupName, sizeof(LookupName), stdin);
    LookupName[strcspn(LookupName, "\n")] = 0;

    int HashIndex = MyHashFunction(LookupName);

    NBAPLAYER *TempPtr = HashTable[HashIndex];
    NBAPLAYER *PrevPtr = NULL;

    if (TempPtr == NULL) 
    {
        printf("\n\nNBA player %s does not exist in the Hash Table\n\n", LookupName);
        result = 0;
    }
    else 
    {
        while (TempPtr != NULL) 
        {
            if (strcmp(TempPtr->name, LookupName) == 0)
            {
                if (TempPtr == HashTable[HashIndex])
                {
                    HashTable[HashIndex] = TempPtr->next_ptr;
                    printf("\nNBA player %s has been deleted from the Hash Table\n\n", TempPtr->name);
                    free(TempPtr->name);
                    free(TempPtr->team);
                    free(TempPtr);
                    TempPtr = NULL;
                }
                else
                {
                    PrevPtr->next_ptr = TempPtr->next_ptr;
                    printf("\nNBA player %s has been deleted from the Hash Table\n\n", TempPtr->name);
                    free(TempPtr->name);
                    free(TempPtr->team);
                    free(TempPtr);
                    TempPtr = NULL;
                }
                result = 1;
                break;
            }
            else
            {
                PrevPtr = TempPtr;
                TempPtr = TempPtr->next_ptr;
            }
        }
        if (result == 0)
        {
            printf("\n\nNBA player %s does not exist in the Hash Table\n\n", LookupName);
        }
    }
    return result;
}

void DisplayHashTable(NBAPLAYER *HashTable[])
{
    int i;
    NBAPLAYER *TempPtr = NULL;

    for (i = 0; i < HASHTABLESIZE; i++) 
    {
        TempPtr = HashTable[i];

        printf("\nHashTable[%d]-", i);

        if (TempPtr != NULL) 
        {
            while (TempPtr != NULL)
            {
                printf("%s|", TempPtr->name);
                TempPtr = TempPtr->next_ptr;
            }
        }
    }
}

void ShowByLetter(NBAPLAYER *HashTable[])
{
    int i;
    NBAPLAYER *TempPtr = NULL;
    char LookupLetter = 'A';

    printf("\n\nEnter a letter of the alphabet: ");
    scanf(" %c", &LookupLetter);
    LookupLetter = toupper(LookupLetter);
    getchar(); // to consume the newline character left in the buffer

    for (i = 0; i < HASHTABLESIZE; i++) 
    {
        TempPtr = HashTable[i];

        if (TempPtr != NULL) 
        {
            while (TempPtr != NULL)
            {
                if (toupper(TempPtr->name[0]) == LookupLetter)
                {
                    printf("%s\n", TempPtr->name);
                }
                TempPtr = TempPtr->next_ptr;
            }
        }
    }
}

void ShowByName(NBAPLAYER *HashTable[])
{
    NBAPLAYER *TempPtr = NULL;
    char LookupName[100] = {};
    int FoundIt = 0;

    printf("\n\nEnter NBA player's name: ");
    fgets(LookupName, sizeof(LookupName), stdin);
    LookupName[strcspn(LookupName, "\n")] = 0; // Remove newline character

    #if TIMING
    clock_t start, end;
    start = clock();
    #endif

    for (int i = 0; i < HASHTABLESIZE; i++) 
    {
        TempPtr = HashTable[i];

        if (TempPtr != NULL) 
        {
            while (TempPtr != NULL)
            {
                if (strcmp(TempPtr->name, LookupName) == 0)
                {
                    #if TIMING
                    end = clock();
                    printf("\n\nSearch took %ld ticks\n\n", end - start);
                    #endif

                    FoundIt = 1;
                    printf("\n\n%s\n", TempPtr->name);
                    printf("Height\t\t%.2f inches\n", TempPtr->height);
                    printf("Weight\t\t%.1f lbs\n", TempPtr->weight);
                    printf("Position\t%c\n", TempPtr->position);
                    printf("Jersey Number\t%d\n", TempPtr->jersey_number);
                    printf("Team\t\t%s\n", TempPtr->team);
                    break;
                }
                TempPtr = TempPtr->next_ptr;
            }
        }
    }

    if (FoundIt == 0)
        printf("\n\nNBA player %s not found in Hash Table\n\n", LookupName);
}

void AddNewNBAPlayer(NBAPLAYER *HashTable[])
{
    NBAPLAYER *NewNode = malloc(sizeof(NBAPLAYER));
    NewNode->next_ptr = NULL;
    char TempBuffer[100] = {};

    printf("\n\nEnter new NBA player's name: ");
    fgets(TempBuffer, sizeof(TempBuffer), stdin);
    TempBuffer[strcspn(TempBuffer, "\n")] = 0; // Remove newline character
    NewNode->name = malloc(strlen(TempBuffer) * sizeof(char) + 1);
    strcpy(NewNode->name, TempBuffer);

    printf("\nEnter %s's height in inches: ", NewNode->name);
    scanf("%f", &(NewNode->height));
    getchar(); // to consume the newline character left in the buffer

    printf("\nEnter %s's weight in pounds: ", NewNode->name);
    scanf("%f", &(NewNode->weight));
    getchar(); // to consume the newline character left in the buffer

    printf("\nEnter %s's position (e.g., G, F, C): ", NewNode->name);
    scanf(" %c", &(NewNode->position));
    NewNode->position = toupper(NewNode->position);
    getchar(); // to consume the newline character left in the buffer

    printf("\nEnter %s's jersey number: ", NewNode->name);
    scanf("%d", &(NewNode->jersey_number));
    getchar(); // to consume the newline character left in the buffer

    printf("\nEnter %s's team: ", NewNode->name);
    fgets(TempBuffer, sizeof(TempBuffer), stdin);
    TempBuffer[strcspn(TempBuffer, "\n")] = 0; // Remove newline character
    NewNode->team = malloc(strlen(TempBuffer) * sizeof(char) + 1);
    strcpy(NewNode->team, TempBuffer);

    AddNode(NewNode, HashTable);
}

int ReadFileIntoHashTable(int argc, char *argv[], NBAPLAYER *HashTable[])
{
    FILE *FH = NULL;
    char TempBuffer[100] = {};
    int counter = 0;

    if (argc == 2)
    {
        FH = fopen(argv[1], "r");

        if (FH == NULL)
        {
            printf("\nFile %s could not be opened...exiting\n", argv[1]);
            exit(0);
        }

        while (fgets(TempBuffer, sizeof(TempBuffer), FH) != NULL)
        {
            TempBuffer[strcspn(TempBuffer, "\n")] = 0; // Remove newline character

            NBAPLAYER *NewNode = malloc(sizeof(NBAPLAYER));
            NewNode->next_ptr = NULL;

            char *token = strtok(TempBuffer, "|");
            NewNode->name = malloc(strlen(token) * sizeof(char) + 1);
            strcpy(NewNode->name, token);

            token = strtok(NULL, "|");
            NewNode->height = atof(token);

            token = strtok(NULL, "|");
            NewNode->weight = atof(token);

            token = strtok(NULL, "|");
            NewNode->position = *token;

            token = strtok(NULL, "|");
            NewNode->jersey_number = atoi(token);

            token = strtok(NULL, "|");
            NewNode->team = malloc(strlen(token) * sizeof(char) + 1);
            strcpy(NewNode->team, token);

            AddNode(NewNode, HashTable);
            counter++;
        }
    }
    else
    {
        printf("\nFile must be provided on command line...exiting\n\n");
        exit(0);
    }
    fclose(FH);
    return counter;
}

void ShowMenu()
{
    printf("\n\n1. Show Hash Table\n");
    printf("2. Show NBA players by name\n");
    printf("3. Show NBA players by letter\n");
    printf("4. Add a new NBA player to Hash Table\n");
    printf("5. Delete an NBA player from Hash Table\n");
    printf("6. Count NBA players\n");
    printf("7. Quit\n");
    printf("Enter Menu Choice: ");
}

int main(int argc, char *argv[])
{
    NBAPLAYER *HashTable[HASHTABLESIZE] = {};
    int MenuChoice = 0;
    int counter = 0;

    counter = ReadFileIntoHashTable(argc, argv, HashTable);

    do
    {
        ShowMenu();
        scanf("%d", &MenuChoice);
        getchar(); // to consume the newline character left in the buffer

        switch (MenuChoice)
        {    
            case DISPLAY:
                DisplayHashTable(HashTable);
                break;
            case SHOWBYNAME:
                ShowByName(HashTable);
                break;
            case SHOWBYLETTER:
                ShowByLetter(HashTable);
                break;
            case ADD:
                AddNewNBAPlayer(HashTable);
                counter++;
                break;
            case DELETE:
                if (DeleteNode(HashTable))
                {
                    counter--;
                }
                break;
            case COUNT:
                printf("Your Hash Table contains %d NBA players\n", counter);
                break;
            case EXIT:
                break;
            default : 
                printf("Invalid menu choice\n\n"); 
        }
    }
    while (MenuChoice != EXIT);
    
    FreeDynamicMemory(HashTable);

    return 0;
}