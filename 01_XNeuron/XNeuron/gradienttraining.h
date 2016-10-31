#ifndef GRADIENTTRAINING_H
#define GRADIENTTRAINING_H
#include "xneuron.h"
#include <QDebug>

class GradientTraining : public XNeuron
{
public:
    GradientTraining();
    bool train(QList<QList<bool>> &input, QList<bool> &mOutputRequired);
    bool train(QList<QList<double>> &input, QList<double> &mOutputRequired,ActivityFunction::ActFunction);
    void setInput(const QList<double> &input);
    void setInput(const QList<bool> &input);
};

#endif // GRADIENTTRAINING_H
