#include "gradienttraining.h"

GradientTraining::GradientTraining()
{

}


bool GradientTraining::train(QList<QList<bool>> &xInput, QList<bool> &xOutputRequired)
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

bool GradientTraining::train(QList<QList<double>> &xInput, QList<double> &xOutputRequired, ActivityFunction::ActFunction xFunc)
{
    mFunc = xFunc;
    qsrand(QTime::currentTime().msec());
    int check = 5;
    double N = 1;
    double mDelta=10000;
    double tDelta=10000;
    for (int var = 0; true; var++)
    {
        do
        {
            N = ((double)((qrand()) % 1000)) / (1000);
        } while (N == 0);

        double error = xInput.size() + check;

        for (QList<double>& x : xInput)
        {
            tDelta = mDelta;
            setInput(x);
            int tIndexOfInput = xInput.indexOf(x);
            mDelta = xOutputRequired[tIndexOfInput] - mOutput;

            if (/*abs(mDelta) > 10 || */std::isnan(mDelta) || std::isinf(mDelta))
            {
                //mBias = 0;
                for (int i = 0; i < mWeight.length(); i++)
                {
                    mWeight[i] = ((double)(qrand()%2000))/1000;
                }
            }
            else  if (mDelta != 0)
            {
                for (int i = 0; i < mWeight.length(); i++)
                {
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

void GradientTraining::setInput(bool A, bool B)
{
    setInput(QList<bool>() << A << B);
}

void GradientTraining::setInput(const QList<double> &input)
{
    mInput = input;
    mInput.insert(0,1);
    ResetWeight(mInput);
    CalcOutput();
}


void GradientTraining::setInput(const QList<bool> &input)
{
    QList<double> tInput;
    for (bool elem : input)
    {
        tInput << (elem ? 1.0 : -1.0);
    }
    setInput(tInput);
}
