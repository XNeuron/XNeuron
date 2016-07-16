#include "GradienteTraining2.h"

GradientTraining2::GradientTraining2()
{

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

bool GradientTraining2::train(QList<QList<double>> &xInput, QList<double> &xOutputRequired, ActivityFunction::ActFunction xFunc)
{
    mFunc = xFunc;
    qsrand(QTime::currentTime().msec());
    int check = 5;
    double N = 1;
    double mDelta=10000;
    double tDelta=10000;
    int i=0;
    auto tWeight = mWeight;
    auto tOutput = mOutput;
    double n=0.1;

    for (int var = 0; true; var++)
    {

        if(var==0)
            CalcN(N);

        double error = xInput.size() + check;
        qDebug() << "N: " << N << "\n";
        for(QList<double>& x : xInput)
        {
            tDelta = mDelta;
            tOutput = mOutput;
            tWeight = mWeight;
            setInput(x);
            qDebug() << "Output: " << mOutput << "\n";
            int tIndexOfInput = xInput.indexOf(x);
            mDelta = xOutputRequired[tIndexOfInput] - mOutput;
            qDebug() << "Output: " << mDelta << "\n";

            if(qAbs(mDelta)>qAbs(tDelta))
            {
                mDelta = tDelta;
                mOutput = tOutput;
                mWeight = tWeight;
                N=N-0.01;
                if(N<=0)
                {
                    N=CalcN(N);
                    n*=0.1;
                    if(n<=0)
                        n=0.1;
                }
            }
            else
            {
                if(var != 0)
                {
                    i++;
                    if(i>=tWeight.length())
                        i=0;
                }
            }

            if (std::isnan(mDelta) || std::isinf(mDelta))
            {
                //mBias = 0;
                for (int i = 0; i < mWeight.length(); i++)
                {
                    qDebug() << "Weight[" << i << "]: " << mWeight[i] << "\n";
                    mWeight[i] = ((double)(qrand()%2000))/1000;
                }
            }
            else  if (mDelta != 0)
            {
                //for (int i = 0; i < mWeight.length(); i++)
                {
                    qDebug() << "Weight[" << i << "]: " << mWeight[i] << "\n";
                    mWeight[i] = mWeight[i] + N*mDelta*mInput[i];
                }
            }
            else
                error--;
        }
        if (error <= check)
            check--;
        if (error <= 0)
            return true;
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
