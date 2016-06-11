#ifndef ACTIVITYFUNCTION_H
#define ACTIVITYFUNCTION_H

#include <QObject>

class ActivityFunction : public QObject
{
    Q_OBJECT

public:
    ActivityFunction(QObject *parent);
    ~ActivityFunction();

    enum ActFunction{ Binary, Line, Line2,	logistic, Hyperbolic, NormallyDistributed	};

    static double giveActivityFunction(ActFunction func, double out, double bias);

    static double BinaryFunc(double out, double bias);
    static double LineFunc(double out, double bias);
private:

};

#endif // ACTIVITYFUNCTION_H
