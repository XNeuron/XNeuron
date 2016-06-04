#include "xneuron.h"

XNeuron::XNeuron()
{
}

bool XNeuron::train(QList<QList<bool>> &input, QList<bool> &mOutputRequired)
{
    bool ok=false;
    for (int var = 0; true; ++var)
    {
        for (QList<bool>& x: input)
        {
            setInput(x);
            double Required =mOutputRequired[input.indexOf(x)];
            if(outputBinary()!=Required)
            {
                ok=false;
                for (double& w: mWeight)
                {
                    double dx=x[mWeight.indexOf(w)]?1.0:0.0;
                    w+=(Required-outputReal())*dx/mWeight.length();
                }
                break;
            }else
            {
                ok=true;
            }
        }
        if(ok)
            return true;
    }

    return false;
}

double XNeuron::outputReal() const
{
    return mOutputReal;
}

bool XNeuron::outputBinary() const
{
    return (mOutputReal<0.75?0:1);
}

void XNeuron::setOutputReal(double outputReal)
{
    mOutputReal = outputReal;
}

QList<double> XNeuron::input() const
{
    return mInput;
}

void XNeuron::setInput(bool A, bool B)
{
    setInput(QList<bool>()<<A<<B);
}

void XNeuron::setInput(const QList<bool> &input)
{
    QList<double> tInput;
    clearWeight(input);
    for (bool elem: input)
    {
        initWeight(input);
        tInput << (elem?1.0:0.0);
    }
    setInput(tInput);
}

void XNeuron::initWeight(const QList<double> &input)
{
    if(mWeight.length()!=input.length())
    {
        mWeight.append(0.5);
    }
}

void XNeuron::clearWeight(const QList<double> &input)
{
    if(mWeight.length()!=input.length())
    {
        mWeight.clear();
    }
}

void XNeuron::initWeight(const QList<bool> &input)
{
    if(mWeight.length()!=input.length())
    {
        mWeight.append(0.5);
    }
}

void XNeuron::clearWeight(const QList<bool> &input)
{
    if(mWeight.length()!=input.length())
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
    mWeight = QList<double>()<<A<<B;
}

void XNeuron::setWeight(const QList<double> &weight)
{
    mWeight = weight;
}

void XNeuron::setInput(const QList<double> &input)
{
    mInput = input;
    clearWeight(input);
    mOutputReal=0;
    for (const double& elem: input)
    {
        initWeight(input);
        mOutputReal+= elem*mWeight[input.indexOf(elem)];
    }
}
