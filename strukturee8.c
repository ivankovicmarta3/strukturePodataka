#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct tree* treePos;

struct tree {
    int value;
    treePos left;
    treePos right;
};
treePos Insert(treePos root, int value);
treePos Delete(treePos root, int value);
treePos FindMin(treePos root);
int Search(treePos root, int value);
void Inorder(treePos root);
void Preorder(treePos root);
void Postorder(treePos root);

int main() {
    treePos root = NULL;            // root 
    int choice, value;

    do {
        
        printf("1. Insert element\n");
        printf("2. Inorder\n");
        printf("3. Preorder\n");
        printf("4. Postorder\n");
        printf("5. Search element\n");
        printf("6. Delete element\n");
        printf("0. Exit\n");
        printf("Choice: "); scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter value: ");
            scanf("%d", &value);
            root = Insert(root, value);       // insert value
            break;
        case 2:
            printf("Inorder: ");
            Inorder(root);                    // print inorder
            printf("\n");
            break;
        case 3:
            printf("Preorder: ");
            Preorder(root);                   // print preorder
            printf("\n");
            break;
        case 4:
            printf("Postorder: ");
            Postorder(root);                  // print postorder
            printf("\n");
            break;
        case 5:
            printf("Enter value: ");
            scanf("%d", &value);
            printf(Search(root, value) ? "Found\n" : "Not found\n"); // search
            break;
        case 6:
            printf("Enter value to delete: ");
            scanf("%d", &value);
            root = Delete(root, value);       // delete value
            break;
        }
    } while (choice != 0);                      // repeat until exit

    return 0;
}



treePos Insert(treePos root, int value) {

    if (root == NULL) {                                      // if tree is empty

        root = (treePos)malloc(sizeof(struct tree));         // allocate memory
        if (root == NULL)                                   // check allocation
            return NULL;

        root->value = value;                                 // set value
        root->left = NULL;                                   // left child empty
        root->right = NULL;                                  // right child empty

        return root;                                         // return new node
    }

    if (value < root->value)                                 // smaller value
        root->left = Insert(root->left, value);              // go left

    else if (value > root->value)                            // larger value
        root->right = Insert(root->right, value);            // go right

    return root;                                             // return root
}

treePos FindMin(treePos root) {

    while (root->left != NULL)           // go to leftmost node
        root = root->left;

    return root;                          // return smallest node
}

treePos Delete(treePos root, int value) {

    if (root == NULL)                     // if tree is empty
        return NULL;

    if (value < root->value)              // value is smaller
        root->left = Delete(root->left, value);   // go left

    else if (value > root->value)         // value is larger
        root->right = Delete(root->right, value); // go right

    else {                                // node found

        if (root->left != NULL && root->right != NULL) { // two children

            treePos temp = FindMin(root->right);          // find min in right subtree
            root->value = temp->value;                    // copy value
            root->right = Delete(root->right, temp->value); // delete duplicate
        }
        else {                             // one or no child

            treePos temp = root;           // save current node

            if (root->left == NULL)        // no left child
                root = root->right;        // move to right child
            else                           // left child exists
                root = root->left;         // move to left child

            free(temp);                    // free old node
        }
    }

    return root;                           // return root
}



int Search(treePos root, int value) {

    if (root == NULL)                                      // empty tree
        return 0;
    if (root->value == value)                               // found
        return 1;
    if (value < root->value)                                // go left
        return Search(root->left, value);
    else                                                    // go right
        return Search(root->right, value);
}

void Inorder(treePos root) {
    if (root == NULL) return;                    // empty node
    Inorder(root->left);                         // visit left
    printf("%d ", root->value);                  // visit root
    Inorder(root->right);                        // visit right
}

void Preorder(treePos root) {
    if (root == NULL) return;                    // empty node
    printf("%d ", root->value);                  // visit root
    Preorder(root->left);                        // visit left
    Preorder(root->right);                       // visit right
}

void Postorder(treePos root) {
    if (root == NULL) return;                    // empty node
    Postorder(root->left);                       // visit left
    Postorder(root->right);                      // visit right
    printf("%d ", root->value);                  // visit root
}

