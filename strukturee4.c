#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Polynomial* PolyList;

struct Polynomial {
    int Coef;        // coefficient of the term
    int Exp;         // exponent of the term
    PolyList Next;   // pointer to next term
};

int insertSorted(PolyList P, int Coef, int Exp);
int printPoly(char* name, PolyList P);
int sumPoly(PolyList P1, PolyList P2, PolyList S);
int productPoly(PolyList P1, PolyList P2, PolyList P);
void parseAndInsert(char* line, PolyList P);
void loadPolyFromFile(char* filename, PolyList P);

int insertSorted(PolyList P, int Coef, int Exp) {
    if (Coef == 0) return 0;  // ignore zero coefficient terms

    PolyList newNode = (PolyList)malloc(sizeof(struct Polynomial)); // allocate memory
    newNode->Coef = Coef;  // set the coefficient
    newNode->Exp = Exp;    // set the exponent
    newNode->Next = NULL;  // not linked yet

    PolyList prev = P;       // start from the header node
    PolyList curr = P->Next; // first real term

    // find the position in descending exponent order
    while (curr != NULL && curr->Exp > Exp) {
        prev = curr;         // move previous forward
        curr = curr->Next;   // move current forward
    }

    // if a term with the same exponent exists, add the coefficients
    if (curr != NULL && curr->Exp == Exp) {
        curr->Coef += Coef; // sum coefficients
        free(newNode);      // new node not needed
        if (curr->Coef == 0) { // if sum is zero, remove the node
            prev->Next = curr->Next;
            free(curr);
        }
        return 0;
    }

    // insert new node between prev and curr
    newNode->Next = curr;
    prev->Next = newNode;

    return 0;
}

int printPoly(char* name, PolyList P) {
    printf("%s = ", name);   // print polynomial name
    P = P->Next;             // skip header node
    if (!P) {                // if the list is empty
        printf("0\n");       // print zero
        return 0;
    }

    while (P) {              // loop through all terms
        if (P->Coef > 0 && P != P->Next) printf("+"); // add '+' if positive and not first term
        if (P->Exp == 0) printf("%d", P->Coef);      // x^0 is constant term
        else if (P->Exp == 1) printf("%dx", P->Coef); // x^1 format
        else printf("%dx^%d", P->Coef, P->Exp);       // general x^n format
        P = P->Next;                                   // move to next term
        if (P) printf(" ");                            // add space between terms
    }
    printf("\n");                                     // end line
    return 0;
}

int sumPoly(PolyList P1, PolyList P2, PolyList S) {
    P1 = P1->Next;  // skip header of first polynomial
    P2 = P2->Next;  // skip header of second polynomial

    // loop through both polynomials
    while (P1 && P2) {
        if (P1->Exp == P2->Exp) { // same exponent
            insertSorted(S, P1->Coef + P2->Coef, P1->Exp); // sum coefficients
            P1 = P1->Next; P2 = P2->Next;                  // move to next terms
        }
        else if (P1->Exp > P2->Exp) { // P1 term has bigger exponent
            insertSorted(S, P1->Coef, P1->Exp);           // insert P1 term
            P1 = P1->Next;                                // move to next
        }
        else {                          // P2 term has bigger exponent
            insertSorted(S, P2->Coef, P2->Exp);           // insert P2 term
            P2 = P2->Next;                                // move to next
        }
    }

    // insert remaining terms if any
    while (P1) { insertSorted(S, P1->Coef, P1->Exp); P1 = P1->Next; }
    while (P2) { insertSorted(S, P2->Coef, P2->Exp); P2 = P2->Next; }

    return 0;
}
int productPoly(PolyList P1, PolyList P2, PolyList P) {
    PolyList a = P1->Next; // start with first term of P1
    while (a) {             // for each term in P1
        PolyList b = P2->Next; // start with first term of P2
        while (b) {             // multiply with each term of P2
            insertSorted(P, a->Coef * b->Coef, a->Exp + b->Exp); // multiply and insert
            b = b->Next;        // next term in P2
        }
        a = a->Next;            // next term in P1
    }
    return 0;
}
void parseAndInsert(char* line, PolyList P) {
    char* ptr = line;         // start pointer at the beginning

    while (*ptr) {            // loop through the line
        int sign = 1;         // default sign
        int coef = 0;         // coefficient
        int exp = 0;          // exponent

        while (isspace(*ptr)) ptr++;    // skip spaces

        // check for + or - sign
        if (*ptr == '+') { sign = 1; ptr++; }
        else if (*ptr == '-') { sign = -1; ptr++; }

        while (isspace(*ptr)) ptr++;    // skip spaces after sign

        // read coefficient if it exists
        if (isdigit(*ptr)) {
            coef = 0;
            while (isdigit(*ptr)) { coef = coef * 10 + (*ptr - '0'); ptr++; }
        }
        else {
            coef = 1;   // default coefficient is 1 if not specified
        }
        coef *= sign;    // apply the sign

        // read exponent
        if (*ptr == 'x') {      // check for variable x
            ptr++;              // move past x
            if (*ptr == '^') {  // check for ^ exponent
                ptr++;
                exp = 0;
                while (isdigit(*ptr)) { exp = exp * 10 + (*ptr - '0'); ptr++; }
            }
            else {
                exp = 1;        // x without ^ means exponent 1
            }
        }
        else {
            exp = 0;            // no x means constant term, exponent 0
        }

        // insert term into polynomial list
        insertSorted(P, coef, exp);

        while (isspace(*ptr)) ptr++; // skip spaces before next term
    }
}
void loadPolyFromFile(char* filename, PolyList P) {
    FILE* f = fopen(filename, "r");  // open the file
    if (!f) { printf("Cannot open file %s\n", filename); exit(1); }

    char line[256];
    while (fgets(line, sizeof(line), f)) {  // read each line
        parseAndInsert(line, P);            // parse and insert terms
    }

    fclose(f);                             // close the file
}
int main() {
    // create header nodes for polynomials (dummy nodes)
    struct Polynomial H1 = { 0,0,NULL };
    struct Polynomial H2 = { 0,0,NULL };
    struct Polynomial HS = { 0,0,NULL };
    struct Polynomial HP = { 0,0,NULL };

    // load polynomials from files
    loadPolyFromFile("poly1.txt", &H1);
    loadPolyFromFile("poly2.txt", &H2);

    // print the polynomials loaded
    printPoly("P1(x)", &H1);
    printPoly("P2(x)", &H2);

    // calculate sum and product
    sumPoly(&H1, &H2, &HS);
    productPoly(&H1, &H2, &HP);

    // print results
    printf("\nSum:\n");
    printPoly("S(x)", &HS);

    printf("\nProduct:\n");
    printPoly("P(x)", &HP);

    return 0;
}
