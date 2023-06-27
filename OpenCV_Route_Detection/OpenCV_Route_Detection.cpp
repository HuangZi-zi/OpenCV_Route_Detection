// OpenCV_Route_Detection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

vector<vector<double>> creatmatrix(int h, int l)
{
    vector<vector<double>> v;
    for (int i = 0; i < h; i++)
    {
        vector<double>v1(l, 0);
        v.push_back(v1);
    }
    return v;
}

int main()
{   //导入图片
    string path = "Resources/square.png";
    Mat img = imread(path);
    Mat matrix, imgWarp;
    float w = 210, h = 297;
    Point2f src[4] = { {213, 181}, {527, 168} ,{102, 423}, {618, 396}};
    Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
    matrix = getPerspectiveTransform(src, dst);
    
    warpPerspective(img, imgWarp, matrix,Point(640,480));

        for (int i = 0; i < 4; i++) {
            circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
        }

        imshow("Image", img);
        imshow("ImageWarp", imgWarp);
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
