#ifndef XNEURON_H
#define XNEURON_H

/*#include "xneuron_global.h"*/
#include <QList>

class /*XNEURONSHARED_EXPORT*/ XNeuron
{

public:
	XNeuron();

	QList<XNeuron> mNextNeuron;

	bool train(QList<QList<bool>> &input, QList<bool> &mOutputRequired);
	bool train(QList<QList<double>> &input, QList<double> &mOutputRequired);

	double outputReal() const;
	bool outputBinary() const;
	void setOutputReal(double outputReal);

	QList<double> input() const;
	void setInput(bool A, bool B);
	void setInput(const QList<bool> &input);
	void setInput(const QList<double> &input);

	void initWeight(const QList<bool> &input);
	void clearWeight(const QList<bool> &input);

	void initWeight(const QList<double> &input);
	void clearWeight(const QList<double> &input);

	QList<double> weight() const;
	void setWeight(double A, double B);
	void setWeight(double A, double B, double C);
	void setWeight(const QList<double> &weight);

private:
	QList<double> mInput;
	double mOutputReal = 0;
	double mOutputRequired = 0;
	double mBase = 0;
	QList<double> mWeight;

};

#endif // XNEURON_H
