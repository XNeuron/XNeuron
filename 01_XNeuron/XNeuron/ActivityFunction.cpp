#include "ActivityFunction.h"

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
        return LineFunc(out, bias,round);
    case ActivityFunction::Line2:
        break;
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
