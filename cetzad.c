#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for one node in the stack
typedef struct Node {
    double value;
    struct Node* next;
} Node;

// Function declarations
int push(Node** top, double val);
int pop(Node** top, double* val);
int evaluatePostfix(const char* fileName, double* result);

int main() {
    char fileName[100];
    double result;

    printf("Unesite ime datoteke: ");   // ask user for file name
    scanf("%s", fileName);           // read file name safely

    if (evaluatePostfix(fileName, &result))    // evaluate expression
        printf("Rezultat: %.2f\n", result);   // print result
    else
        printf("Izracun nije uspio\n");       // print error message if failed

    return 0; // end of program
}

// Push a value onto the stack
int push(Node** top, double val) {
    Node* newNode = (Node*)malloc(sizeof(Node));  // allocate memory for new node
    if (!newNode) {                              // check if allocation failed
        printf("Greska: alokacija memorije nije uspjela\n");
        return 0;                                // return 0 for failure
    }

    newNode->value = val;                        // store the value in node
    newNode->next = *top;                        // new node points to current top
    *top = newNode;                              // update top to new node
    return 1;                                    // success
}

// Pop a value from the stack
int pop(Node** top, double* val) {
    if (*top == NULL) {                // check if stack is empty
        printf("Greska: stog je prazan\n");
        return 0;
    }

    Node* temp = *top;                 // store top node
    *val = temp->value;                // take its value
    *top = temp->next;                 // move top to next node
    free(temp);                        // free old top
    return 1;                          // success
}

// Evaluate postfix expression from file
int evaluatePostfix(const char* fileName, double* result) {
    FILE* file = fopen(fileName, "r"); // open file
    if (!file) {
        printf("Greska: ne mogu otvoriti datoteku '%s'\n", fileName);
        return 0;
    }

    Node* stack = NULL;
    char token[100];

    // read one token (string) at a time
    while (fscanf(file, "%s", token) == 1) {
        char* endPtr;
        double num = strtod(token, &endPtr); // try to convert to double

        if (endPtr != token && *endPtr == '\0') {
            // it's a number ? push on stack
            push(&stack, num);
        }
        else {
            // it's an operator
            double a, b, res;
            if (!pop(&stack, &b) || !pop(&stack, &a)) {
                printf("Greska: neispravan izraz\n");
                fclose(file);
                return 0;
            }

            if (strcmp(token, "+") == 0)
                res = a + b;
            else if (strcmp(token, "-") == 0)
                res = a - b;
            else if (strcmp(token, "*") == 0)
                res = a * b;
            else if (strcmp(token, "/") == 0) {
                if (b == 0) {
                    printf("Greska: dijeljenje s nulom\n");
                    fclose(file);
                    return 0;
                }
                res = a / b;
            }
            else {
                printf("Greska: nepoznat operator '%s'\n", token);
                fclose(file);
                return 0;
            }

            push(&stack, res); // push result back
        }
    }

    fclose(file);

    if (!pop(&stack, result)) return 0; // final result

    if (stack != NULL)
        printf("Upozorenje: elementi su ostali na stogu\n");

    return 1;
}