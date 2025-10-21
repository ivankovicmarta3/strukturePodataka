#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Person {
    char Name[50];
    char last_name[50];
    int yearob;
    struct _Person* next;
} Person;

// Deklaracije funkcija
Person* add_at_beginning(Person* head);
Person* add_at_end(Person* head);
void printlist(Person* head);
void findByLname(Person* head);
Person* deleteByLname(Person* head);


int main() {
    Person* head = NULL; //prazna lista

    // Dodavanje  osobe na pocetak
    head = add_at_beginning(head);
    head = add_at_beginning(head);

    // Ispis liste
    printlist(head);


    // Dodavanje osobe na kraj
    head = add_at_end(head);

    printlist(head);
    // Trazenje osobe po prezimenu
    findByLname(head);

    //Brisanje osobe po prezimenu
    head = deleteByLname(head);

    // Ispis liste nakon brisanja
    printlist(head);

    //  Oslobadanje memorije svih elemenata liste
    Person* temp = NULL;
    while (head != NULL) {
        temp = head;          // Spremi trenutni cvor
        head = head->next;    // Pomakni pokazivac na sljedeci
        free(temp);           // Oslobodi memoriju prethodnog
    }

    return 0;

}
    
Person* add_at_beginning(Person* head) {
    Person* new_person = (Person*)malloc(sizeof(Person)); // Alociramo memoriju za novu osobu

    if (new_person == NULL) {   // Provjera je li alokacija uspjela
        printf("Greska pri alokaciji memorije.\n");
        return head;
    }

    // Unos podataka od korisnika
    printf("Unesite ime: ");
    scanf("%s", new_person->Name);

    printf("Unesite prezime: ");
    scanf("%s", new_person->last_name);

    printf("Unesite godinu rodenja: ");
    scanf("%d", &new_person->yearob);

    
    new_person->next = head;   // nova osoba pokazuje na stari pocetak liste

    return new_person;  // Novi pocetak je nova osoba
}

void printlist(Person* head) {
    Person* current = head;

    if (current == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    printf("\nLista osoba \n");
    while (current != NULL) {
        printf("%s %s, %d\n", current->Name, current->last_name, current->yearob);
        current = current->next; // Pomakni se na sljedeæu osobu
    }
}

Person* add_at_end(Person* head) {
    Person* new_person = (Person*)malloc(sizeof(Person));  // Alokacija memorije za novu osobu

    if (new_person == NULL) {
        printf("Greška pri alokaciji memorije.\n");
        return head;
    }

    // Unos podataka od korisnika
    printf("Unesite ime: ");
    scanf( "%s", new_person->Name);

        printf("Unesite prezime: ");
    scanf("%s", new_person->last_name);

    printf("Unesite godinu roðenja: ");
    scanf("%d", &new_person->yearob);

    new_person->next = NULL;  // Nova osoba je zadnji element, nema sljedeceg

    // Ako je lista prazna
    if (head == NULL) {
        return new_person;
    }

    // Traži kraj liste
    Person* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_person;  // Povezi zadnju osobu s novom

    return head;
}
void findByLname(Person* head) {
    char last_name[50];
    printf("Unesite prezime za pretragu: ");
    scanf("%s", last_name);

    Person* current = head;
    int found = 0;

    // Prolazimo kroz cijelu listu
    while (current != NULL) {
        if (strcmp(current->last_name, last_name) == 0) {    // Ako prezime odgovara, ispisemo osobu
            printf("Pronaðena osoba: %s %s, %d\n", current->Name, current->last_name, current->yearob);
            found = 1;
        }
        current = current->next;    // Pomjeramo se na sljedeci cvor u listi
    }

    if (!found)
        printf("Nijedna osoba s prezimenom '%s' nije pronaðena.\n", last_name);
}

Person* deleteByLname(Person* head) {
    char deleteLastName[50];               //  za unos prezimena koje zelimo obrisati

    printf("Unesite prezime osobe koju zelite obrisati: ");
    scanf("%s", deleteLastName);            // Ucitavanje prezimena za brisanje

    Person* current = head;         // Pokazivac na trenutni cvor u listi
    Person* previous = NULL;        // Pokazivac na prethodni cvor 


    // Trazimo cvor s prezimenom koji zelimo brisat
    while (current != NULL && strcmp(current->last_name, deleteLastName) != 0) {
        previous = current;           // Pomjeramo previous na current
        current = current->next;      // Pomjeramo current na sljedeci cvor
    }

    if (current == NULL) {            // Ako nismo pronasli osobu s tim prezimenom
        printf("Osoba s prezimenom %s nije pronaðena.\n", deleteLastName);
        return head;                 // Vracamo nepromijenjenu listu
    }

    if (previous == NULL) {      // Ako je osoba na pocetku liste
        head = current->next;    // Azuriramo pocetak liste
    }
    else {
        previous->next = current->next;    // Preskacemo cvor koji brisemo
    }

    free(current);    // Oslobadamo memoriju obrisanog cvora

    printf("Osoba s prezimenom %s je obrisana.\n", deleteLastName);
    return head;                                                  
}

