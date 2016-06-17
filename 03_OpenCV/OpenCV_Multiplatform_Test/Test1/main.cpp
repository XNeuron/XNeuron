#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <string>
#include <QDir>
#include <QString>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QDir dir;
    string imageName=dir.absoluteFilePath("Wall.E.png").toStdString();
    QString temp;
    imageName=QDir::toNativeSeparators(temp.fromStdString(imageName)).toStdString();
    cout << imageName << "\n";
    Mat image;
    image = imread(imageName.c_str(), CV_LOAD_IMAGE_COLOR); // Read the file
    if( image.empty() )                      // Check for invalid input
    {
        cout <<  " Could not open or find the image " << std::endl ;
        return -1;
    }
    for(int l = 2;l<100;l++)
    {
        for(int i = 2;i<image.rows-3;i++)
        {
            uchar* p = image.ptr<uchar>(i);
            for(int j = 3;j<image.cols*3-3;j+=3)
            {
                p[j]=(uchar)(((double)p[j-1]+p[j])/2);
                p[j+1]=(uchar)(((double)p[j]+p[j+3])/2);
            }
        }
        namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
        imshow( "Display window", image );         // Show our image inside it.

        cvWaitKey(100);
    }


    return 0;
}
