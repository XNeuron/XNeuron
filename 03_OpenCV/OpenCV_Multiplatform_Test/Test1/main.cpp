#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Mat image;
        image = imread("Wall.E.png", CV_LOAD_IMAGE_COLOR);
        namedWindow( "Display window", WINDOW_AUTOSIZE );
        imshow( "Display window", image );
    return a.exec();
}
