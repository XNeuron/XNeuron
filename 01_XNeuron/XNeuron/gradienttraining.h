#ifndef GRADIENTTRAINING_H
#define GRADIENTTRAINING_H
#include "xneuron.h"

class GradientTraining : public XNeuron
{
public:
    GradientTraining();
    bool train(QList<QList<bool>> &input, QList<bool> &mOutputRequired);
    bool train(QList<QList<double>> &input, QList<double> &mOutputRequired,ActivityFunction::ActFunction);
};

#endif // GRADIENTTRAINING_H
