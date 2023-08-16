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



class Img {
public:
    Mat img_1, img_2, img_3;//�ϡ��С��������б�����
    int position_1[2], position_2[2], position_3[2];//���ұ��ߵ�����X����
};

class ImgProcess: public Img{
public:
    Mat kernel_dil = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat kernel_erode = getStructuringElement(-MORPH_RECT, Size(9, 9));
    Mat kernel_canny_dil = getStructuringElement(MORPH_RECT, Size(3, 3));

public:
    void Basic_Process(Mat& img);
    void Find_Position();

};

/*Ѱ����������*/
void ImgProcess::Find_Position()
{
    
    //��Ե����
    Canny(img_1, img_1, 25, 75);
    dilate(img_1, img_1, kernel_dil);
    //��������
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    findContours(img_1, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int size = contours.size();
    Moments M;
    switch (size)
    {
    case 0:
        position_1[0] = 0;
        position_1[1] = 0;
        break;
    case 1:
        M = moments(contours[0]);
        position_1[0] = double(M.m10 / M.m00);
        position_1[1] = 0;
        break;
    default:
        M = moments(contours[0]);
        position_1[0] = double(M.m10 / M.m00);
        M = moments(contours[1]);
        position_1[1] = double(M.m10 / M.m00);
        break;
    }
    cout << "1: " << position_1[0] << "; " << "2: " << position_1[1] << endl;
}

/*ͼ���������*/
void ImgProcess::Basic_Process(Mat& img)
{
    //imshow("origin", img);
    //��Ӱ�ɫ�߿�
    //copyMakeBorder(origin, img, 3, 3, 3, 3, BORDER_CONSTANT, Scalar(255, 255, 255));
    int size = 0;

    //�Ҷ�
    cvtColor(img, img, COLOR_BGR2GRAY);
    //��ֵ��
    threshold(img, img, 80, 255, cv::THRESH_BINARY);
    //����
    dilate(img, img, kernel_dil);
    //ģ��
    GaussianBlur(img, img, Size(9, 9), 3, 3);
    //imshow("blur", img);
    //��ʴ
    erode(img, img, kernel_erode);
    //imshow("erode", img);
    //�ٴζ�ֵ��
    threshold(img, img, 80, 255, cv::THRESH_BINARY);
    imshow("Processed", img);

    //ͼ���з�
    img_1 = img(Rect(0, 0, img.cols, 40));
    img_2 = img(Rect(0, 80, img.cols, 40));
    img_3 = img(Rect(0, 160, img.cols, 40));
    //imshow("ImageUP", img_1);
    //imshow("ImageMID", img_2);
    //imshow("ImageBOT", img_3);
}

/*��������*/
class PixelVisit {
public:
    int high_pixel = 0;
    int low_pixel = 0;
public:
    void mat_pixel_visit_ptr(Mat& image);
    PixelVisit() {  }
    ~PixelVisit() {  }
};

void PixelVisit::mat_pixel_visit_ptr(Mat& image)
{
    
}



int main()
{
    PixelVisit pixelvisit;
    ImgProcess IMGPROCESS;
    Img IMG;
    Mat origin, imgCanny;
    
    //����ͼƬ
    //string path = "Resources/square.png";
    string path = "Resources/forward.png";

    //������Ƶ
    //string Vpath = "http://192.168.1.1:8080/?action=stream";
    //VideoCapture cap(Vpath);


    int size;
    while(1)
    {
        origin = imread(path);
        //cap.read(origin);
        if (origin.empty())
        {
            cout << "could not load image...." << endl;
            return -1;
        }
        IMGPROCESS.Basic_Process(origin);
        IMGPROCESS.Find_Position();

        //if (size >= 2)
        //{
        //    //ǰ��
        //    cout << "forward" << endl;
        //    Send(COMM_FORWARD);
        //}
        //else
        //{

            //qd.mat_pixel_visit_ptr(img_1);
            //if (qd.high_pixel > qd.low_pixel)
            //{
            //    //��ת
            //    cout << "right" << endl;
            //    Send(COMM_RIGHT);
            //}
            //else
            //{
            //    //��ת
            //    cout << "left" << endl;
            //    Send(COMM_LEFT);
            //}
        

        waitKey(0);
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
