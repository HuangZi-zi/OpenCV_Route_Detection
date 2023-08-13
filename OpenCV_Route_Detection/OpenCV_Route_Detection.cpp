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

using namespace cv;
using namespace std;

/*ͼ����̬ѧ����*/
int ProcessImg(Mat& origin, Mat& img, Mat& imgCanny)
{
    //��Ӱ�ɫ�߿�
    copyMakeBorder(origin, img, 3, 3, 3, 3, BORDER_CONSTANT, Scalar(255, 255, 255));
    int size = 0;
    //imshow("Image", img);
    
    //ͼ����
    Mat kernel_dil = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat kernel_erode = getStructuringElement(-MORPH_RECT, Size(9, 9));
    //�Ҷ�
    cvtColor(img, img, COLOR_BGR2GRAY);
    //��ֵ��
    threshold(img, img, 80, 255, cv::THRESH_BINARY);
    //����
    dilate(img, img, kernel_dil);
    //ģ��
    GaussianBlur(img, img, Size(9, 9), 3, 3);
    //imshow("Image", img);
        //��ʴ
    erode(img, img, kernel_erode);
    //imshow("Image", img);
        //�ٴζ�ֵ��
    threshold(img, img, 80, 255, cv::THRESH_BINARY);
    imshow("Processed", img);

    //��Ե����
    Canny(img, imgCanny, 25, 75);
    dilate(imgCanny, imgCanny, kernel_dil);
    //imshow("ImageCanny", imgCanny);
   
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    findContours(imgCanny, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //cout << "Total contours size " << contours.size() << endl;
    for (int i = 0; i < contours.size(); i++)
    {
        //drawContours(origin, contours, i, Scalar(255, 0, 255), 2);
        int area = contourArea(contours[i]);
        //cout << "contour " << i << " " << area << endl;
        if (area > 2000)
        {
            size++;
        }
    }
    //cout << "Recognized contours size "<< size << endl;
    //imshow("contours", origin);
    return size;
}

/*��������*/
class PixelVisit {
public:
    int high_pixel=0;
    int low_pixel=0;
public:
    void mat_pixel_visit_ptr(Mat& image);
    PixelVisit() {  }
    ~PixelVisit() {  }
};

void PixelVisit::mat_pixel_visit_ptr(Mat& image)
{
    int w = image.cols, h = image.rows;
    //high
    int blackPixelCount_High = 0;
    int blackPixelAddress_High[640];
    int blackPixelCount_Low = 0;
    int blackPixelAddress_Low[640];

        uchar* rowData1 = image.ptr(90);
        for (int col = 0; col < image.cols; ++col)
        {
            if (rowData1[col] == 0)
            {
                blackPixelAddress_High[blackPixelCount_High++] = col;
            }
        }
    
    //low
    
        uchar* rowData2 = image.ptr(150);
        for (int col = 0; col < image.cols; ++col)
        {
            if (rowData2[col] == 0)
            {
                blackPixelAddress_Low[blackPixelCount_Low++] = col;
            }
        }
    
    high_pixel = blackPixelAddress_High[5];
    low_pixel = blackPixelAddress_Low[5];
}



int main()
{
    Mat origin,imgCanny;
    Mat img;
    //����ͼƬ
    //string path = "Resources/square.png";
    string path = "Resources/forward2.png";
    
    //������Ƶ
    //string Vpath = "http://192.168.1.1:8080/?action=stream";
    //VideoCapture cap(Vpath);
    
    PixelVisit qd;
    int size;
    //while(1)
    { 
        origin = imread(path);
        //cap.read(origin);
        if (origin.empty())
        {
            cout << "could not load image...." << endl;
            return -1;
        }

        size = ProcessImg(origin, img, imgCanny);
        if (size >= 2)
        {
            //ǰ��
            cout << "forward" << endl;
            //Send(COMM_FORWARD);
        }
        else
        {

            qd.mat_pixel_visit_ptr(img);
            if (qd.high_pixel > qd.low_pixel)
            {
                //��ת
                cout << "right" << endl;
                //Send(COMM_RIGHT);
            }
            else
            {
                //��ת
                cout << "left" << endl;
                //Send(COMM_LEFT);
            }
        }
    
    waitKey(10);
    }
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
