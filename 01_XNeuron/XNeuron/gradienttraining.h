#ifndef GRADIENTTRAINING_H
#define GRADIENTTRAINING_H
#include "xneuron.h"

class GradientTraining : public XNeuron
{
public:
    GradientTraining();

    bool train(QList<QList<double> > &xInput, QList<double> &xOutputRequired, ActivityFunction::ActFunction xFunc);
};

#endif // GRADIENTTRAINING_H
