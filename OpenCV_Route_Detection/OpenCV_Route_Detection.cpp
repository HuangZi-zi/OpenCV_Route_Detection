// OpenCV_Route_Detection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
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
private:
    //导入图片
    //string path = "Resources/square.png";
    string path = "Resources/right.png";
    //导入视频
    string Vpath = "http://192.168.1.1:8080/?action=stream&type=.mjpg";  
    //string Vpath = "http://192.168.1.1:8080/?action=snapshot&type=.mjpg";
public:
    void import_picture(Mat& origin);
    void import_video(Mat& origin);
public:
    Mat output;
    Mat img_segment[3];//上、中、下三个判别区域
    int position[3][2];//左、右标线的中心X座标
    int status[3];//道路中心座标
    int flag=0;//上中下三个判别区域的标识点情况
    char command = COMM_BRAKE;
};

void Img::import_picture(Mat& origin)
{
    origin = imread(path);
}
void Img::import_video(Mat& origin)
{
    static VideoCapture cap(Vpath);
    cap.read(origin);
    //origin = imread(Vpath);
    imshow("origin", origin);
}

class ImgProcess: public Img{
public:
    //膨胀白色变多
    Mat kernel_dil_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat kernel_dil_5 = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat kernel_dil_9 = getStructuringElement(MORPH_RECT, Size(9, 9));
    //腐蚀黑色变多
    Mat kernel_erode_3 = getStructuringElement(-MORPH_RECT, Size(3, 3));
    Mat kernel_erode_9 = getStructuringElement(-MORPH_RECT, Size(9, 9));
public:
    void Basic_Process(Mat& img);
    void Find_Position();
    void Get_Command();
};

/*图像基本处理*/
void ImgProcess::Basic_Process(Mat& img)
{
    //imshow("origin", img);

    //灰度
    cvtColor(img, img, COLOR_BGR2GRAY);

    //二值化
    threshold(img, img, 80, 255, cv::THRESH_BINARY);
    //膨胀
    dilate(img, img, kernel_dil_5);
    //模糊
    GaussianBlur(img, img, Size(9, 9), 3, 3);
    //imshow("blur", img);
    //腐蚀
    erode(img, img, kernel_erode_9);
    //imshow("erode", img);
    //再次二值化
    threshold(img, img, 80, 255, cv::THRESH_BINARY);
    //imshow("Processed", img);
    img.copyTo(output);
    cvtColor(output, output, COLOR_GRAY2BGR);
    //图像切分
    Mat temp;
    for (int i = 0; i < 3; i++)
    {
        img(Rect(0, i*80, img.cols, 40)).copyTo(temp);
            /*注意这里必须使用深拷贝，否则实际上分割后图像仅仅是原图像的新“指针”，会影响后续的处理*/
        //imshow("temp", temp);
        copyMakeBorder(temp, img_segment[i], 5, 5, 5, 5, BORDER_CONSTANT, 255);
    }
   
    //imshow("ImageUP", img_segment[0]);
    //imshow("ImageMID", img_segment[1]);
    //imshow("ImageBOT", img_segment[2]);
}

/*寻找轮廓中心*/
void ImgProcess::Find_Position()
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    Moments M;
    for (int i = 0; i < 3; i++)
    {
        //边缘查找
        Canny(img_segment[i], img_segment[i], 25, 75);
        dilate(img_segment[i], img_segment[i], kernel_dil_3);
        //轮廓查找
        findContours(img_segment[i], contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
        int size = contours.size();
        //cout << "size of segment " << i << " is " << size << endl;
        flag *= 10;
        switch (size)
        {
        case 0:
            position[i][0] = 0;
            position[i][1] = 0;
            break;
        case 1:
            M = moments(contours[0]);
            position[i][0] = int(M.m10 / M.m00);
            position[i][1] = 0;
            flag += 1;
            break;
        default:
            M = moments(contours[0]);
            position[i][0] = int(M.m10 / M.m00);
            M = moments(contours[1]);
            position[i][1] = int(M.m10 / M.m00);
            flag += 2;
            break;
        }
        //cout << "1: " << position[i][0] << "; " << "2: " << position[i][1] << endl;
        status[i] = (position[i][0] + position[i][1]) / 2;
        //drawContours(img_segment[i], contours, -1, Scalar(0, 255, 0), 2);
        circle(output, Point2d(position[i][0], i * 80 + 20), 1, Scalar(255, 0, 255), 2, 8, 0);
        circle(output, Point2d(position[i][1], i * 80 + 20), 1, Scalar(255, 0, 255), 2, 8, 0);

    }
    //imshow("up", img_segment[0]);
    //imshow("mid", img_segment[1]);
    //imshow("bot", img_segment[2]);
    imshow("output", output);
    
}

/*判断控制指令*/
void ImgProcess::Get_Command()
{
    switch (flag)
    {
    case 222:
    case  22:
    case 202:
    case 220:
    case 122:
    case 212:
    case 221:
    case 112:
    case 121:
    case 211:
    case  12:
    case 102:
    case 120:
    case  21:
    case 201:
    case 210:
    case 200:
    case  20:
    case   2:
        command = COMM_FORWARD;
        //cout << "FORWARD" << endl;
        //cout << command << endl;
        break;
    case 111:
    case  11:
        if (status[1] - status[2] > 0 && position[2][0] < 160)
        {
            command = COMM_RIGHT;
            //cout << "RIGHT" << endl;
        }
        else
        {
            command = COMM_LEFT;
            //cout << "LEFT" << endl;
        }
        break;
    case 110:
        if (status[0] - status[1] > 0 && position[1][0] < 200)
        {
            command = COMM_RIGHT;
            //cout << "RIGHT" << endl;
        }
        else
        {
            command = COMM_LEFT;
            //cout << "LEFT" << endl;
        }
        break;
    case 101:
        if (status[0] - status[2] > 0 && position[2][0] < 160)
        {
            command = COMM_RIGHT;
            //cout << "RIGHT" << endl;
        }
        else
        {
            command = COMM_LEFT;
            //cout << "LEFT" << endl;
        }
        break;
    case 100:
    case  10:
    case   1:
    case   0:
        command = COMM_BACK;
        //cout << "BACK" << endl;
        break;
    default:
        command = COMM_BRAKE;
        cout << "ERR!!!" << endl;
        break;
    }
    flag = 0;
}


int main()
{
    ImgProcess IMGPROCESS;
    Img IMG;
    Mat origin, imgCanny;

    int size;


    // Create a thread for sending data
    std::thread senderThread([&IMGPROCESS]()
        {
            while (true)
            {
                Send(IMGPROCESS.command);
                cout << IMGPROCESS.command << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });

    while(1)
    {

        //IMG.import_picture(origin);
        IMG.import_video(origin);
        if (origin.empty())
        {
            cout << "could not load image...." << endl;
            return -1;
        }
        IMGPROCESS.Basic_Process(origin);
        IMGPROCESS.Find_Position();

        IMGPROCESS.Get_Command();
        //cout << IMGPROCESS.command << endl;
        
        waitKey(1);

        // Join the sender thread to wait for it to finish before exiting
        //senderThread.join();
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
