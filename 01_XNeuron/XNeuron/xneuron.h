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

    virtual double output() const;
    virtual bool outputBinary() const;
    virtual double output(ActivityFunction::ActFunction xFunc) const;


    virtual QList<double> input() const;
    virtual void setInput(bool A, bool B);
    virtual void setInput(const QList<bool> &input);
    virtual void setInput(const QList<double> &input);

    virtual void initWeight(const QList<bool> &input);
    virtual void ResetWeight(const QList<bool> &input);

    virtual void initWeight(const QList<double> &input);
    virtual void ResetWeight(const QList<double> &input);

    virtual QList<double> weight() const;
    virtual void setWeight(double A, double B);
    virtual void setWeight(double A, double B, double C);
    virtual void setWeight(const QList<double> &weight);

    virtual double bias() const;

    virtual void CalcOutput();

    void setWeight(double A, double B, double C, double D);
protected:
    QList<double> mInput;
    double mOutput = 0;
    double mBias = 0;
    ActivityFunction::ActFunction mFunc;
	QList<double> mWeight;
};
Q_DECLARE_INTERFACE(XNeuron, "Xma1.XNeuron/1.0")
#endif // XNEURON_H
