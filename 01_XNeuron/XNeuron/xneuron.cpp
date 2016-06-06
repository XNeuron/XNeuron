#include "xneuron.h"

XNeuron::XNeuron()
{
}

bool XNeuron::train(QList<QList<bool>> &xInput, QList<bool> &xOutputRequired)
{
	QList<QList<double>> mInput;
	QList<double> mOutputRequired;

	for (QList<bool>& x : xInput)
	{
		QList<double> temp;
		for (bool& y : x)
		{
			temp.append((y) ? (1) : (0));
		}
		mInput.append(temp);
		mOutputRequired.append((xOutputRequired[xInput.indexOf(x)]) ? (1) : (0));
	}
	return train(mInput, mOutputRequired);
}

bool XNeuron::train(QList<QList<double>> &input, QList<double> &mOutputRequired)
{
	for (int var = 0; 1000; ++var)
	{
		double error = 0;
		for (QList<double>& x : input)
		{
			double temp = 0;
			for (int i = 0; i < x.length(); i++)//double& value : x)
			{
				temp += x[i]*mWeight[i];
			}

			double y = (temp >= mBase) ? (1) : (0);

			if (y != mOutputRequired[input.indexOf(x)]) 
			{

				double delta = mOutputRequired[input.indexOf(x)] - y;

				mBase = mBase - delta;

				for (int i = 0; i < mWeight.length();i++)//double& value : mWeight)
				{
					mWeight[i] = mWeight[i] + delta*x[i];
				}

				error ++;
			}
		}
		if (error <= 0)
			return true;
	}

	return false;
}

double XNeuron::outputReal() const
{
	return mOutputReal;
}

bool XNeuron::outputBinary() const
{
	return   mOutputReal >= mBase;//(mOutputReal < 0.75 ? 0 : 1);
}

void XNeuron::setOutputReal(double outputReal)
{
	mOutputReal = outputReal;
}

QList<double> XNeuron::input() const
{
	return mInput;
}

void XNeuron::setInput(bool A, bool B)
{
	setInput(QList<bool>() << A << B);
}

void XNeuron::setInput(const QList<bool> &input)
{
	QList<double> tInput;
	clearWeight(input);
	for (bool elem : input)
	{
		initWeight(input);
		tInput << (elem ? 1.0 : 0.0);
	}
	setInput(tInput);
}

void XNeuron::initWeight(const QList<double> &input)
{
	if (mWeight.length() != input.length())
	{
		mWeight.append(0.5);
	}
}

void XNeuron::clearWeight(const QList<double> &input)
{
	if (mWeight.length() != input.length())
	{
		mWeight.clear();
	}
}

void XNeuron::initWeight(const QList<bool> &input)
{
	if (mWeight.length() != input.length())
	{
		mWeight.append(0.5);
	}
}

void XNeuron::clearWeight(const QList<bool> &input)
{
	if (mWeight.length() != input.length())
	{
		mWeight.clear();
	}
}

QList<double> XNeuron::weight() const
{
	return mWeight;
}

void XNeuron::setWeight(double A, double B)
{
	mWeight = QList<double>() << A << B;
}

void XNeuron::setWeight(double A, double B, double C)
{
	mWeight = QList<double>() << A << B << C;
}

void XNeuron::setWeight(const QList<double> &weight)
{
	mWeight = weight;
}

void XNeuron::setInput(const QList<double> &input)
{
	mInput = input;
	clearWeight(input);
	mOutputReal = 0;
	for (const double& elem : input)
	{
		initWeight(input);
		mOutputReal += elem*mWeight[input.indexOf(elem)];
	}
}
