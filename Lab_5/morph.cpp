#include <iostream>
#include <opencv2/opencv.hpp>
#include <omp.h>
#include <cmath>

using namespace cv;
using namespace std;

void morphImages(Mat& img1, const Mat& img2, int steps) {
    int rows = img1.rows;
    int cols = img1.cols;
    
    for (int step = 1; step <= steps; ++step) {
        float alpha = step / (float)steps;
        
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Vec3b &pixel1 = img1.at<Vec3b>(i, j);
                const Vec3b &pixel2 = img2.at<Vec3b>(i, j);
                
                for (int c = 0; c < 3; c++) {
                    pixel1[c] = pixel1[c] + alpha * (pixel2[c] - pixel1[c]);
                }
            }
        }
        
        imshow("Morphing", img1);
        waitKey(10);
    }
}

int main() {
    Mat img1 = imread("actor1.jpg");
    Mat img2 = imread("actor2.jpg");
    
    if (img1.size() != img2.size()) {
        cerr << "Images must be the same size." << endl;
        return -1;
    }

    int steps = 100;
    morphImages(img1, img2, steps);

    imshow("Final Morph", img1);
    waitKey(0);
    
    return 0;
}
