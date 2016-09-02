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
#include <QApplication>

using namespace cv;
using namespace std;

void SaveToImages(int index, QString xPath, int w, int h, Mat xOutPutImage)
{
    Mat image2( Size( w, h ), CV_32FC3);
    auto im1=image2.ptr<float>(0);
    int index2=0;
    int index3=0;
    for( int r=0; r < xOutPutImage.rows;r++)
    {

        auto Out1=xOutPutImage.ptr<float>(r);
        for( int c=0; c < xOutPutImage.cols;c++)
        {

            if(index2<w*h*3)
            {
                *im1=Out1[c]*255;
                im1++;
                index2++;
            }else
            {
                index2++;
                break;
            }
        }
        if(index2>=w*h*3)
        {
            //            namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
            //            imshow( "Display window", image2 );
            //            waitKey(1);

            QString Name=QDir().currentPath()+xPath+QString::number(index3++)+".jpg";
            imwrite( Name.toStdString(), image2 );

            im1=image2.ptr<float>(0);
            index2=0;
        }
    }

}


void SaveToImages2(QString xPath,int xLayerSize, int s, int w, int h, Mat xOutPutImages)
{
    Mat image2;
    if(xLayerSize==1)
        image2=Mat::zeros(h,w,CV_32FC1);
    if(xLayerSize==2)
        image2=Mat::zeros(h,w,CV_32FC2);
    if(xLayerSize==3)
        image2=Mat::ones(h,w,CV_32FC3);
    if(xLayerSize==4)
        image2=Mat::zeros(h,w,CV_32FC4);
    int x=0;
    int y=0;
    int nameID=0;
    auto last=image2.ptr<float>(0);
    for( int r=0; r < xOutPutImages.rows;r++)
    {
        auto tList=xOutPutImages.ptr<float>(r);
        Mat tTempOut;
        if(xLayerSize==1)
            tTempOut=Mat::zeros(s,s,CV_32FC1);
        if(xLayerSize==2)
            tTempOut=Mat::zeros(s,s,CV_32FC2);
        if(xLayerSize==3)
            tTempOut=Mat::zeros(s,s,CV_32FC3);
        if(xLayerSize==4)
            tTempOut=Mat::zeros(s,s,CV_32FC4);
        last=tTempOut.ptr<float>(0);
        for (int col = 0;col<xOutPutImages.cols;tList++,last++,col++)
        {
            *last=(*tList)*255;
        }

        //tSplitedImage[r].copyTo(image2(Rect( x*s, y*s, s, s)));
        tTempOut.copyTo(image2(Rect( x*s, y*s, s, s)));
        if((++y)*s>=h)
        {
            x++;
            y=0;
        }
        if(x*s>=w)
        {
            QString Name=QDir().currentPath()+xPath+QString::number(++nameID)+".jpg";
            qDebug() << Name;
            imwrite( Name.toStdString(), image2 );
            x=0;
            y=0;
        }
    }
}
QList<Mat> LoadImage(QString Path)
{
    QList<Mat> tInput;
    int file=1;
    QString Name=QDir().currentPath()+Path+QString::number(file++)+".jpg";
    do
    {
        if(QFileInfo(Name).exists())
        {
            Mat temp=imread(Name.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
            tInput.append(temp);
            Name=QDir().currentPath()+Path+QString::number(file++)+".jpg";
        }
    }while(QFileInfo(Name).exists());

    return tInput;
}

QList<Mat> LoadImage(QString Path, int Frames)
{
    QList<Mat> tInput;
    int file=1;
    int tFrameCount=0;
    QString Name=QDir().currentPath()+Path+QString::number(file++)+".jpg";
    do
    {
        if(QFileInfo(Name).exists())
        {
            Mat temp=imread(Name.toStdString());
            if(Frames>0)
            {
                cvtColor( temp, temp, CV_BGR2GRAY );
                if(tFrameCount==0)
                {
                    cvtColor( temp, temp, CV_GRAY2BGR );
                    tInput.append(temp);
                }
                else
                {
                    tInput.last()(0) = temp(0);
                }
                tFrameCount++;
                if(tFrameCount<=Frames)
                    tFrameCount=0;
            }
            else
            {
                tInput.append(temp);
            }
            Name=QDir().currentPath()+Path+QString::number(file++)+".jpg";
        }
    }while(QFileInfo(Name).exists());

    return tInput;
}

void SplitImages(int &h, int &w, QList<Mat> &tInput, QList<QList<float>> &tBWList, int &max)
{
    int current=0;
    for(int m=0;m<tInput.length();m++)
    {
        Mat tInputImage=tInput[m];
        h=tInputImage.rows;
        w=tInputImage.cols;
        auto tPtr=tInputImage.ptr<uchar>(0);
        QList<float> tBWRow;
        for (int i = 0; i < tInputImage.rows; ++i)
        {
            for (int j = 0; j < tInputImage.cols; ++j)
            {
                current+=3;
                if(current>max)
                {
                    while(tBWRow.count()<max)
                        tBWRow.append(0);

                    tBWList.append(tBWRow);
                    tBWRow.clear();
                    current=3;
                }

                tBWRow.append((float)(*tPtr++)/255);
                tBWRow.append((float)(*tPtr++)/255);
                tBWRow.append((float)(*tPtr++)/255);
            }
        }
        if(tBWRow.count()!=0)
        {
            while(tBWRow.count()<max)
                tBWRow.append(0);

            tBWList.append(tBWRow);
            current=0;
        }
    }
}

void SplitImages2(int xLayetSize, int xSize, int h, int w, QList<Mat> &tInput, QList<QList<float>> &tBWList, int &max)
{
    for(int m=0;m<tInput.length();m++)
    {
        int tH=(tInput[m].rows>h)?(h):(tInput[m].rows);
        int tW=(tInput[m].cols>w)?(w):(tInput[m].cols);
        Mat tInputImage=tInput[m](Rect( 0, 0, tW, tH));
        Mat tTempImage;
        if(xLayetSize==1)
            tTempImage=Mat::zeros(h,w,CV_8UC1);
        if(xLayetSize==2)
            tTempImage=Mat::zeros(h,w,CV_8UC2);
        if(xLayetSize==3)
            tTempImage=Mat::zeros(h,w,CV_8UC3);
        if(xLayetSize==4)
            tTempImage=Mat::zeros(h,w,CV_8UC4);
        tInputImage.copyTo(tTempImage(Rect( 0, 0, tW, tH)));

        // namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
        // imshow( "Display window", tTempImage );
        // waitKey(100);


        QList<Mat> tSplitedImage;

        for (int iCol = 0; iCol < (w/xSize); iCol++)
        {
            for (int iRow = 0; iRow < (h/xSize); iRow++)
            {
                Rect tRect = Rect( iCol*xSize, iRow*xSize, xSize, xSize);
                Mat tTemp=tTempImage(tRect);
                tSplitedImage.append(tTemp);
            }
        }

        for(auto tIterator=tSplitedImage.begin();tIterator<tSplitedImage.end();tIterator++)
        {
            Mat tQuad = (*tIterator).clone();
            auto tPtr=tQuad.ptr<uchar>(0);
            QList<float> tBWRow;
            for (int tY = 0; tY < xSize; tY++)
            {
                for (int tX = 0; tX < xSize*xLayetSize; tX++)
                {
                    tBWRow.append((float)(*tPtr++)/255);
                }
            }
            tBWList.append(tBWRow);
        }
    }
}


Mat normolizeImage(int m, int xLayetSize, int w, int h, QList<Mat> &tInput)
{
    int tH=(tInput[m].rows>h)?(h):(tInput[m].rows);
    int tW=(tInput[m].cols>w)?(w):(tInput[m].cols);
    Mat tInputImage=tInput[m](Rect( 0, 0, tW, tH));
    Mat tTempImage;
    if(xLayetSize==1)
        tTempImage=Mat::zeros(h,w,CV_8UC1);
    if(xLayetSize==2)
        tTempImage=Mat::zeros(h,w,CV_8UC2);
    if(xLayetSize==3)
        tTempImage=Mat::zeros(h,w,CV_8UC3);
    if(xLayetSize==4)
        tTempImage=Mat::zeros(h,w,CV_8UC4);
    tInputImage.copyTo(tTempImage(Rect( 0, 0, tW, tH)));

    return tTempImage;
}

void SplitImages2(int xIntSize, int xLayetSize, int xSize, int h, int w, QList<Mat> &tInput, QList<QList<float>> &tBWList, int &max)
{
    for(int m=0;m < tInput.length() - (xIntSize-1);m++)
    {
        QList<Mat> tTempImage;
        for (int t = 0; t < xIntSize; ++t) {
            tTempImage.append(normolizeImage(m+t, xLayetSize, w, h, tInput));
        }


        // namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
        // imshow( "Display window", tTempImage );
        // waitKey(100);


        QList<Mat> tSplitedImage;

        for (int iCol = 0; iCol < (w/xSize); iCol++)
        {
            for (int iRow = 0; iRow < (h/xSize); iRow++)
            {
                //Mat tRect(xSize,xSize);

                Rect tRect = Rect( iCol*xSize/sqrt(xIntSize), iRow*xSize/sqrt(xIntSize), xSize/sqrt(xIntSize), xSize/sqrt(xIntSize));
                Mat tTemp/*(xSize,xSize,CV_8UC3,255.0)*/;

                if(xLayetSize==1)
                    tTemp=Mat::zeros(xSize,xSize,CV_8UC1);
                if(xLayetSize==2)
                    tTemp=Mat::zeros(xSize,xSize,CV_8UC2);
                if(xLayetSize==3)
                    tTemp=Mat::zeros(xSize,xSize,CV_8UC3);
                if(xLayetSize==4)
                    tTemp=Mat::zeros(xSize,xSize,CV_8UC4);

                for (int im = 0; im < tTempImage.count(); ++im)
                {
                    int x=tRect.width*(im%((int)sqrt(xIntSize)));
                    int y=tRect.height*((int)im/((int)sqrt(xIntSize)));
                    Mat dst_roi = tTemp(Rect(x,y,tRect.width,tRect.height));
                    Mat tCopy=tTempImage[im](tRect);
                    tCopy.copyTo(dst_roi);
                }
                //namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
                //imshow( "Display window", tTemp );
                //waitKey(10);
                tSplitedImage.append(tTemp);
            }
        }

        for(auto tIterator=tSplitedImage.begin();tIterator<tSplitedImage.end();tIterator++)
        {
            Mat tQuad = (*tIterator).clone();
            auto tPtr=tQuad.ptr<uchar>(0);
            QList<float> tBWRow;
            for (int tY = 0; tY < xSize; tY++)
            {
                for (int tX = 0; tX < xSize*xLayetSize; tX++)
                {
                    tBWRow.append((float)(*tPtr++)/255);
                }
            }
            tBWList.append(tBWRow);
        }
    }
}

int main(int argc, char** argv)
{
    QApplication a(argc,argv);
    QDateTime end ;
    QDateTime start = QDateTime::currentDateTime();
    qDebug() << "start time: " << start << "\n";

    int chanalls=1;
    QList<Mat> tInput=LoadImage("/Input/");

    QList<Mat> tResponses=LoadImage("/Responses/");

    QList<QList<float>> tInList;
    QList<QList<float>> tOutList;
    int w, h, s, sout;
    h=360;//576;//1008;
    w=360;//704;
    s=8;
    sout=s/2;
    int maxInput=s*s*chanalls;
    int maxOutput=sout*sout*chanalls;

    SplitImages2(4, chanalls, s, h*2, w*2, tInput, tInList, maxInput);
    SplitImages2(1, chanalls, sout, h, w, tResponses, tOutList, maxOutput);

    int inputLayerSize = maxInput;
    int outputLayerSize = maxOutput;
    int numSamples = tOutList.length();

    Mat samples( Size( inputLayerSize, numSamples ), CV_32F );
    Mat responses( Size( outputLayerSize, numSamples ), CV_32F );

    int index=0;
    while(index < maxInput)
    {
        for(int s = 0; s < tInList.length(); s++)
        {
            samples.at<float>( Point( index, s ) ) = tInList[s][index];
        }
        index++;
    }
    index=0;
    while(index < maxOutput)
    {
        for(int s = 0; s < tOutList.length(); s++)
        {
            responses.at<float>( Point( index, s ) ) = tOutList[s][index];
        }
        index++;
    }

    //SaveToImages2("/Output/",3, sout, w, h, responses);
    vector<int> layerSizes = { maxInput, (maxInput+maxOutput), maxOutput };
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
        nnPtr->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 1, 1 );
        nnPtr->setTrainMethod(ml::ANN_MLP::BACKPROP,0.000001,0.000001);

        TermCriteria tc;
        tc.epsilon=0.0001;
        tc.maxCount=1000;
        tc.type = TermCriteria::MAX_ITER + TermCriteria::EPS;

        cout << "begin training:\n" << endl;
        if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
            return 1;
        cout << "end training:\n" << endl;
        //nnPtr->save("Color.yml");
        QDateTime end = QDateTime::currentDateTime();
        qDebug() << "end training: " << end << "\n";
        qDebug() << "diff time training: " << start.msecsTo(end) << "\n";
    }

    cout << "Predicting\n" << endl;
    Mat output;
    nnPtr->predict( samples, output );

    double tErrorDiff=0;
    double tError=1;
    int tDiff=1;

    double oldErrorDiff=0;
    do
    {
        cout << "Calculate Error:\n" << endl;

        tErrorDiff=0;
        tError=1;
        tDiff=1;
        for( int r=0; r < samples.rows;r++)
        {
            auto Out1=output.ptr<float>(r);
            auto Out2=responses.ptr<float>(r);
            for( int c=0; c < output.cols;c++)
            {
                tErrorDiff+=abs(Out1[c]-Out2[c]);
                if(Out1[c]!=0&&Out2[c]!=0)
                {
                    tError+=Out2[c]/Out1[c];
                }
                tDiff++;
            }
        }

        tErrorDiff=tErrorDiff/tDiff;
        tError=tError/tDiff;

        cout << "Error Diff: " << tErrorDiff << endl;
        cout << "Error Factor: " << tError << endl;

        end = QDateTime::currentDateTime();
        qDebug() << "loop end time: " << end << "\n";
        qDebug() << "loop diff time: " << start.msecsTo(end) << "\n";

        if(oldErrorDiff!=tErrorDiff||tErrorDiff==0)
        {
            //nnPtr->save("Color.yml");
            cout << "Saving\n" << endl;
            SaveToImages2("/Output/"+QString::number(tErrorDiff),chanalls, sout, w, h, output);
            cout << "Finished\n" << endl;
            oldErrorDiff=tErrorDiff;
        }

        cout << "begin training:\n" << endl;
        if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
            return 1;
        cout << "end training:\n" << endl;
    }while(tErrorDiff>0.01);


    end = QDateTime::currentDateTime();
    qDebug() << "end time: " << end << "\n";
    qDebug() << "diff time: " << start.msecsTo(end) << "\n";
    return a.exec();
}
