#include "GradienteTraining2.h"

GradientTraining2::GradientTraining2()
{
    hConsole_c = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}

GradientTraining2::~GradientTraining2()
{
    CloseHandle(hConsole_c);
}


bool GradientTraining2::train(QList<QList<bool>> &xInput, QList<bool> &xOutputRequired)
{
    QList<QList<double>> mInput;
    QList<double> mOutputRequired;

    for (QList<bool>& x : xInput)
    {
        QList<double> temp;
        for (bool& y : x)
        {
            temp.append((y) ? (1) : (-1));
        }
        mInput.append(temp);
        mOutputRequired.append((xOutputRequired[xInput.indexOf(x)]) ? (1) : (-1));
    }
    return train(mInput, mOutputRequired, ActivityFunction::Binary);
}

double GradientTraining2::CalcN(double N)
{
    do
    {
        N = qAbs(((double)((qrand()) % 1000)) / (1000));
    } while (N == 0);
    return N;
}

void GradientTraining2::Print(QString text, int y)
{
    const wchar_t * str=text.toStdWString().c_str();
    COORD pos = {5, y};
    SetConsoleActiveScreenBuffer(hConsole_c);
    DWORD len = (text.length()<60)?(text.length()):(60);
    DWORD dwBytesWritten = 0;
    WriteConsoleOutputCharacter(hConsole_c, (LPCWSTR)str, len, pos, &dwBytesWritten);
}

double cint(double x){
    if (modf(x,0)>=.5)
        return x>=0?ceil(x):floor(x);
    else
        return x<0?ceil(x):floor(x);
}

