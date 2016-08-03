#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <vector>
#include <QList>
#include <QFileInfo>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QDebug>

using namespace cv;
using namespace std;

int main(int, char**)
{

    QDateTime start = QDateTime::currentDateTime();
    qDebug() << "start time: " << start << "\n";
    QList<Mat> tInput;
    int file=1;
    QString Name=QDir().currentPath()+"/Input/"+QString::number(file++)+".jpg";
    do
    {
        if(QFileInfo(Name).exists())
        {
            Mat temp=imread(Name.toStdString());
            tInput.append(temp);
            Name=QDir().currentPath()+"/Input/"+QString::number(file++)+".jpg";
        }
    }while(QFileInfo(Name).exists());



    QList<Mat> tResponses;
    file=1;
    Name=QDir().currentPath()+"/Input/"+QString::number(file++)+".jpg";
    do
    {
        Mat temp=imread(Name.toStdString());
        pyrDown( temp, temp );
        tResponses.append(temp);
        Name=QDir().currentPath()+"/Responses/"+QString::number(file++)+".jpg";
    }while(QFileInfo(Name).exists());


    int max=tInput[0].rows*tInput[0].cols*3;
    QList<QList<float>> tBWList;
    QList<QList<float>> tColorList;
    int w,h;
    h=tInput[0].rows;
    w=tInput[0].cols*3;

    for(int m=0;m<tInput.length();m++)
    {
        Mat tBW=tInput[m];

        if(max>tBW.rows*tBW.cols*3)
        {   h=tBW.rows;
            w=tBW.cols;
            max=tBW.rows*tBW.cols*3;
        }
        auto tPtr=tBW.ptr<uchar>(0);
        QList<float> tBWRow;
        for (int i = 0; i < tBW.rows; ++i)
        {
            for (int j = 0; j < tBW.cols; ++j)
            {
                tBWRow.append((float)(*tPtr++)/255);
                tBWRow.append((float)(*tPtr++)/255);
                tBWRow.append((float)(*tPtr++)/255);
            }
        }
        tBWList.append(tBWRow);
    }

    for(int m=0; m<tResponses.length();m++)
    {
        Mat tColor = tResponses[m];
        if(max>tColor.rows*tColor.cols*3)
        {
            h=tColor.rows;
            w=tColor.cols;
            max=tColor.rows*tColor.cols*3;
        }
        auto tPtr=tColor.ptr<uchar>(0);
        QList<float> tColorRow;
        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                tColorRow.append((float)(*tPtr++)/255);
                tColorRow.append((float)(*tPtr++)/255);
                tColorRow.append((float)(*tPtr++)/255);
            }
        }
        tColorList.append(tColorRow);
    }

    int inputLayerSize = max;
    int outputLayerSize = max;
    int numSamples = tColorList.length();

    Mat samples( Size( inputLayerSize, numSamples ), CV_32F );
    Mat responses( Size( outputLayerSize, numSamples ), CV_32F );

    int index=0;
    while(index < max)
    {
        for(int s = 0; s < tColorList.length(); s++)
        {
            samples.at<float>( Point( index, s ) ) = tBWList[s][index];
            responses.at<float>( Point( index, s ) ) = tColorList[s][index];
        }
        index++;
    }

    vector<int> layerSizes = { inputLayerSize, 50, inputLayerSize };
    Ptr<ml::ANN_MLP> nnPtr;
    if(QFileInfo("Color.yml").exists())
    {
        cout << "Loading\n" << endl;
        nnPtr=Algorithm::load<ml::ANN_MLP>("Color.yml");
    }
    else
    {
        nnPtr = ml::ANN_MLP::create();

        nnPtr->setLayerSizes( layerSizes );
        nnPtr->setActivationFunction( cv::ml::ANN_MLP::SIGMOID_SYM );
        nnPtr->setTrainMethod(ml::ANN_MLP::BACKPROP,0.01,0.01);

        TermCriteria tc;
        tc.epsilon=0.01;
        tc.maxCount=10000;
        tc.type = TermCriteria::MAX_ITER + TermCriteria::EPS;

        cout << "begin training:\n" << endl;
        if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
            return 1;
        cout << "end training:\n" << endl;
        nnPtr->save("Color.yml");
        QDateTime end = QDateTime::currentDateTime();
        qDebug() << "end training: " << end << "\n";
        qDebug() << "diff time training: " << start.msecsTo(end) << "\n";
    }

    cout << "Predicting\n" << endl;
    Mat output;
    nnPtr->predict( samples, output );

    cout << "Saving\n" << endl;
    for( int r=0; r < output.rows;r++)
    {
        index=0;
        Mat image2( Size( w, h ), CV_32FC3);
        auto Out1=output.ptr<float>(r);
        auto im1=image2.ptr<float>(0);
        for( int c=0; c < output.cols;c++)
        {
            *im1=Out1[c];
            im1++;
            index++;
        }
        Name=QDir().currentPath()+"/Output/"+QString::number(r)+".jpg";
        imwrite( Name.toStdString(), image2 );
    }
    cout << "Finished\n" << endl;
    QDateTime end = QDateTime::currentDateTime();
    qDebug() << "end time: " << end << "\n";
    qDebug() << "diff time: " << start.msecsTo(end) << "\n";
    return 0;
}
