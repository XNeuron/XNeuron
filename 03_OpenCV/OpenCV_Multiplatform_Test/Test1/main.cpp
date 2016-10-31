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
    image = imread(imageName.c_str(), CV_LOAD_IMAGE_COLOR);           // Read the file

    if( image.empty() )                                               // Check for invalid input
    {
        cout << " Could not open or find the image " << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );                 // Create a window for display.
    imshow( "Display window", image );                                // Show our image inside it.

    cvWaitKey(10000);

    return 0;
}
