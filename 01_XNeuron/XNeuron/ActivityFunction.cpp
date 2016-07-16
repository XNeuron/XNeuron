#include "ActivityFunction.h"
int ActivityFunction::mRound=-1;
int ActivityFunction::mMin=0;
int ActivityFunction::mMax=200;

ActivityFunction::ActivityFunction(QObject *parent)
    : QObject(parent)
{

}

ActivityFunction::~ActivityFunction()
{

}

double ActivityFunction::giveActivityFunction(ActFunction func, double out, double bias)
{
    switch (func)
    {
    case ActivityFunction::Binary:
        return BinaryFunc(out, bias);
    case ActivityFunction::Line:
        return LineFunc(out, bias,mRound);
    case ActivityFunction::Line2:
        return Line2Func(out, bias,mRound);
    case ActivityFunction::Line3:
        return Line3Func(out, bias,mRound);
    case ActivityFunction::logistic:
        break;
    case ActivityFunction::Hyperbolic:
        break;
    case ActivityFunction::NormallyDistributed:
        break;
    }

    return 0;
}

double ActivityFunction::BinaryFunc(double out, double bias)
{
    if(bias < 0)
        return ((out > bias) ? 1 : -1);
    return ((out < bias) ? -1 : 1);

}

double ActivityFunction::LineFunc(double out, double bias, int round)
{
    if(round>0)
        return ((double)((int)((out+bias)*round)))/round;
    return out+bias;
}

double ActivityFunction::Line2Func(double out, double bias, int round)
{
    if(out<=mMin)
        return mMin;
    return LineFunc(out, bias,mRound);
}

double ActivityFunction::Line3Func(double out, double bias, int round)
{
    if(out>=mMax)
        return mMax;
    return Line2Func(out, bias,mRound);
}
