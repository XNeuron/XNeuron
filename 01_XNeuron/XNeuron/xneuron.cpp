#include "xneuron.h"

bool XNeuron::train(QList<QList<bool>> &xInput, QList<bool> &xOutputRequired)
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

bool XNeuron::train(QList<QList<double>> &xInput, QList<double> &xOutputRequired,ActivityFunction::ActFunction xFunc )
{
    return false;
}

double XNeuron::output() const
{
    return mOutput;
}

double XNeuron::output(ActivityFunction::ActFunction xFunc) const
{
    return ActivityFunction::BinaryFunc(mOutput,mBias);
}

bool XNeuron::outputBinary() const
{
    return mOutput > 0;
}

QList<double> XNeuron::input() const
{
    return mInput;
}

void XNeuron::setInput(bool A, bool B)
{
    setInput(QList<bool>() << A << B);
}

void XNeuron::setInput(const QList<bool> &input)
{
    QList<double> tInput;
    ResetWeight(input);
    for (bool elem : input)
    {
        initWeight(input);
        tInput << (elem ? 1.0 : -1.0);
    }
    setInput(tInput);
}

void XNeuron::initWeight(const QList<double> &input)
{
    qsrand(QTime::currentTime().msec());
    if (mWeight.length() != input.length())
    {
        for(int i = 0;i<input.length();i++)
            mWeight.append(qrand());
    }
}

void XNeuron::ResetWeight(const QList<double> &input)
{
    if (mWeight.length() != input.length())
    {
        mWeight.clear();
        initWeight(input);
    }
}

void XNeuron::initWeight(const QList<bool> &input)
{
    qsrand(QTime::currentTime().msec());
    if (mWeight.length() != input.length())
    {
        mWeight.append(qrand());
    }
}

void XNeuron::ResetWeight(const QList<bool> &input)
{
    if (mWeight.length() != input.length())
    {
        mWeight.clear();
    }
}

QList<double> XNeuron::weight() const
{
    return mWeight;
}

void XNeuron::setWeight(double A, double B)
{
    mWeight = QList<double>() << A << B;
}

void XNeuron::setWeight(double A, double B, double C)
{
    mWeight = QList<double>() << A << B << C;
}

void XNeuron::setWeight(double A, double B, double C, double D)
{
    mWeight = QList<double>() << A << B << C << D;
}

void XNeuron::setWeight(const QList<double> &weight)
{
    mWeight = weight;
}

double XNeuron::bias() const
{
    return mBias;
}

void XNeuron::CalcOutput()
{
    mOutput = 0;
    for (int i = 0; i < mInput.length(); i++)
    {
        mOutput += mInput[i]*mWeight[i];
    }
    mOutput = ActivityFunction::giveActivityFunction(mFunc,mOutput,mBias);
}

void XNeuron::setInput(const QList<double> &input)
{
    mInput = input;
    ResetWeight(input);
    CalcOutput();
}
