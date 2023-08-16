//这里盛放被淘汰的算法，谨供纪念

/*
简述：通过扫描两行像素，比较黑色区域出现的位置，进而判断图像中的线是向左倾斜还是向右倾斜
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
*/