#define _CRT_SECURE_NO_WARNINGS                     

#include <stdio.h>                                  
#include <stdlib.h>                                 // malloc, free
#include <string.h>                                 // strcpy, strcmp

#define MAX 100                                     // maximum length of names
#define HASH_SIZE 11                                // hash table size

typedef struct Town {
    char name[MAX];                                 // town name
    int peopleCount;                                // number of inhabitants
    struct Town* left;                              // left child in BST
    struct Town* right;                             // right child in BST
} Town;

typedef struct State {
    char name[MAX];                                 // state name (key)
    Town* townRoot;                                 // root of town BST
    struct State* next;                             // next state in hash bucket list
} State;

/* calculates hash key using first five letters of state name */
int hashFunction(char* name) {
    int sum = 0;                                    // initialize ASCII sum

    for (int i = 0; i < 5 && name[i] != '\0'; i++)  // iterate through first 5 characters
        sum += (int)name[i];                        // add ASCII value of character

    return sum % HASH_SIZE;                         // calculate hash index
}

/* inserts town into binary search tree */
Town* addTownTree(Town* root, char* name, int peopleCount) {
    if (root == NULL) {                             // if tree is empty
        root = (Town*)malloc(sizeof(Town));         // allocate memory for town
        if (!root) {                                // check allocation
            printf("Memory allocation failed.\n");  // error message
            return NULL;                            // return NULL
        }

        strcpy(root->name, name);                   // copy town name
        root->peopleCount = peopleCount;            // set population
        root->left = root->right = NULL;            // initialize children
        return root;                                // return new root
    }

    if (peopleCount < root->peopleCount ||          // compare by population
        (peopleCount == root->peopleCount &&
            strcmp(name, root->name) < 0))              // compare by name if equal
        root->left = addTownTree(root->left, name, peopleCount); // insert left
    else
        root->right = addTownTree(root->right, name, peopleCount); // insert right

    return root;                                    // return unchanged root
}

/* reads towns from file and creates BST */
Town* readTownsFromFile(char* filename) {
    FILE* file = fopen(filename, "r");              // open town file
    if (!file) {                                    // check if file opened
        printf("Cannot open town file.\n");         // error message
        return NULL;                                // return NULL
    }

    Town* root = NULL;                              // initialize tree root
    char townName[MAX];                             // buffer for town name
    int people;                                     // population variable

    while (fscanf(file, " %[^,],%d", townName, &people) == 2) // read town data
        root = addTownTree(root, townName, people); // insert into BST

    fclose(file);                                   // close file
    return root;                                    // return tree root
}

/* inserts state into sorted linked list */
State* addStateToList(State* head, State* newState) {
    if (!head || strcmp(newState->name, head->name) < 0) { // insert at beginning
        newState->next = head;                      // connect list
        return newState;                            // return new head
    }

    State* temp = head;                             // start traversal
    while (temp->next &&
        strcmp(temp->next->name, newState->name) < 0) // find correct position
        temp = temp->next;                          // move forward

    newState->next = temp->next;                    // link new element
    temp->next = newState;                          // insert into list

    return head;                                    // return unchanged head
}

State* addStateToHash(State* hashTable[], char* stateName, char* townFile) {
    int key = hashFunction(stateName);               // calculate hash index

    State* newState = (State*)malloc(sizeof(State)); // allocate memory
    if (!newState) {                                 // check allocation
        printf("Memory allocation failed.\n");       // error message
        return NULL;                                 // return NULL on failure
    }

    strcpy(newState->name, stateName);               // copy state name
    newState->townRoot = readTownsFromFile(townFile); // create town BST
    newState->next = NULL;                           // initialize next pointer

    hashTable[key] = addStateToList(hashTable[key], newState); // insert into bucket

    return newState;                                 // return inserted state
}


/* prints towns with population greater than given limit */
int printTownsAboveLimit(Town* root, int limit) {
    if (!root) return 0;                            // stop recursion

    int count = 0;                                  // counter

    count += printTownsAboveLimit(root->left, limit); // traverse left subtree

    if (root->peopleCount > limit) {                // check condition
        printf("%s (%d)\n", root->name, root->peopleCount); // print town
        count++;                                    // increment counter
    }

    count += printTownsAboveLimit(root->right, limit); // traverse right subtree

    return count;                                   // return number of printed towns
}

int main() {
    State* hashTable[HASH_SIZE] = { NULL };         // initialize hash table

    FILE* file = fopen("drzave.txt", "r");          // open main file
    if (!file) {                                    // check file opening
        printf("Cannot open file drzave.txt\n");    // error message
        return 1;                                   // exit program
    }

    char stateName[MAX], townFile[MAX];             // input buffers

    while (fscanf(file, "%99s %99s", stateName, townFile) == 2) {
        State* inserted = addStateToHash(hashTable, stateName, townFile); // add state
        if (!inserted) {                             // check insertion
            printf("Failed to add state: %s\n", stateName);
        }
    }

    fclose(file);                                   // close main file

    char wantedState[MAX];                          // searched state
    int limit;                                      // population limit

    printf("Enter state name: ");
    scanf("%99s", wantedState);                     // read state name

    printf("Enter population limit: ");
    scanf("%d", &limit);                            // read limit

    int key = hashFunction(wantedState);             // calculate hash key

    printf("Hash index: %d\n", key);                 // SHOW THAT HASH IS USED

    State* temp = hashTable[key];                    // access bucket

    while (temp && strcmp(temp->name, wantedState) != 0) // search list
        temp = temp->next;                          // move forward

    if (temp) {                                     // if state is found
        int printed = printTownsAboveLimit(temp->townRoot, limit); // print towns
        if (printed == 0)
            printf("No towns match the condition.\n"); // no results
    }
    else {
        printf("State not found.\n");               // state not found
    }

    return 0;                                       // end of program
}
