#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a single person in the list
typedef struct _Person {
    char Name[50];
    char last_name[50];
    int yearob;
    struct _Person* next;
} Person;

// Function declarations
Person* add_at_beginning(Person* head);
Person* add_at_end(Person* head);
void printlist(Person* head);
void findByLname(Person* head);
Person* deleteByLname(Person* head);
Person* addAfter(Person* head);
Person* addBefore(Person* head);
Person* sortByLastName(Person* head);
void writeToFile(Person* head, const char* filename);
Person* readFromFile(const char* filename);
void freeList(Person* head);

int main() {
    Person* head = NULL;
    Person* temp = NULL;

    // Add people to the beginning
    head = add_at_beginning(head);
    head = add_at_beginning(head);

    // Print current list
    printlist(head);

    // Add a person at the end
    head = add_at_end(head);
    printlist(head);

    // Find a person by last name
    findByLname(head);

    // Delete a person by last name
    head = deleteByLname(head);
    printlist(head);

    // Add new person after a given one
    head = addAfter(head);
    printlist(head);

    // Add new person before a given one
    head = addBefore(head);
    printlist(head);

    // Sort list alphabetically by last name
    head = sortByLastName(head);
    printlist(head);

    // Write list to file
    writeToFile(head, "people.txt");

    // Free memory
    freeList(head);
    head = NULL;

    // Read list from file
    head = readFromFile("people.txt");
    printlist(head);

    // Free memory again before exit
    freeList(head);
    return 0;
}

// Adds a new person at the beginning of the list
Person* add_at_beginning(Person* head) {
    Person* new_person = (Person*)malloc(sizeof(Person));
    if (new_person == NULL) {
        printf("Greska pri alokaciji.\n");
        return head;
    }

    printf("Upisi ime: ");
    scanf("%s", new_person->Name);

    printf("Upisi prezime: ");
    scanf("%s", new_person->last_name);

    printf("Upisi godinu rodjenja: ");
    scanf("%d", &new_person->yearob);

    // New node points to old head
    new_person->next = head;
    // New node becomes new head
    return new_person;
}

// Prints the entire linked list
void printlist(Person* head) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    printf("\nLista ljudi:\n");
    while (head != NULL) {
        printf("%s %s, %d\n", head->Name, head->last_name, head->yearob);
        head = head->next;
    }
}

// Adds a person at the end of the list
Person* add_at_end(Person* head) {
    Person* new_person = (Person*)malloc(sizeof(Person));
    if (new_person == NULL) {
        printf("Greska pri alokaciji.\n");
        return head;
    }

    printf("Upisite ime: ");
    scanf("%s", new_person->Name);

    printf("Upisite prezime: ");
    scanf("%s", new_person->last_name);

    printf("Upisite godinu rodjenja: ");
    scanf("%d", &new_person->yearob);

    new_person->next = NULL;

    if (head == NULL) {
        return new_person;
    }

    Person* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_person;
    return head;
}

// Finds a person by last name
void findByLname(Person* head) {
    char last_name[50];
    printf("Upisite prezime koje zelite pronaci: ");
    scanf("%s", last_name);

    Person* current = head;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->last_name, last_name) == 0) {
            printf("Pronadjena osoba: %s %s, %d\n", current->Name, current->last_name, current->yearob);
            found = 1;
        }
        current = current->next;
    }

    if (!found)
        printf("Nije pronadjena osoba s prezimenom '%s'.\n", last_name);
}

// Deletes a person by last name
Person* deleteByLname(Person* head) {
    char last_name[50];
    printf("Upisite prezime osobe koju zelite obrisati: ");
    scanf("%s", last_name);

    if (head == NULL) {
        printf("Lista je prazna.\n");
        return head;
    }

    // If person to delete is at the beginning
    if (strcmp(head->last_name, last_name) == 0) {
        Person* temp = head;
        head = head->next;
        free(temp);
        printf("Oosoba prezimena %s je obrisana.\n", last_name);
        return head;
    }

    Person* current = head;
    while (current->next != NULL && strcmp(current->next->last_name, last_name) != 0) {
        current = current->next;
    }

    if (current->next == NULL) {
        printf("Osoba prezimena %s nije pronadjena.\n", last_name);
        return head;
    }

    Person* temp = current->next;
    current->next = temp->next;
    free(temp);
    printf("Oosba prezimena %s je obrisana.\n", last_name);

    return head;
}

