#ifndef ACTIVITYFUNCTION_H
#define ACTIVITYFUNCTION_H

#include <QObject>

class ActivityFunction : public QObject
{
    Q_OBJECT

public:
    ActivityFunction(QObject *parent);
    ~ActivityFunction();

    enum ActFunction{ Binary, Line, Line2, Line3, logistic, Hyperbolic, NormallyDistributed	};

    static double giveActivityFunction(ActFunction func, double out, double bias);

    static double BinaryFunc(double out, double bias);
    static double LineFunc(double out, double bias, int round);
    static double Line2Func(double out, double bias, int round);
    static double Line3Func(double out, double bias, int round);
private:
    static int mRound;
    static int mMin;
    static int mMax;

};

#endif // ACTIVITYFUNCTION_H
