#include "xneuron.h"


XNeuron::XNeuron()
{
}

double XNeuron::outputReal() const
{
    return mOutputReal;
}

void XNeuron::setOutputReal(double outputReal)
{
    mOutputReal = outputReal;
}

double XNeuron::outputRequired() const
{
    return mOutputRequired;
}

void XNeuron::setOutputRequired(double outputRequired)
{
    mOutputRequired = outputRequired;
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
    for (auto& elem: input)
    {
        initWeight(input);
        tInput << (elem?1.0:0.0);
    }
    setInput(tInput);
}

void XNeuron::initWeight(const QList<auto> &input)
{
    if(mWeight.length()!=input.length())
    {
        mWeight.append(0.5);
    }
}

void XNeuron::clearWeight(const QList<auto> &input)
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
    for (auto& elem: input)
    {
        initWeight(input);
        mOutputReal+= elem*mWeight[input.indexOf(elem)];
    }
}
