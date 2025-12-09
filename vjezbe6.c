#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Represents one product in a receipt
typedef struct Product {
    char productName[100];
    int amount;
    double unitPrice;
    struct Product* next;
} Product;

// Represents a receipt/bill
typedef struct Bill {
    char billDate[11];      // date in format YYYY-MM-DD
    Product* products;      // linked list of products in the bill
    struct Bill* next;
} Bill;

// Function declarations with descriptive names
Bill* loadAllBills(const char* fileList);
Bill* loadSingleBill(const char* fileName);
void insertBillSorted(Bill** head, Bill* b);
void insertProductSorted(Product** head, Product* p);
void runUserQuery(Bill* head);
int isDateEarlier(const char* date1, const char* date2);

int main() {
    Bill* bills = loadAllBills("receipts.txt");     // read all bills from the list file
    runUserQuery(bills);                           // ask user for queries
    return 0;
}

// Reads all bill filenames from list and loads them
Bill* loadAllBills(const char* fileList) {
    FILE* file = fopen(fileList, "r");
    if (!file) { printf("Cannot open the list file.\n"); exit(1); }

    Bill* head = NULL;
    char billFile[100];

    while (fscanf(file, "%s", billFile) == 1) {
        Bill* b = loadSingleBill(billFile);      // load a single bill
        if (b) insertBillSorted(&head, b);       // insert into list by date
    }

    fclose(file);
    return head;
}

// Reads one bill file and creates linked list of products
Bill* loadSingleBill(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) return NULL;

    Bill* b = malloc(sizeof(Bill));
    b->products = NULL;
    b->next = NULL;

    fscanf(file, "%s", b->billDate);            // first line: date

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        Product* p = malloc(sizeof(Product));
        p->next = NULL;

        sscanf(line, " %[^,], %d, %lf", p->productName, &p->amount, &p->unitPrice); // parse product info

        insertProductSorted(&b->products, p);   // add product alphabetically
    }

    fclose(file);
    return b;
}

// Inserts a bill into the list in chronological order
void insertBillSorted(Bill** head, Bill* b) {
    if (*head == NULL || isDateEarlier(b->billDate, (*head)->billDate)) {
        b->next = *head; *head = b; return;     // insert at beginning
    }

    Bill* current = *head;
    while (current->next && !isDateEarlier(b->billDate, current->next->billDate))
        current = current->next;               // find insertion point

    b->next = current->next;
    current->next = b;
}

// Inserts a product into the list alphabetically
void insertProductSorted(Product** head, Product* p) {
    if (*head == NULL || strcmp(p->productName, (*head)->productName) < 0) {
        p->next = *head; *head = p; return;    // insert at start
    }

    Product* current = *head;
    while (current->next && strcmp(p->productName, current->next->productName) > 0)
        current = current->next;               // find insertion point

    p->next = current->next;
    current->next = p;
}

// Returns 1 if first date is earlier than second
int isDateEarlier(const char* date1, const char* date2) {
    return strcmp(date1, date2) < 0;
}

// Allows the user to check total spent and quantity for a product in a date range
void runUserQuery(Bill* head) {
    char searchProduct[100], startDate[11], endDate[11];

    printf("Enter product name: ");
    scanf(" %99[^\n]", searchProduct);          // read product name

    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%s", startDate);                      // read starting date

    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%s", endDate);                        // read ending date

    double totalCost = 0; int totalAmount = 0;

    Bill* b = head;
    while (b) {
        if (strcmp(b->billDate, startDate) >= 0 && strcmp(b->billDate, endDate) <= 0) { // check date range
            Product* p = b->products;
            while (p) {
                if (strcmp(p->productName, searchProduct) == 0) {                        // matching product
                    totalCost += p->amount * p->unitPrice;                                // add to total cost
                    totalAmount += p->amount;                                            // add to total quantity
                }
                p = p->next;
            }
        }
        b = b->next;
    }

    printf("\nTotal money spent: %.2f EUR\n", totalCost);
    printf("Total quantity purchased: %d\n\n", totalAmount);
}
