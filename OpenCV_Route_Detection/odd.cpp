//����ʢ�ű���̭���㷨����������

/*
������ͨ��ɨ���������أ��ȽϺ�ɫ������ֵ�λ�ã������ж�ͼ���е�����������б����������б
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