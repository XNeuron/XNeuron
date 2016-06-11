#ifndef ONLINE_H
#define ONLINE_H

/*#include "xneuron_global.h"*/
#include <QList>
#include <QTime>
#include <QtMath>
#include "xneuron.h"
#include "ActivityFunction.h"

class OnlineTraining : public XNeuron {
public:
    OnlineTraining();
	bool train(QList<QList<bool>> &input, QList<bool> &mOutputRequired);
    bool train(QList<QList<double>> &input, QList<double> &mOutputRequired,ActivityFunction::ActFunction);
};

#endif // ONLINE_H
