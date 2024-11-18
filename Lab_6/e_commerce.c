#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void data_cleaning(double* raw_data, double* cleaned_data, int data_size);
void data_transformation(double* cleaned_data, double* transformed_data, int data_size);
void feature_extraction(double* transformed_data, double* features, int data_size);
void model_prediction(double* features, double* predictions, int data_size);

int main() {
    int data_size = 1000000;

    double* raw_data = (double*)malloc(data_size * sizeof(double));
    double* cleaned_data = (double*)malloc(data_size * sizeof(double));
    double* transformed_data = (double*)malloc(data_size * sizeof(double));
    double* features = (double*)malloc(data_size * sizeof(double));
    double* predictions = (double*)malloc(data_size * sizeof(double));

    for (int i = 0; i < data_size; i++) {
        raw_data[i] = (double)(rand() % 100);
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            {
                data_cleaning(raw_data, cleaned_data, data_size);
                printf("Data cleaning completed.\n");
            }

            #pragma omp task depend(in: cleaned_data)
            {
                data_transformation(cleaned_data, transformed_data, data_size);
                printf("Data transformation completed.\n");
            }

            #pragma omp task depend(in: transformed_data)
            {
                feature_extraction(transformed_data, features, data_size);
                printf("Feature extraction completed.\n");
            }

            #pragma omp task depend(in: features)
            {
                model_prediction(features, predictions, data_size);
                printf("Model prediction completed.\n");
            }
        }
    }

    free(raw_data);
    free(cleaned_data);
    free(transformed_data);
    free(features);
    free(predictions);

    return 0;
}

void data_cleaning(double* raw_data, double* cleaned_data, int data_size) {
    for (int i = 0; i < data_size; i++) {
        cleaned_data[i] = raw_data[i];
    }
}

void data_transformation(double* cleaned_data, double* transformed_data, int data_size) {
    for (int i = 0; i < data_size; i++) {
        transformed_data[i] = cleaned_data[i] * 0.5;
    }
}

void feature_extraction(double* transformed_data, double* features, int data_size) {
    for (int i = 0; i < data_size; i++) {
        features[i] = transformed_data[i] + 10.0;
    }
}

void model_prediction(double* features, double* predictions, int data_size) {
    for (int i = 0; i < data_size; i++) {
        predictions[i] = features[i] * 1.5;
    }
}