// Adds a new person after a given last name
Person* addAfter(Person* head) {
    char target[50];
    printf("Upisite prezime osobe nakon koje zelite dodati novu: ");
    scanf("%s", target);

    Person* current = head;
    while (current && strcmp(current->last_name, target) != 0)
        current = current->next;

    if (!current) {
        printf("Osoba nije pronadjena.\n");
        return head;
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Greska pri alokaciji.\n");
        return head;
    }

    printf("Upisite ime: ");
    scanf("%s", newPerson->Name);
    printf("Upisite prezime: ");
    scanf("%s", newPerson->last_name);
    printf("Upisite godinu rodjenja: ");
    scanf("%d", &newPerson->yearob);

    newPerson->next = current->next;
    current->next = newPerson;

    printf("Nova osoba dodana nakon %s.\n", target);
    return head;
}

// Adds a new person before a given last name
Person* addBefore(Person* head) {
    char target[50];
    printf("Upisite prezime osobe prije koje zelite dodati novu: ");
    scanf("%s", target);

    if (head == NULL) {
        printf("Lista je prazna.\n");
        return head;
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Greska pri alokaciji.\n");
        return head;
    }

    printf("Upisite ime: ");
    scanf("%s", newPerson->Name);
    printf("Upisite prezime: ");
    scanf("%s", newPerson->last_name);
    printf("Upisite godinu rodjenja: ");
    scanf("%d", &newPerson->yearob);

    // If target is the first element
    if (strcmp(head->last_name, target) == 0) {
        newPerson->next = head;
        return newPerson;
    }

    Person* current = head;
    while (current->next && strcmp(current->next->last_name, target) != 0)
        current = current->next;

    if (current->next == NULL) {
        printf("Osoba nije pronadjena.\n");
        free(newPerson);
        return head;
    }

    newPerson->next = current->next;
    current->next = newPerson;

    printf("Nova osoba dodana prije %s.\n", target);
    return head;
}
Person* sortByLastName(Person* head) {
    if (head == NULL)
        return head;

    struct Node* i;
    struct Node* j;
    struct Node* end = NULL;

    // Outer loop: each pass moves the largest element to the end of the list
    for (i = head; i->next != end; i = head) {
        // Inner loop: compare adjacent elements up to the "end" marker
        for (j = head; j->next != end; j = j->next) {
            // Compare last names; if out of order, swap their data fields
            if (strcmp(j->last_name, j->next->last_name) > 0) {
                char tempFirst[50], tempLast[50];
                int tempYear;

                // Temporarily store data from the current node
                strcpy(tempFirst, j->Name);
                strcpy(tempLast, j->last_name);
                tempYear = j->yearob;

                // Copy next node’s data into the current node
                strcpy(j->Name, j->next->Name);
                strcpy(j->last_name, j->next->last_name);
                j->yearob = j->next->yearob;

                // Move the saved data into the next node
                strcpy(j->next->Name, tempFirst);
                strcpy(j->next->last_name, tempLast);
                j->next->yearob = tempYear;
            }
        }
        // After each full inner pass, the last node is in its correct position
        end = j;
    }

    printf("List has been sorted by last name.\n");
    return head;
}

// Writes list data to a file
void writeToFile(Person* head, const char* people) {
    FILE* f = fopen(people, "w");
    if (!f) {
        printf("greska pri otvaranju datoteku za pisanje.\n");
        return;
    }

    while (head) {
        fprintf(f, "%s %s %d\n", head->Name, head->last_name, head->yearob);
        head = head->next;
    }

    fclose(f);
    printf("List spremljena u datoteku'%s'.\n", people);
}

// Reads list data from a file
Person* readFromFile(const char* people) {
    FILE* f = fopen(people, "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke za citanje.\n");
        return NULL;
    }

    Person* head = NULL;
    Person* tail = NULL;

    while (1) {
        Person* newPerson = (Person*)malloc(sizeof(Person));
        if (fscanf(f, "%s %s %d", newPerson->Name, newPerson->last_name, &newPerson->yearob) != 3) {
            free(newPerson);
            break;
        }
        newPerson->next = NULL;

        if (!head)
            head = newPerson;
        else
            tail->next = newPerson;

        tail = newPerson;
    }

    fclose(f);
    printf("Lista ucitana iz datoteke'%s'.\n", people);
    return head;
}

// Frees all allocated memory in the list
void freeList(Person* head) {
    Person* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }

}
