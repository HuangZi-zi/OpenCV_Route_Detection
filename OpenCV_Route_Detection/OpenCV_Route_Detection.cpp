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
private:
    //导入图片
    //string path = "Resources/square.png";
    string path = "Resources/right.png";
    //导入视频
    string Vpath = "http://192.168.1.1:8080/?action=stream";
    
public:
    void import_picture(Mat& origin);
    void import_video(Mat& origin);
public:
    Mat output;
    Mat img_segment[3];//上、中、下三个判别区域
    int position[3][2];//左、右标线的中心X座标
    int status[3];//道路中心座标
    int flag=0;//有效标识点数目
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
            flag++;
            break;
        default:
            M = moments(contours[0]);
            position[i][0] = int(M.m10 / M.m00);
            M = moments(contours[1]);
            position[i][1] = int(M.m10 / M.m00);
            flag++;
            break;
        }
        cout << "1: " << position[i][0] << "; " << "2: " << position[i][1] << endl;
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
    case 0:
    case 1:
        command = COMM_BACK;
        cout << "BACK" << endl;
        break;
    case 2:
    default:
        break;
    }
    int left = 0, right = 0;
    //left = position[0][0] + position[1][0] + position[2][0];
    //right = position[0][1] + position[1][1] + position[2][1];
    for (int i = 0; i < 3; i++)
    {
        if (status[i])
        {
        }
    }
    if (status[0]==0&&status[1] == 0)
    {
        
    }

    if (command == COMM_BRAKE)
    {
        if (position[0][0] - position[2][0] > 0)
        {
            command = COMM_RIGHT;
            cout << "RIGHT" << endl;
        }
        else
        {
            command = COMM_LEFT;
            cout << "LEFT" << endl;
        }
    }
    
    command = COMM_FORWARD;
    cout << "FORWARD" << endl;

}



int main()
{
    ImgProcess IMGPROCESS;
    Img IMG;
    Mat origin, imgCanny;

    int size;
    while(1)
    {
        IMG.import_picture(origin);
        //IMG.import_video(origin);
        if (origin.empty())
        {
            cout << "could not load image...." << endl;
            return -1;
        }
        IMGPROCESS.Basic_Process(origin);
        IMGPROCESS.Find_Position();

        //Send(IMG.command);
        

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
