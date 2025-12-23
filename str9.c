#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree* treePos;                    // pointer to tree node

struct tree {
    int value;                                  // node value
    treePos left;                               // left child
    treePos right;                              // right child
};

treePos insert(treePos root, int value);        // insert function
int replace(treePos root);                      // replace function
void inorder(treePos root, FILE* fp);           // inorder print
void freeTree(treePos root);                    // free memory

int main() {
    treePos root = NULL;                        // tree root
    int arr[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };// given array
    int n = sizeof(arr) / sizeof(arr[0]);       // array size
    int i = 0;                                  // loop counter

    FILE* fp = fopen("inorder.txt", "w");       // open file
    if (!fp) return -1;                         // check file

    
    for (i = 0; i < n; i++)
        root = insert(root, arr[i]);            // insert element

    printf("Inorder before replace:\n");        
    inorder(root, NULL);                        // print to screen
    printf("\n");

    fprintf(fp, "Inorder before replace:\n");   
    inorder(root, fp);                          // write to file
    fprintf(fp, "\n");

    
    replace(root);                              // call replace

    printf("Inorder after replace:\n");         
    inorder(root, NULL);                        // print to screen
    printf("\n");

    fprintf(fp, "Inorder after replace:\n");    
    inorder(root, fp);                          // write to file
    fprintf(fp, "\n");

    
    freeTree(root);                             // free tree
    root = NULL;                                // reset root

    srand((unsigned)time(NULL));                // init random

    // generate 10 random values [10, 90]
    for (i = 0; i < 10; i++) {
        int r = rand() % 81 + 10;                // random number
        root = insert(root, r);                 // insert random value
    }

    fprintf(fp, "Inorder of random tree:\n");   // write title
    inorder(root, fp);                          // write random inorder
    fprintf(fp, "\n");

    fclose(fp);                                 // close file
    freeTree(root);                             // free tree
    return 0;                                   // end program
}

treePos insert(treePos root, int value) {
    if (root == NULL) {                                  // if empty place
        root = (treePos)malloc(sizeof(struct tree));     // allocate node
        root->value = value;                             // set value
        root->left = NULL;                               // set left child
        root->right = NULL;                              // set right child
        return root;                                     // return node
    }

    if (value > root->value)                             // if bigger
        root->left = insert(root->left, value);          // go left
    else                                                  // if smaller or equal
        root->right = insert(root->right, value);        // go right

    return root;                                         // return root
}

int replace(treePos root) {
    if (root == NULL) return 0;           // if empty node

    int old = root->value;                // save old value
    root->value = replace(root->left)     // sum of left
        + replace(root->right);  // sum of right

    return root->value + old;             // return subtree sum

}


void inorder(treePos root, FILE* fp) {
    if (root != NULL) {
        inorder(root->left, fp);                          // visit left

        if (fp == NULL)
            printf("%d ", root->value);                   // print to screen
        else
            fprintf(fp, "%d ", root->value);              // print to file

        inorder(root->right, fp);                         // visit right
    }
}

void freeTree(treePos root) {
    if (root != NULL) {
        freeTree(root->left);             // free left subtree
        freeTree(root->right);            // free right subtree
        free(root);                       // free node
    }
}
