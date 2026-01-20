#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>          // printf, scanf, FILE
#include <stdlib.h>         // malloc, free
#include <string.h>         // strcpy, strcmp

#define MAX 100                              // maximum length for names

typedef struct Town {
    char name[MAX];
    int peopleCount;
    struct Town* left;                       // left child in BST
    struct Town* right;                      // right child in BST
} Town;

typedef struct State {
    char name[MAX];                          // state name
    struct State* next;                      // next state in linked list
    Town* townRoot;                          // pointer to towns structure
} State;



Town* addTownTree(Town* root, char* name, int peopleCount) {
    if (root == NULL) {                      // if tree is empty
        root = (Town*)malloc(sizeof(Town));  // allocate memory for new town
        if (!root) {                         // allocation check
            printf("Memory allocation failed for town.\n");
            return NULL;
        }

        strcpy(root->name, name);             // copy town name
        root->peopleCount = peopleCount;     // store population
        root->left = root->right = NULL;     // initialize children
        return root;                         // return new root
    }

    if (peopleCount < root->peopleCount ||   // compare by population
        (peopleCount == root->peopleCount && // if population is equal
            strcmp(name, root->name) < 0))       // compare by name
        root->left = addTownTree(root->left, name, peopleCount); // go left
    else
        root->right = addTownTree(root->right, name, peopleCount); // go right

    return root;                             // return unchanged root
}


/* insert state into sorted linked list */
State* addStateList(State* head, State* newState) {
    if (head == NULL ||                      // if list is empty
        strcmp(newState->name, head->name) < 0) {
        newState->next = head;               // insert at beginning
        return newState;                    // new state becomes head
    }

    State* temp = head;                      // traversal pointer
    while (temp->next &&                     // find correct position
        strcmp(temp->next->name, newState->name) < 0)
        temp = temp->next;                   // move through list

    newState->next = temp->next;              // insert state
    temp->next = newState;                   // link list

    return head;                             // head does not change
}


/* read towns from file and store them in BST */
Town* readTownsFromFile(char* filename) {
    FILE* file = fopen(filename, "r");       // open file
    if (!file) {                             // file open check
        printf("Cannot open town file: %s\n", filename);
        return NULL;
    }

    Town* root = NULL;
    char townName[MAX];                      // buffer for town name
    int people;                              // population value

    while (fscanf(file, " %[^,],%d",         // read line from file
        townName, &people) == 2)
        root = addTownTree(root, townName, people);

    fclose(file);                            // close file
    return root;                             // return root of town BST
}



/* print towns with population greater than limit */
int printTownsAboveLimit(Town* root, int limit) {
    if (root == NULL) return 0;               // stop if tree is empty

    int count = 0;

    count += printTownsAboveLimit(root->left, limit);  // traverse left subtree

    if (root->peopleCount > limit) {          // check condition
        printf("%s (%d)\n", root->name, root->peopleCount); // print town
        count++;
    }

    count += printTownsAboveLimit(root->right, limit); // traverse right subtree

    return count;                             // number of printed towns
}



int main() {
    FILE* file = fopen("drzave.txt", "r");    // open main file
    if (!file) {                              // file open check
        printf("Cannot open file drzave.txt\n");
        return 1;
    }

    State* stateList = NULL;                  // initialize list of states
    char stateName[MAX], townFile[MAX];       // buffers for input

    while (fscanf(file, "%99s %99s", stateName, townFile) == 2) {
        State* s = (State*)malloc(sizeof(State)); // allocate state
        if (!s) {                             // allocation check
            printf("Memory allocation failed for state.\n");
            fclose(file);
            return 1;
        }

        strcpy(s->name, stateName);           // copy state name
        s->next = NULL;                       // initialize pointer

        s->townRoot = readTownsFromFile(townFile); // read towns
        stateList = addStateList(stateList, s);    // add to list
    }

    fclose(file);                             // close file

    char wantedState[MAX];                    // state for search
    int limit;                                // population limit

    printf("Enter state name: ");
    scanf("%99s", wantedState);

    printf("Enter population limit: ");
    scanf("%d", &limit);

    State* temp = stateList;                  // start from list head
    while (temp && strcmp(temp->name, wantedState) != 0)
        temp = temp->next;                    // move through list

    if (temp) {                               // if state is found
        int printed = printTownsAboveLimit(temp->townRoot, limit);
        if (printed == 0)
            printf("No towns match the condition.\n");
    }
    else {
        printf("State not found.\n");          // error message
    }

    return 0;                                 // end program
}





