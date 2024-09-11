#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Function to create a new tree node
struct TreeNode* createNode(int value) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void parallelPreorder(struct TreeNode* node) {
    if (node == NULL) return;

    #pragma omp critical
    {
        printf("%d ", node->data);
    }

    #pragma omp task shared(node)
    parallelPreorder(node->left);

    #pragma omp task shared(node)
    parallelPreorder(node->right);

    #pragma omp taskwait
}

int main() {
    struct TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->left = createNode(6);
    root->right->right = createNode(7);

    #pragma omp parallel
    {
        #pragma omp single
        parallelPreorder(root);
    }

    free(root->left->left);
    free(root->left->right);
    free(root->right->left);
    free(root->right->right);
    free(root->left);
    free(root->right);
    free(root);

    return 0;
}
