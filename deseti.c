#define _CRT_SECURE_NO_WARNINGS              
#include <stdio.h>                           
#include <stdlib.h>                          
#include <string.h>                     

#define MAX 100                              // maximum length for names

typedef struct Town {
    char name[MAX];                          
    int peopleCount;                         
    struct Town* left;                       // left child in BST
    struct Town* right;                      // right child in BST
    struct Town* next;                       // next element in linked list
} Town;

typedef struct State {
    char name[MAX];                          // state name
    struct State* next;                      // next state in linked list
    struct State* left;                      // left child in BST
    struct State* right;                     // right child in BST
    Town* townRoot;                          // pointer to towns structure
} State;

Town* addTownTree(Town* root, char* name, int peopleCount) {
    if (root == NULL) {                      // if tree is empty
        root = malloc(sizeof(Town));         // allocate memory for new town
        strcpy(root->name, name);             // copy town name
        root->peopleCount = peopleCount;     // store population
        root->left = root->right = NULL;     // initialize children
        return root;                         // return new root
    }

    if (peopleCount < root->peopleCount ||   // compare by population
        (peopleCount == root->peopleCount &&  // if population is equal
            strcmp(name, root->name) < 0))       // compare by name
        root->left = addTownTree(root->left, name, peopleCount); // go left
    else
        root->right = addTownTree(root->right, name, peopleCount); // go right

    return root;                             // return unchanged root
}

/* insert town into sorted linked list */
Town* addTownList(Town* head, char* name, int peopleCount) {
    Town* newTown = malloc(sizeof(Town));    // allocate memory
    strcpy(newTown->name, name);              // copy town name
    newTown->peopleCount = peopleCount;       // store population
    newTown->next = NULL;                     // initialize next pointer

    if (head == NULL ||                      // if list is empty
        peopleCount < head->peopleCount) {// or goes to beginning
        newTown->next = head;                // connect new node
        head = newTown;                      // update head
        return newTown                               // new town becomes new head
    }


    Town* temp = head;                       // pointer for traversal
    while (temp->next &&                     // while next exists
        temp->next->peopleCount < peopleCount)
        temp = temp->next;                   // move forward

    newTown->next = temp->next;               // insert new town
    temp->next = newTown;                    // link list

    return head;                          // head of list remains unchanged
}

/* insert state into sorted linked list */
State* addStateList(State* head, State* newState) {
    if (head == NULL ||                      // if list is empty
        strcmp(newState->name, head->name) < 0) {
        newState->next = head;               // insert at beginning
        head = newState;                    // update head
        return newState                            // new state becomes head
    }

    State* temp = head;                      // traversal pointer
    while (temp->next &&                     // find correct position
        strcmp(temp->next->name, newState->name) < 0)
        temp = temp->next;                   // move through list

    newState->next = temp->next;              // insert state
    temp->next = newState;                   // link list


    return head;                          // head does not change
}

/* insert state into binary search tree */
State* addStateTree(State* root, State* newState) {
    if (root == NULL)                         // if tree is empty
        return newState;                     // new state becomes root

    if (strcmp(newState->name, root->name) < 0) // compare by name
        root->left = addStateTree(root->left, newState); // go left
    else
        root->right = addStateTree(root->right, newState); // go right

    return root;                              // return root
}

/* read towns from file and store them in BST */
Town* readTownsFromFile(char* filename) {
    FILE* file = fopen(filename, "r");        // open file
    char townName[MAX];                       // buffer for town name
    int people;                               // population value

    while (fscanf(file, " %[^,],%d",          // read line from file
        townName, &people) == 2)
        root = addTownTree(root,            // insert town into tree
            townName, people);

    fclose(file);                             // close file
    return root;                          // return root of town BST
}
/* print towns with population greater than limit */
int printTownsAboveLimit(Town* root, int limit) {
    if (root == NULL) return;                 // stop if tree is empty

    int count = 0;

    count += printTownsAboveLimit(root->left, limit);  // traverse left subtree

    if (root->peopleCount > limit) {          // check condition
        printf("%s (%d)\n", root->name, root->peopleCount); //print town
        count++;
    }

    count += printTownsAboveLimit(root->right, limit); // traverse right subtree

    return count;                         // number of printed towns
}

int main() {
    FILE* file = fopen("drzave.txt", "r");    // open main file
    State* stateList = NULL;                  // initialize list of states

    char stateName[MAX], townFile[MAX];       // buffers for input

    while (fscanf(file, "%s %s",stateName, townFile) == 2) {
        State* s = malloc(sizeof(State));                      // allocate state
        strcpy(s->name, stateName);                            // copy state name
                                           
        s->next = s->left = s->right = NULL;                       // initialize pointers

        s->townRoot = readTownsFromFile(townFile);   // returns Town*
        stateList = addStateList(stateList, s);      // returns State*
    }
    fclose(file);                             // close file

    char wantedState[MAX];                    // state for search
    int limit;                                // population limit

    printf("Enter state name: ");              // user input
    scanf("%s", wantedState);

    printf("Enter population limit: ");        // user input
    scanf("%d", &limit);

    State* temp = stateList;                  // start from list head
    while (temp &&                            // search for state
        strcmp(temp->name, wantedState) != 0)
        temp = temp->next;                    // move through list

    if (temp) {        // if state is found

        int printed = printTownsAboveLimit(temp->townRoot, limit); // print towns
        if (printed == 0)
            printf("No towns match the condition.\n");
    }
    else {
        printf("State not found.\n");          // error message
    }
    return 0;                                 // end program
}


