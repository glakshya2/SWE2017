#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

typedef struct {
    double* values; 
    int numValues;
} CleanedData;

typedef struct {
    double average;
    double stdDev;
} AnalysisResults;


void dataPreprocessing(CleanedData* cleanedData) {
    printf("Preprocessing\n");
    printf("Data Preprocessing started\n");
    cleanedData->numValues = 1000000;
    cleanedData->values = (double*)malloc(sizeof(double) * cleanedData->numValues);
    for (int i = 0; i < cleanedData->numValues; i++) {
        cleanedData->values[i] = (double)rand() / RAND_MAX; 
    }
    printf("Data Preprocessing completed\n");
}

void dataAnalysis(CleanedData* cleanedData, AnalysisResults* results) {
    printf("Data Analysis started\n");
    double sum = 0.0;
    for (int i = 0; i < cleanedData->numValues; ++i) {
        sum += cleanedData->values[i];
    }
    results->average = sum / cleanedData->numValues;

    double sqSum = 0.0;
    for (int i = 0; i < cleanedData->numValues; ++i) {
        sqSum += pow(cleanedData->values[i] - results->average, 2);
    }
    results->stdDev = sqrt(sqSum / cleanedData->numValues);
    printf("Data Analysis completed\n");
}

void dataVisualization(AnalysisResults* results) {
    printf("Data Visualization started\n");
    printf("Average: %f\n", results->average);
    printf("Standard Deviation: %f\n", results->stdDev);
    printf("Data Visualization completed\n");
}

int main() {
    CleanedData cleanedData;
    AnalysisResults results;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            dataPreprocessing(&cleanedData);
        }

        #pragma omp section
        {
            dataAnalysis(&cleanedData, &results); 
        }

        #pragma omp section
        {
            dataVisualization(&results);
        }
    }

    free(cleanedData.values);
    return 0;
}