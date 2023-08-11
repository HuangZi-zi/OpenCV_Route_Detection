// OpenCV_Route_Detection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/types_c.h>

#include "SendByte.h"
#include "Image.h"

using namespace cv;
using namespace std;

int ProcessImg(Mat& img)
{
    //视角变换
    Mat imgWarp;
    //float w = 240, h = 630;
    //Point2f src[4] = { {182, 56}, {440, 37} ,{3, 457}, {622, 413}};
    //Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
    //matrix = getPerspectiveTransform(src, dst);
    //warpPerspective(img, imgWarp, matrix,Point(w,h));
    imgWarp = img;

    //图像处理
    Mat kernel_dil = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat kernel_erode = getStructuringElement(-MORPH_RECT, Size(9, 9));
    //灰度
    cvtColor(imgWarp, imgWarp, COLOR_BGR2GRAY);
    //二值化
    threshold(imgWarp, imgWarp, 80, 255, cv::THRESH_BINARY);
    //膨胀
    dilate(imgWarp, imgWarp, kernel_dil);
    //模糊
    GaussianBlur(imgWarp, imgWarp, Size(9, 9), 3, 3);
    //imshow("ImageWarp", imgWarp);
        //腐蚀
    erode(imgWarp, imgWarp, kernel_erode);
    //imshow("ImageWarp", imgWarp);
        //再次二值化
    threshold(imgWarp, imgWarp, 80, 255, cv::THRESH_BINARY);
    //imshow("ImageWarp", imgWarp);

    //边缘查找
    Mat imgCanny;
    Canny(imgWarp, imgCanny, 25, 75);
    imshow("ImageCanny", imgCanny);

    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    findContours(imgCanny, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    return contours.size();
}

int main()
{
    Mat img;
    //导入图片
    //string path = "Resources/square.png";
    string path = "Resources/left.png";
    img = imread(path);
    imshow("img", img);
    int size;
    size=ProcessImg(img);
    cout << size << endl;
    if (size>= 2)
    {
        //前进
        cout << "forward" << endl;
        //Send(COMM_FORWARD);
    }
    vector<Vec4i> plines;
    HoughLinesP(img, plines, 1, CV_PI / 180, 150, 10, 10);
    for (size_t i = 0; i < plines.size(); i++)
    {
        Vec4i points = plines[i];
        line(img, Point(points[0], points[1]), Point(points[2], points[3]), Scalar(0, 255, 255), 3, CV_AA);
    }
    imshow("output", img);

    waitKey(0);





    
    //导入视频
    //string Vpath = "http://192.168.1.1:8080/?action=stream";
    //VideoCapture cap(Vpath);
    //Mat img;
    //while (true)
    //{
    //    cap.read(img);
    //    imshow("Image", img);
    //    waitKey(20);
    //}




}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
