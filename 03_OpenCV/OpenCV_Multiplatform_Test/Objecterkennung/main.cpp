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
#include <QFile>
#include <QMap>
#include <QApplication>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc,argv);
    QDateTime start = QDateTime::currentDateTime();
    qDebug() << "start time: " << start << "\n";
    QMap<QString, Mat> tInput;
    //int file=1;
    //QString Name=QDir().currentPath()+"/Input/"+QString::number(file++)+".jpg";
    QString InputListName = QDir().currentPath()+"/Input/"+ "images.list";

    QFile file(InputListName);
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            QString tempInput= file.readLine().replace("\n","");
            if(tempInput!="")
            {
                QString tPath = QDir().currentPath()+"/Input/"+ tempInput;
                tPath = tPath.replace("/","\\");
                Mat temp=imread(tPath.toStdString());
                pyrDown(temp,temp);
                cvtColor( temp, temp, CV_BGR2GRAY );
                tInput.insert(tempInput, temp);
            }
        }
    }
    InputListName = QDir().currentPath()+"/Responses/"+ "4weds_sticks_standard.txt";
    QMap<QString,QList<double>> tResponses;


    int max=0;
    QFile file2(InputListName);
    if(file2.open(QIODevice::ReadOnly))
    {
        QList<double>* tTempResponses = new QList<double>();
        QString tName="";
        while(!file2.atEnd())
        {
            QString tempInput= file2.readLine();
            if(tempInput.contains("jpg"))
            {
                if(tName!="")
                {
                    if(tName!="")
                        tResponses.insert(tName,*tTempResponses);
                    if(max<tTempResponses->count())
                        max=tTempResponses->count();
                }
                tName=tempInput.replace(" \n","").split(' ')[0];
                tTempResponses = new QList<double>();
            }else
            {
                QStringList s = tempInput.replace("\n","").split(' ');
                for(QString t: s)
                {
                    if(t!="")
                    {
                        double d = t.toDouble();
                        tTempResponses->append(d);
                    }
                }

            }
        }
    }


    int Imax=tInput.first().rows*tInput.first().cols;
    QList<QList<float>> tInputList;
    QList<QList<double>> tResponsesList;
    int w,h;
    h=tInput.first().rows;
    w=tInput.first().cols;

    int count=0;
    for(auto m: tResponses.keys())
    {
        try
        {
            Mat tIn=tInput[m];

            if(tIn.rows*tIn.cols==Imax)
            {
                if(Imax>tIn.rows*tIn.cols)
                {   h=tIn.rows;
                    w=tIn.cols;
                    Imax=tIn.rows*tIn.cols;
                }
                auto tPtr=tIn.ptr<uchar>(0);
                QList<float> tBWRow;
                for (int i = 0; i < tIn.rows; ++i)
                {
                    for (int j = 0; j < tIn.cols; ++j)
                    {
                        tBWRow.append((float)(*tPtr++)/255);
                    }
                }
                tInputList.append(tBWRow);
                tResponsesList.append(tResponses[m]);
                tIn.release();
                count++;
            }
        }catch(Exception ex)
        {
            qDebug()<< ex.msg.c_str();
        }
        if(count>100)
            break;
    }

    int inputLayerSize = Imax;
    int outputLayerSize = max;
    int numSamples = tInputList.length();

    Mat samples( Size( inputLayerSize, numSamples ), CV_32F );
    Mat responses( Size( outputLayerSize, numSamples ), CV_32F );

    int index=0;
    while(index < numSamples)
    {
        for(int s = 0; s < inputLayerSize; s++)
        {
            if(s<tInputList[index].count())
                samples.at<float>( Point( s, index ) ) = tInputList[index][s];
            else
                samples.at<float>( Point( s, index ) ) = 0;
        }
        for(int s = 0; s < outputLayerSize; s++)
        {
            if(s<tResponsesList[index].count())
                responses.at<float>( Point( s, index ) ) = tResponsesList[index][s];
            else
                responses.at<float>( Point( s, index ) ) = 0;
        }

        index++;
    }

    vector<int> layerSizes = { inputLayerSize, 300, 300, outputLayerSize };
    Ptr<ml::ANN_MLP> nnPtr;
    if(QFileInfo("Color.yml").exists())
    {
        cout << "Loading\n" << endl;
        nnPtr=Algorithm::load<ml::ANN_MLP>("Color.yml");
    }
    //else
    {
        nnPtr = ml::ANN_MLP::create();

        nnPtr->setLayerSizes( layerSizes );
        nnPtr->setActivationFunction( cv::ml::ANN_MLP::SIGMOID_SYM );
        nnPtr->setTrainMethod(ml::ANN_MLP::BACKPROP,1000,1000);

        TermCriteria tc;
        tc.epsilon=0.01;
        tc.maxCount=1000000;
        tc.type = TermCriteria::MAX_ITER + TermCriteria::EPS;

        nnPtr->setTermCriteria(tc);


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
    QString Name=QDir().currentPath()+"/Output/"+"Out"+".txt";
    QFile filewrite(Name);
    double tErrorDiff=0;
    double tError=0;
    if(filewrite.open(QFile::WriteOnly|QFile::Text))
    {
        for( int r=0; r < samples.rows;r++)
        {
            index=0;
            //Mat image2( Size( w, h ), CV_32FC3);
            auto Out1=output.ptr<float>(r);
            auto Out2=responses.ptr<float>(r);
            //auto im1=image2.ptr<float>(0);
            QStringList outStr;
            for( int c=0; c < output.cols;c++)
            {
                tErrorDiff+=abs(Out1[c]-Out2[c]);
                if(Out2[c]!=0)
                    tError*=Out1[c]/Out2[c];
                outStr.append(QString::number(Out1[c]));
                index++;
            }

            QTextStream s(&filewrite);
            s << outStr.join(" ");
            s << "\n";
        }

        //imwrite( Name.toStdString(), image2 );
    }
    cout << "Error Diff: " << tErrorDiff << endl;
    cout << "Error Factor: " << tError << endl;

    cout << "Finished\n" << endl;
    QDateTime end = QDateTime::currentDateTime();
    qDebug() << "end time: " << end << "\n";
    qDebug() << "diff time: " << start.msecsTo(end) << "\n";
    return a.exec();
}
