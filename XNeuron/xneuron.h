#ifndef XNEURON_H
#define XNEURON_H

/*#include "xneuron_global.h"*/
#include <QList>

class /*XNEURONSHARED_EXPORT*/ XNeuron
{

public:
    XNeuron();

    QList<double> mWeight;
    QList<XNeuron> mNextNeuron;

    double outputReal() const;
    void setOutputReal(double outputReal);

    double outputRequired() const;
    void setOutputRequired(double outputRequired);

    QList<double> input() const;
    void setInput(bool A,bool B);
    void setInput(const QList<bool> &input);
    void setInput(const QList<double> &input);

    void initWeight(const QList<auto> &input);
    void clearWeight(const QList<auto> &input);

private:
    QList<double> mInput;
    double mOutputReal;
    double mOutputRequired;

};

#endif // XNEURON_H