bool GradientTraining2::train(QList<QList<double>> &xInput, QList<double> &xOutputRequired, ActivityFunction::ActFunction xFunc)
{
    QDateTime start = QDateTime::currentDateTime();
    Print(QString("Start:          %1").arg(start.toString()),22);
    mFunc = xFunc;
    qsrand(QTime::currentTime().msec());
    int check = 5;
    double N = 1;
    double mDelta=10000;
    auto tDelta=xOutputRequired;
    int i=0;
    auto tWeight = mWeight;
    auto tOutput = xOutputRequired;
    double n=0.1;
    bool SetEnd;

    for(int i=0;i<tDelta.length();i++)
    {
        tDelta[i] = std::numeric_limits<double>::max();
        tOutput[i] = std::numeric_limits<double>::max();
    }
    tWeight = mWeight;

    for (int var = 0; true; var++)
    {

        if(var==0)
            CalcN(N);

        double error = xInput.size() + check;
        SetEnd=false;
        for(QList<double>& x : xInput)
        {
            {
                Print(QString("Start:          %1").arg(start.toString()),22);
                int tIndexOfInput = xInput.indexOf(x);
                Print(QString("Input:          %1").arg(tIndexOfInput, 3, 10, '0'),1);
                Print(QString("Soll:           %1").arg(xOutputRequired[tIndexOfInput], 5, 10, '0'),2);
                Print(QString("N is <= 0."),7);

                Print(QString("Output Old:     %1").arg(tOutput[tIndexOfInput], 5, 10, '0'),3);
                //qDebug() << "Output Old: " << tOutput << "\n";
                setInput(x);
                {
                    double tRound=((double)(cint(mOutput*10000)))/10000;
                    mOutput=((double)(cint(mOutput*100)))/100;
                    mOutput=((tRound-mOutput)>0.005)?(mOutput+1):(mOutput);
                }
                Print(QString("Output New:     %1").arg(mOutput, 5, 10, '0'),4);
                //qDebug() << "Output New: " << mOutput << "\n";
                Print(QString("Delta Old:      %1").arg(tDelta[tIndexOfInput], 5, 10, '0'),5);
                //qDebug() << "Delta Old: " << tDelta << "\n";
                mDelta = xOutputRequired[tIndexOfInput] - mOutput;
                Print(QString("Delta New:      %1").arg(mDelta, 5, 10, '0'),6);
                //qDebug() << "Delta New: " << mDelta << "\n";

                if(qAbs(mDelta)>qAbs(tDelta[tIndexOfInput]))
                {
                    mDelta = tDelta[tIndexOfInput];
                    mOutput = tOutput[tIndexOfInput];
                    mWeight = tWeight;
                    N=N-n;
                    if(N<=0)
                    {
                        Print(QString("N is <= 0."),7);
                        //qDebug() << "Set N to 0.\n";
                        Print(QString("N Old:          %1").arg(N, 5, 10, '0'),8);
                        //qDebug() << "N Old: " << N << "\n";
                        N=CalcN(N)*n;
                        n*=0.1;
                        if(n<=qPow(10,-40))
                        {
                            n=0.1;
                            N=CalcN(N)*n;
                            i++;
                            if(i>=tWeight.length())
                                i=0;
                        }
                    }
                    N*=qPow(-1,var);
                    Print(QString("N:              %1").arg(N, 5, 10, '0'),9);
                    //qDebug() << "N: " << N << "\n";
                    Print(QString("n:              %1").arg(n, 5, 10, '0'),10);
                    //qDebug() << "n: " << n << "\n";
                }
                else
                {
                    if(var != 0&&SetEnd)
                    {
                        i++;
                        if(i>=tWeight.length())
                            i=0;
                    }
                    tDelta[tIndexOfInput] = mDelta;
                    tOutput[tIndexOfInput] = mOutput;
                    tWeight = mWeight;
                }
                if (std::isnan(mDelta) || std::isinf(mDelta))
                {
                    //mBias = 0;
                    for (int i = 0; i < mWeight.length(); i++)
                    {
                        Print(QString("Weight[%1]:  %2").arg(i).arg(mWeight[i], 5, 10, '0'),11);
                        //qDebug() << "Weight[" << i << "]: " << mWeight[i] << "\n";
                        mWeight[i] = ((double)(qrand()%2000))/1000;
                    }
                }
                else  if (mDelta != 0)
                {
                    for (int i = 0; i < mWeight.length(); i++)
                    {
                        Print(QString("Weight_old[%1]:  %2").arg(i).arg(tWeight[i], 5, 10, '0'),12+i);
                    }
                    //for (int i = 0; i < mWeight.length(); i++)
                    {
                        //qDebug() << "Weight_old[" << i << "]: " << tWeight[i] << "\n";
                        mWeight[i] = mWeight[i] + N*mDelta*mInput[i];
                        //qDebug() << "Weight_old[" << i << "]: " << mWeight[i] << "\n";

                    }
                    for (int i = 0; i < mWeight.length(); i++)
                    {
                        Print(QString("Weight_new[%1]:  %2").arg(i).arg(mWeight[i], 5, 10, '0'),12+mWeight.length()+1+i);
                    }
                }
                else
                    error--;
            }
            SetEnd=true;
            if (error <= check)
                check--;
            if (error <= 0)
            {
                return true;
            }
            double tError=0;
            for(double delta:tDelta)
                tError+=delta;
            Print(QString("Error:          %1").arg(tError, 5, 10, '0'),21);
            QTime n(0, 0, 0);
            Print(QString("Diff Time:      %1").arg(n.addMSecs(start.msecsTo(QDateTime::currentDateTime())).toString()),23);
            Print(QString("End:            %1").arg(QDateTime::currentDateTime().toString()),24);
        }
    }

    return false;
}

void GradientTraining2::setInput(const QList<double> &input)
{
    mInput = input;
    mInput.insert(0,1);
    ResetWeight(mInput);
    CalcOutput();
}


void GradientTraining2::setInput(const QList<bool> &input)
{
    QList<double> tInput;
    for (bool elem : input)
    {
        tInput << (elem ? 1.0 : -1.0);
    }
    setInput(tInput);
}
