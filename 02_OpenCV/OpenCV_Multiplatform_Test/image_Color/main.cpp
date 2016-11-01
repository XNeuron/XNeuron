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

int mInputWidth, mInputHeight,mOutputWidth, mOutputHeight, mInputSize, mOutputSize, tXBegin, tYBegin, tImageHeight, tImageWidth;

void SaveToImages3(QString xPath,int xTeiler, int xLayerSize, int s, int w, int h, Mat xOutPutImages, bool xCutOff=false, int xFrame=1,double xError=0, int xX=0, int xY=0)
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
        Mat tTempOut=Mat::zeros(s,s,image2.type());
        if(xCutOff)
        {
            tTempOut=Mat::zeros(s/xTeiler,s/xTeiler,image2.type());
        }
        last=tTempOut.ptr<float>(0);
        for (int col = 0;col<xOutPutImages.cols;tList++,last++,col++)
        {
            *last=(*tList)*255;
        }

        //tSplitedImage[r].copyTo(image2(Rect( x*s, y*s, s, s)));
        tTempOut.copyTo(image2(Rect( x*s/xTeiler, y*s/xTeiler, s/xTeiler, s/xTeiler)));

        //namedWindow( "Display window", WINDOW_KEEPRATIO );// Create a window for display.
        //imshow( "Display window", image2 );
        //waitKey(1);

        if(xCutOff)
        {
            if((y++)*s/xTeiler+s>=(h))
            {
                x++;
                y=0;
            }
            if(x*s/xTeiler+s>w)
            {
                //namedWindow( "Display window", WINDOW_KEEPRATIO );// Create a window for display.
                //imshow( "Display window", image2 );
                //waitKey(1000);

                QFile outputFile(QDir().currentPath()+xPath+QString::number(xFrame)+".txt");
                outputFile.open(QIODevice::WriteOnly | QIODevice::Append);

                QTextStream out(&outputFile);

                out << QString::number(xError) << endl;

                outputFile.close();

                QString Name=QDir().currentPath()+xPath+QString::number(xFrame)+".jpg";
                qDebug() << Name;
                Mat tImage;

                if(QFileInfo(Name).exists())
                    tImage = imread(Name.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
                else
                    tImage = Mat(tImageHeight*1.5,tImageWidth*1.5,image2.type(),255);
                image2.copyTo(tImage(Rect(xX,xY,image2.cols,image2.rows)));
                imwrite( Name.toStdString(), tImage );
                x=0;
                y=0;
            }
        }
        else
        {
            if((y++)*s/xTeiler+s>=(h))
            {
                x++;
                y=0;
            }
            if(x*s/xTeiler+s>w)
            {
                QString Name=QDir().currentPath()+xPath+QString::number(++nameID)+".jpg";
                qDebug() << Name;
                imwrite( Name.toStdString(), image2 );
                x=0;
                y=0;
            }
        }
    }
}

