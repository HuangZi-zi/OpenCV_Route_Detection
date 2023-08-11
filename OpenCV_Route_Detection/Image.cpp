#include "Image.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

class PixelVisit {
public:

    void mat_pixel_visit_ptr(Mat& image);
    PixelVisit() {  }
    ~PixelVisit() {  }
};

void PixelVisit::mat_pixel_visit_ptr(Mat& image)
{
    int w = image.cols, h = image.rows, dims = image.channels();
    for (int row = 0; row < h; ++row) {
        uchar* current_row = image.ptr<uchar>(row);
        for (int col = 0; col < w; ++col) {
            for (int i = 0; i < dims; ++i) {
                *current_row++ = 255 - *current_row;
            }
        }
    }
    imshow("demo", image);
}

int VisitPixel(Mat& img)
{
    if (img.empty())
    {
        cout << "could not load image...." << endl;
        return -1;
    }
    namedWindow("输入窗口", WINDOW_FREERATIO);
    imshow("输入窗口", img);
    PixelVisit qd;
    qd.mat_pixel_visit_ptr(img);
    //    qd.mat_pixel_visit_at(src);
    waitKey(0);
    destroyAllWindows();
    return 0;
}



