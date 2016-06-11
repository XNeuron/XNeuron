#ifndef XNEURON_H
#define XNEURON_H

/*#include "xneuron_global.h"*/
#include <QList>
#include <QTime>
#include <QtMath>
#include "ActivityFunction.h"

class /*XNEURONSHARED_EXPORT*/ XNeuron
{

public:
    QList<XNeuron> mNextNeuron;

    virtual bool train(QList<QList<bool>> &input, QList<bool> &mOutputRequired);
    virtual bool train(QList<QList<double>> &input, QList<double> &mOutputRequired,ActivityFunction::ActFunction);

    virtual double outputLine() const;
    virtual bool outputBinary() const;
    virtual double outputLine(ActivityFunction::ActFunction xFunc) const;


    virtual QList<double> input() const;
    virtual void setInput(bool A, bool B);
    virtual void setInput(const QList<bool> &input);
    virtual void setInput(const QList<double> &input);

    virtual void initWeight(const QList<bool> &input);
    virtual void ClearWeight(const QList<bool> &input);

    virtual void initWeight(const QList<double> &input);
    virtual void ClearWeight(const QList<double> &input);

    virtual QList<double> weight() const;
    virtual void setWeight(double A, double B);
    virtual void setWeight(double A, double B, double C);
    virtual void setWeight(const QList<double> &weight);

    virtual double bias() const;

    virtual void CalcOutput();

protected:
    QList<double> mInput;
    double mOutput = 0;
    double mBias = 0;
    ActivityFunction::ActFunction mFunc;
	QList<double> mWeight;

};
Q_DECLARE_INTERFACE(XNeuron, "Xma1.XNeuron/1.0")
#endif // XNEURON_H