QList<Mat> LoadImage2(QString Path, int xLayerSize,int file=1, int filestoload=4)
{
    QList<Mat> tInput;

    int filenr=0;
    QString Name=QDir().currentPath()+Path+QString::number(file++)+".jpg";
    do
    {
        if(QFileInfo(Name).exists())
        {Mat temp;
            if(xLayerSize==1)
            {
                temp=imread(Name.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
                imwrite((Name+"BW.png").toStdString(),temp);// Speichere Schwarzweises Bils für PSNR
            }
            else
                temp=imread(Name.toStdString());
            tInput.append(temp);
            Name=QDir().currentPath()+Path+QString::number(file++)+".jpg";
        }
    }while(QFileInfo(Name).exists()&&++filenr<filestoload);

    return tInput;
}

Mat normolizeImage(int m, int w, int h, QList<Mat> &tInput)
{
    int tH=(tInput[m].rows>h)?(h):(tInput[m].rows);
    int tW=(tInput[m].cols>w)?(w):(tInput[m].cols);
    Mat tInputImage=tInput[m](Rect( 0, 0, tW, tH));
    Mat tTempImage;
    tTempImage=Mat::zeros(h,w,tInputImage.type());

    tInputImage.copyTo(tTempImage(Rect( 0, 0, tW, tH)));

    return tTempImage;
}

Mat normolizeImage(int x, int y, int m, int w, int h, QList<Mat> &tInput)
{
    Mat tInputImage;
    int tH=(tInput[m].rows-y>h)?(h):(tInput[m].rows-y);
    int tW=(tInput[m].cols-x>w)?(w):(tInput[m].cols-x);
    if(tInput[m].rows-y>0&&tInput[m].cols-x)
        tInputImage=tInput[m](Rect( x, y, tW, tH));
    Mat tTempImage;
    tTempImage=Mat::zeros(h,w,tInputImage.type());
    if(tInput[m].rows-y>0&&tInput[m].cols-x)
        tInputImage.copyTo(tTempImage(Rect( 0, 0, tW, tH)));

    return tTempImage;
}

void MatToArray(QList<QList<float> > &tBWList, QList<Mat> tSplitedImage, int xLayetSize)
{
    for(auto tIterator=tSplitedImage.begin();tIterator<tSplitedImage.end();tIterator++)
    {
        Mat tQuad = (*tIterator).clone();
        auto tPtr=tQuad.ptr<uchar>(0);
        QList<float> tBWRow;
        for (int tY = 0; tY < tQuad.rows; tY++)
        {
            for (int tX = 0; tX < tQuad.cols*xLayetSize; tX++)
            {
                tBWRow.append((float)(*tPtr++)/255);
            }
        }
        tQuad.release();
        tBWList.append(tBWRow);
    }
    tSplitedImage.clear();
}

void SplitImages(int startPosX, int startPosY,int xTeiler, int xFrameCount, int xLayetSize, int xSize, int h, int w, QList<Mat> &xInput, QList<QList<float> > &xBWList, bool xCutOff=false, bool xFullImage=false)
{
    //Zusammenfüge x Bilder
    for(int m=0;m < xInput.length() - (xFrameCount-1);m++)
    {
        QList<Mat> tTempImage;
        for (int t = 0; t < xFrameCount; ++t) {
            tTempImage.append(normolizeImage(startPosX,startPosY, m+t, mOutputWidth, mOutputHeight, xInput));

//            namedWindow( "Display window0", WINDOW_AUTOSIZE );// Create a window for display.
//            imshow( "Display window0", tTempImage.last() );
//            waitKey(1000);
        }

        QList<Mat> tSplitedImage;
        int ColMax=(w/xSize)*xTeiler*sqrt(xFrameCount)-xTeiler-(xFullImage)?(1):(0);
        int RowMax=(h/xSize)*xTeiler*sqrt(xFrameCount)-xTeiler-(xFullImage)?(1):(0);

        for (int iCol = 0; iCol <= ColMax; iCol++)
        {
            for (int iRow = 0; iRow <= RowMax; iRow++)
            {
                Rect tRect = Rect( iCol*xSize/(sqrt(xFrameCount)*xTeiler), iRow*xSize/(sqrt(xFrameCount)*xTeiler), xSize/sqrt(xFrameCount), xSize/sqrt(xFrameCount));
                Mat tTemp(xSize,xSize,tTempImage.last().type(),255.0);s

                if(xCutOff)
                {
                    tTemp=Mat(xSize/xTeiler,xSize/xTeiler,tTempImage.last().type(),255.0);
                }

                for (int im = 0; im < tTempImage.count(); ++im)
                {
                    int x=tRect.width*(im%((int)sqrt(xFrameCount)));
                    int y=tRect.height*((int)im/((int)sqrt(xFrameCount)));
                    Mat dst_roi;
                    if(xCutOff)
                    {
                        dst_roi = tTemp(Rect(x,y,tRect.width/xTeiler,tRect.height/xTeiler));
                        tRect.height=tRect.height/xTeiler;
                        tRect.width=tRect.width/xTeiler;
                    }
                    else
                        dst_roi = tTemp(Rect(x,y,tRect.width,tRect.height));

                    Mat tCopy=tTempImage[im](tRect);
                    tCopy.copyTo(dst_roi);
                    //if(xCutOff)
//                    {
//                        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
//                        imshow( "Display window", tTemp );
//                        waitKey(1000);
//                    }
                }
                tSplitedImage.append(tTemp);
            }
        }

        MatToArray(xBWList, tSplitedImage, xLayetSize);
    }
}

void ConvertToMat(int maxOutput, Mat responses, QList<QList<float>> tOutList, Mat samples, int maxInput, QList<QList<float>> tInList, QList<Mat> tResponses)
{
    int index=0;
    while(index < maxInput)
    {
        for(int s = 0; s < tInList.length(); s++)
        {
            auto t= tInList[s][index];
            samples.at<float>( Point( index, s ) ) = t;
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

    tInList.clear();
    tResponses.clear();
}

int main(int argc, char** argv)
{
    QApplication a(argc,argv);
    QDateTime end ;
    QDateTime start = QDateTime::currentDateTime();
    qDebug() << "start time: " << start << "\n";

    // Parameter setzen
    int tChanalls=1;
    int tTeiler=1;//20;
    int tFrameanzahlgesammt = 4;// wie viel Bilder werden für Training geholt
    int tFrameanzahl=4;// wie viel Bilder werden für ein Input benutzt

    //Die erste Aufteilung
    mInputHeight=40*2;//120;//360;//576;//1008;
    mInputWidth=40*2;//120;//360;//704;
    mOutputHeight=40;//120;//360;//576;//1008;
    mOutputWidth=40;//120;//360;//704;

    //die Zweite Aufteilung
    mInputSize=40*2;//120;//40;
    mOutputSize=40;//120


    for (int var = 0; var < tFrameanzahlgesammt-tFrameanzahl+1; ++var)
    {
        tXBegin=tYBegin=0;
        //Bilder Laden
        qDebug() << "Load Image";
        QList<Mat> tInput=LoadImage2("/Input/", tChanalls, var+1);
        QList<Mat> tResponses=LoadImage2("/Responses/", tChanalls, var+1, 1);

        QList<QList<float>> tInList;
        QList<QList<float>> tOutList;
        tImageHeight=tInput[0].rows;
        tImageWidth=tInput[0].cols;

        int maxInput=mInputSize*mInputSize*tChanalls;
        int maxOutput=mOutputSize*mOutputSize*tChanalls/(tTeiler*tTeiler);
        int inputLayerSize = maxInput;
        int outputLayerSize = maxOutput;

        qDebug() << "Split Image";
        //Bilder auf kleinere Teile aufteilen
        SplitImages(tXBegin,tXBegin,tTeiler,tFrameanzahl, tChanalls, mInputSize, mInputHeight, mInputWidth, tInput, tInLis, false, true);
        SplitImages(tXBegin,tXBegin,tTeiler,1, tChanalls, mOutputSize, mOutputHeight, mOutputWidth, tResponses, tOutList, true);

        int numSamples = tOutList.length();

        //Bilder ins dataset konvertieren
        Mat samples( Size( inputLayerSize, numSamples ), CV_32F );
        Mat responses( Size( outputLayerSize, numSamples ), CV_32F );

        ConvertToMat(maxOutput, responses, tOutList, samples, maxInput, tInList, tResponses);

        //SaveToImages3("/Output/", teiler, chanalls, sout, OutputWidth, OutputHeight, responses, true, var+1,0,tXBegin,tYBegin);
        //SaveToImages3("/Output/", teiler, chanalls, s, InputWidth, InputHeight, samples, false, var+1,0,tXBegin,tYBegin);

        //Ebenen aufteilen
        vector<int> layerSizes = { maxInput, (maxInput+maxOutput)/2,  maxOutput };
        Ptr<ml::ANN_MLP> nnPtr;

        //Wenn die Parameterdatei vorhanden ist laden, sonst neue Netz erstellen
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
            tc.epsilon=0.001;
            tc.maxCount=10000;
            tc.type = TermCriteria::MAX_ITER + TermCriteria::EPS;

            //nnPtr->setTermCriteria(tc);

            cout << "begin training:\n" << endl;
            if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
                return 1;
            cout << "end training:\n" << endl;
            nnPtr->save("Color.yml");
            QDateTime end = QDateTime::currentDateTime();
            qDebug() << "end training: " << end << "\n";
            qDebug() << "diff time training: " << start.msecsTo(end) << "\n";
        }

        Mat output;

        double tErrorDiff=0;
        double tError=1;
        int tDiff=1;

        double oldErrorDiff=0;
        do
        {
            // Das gelernte anwenden
            cout << "Predicting\n" << endl;
            nnPtr->predict( samples, output );
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

            //Bild und gelernte Speichern
            nnPtr->save("Color.yml");
            cout << "Saving\n" << endl;
            SaveToImages3("/Output/", tTeiler, tChanalls, mOutputSize, mOutputWidth, mOutputHeight, output, true, var+1,tErrorDiff,tXBegin,tYBegin);
            cout << "Finished\n" << endl;
            oldErrorDiff=tErrorDiff;

            // Zum nechstem Bildteil rübergehen und wieder Trenieren
            tInList.clear();
            tOutList.clear();

            tXBegin+=mOutputWidth-mInputSize;
            if(tXBegin>tImageWidth)
            {
                tYBegin+=mOutputHeight-mInputSize;
                tXBegin=0;

                //wenn Bildteile durgearbeitet wurden aus der Schleife rausgehen
                if(tYBegin>tImageWidth)
                    break;
            }

            SplitImages(tXBegin,tYBegin,tTeiler,4, tChanalls, mInputSize, mOutputHeight, mOutputWidth, tInput, tInList, maxInput);
            SplitImages(tXBegin,tYBegin,tTeiler,1, tChanalls, mOutputSize, mOutputHeight, mOutputWidth, tResponses, tOutList, maxOutput,true);

            ConvertToMat(maxOutput, responses, tOutList, samples, maxInput, tInList, tResponses);

            cout << "begin training:\n" << endl;
            //if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
            //    return 1;
            cout << "end training:\n" << endl;
        }while(true);

        // Speicher freigeben
        for(auto i :  tInput  )
        {
           i.release();
        }

        for(auto i :  tResponses  )
        {
           i.release();
        }
        tInput.clear();
        tResponses.clear();
    }

    end = QDateTime::currentDateTime();
    qDebug() << "end time: " << end << "\n";
    qDebug() << "diff time: " << start.msecsTo(end) << "\n";
    return a.exec();
}
