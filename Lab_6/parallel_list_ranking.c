#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct Node {
    int value;
    struct Node* next;
    int rank;
} Node;

Node* create_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;
    new_node->rank = 0;
    return new_node;
}

void parallel_list_ranking(Node* head) {
    #pragma omp parallel
    {
        Node* curr = head;
        
        while (curr != NULL) {
            if (curr->next != NULL) {
                curr->rank = 1;
            } else {
                curr->rank = 0;
            }
            curr = curr->next;
        }

        int done = 0;
        while (!done) {
            done = 1;
            
            #pragma omp parallel for
            for (Node* node = head; node != NULL; node = node->next) {
                if (node->next != NULL && node->next->next != NULL) {
                    node->rank += node->next->rank;
                    node->next = node->next->next;
                    done = 0;
                }
            }
        }
    }
}

int detect_cycle(Node* head) {
    Node* slow = head;
    Node* fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return 1;
        }
    }
    return 0;
}

int main() {
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);

    if (detect_cycle(head)) {
        printf("Cycle detected. Aborting ranking.\n");
        return -1;
    }

    parallel_list_ranking(head);

    Node* curr = head;
    while (curr != NULL) {
        printf("Node Value: %d, Rank: %d\n", curr->value, curr->rank);
        curr = curr->next;
    }

    return 0;
}
