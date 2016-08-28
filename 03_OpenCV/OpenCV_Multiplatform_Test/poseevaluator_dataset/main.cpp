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

int LoadResponses(QMap<QString,QList<float>> *tResponses, QString InputListName)
{
    int max=0;
    QFile file2(InputListName);
    if(file2.open(QIODevice::ReadOnly))
    {
        QList<float>* tTempResponses = new QList<float>();
        QString tName="";
        int tXY=0;
        while(!file2.atEnd())
        {
            QString tempInput= file2.readLine();
            if(tempInput.contains("jpg"))
            {
                if(tName!="")
                {
                    if(tName!="")
                        tResponses->insert(tName,*tTempResponses);
                    if(max<tTempResponses->count())
                        max=tTempResponses->count();
                }
                tName=tempInput.replace(" \n","").split(' ')[0];
                tTempResponses = new QList<float>();
            }else
            {
                QStringList s = tempInput.replace("\n","").split(' ');
                for(QString t: s)
                {
                    if(t!="")
                    {
                        double d = t.toDouble();
                        tTempResponses->append(d/720);
                    }
                }

            }
        }
    }

    return max;
}

void LoadInputImages(QMap<QString, Mat> *tInput, QString InputListName)
{
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
                cvtColor( temp, temp, CV_BGR2GRAY );
                pyrDown(temp,temp);
                pyrDown(temp,temp);
                pyrDown(temp,temp);

                tInput->insert(tempInput, temp);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc,argv);
    QDateTime tEnd;
    QDateTime start = QDateTime::currentDateTime();
    qDebug() << "start time: " << start << "\n";
    QMap<QString, Mat> tInput;
    //int file=1;
    //QString Name=QDir().currentPath()+"/Input/"+QString::number(file++)+".jpg";
    QString InputListName = QDir().currentPath()+"/Input/"+ "images.list";
    qDebug() << "Load Input Images";
    LoadInputImages(&tInput, InputListName);
    InputListName = QDir().currentPath()+"/Responses/"+ "Response.txt";
    QMap<QString,QList<float>> tResponses;

    qDebug() << "Load Responses";
    int max = LoadResponses(&tResponses, InputListName);

    int Imax=tInput.first().rows*tInput.first().cols;
    QList<QList<float>> tInputList;
    QList<QList<float>> tResponsesList;
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
        if(count>1000)
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

    vector<int> layerSizes = { inputLayerSize, 1000,  100,  100, outputLayerSize };
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
        nnPtr->setActivationFunction( cv::ml::ANN_MLP::SIGMOID_SYM, 1 ,1 );
        nnPtr->setTrainMethod(ml::ANN_MLP::BACKPROP,0.1,0.1);

        TermCriteria tc;
        tc.epsilon=0.0001;
        tc.maxCount=100000;
        tc.type = TermCriteria::MAX_ITER + TermCriteria::EPS;

        nnPtr->setTermCriteria(tc);


        cout << "begin training:\n" << endl;
        if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
            return 1;
        cout << "end training:\n" << endl;
        //nnPtr->save("Color.yml");
        tEnd = QDateTime::currentDateTime();
        qDebug() << "end training: " << tEnd << "\n";
        qDebug() << "diff time training: " << start.msecsTo(tEnd) << "\n";
    }

    cout << "Predicting\n" << endl;
    Mat output;
    nnPtr->predict( samples, output );

    cout << "Saving\n" << endl;

    double tErrorDiff=0;
    double tError=1;
    int tDiff=1;

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

        tEnd = QDateTime::currentDateTime();
        qDebug() << "loop end time: " << tEnd << "\n";
        qDebug() << "loop diff time: " << start.msecsTo(tEnd) << "\n";

        cout << "begin training:\n" << endl;
        if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
            return 1;
        cout << "end training:\n" << endl;
        //nnPtr->save("Color.yml");
    }while(tErrorDiff>1);

    QString Name=QDir().currentPath()+"/Output/"+"Out"+".txt";
    QFile filewrite(Name);
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
                outStr.append(QString::number(Out2[c]*720));
            }
        }

        //imwrite( Name.toStdString(), image2 );
    }
    cout << "Error Diff: " << tErrorDiff/tDiff << endl;
    cout << "Error Factor: " << tError << endl;

    cout << "Finished\n" << endl;
    tEnd = QDateTime::currentDateTime();
    qDebug() << "end time: " << tEnd << "\n";
    qDebug() << "diff time: " << start.msecsTo(tEnd) << "\n";
    return a.exec();
}
