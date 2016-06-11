#include "gradienttraining.h"

GradientTraining::GradientTraining()
{

}


bool GradientTraining::train(QList<QList<bool>> &xInput, QList<bool> &xOutputRequired)
{
	QList<QList<double>> mInput;
	QList<double> mOutputRequired;

	for (QList<bool>& x : xInput)
	{
		QList<double> temp;
		for (bool& y : x)
		{
			temp.append((y) ? (1) : (-1));
		}
		mInput.append(temp);
		mOutputRequired.append((xOutputRequired[xInput.indexOf(x)]) ? (1) : (-1));
	}
	return train(mInput, mOutputRequired, ActivityFunction::Binary);
}

bool GradientTraining::train(QList<QList<double>> &xInput, QList<double> &xOutputRequired, ActivityFunction::ActFunction xFunc)
{
	mFunc = xFunc;
	qsrand(QTime::currentTime().msec());
	int check = 5;
	for (int var = 0; true; var++)
	{
		double N = 1;
		do
		{
			N = ((double)((qrand()) % 1000)) / (1000);
		} while (N == 0);

		double error = xInput.size() + check;
		for (QList<double>& x : xInput)
		{
			setInput(x);
			int tIndexOfInput = xInput.indexOf(x);

			double mDelta = xOutputRequired[tIndexOfInput] - mOutput;
			if (abs(mDelta) > 10 || isnan(mDelta) || isinf(mDelta))
			{
				mBias = 0;
				for (int i = 0; i < mWeight.length(); i++)
				{
					mWeight[i] = mWeight[i] / 2;
				}
			}
			else  if (mDelta != 0)
			{
				mBias = mBias - N*mDelta;

				double tMax;
				tMax = abs(mInput[0]);
				for each (double var in mInput)
				{
					if (tMax < abs(var))
						tMax = abs(var);
				}

				for (int i = 0; i < mWeight.length(); i++)
				{
					mWeight[i] = mWeight[i] + mDelta*mInput[i] / tMax;
				}
			}
			else
				error--;
		}
		if (error <= check)
			check--;
		if (error <= 0)
			return true;
	}

	return false;
}

