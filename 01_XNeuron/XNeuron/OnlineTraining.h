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

    QList<OnlineTraining> mNextNeuron;

	bool train(QList<QList<bool>> &input, QList<bool> &mOutputRequired);
    bool train(QList<QList<double>> &input, QList<double> &mOutputRequired,ActivityFunction::ActFunction);

    double outputLine() const;
    bool outputBinary() const;
    double outputLine(ActivityFunction::ActFunction xFunc) const;


	QList<double> input() const;
	void setInput(bool A, bool B);
	void setInput(const QList<bool> &input);
	void setInput(const QList<double> &input);

	void initWeight(const QList<bool> &input);
    void ClearWeight(const QList<bool> &input);

	void initWeight(const QList<double> &input);
    void ClearWeight(const QList<double> &input);

	QList<double> weight() const;
	void setWeight(double A, double B);
	void setWeight(double A, double B, double C);
	void setWeight(const QList<double> &weight);

    double bias() const;

    void CalcOutput();

private:
    QList<double> mInput;
    double mOutput = 0;
    double mBias = 0;
    ActivityFunction::ActFunction mFunc;
	QList<double> mWeight;

};

#endif // ONLINE_H
