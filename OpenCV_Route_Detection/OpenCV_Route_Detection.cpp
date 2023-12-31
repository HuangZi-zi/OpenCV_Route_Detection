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
#include <opencv2/core/core.hpp>



#include "SendByte.h"

using namespace cv;
using namespace std;

vector<Mat> m_vec_frame;    //视频缓存容器

class Img {
private:
    //导入图片
    //string path = "Resources/square.png";
    string path = "Resources/light2.png";
public:
    void import_picture(Mat& origin);
public:
    Mat output;
    Mat mask;//高光区遮罩
    Mat img_segment[3];//上、中、下三个判别区域
    int position[3][2];//左、右标线的中心X座标
    int status[3];//道路中心座标
    int flag_position = 0;//标线判别标识
    char command = COMM_BRAKE;
    Scalar t;//二值化阈值
    int flag = 0;//地面反光标识
};

void Img::import_picture(Mat& origin)
{
    origin = imread(path);
}

class ImgProcess: public Img{
public:
    //膨胀白色变多
    Mat kernel_dil_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat kernel_dil_5 = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat kernel_dil_7 = getStructuringElement(MORPH_RECT, Size(7, 7));
    Mat kernel_dil_9 = getStructuringElement(MORPH_RECT, Size(9, 9));
    //腐蚀黑色变多
    Mat kernel_erode_3 = getStructuringElement(-MORPH_RECT, Size(3, 3));
    Mat kernel_erode_9 = getStructuringElement(-MORPH_RECT, Size(9, 9));
public:
    void Basic_Process(Mat& img);
    void Find_Position();
    void Get_Command();
    void Prepare_LUTS();
public:
    Mat lookUpTable;
};

/*图像基本处理*/
void ImgProcess::Basic_Process(Mat& img)
{
    //imshow("origin", img);
 
    //判断地面是否反光
    cvtColor(img, mask, COLOR_BGR2GRAY);
    t = mean(mask);
    threshold(mask, mask, t[0] + 30, 255, cv::THRESH_BINARY);
    t = mean(mask);
    if (t[0] > 55)
    {
        flag = 1;
    }
    else
    {
        flag = 0;
    }
    //灰度
    cvtColor(img, img, COLOR_BGR2GRAY);
    //二值化
    t = mean(img);
    threshold(img, img, t[0]/2, 255, cv::THRESH_BINARY);
    //imshow("first threshold", img);
    //膨胀
    dilate(img, img, kernel_dil_7);
    //imshow("dilate", img);
    //模糊
    GaussianBlur(img, img, Size(11, 11), 5, 5);
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
        flag_position *= 10;
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
            flag_position += 1;
            break;
        default:
            M = moments(contours[0]);
            position[i][0] = int(M.m10 / M.m00);
            M = moments(contours[1]);
            position[i][1] = int(M.m10 / M.m00);
            flag_position += 2;
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
    if (flag)
    {
        command = COMM_IR;
        cout << "IR Tracing!!!" << endl;
        flag = 0;
    }
    else
    {
        switch (flag_position)
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
            cout << "FORWARD" << endl;
            //cout << command << endl;
            break;
        case 111:
        case  11:
            if (status[1] - status[2] > 0 && position[2][0] < 160)
            {
                command = COMM_RIGHT;
                cout << "RIGHT" << endl;
            }
            else
            {
                command = COMM_LEFT;
                cout << "LEFT" << endl;
            }
            break;
        case 110:
            if (status[0] - status[1] > 0 && position[1][0] < 200)
            {
                command = COMM_RIGHT;
                cout << "RIGHT" << endl;
            }
            else
            {
                command = COMM_LEFT;
                cout << "LEFT" << endl;
            }
            break;
        case 101:
            if (status[0] - status[2] > 0 && position[2][0] < 160)
            {
                command = COMM_RIGHT;
                cout << "RIGHT" << endl;
            }
            else
            {
                command = COMM_LEFT;
                cout << "LEFT" << endl;
            }
            break;
        case 100:
        case  10:
        case   1:
        case   0:
            command = COMM_IR;
            cout << "IR Tracing" << endl;
            break;
        default:
            command = COMM_BRAKE;
            cout << "ERR!!!" << endl;
            break;
        }
    }
    flag_position = 0;
}

/*计算查找表*/
void ImgProcess::Prepare_LUTS()
{
    lookUpTable = Mat_<uchar>(1, 256);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, 1.75) * 255.0);
}

/*读取视频*/
void capture_frame(void)
{
    Mat origin;
    //导入视频
    string Vpath = "http://192.168.1.1:8080/?action=stream";
    VideoCapture cap;
    cap.open(Vpath);
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    if (!cap.isOpened())
    {
        cout << "can't open camera!" << endl;
    }
    cap >> origin;

    Sleep(50);//初始化等待延时

    while (1)
    {
        cap >> origin;       
        if (origin.empty())
        {
            cout << "this frame is empty!" << endl;
            return;
        }
        if (m_vec_frame.size() > 3) m_vec_frame.clear();//缓存过多时丢掉缓存的内容以保证实时性
        m_vec_frame.push_back(origin);
    }
}

int main()
{
    ImgProcess IMGPROCESS;
    Img IMG;
    Mat frame;
    int size;
    
    //IMGPROCESS.Prepare_LUTS();

    
    // Create a thread for capturing image
    std::thread captureThread(capture_frame);
    captureThread.detach();

    // Create a thread for sending data
    std::thread senderThread([&IMGPROCESS]()
        {
            while (true)
            {
                Send(IMGPROCESS.command);
                //cout << IMGPROCESS.command << endl;
                cout << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });
    senderThread.detach();

    Sleep(1000);//等待读取视频的线程初始化
    

    ////这一段用来导入图片并处理
    //IMG.import_picture(frame);
    //imshow("origin", frame);
    //IMGPROCESS.Basic_Process(frame);
    //IMGPROCESS.Find_Position();
    //waitKey(0);

    
    while(1)
    {
        if (m_vec_frame.size() >= 1)
        {
            frame = m_vec_frame.back();
            imshow("origin", frame);
            IMGPROCESS.Basic_Process(frame);
            IMGPROCESS.Find_Position();
            IMGPROCESS.Get_Command();
            //cout << IMGPROCESS.command << endl;
        }
        waitKey(1);
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
