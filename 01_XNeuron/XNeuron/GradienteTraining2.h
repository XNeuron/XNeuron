#ifndef GRADIENTTRAINING_H
#define GRADIENTTRAINING_H
#include "xneuron.h"
#include <QDebug>
#include <windows.h>

class GradientTraining2 : public XNeuron
{
public:
    GradientTraining2();
    ~GradientTraining2();
    bool train(QList<QList<bool>> &input, QList<bool> &mOutputRequired);
    bool train(QList<QList<double>> &input, QList<double> &mOutputRequired,ActivityFunction::ActFunction);
    void setInput(const QList<double> &input);
    void setInput(const QList<bool> &input);
    double CalcN(double N);
    void Print(QString text,int y);
private:
    HANDLE hConsole_c;
};

#endif // GRADIENTTRAINING_H
