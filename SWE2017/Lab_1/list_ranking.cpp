#include <iostream>
#include <omp.h>
using namespace std;

struct Node {
  int value;
  Node* next;
  int rank;
};

int main() {
  Node* head = new Node{1, nullptr, 0};
  Node* current = head;
  for (long long i = 2; i <= 100000000; ++i) {
    current->next = new Node{1, nullptr, 0};
    current = current->next;
  }

  // Serial
  double start_time_serial = omp_get_wtime();

  current = head;
  int rank = 0;
  while (current != nullptr) {
    current->rank = rank;
    rank++;
    current = current->next;
  }

  double end_time_serial = omp_get_wtime();
  double duration_serial = end_time_serial - start_time_serial;

  cout << "Serial Execution Time: " << duration_serial << " seconds" << endl;

  // Parallel
  for (int num_threads = 2; num_threads <= 8; num_threads *= 2) {
    
    current = head;
    while (current != nullptr) {
      current->rank = 0;
      current = current->next;
    }

    omp_set_num_threads(num_threads);

    double start_time_parallel = omp_get_wtime();

    #pragma omp parallel
    {
      int rank = 0;
      #pragma omp single
      {
        Node* current = head;
        while (current != nullptr) {
          current->rank = rank;
          rank++;
          current = current->next;
        }
      }
    }

    double end_time_parallel = omp_get_wtime();
    double duration_parallel = end_time_parallel - start_time_parallel;

    cout << "Parallel Execution Time (" << num_threads << " threads): " << duration_parallel << " seconds" << endl;
  }

  return 0;
}