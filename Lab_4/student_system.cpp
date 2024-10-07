#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_STUDENTS 100
#define DEPT_COUNT 4
#define MAX_RECORDS 25

typedef struct {
    int id;
    float grade;
    int enrollment_status;
} Student;

void process_students(Student *students, int count) {
    for (int i = 0; i < count; i++) {
        students[i].grade += 1.0;
        students[i].enrollment_status = (students[i].grade >= 50) ? 1 : 0;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total_students = TOTAL_STUDENTS;
    float grading_scheme = 50.0;
    int semester = 2023;

    Student students[TOTAL_STUDENTS];
    if (rank == 0) {
        for (int i = 0; i < TOTAL_STUDENTS; i++) {
            students[i].id = i;
            students[i].grade = rand() % 100;
            students[i].enrollment_status = 1;
        }
    }

    MPI_Bcast(&total_students, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&grading_scheme, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&semester, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int students_per_dept = total_students / size;
    int remainder = total_students % size;

    Student *department_students = (Student *)malloc(students_per_dept * sizeof(Student));

    MPI_Scatter(students, students_per_dept * sizeof(Student), MPI_BYTE,
                department_students, students_per_dept * sizeof(Student), MPI_BYTE,
                0, MPI_COMM_WORLD);

    process_students(department_students, students_per_dept);

    MPI_Gather(department_students, students_per_dept * sizeof(Student), MPI_BYTE,
               students, students_per_dept * sizeof(Student), MPI_BYTE,
               0, MPI_COMM_WORLD);

    int enrolled_count = 0;
    for (int i = 0; i < students_per_dept; i++) {
        if (department_students[i].enrollment_status == 1) enrolled_count++;
    }

    int *enrolled_counts = NULL;
    if (rank == 0) {
        enrolled_counts = (int *)malloc(size * sizeof(int));
    }
    MPI_Gather(&enrolled_count, 1, MPI_INT, enrolled_counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int total_enrollment = 0;
        for (int i = 0; i < size; i++) {
            total_enrollment += enrolled_counts[i];
        }
        printf("Total enrollment across departments: %d\n", total_enrollment);
    }

    int global_enrollment;
    MPI_Allgather(&enrolled_count, 1, MPI_INT, &global_enrollment, 1, MPI_INT, MPI_COMM_WORLD);

    free(department_students);
    if (rank == 0) free(enrolled_counts);

    MPI_Finalize();
    return 0;
}
