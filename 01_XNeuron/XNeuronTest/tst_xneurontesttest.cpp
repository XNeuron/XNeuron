#include <QString>
#include <QtTest>
#include <../XNeuron/xneuron.h>

class XNeuronTestTest : public QObject
{
	Q_OBJECT

public:
	XNeuronTestTest();

	private Q_SLOTS:
	void AND();
	void AND2();
	void OR();
	void OR2();
	void Plus();
};

XNeuronTestTest::XNeuronTestTest()
{
}

void XNeuronTestTest::AND()
{
	QList<QList<bool>> i;
	QList<bool> o;
	XNeuron XN;
	XN.setWeight(0, 0);

	i.append(QList<bool>() << false << false);
	i.append(QList<bool>() << false << true);
	i.append(QList<bool>() << true << false);
	i.append(QList<bool>() << true << true);
	o.append(false);
	o.append(false);
	o.append(false);
	o.append(true);

	XN.train(i, o);

	XN.setInput(false, false);
	QVERIFY2(!XN.outputBinary(), "0&0");

	XN.setInput(false, true);
	QVERIFY2(!XN.outputBinary(), "0&1");

	XN.setInput(true, false);
	QVERIFY2(!XN.outputBinary(), "1&0");

	XN.setInput(true, true);
	QVERIFY2(XN.outputBinary(), "1&1");
}

void XNeuronTestTest::AND2()
{
	QList<QList<bool>> i;
	QList<bool> o;
	XNeuron XN;
	XN.setWeight(0, 0, 0);

	i.append(QList<bool>() << false << false << false);
	i.append(QList<bool>() << false << true << false);
	i.append(QList<bool>() << true << false << false);
	i.append(QList<bool>() << true << true << false);
	i.append(QList<bool>() << false << false << true);
	i.append(QList<bool>() << false << true << true);
	i.append(QList<bool>() << true << false << true);
	i.append(QList<bool>() << true << true << true);
	o.append(false & false & false);
	o.append(false & true & false);
	o.append(true & false & false);
	o.append(true & true & false);
	o.append(false & false & true);
	o.append(false & true & true);
	o.append(true & false & true);
	o.append(true & true & true);

	XN.train(i, o);

	for (QList<bool>& elem : i)
	{
		XN.setInput(elem);
		int index = i.indexOf(elem);
		//QVERIFY2(XN.outputBinary() == o[index], "AND2");
	}
}


void XNeuronTestTest::OR()
{
	QList<QList<bool>> i;
	QList<bool> o;
	XNeuron XN;
	XN.setWeight(0, 0);

	i.append(QList<bool>() << false << false);
	i.append(QList<bool>() << false << true);
	i.append(QList<bool>() << true << false);
	i.append(QList<bool>() << true << true);
	o.append(false);
	o.append(true);
	o.append(true);
	o.append(true);

	XN.train(i, o);
	XN.setInput(false, false);
	QVERIFY2(XN.outputBinary() == false, "0&0");

	XN.setInput(false, true);
	QVERIFY2(XN.outputBinary() == true, "0&1");

	XN.setInput(true, false);
	QVERIFY2(XN.outputBinary() == true, "1&0");

	XN.setInput(true, true);
	QVERIFY2(XN.outputBinary() == true, "1&1");
}

void XNeuronTestTest::OR2()
{
	QList<QList<bool>> i;
	QList<bool> o;
	XNeuron XN;
	XN.setWeight(0, 0, 0);

	i.append(QList<bool>() << false << false << false);
	i.append(QList<bool>() << false << true << false);
	i.append(QList<bool>() << true << false << false);
	i.append(QList<bool>() << true << true << false);
	i.append(QList<bool>() << false << false << true);
	i.append(QList<bool>() << false << true << true);
	i.append(QList<bool>() << true << false << true);
	i.append(QList<bool>() << true << true << true);
	o.append(false | false | false);
	o.append(false | true | false);
	o.append(true | false | false);
	o.append(true | true | false);
	o.append(false | false | true);
	o.append(false | true | true);
	o.append(true | false | true);
	o.append(true | true | true);

	XN.train(i, o);

	for (QList<bool>& elem : i)
	{
		XN.setInput(elem);
		int index = i.indexOf(elem);
		QVERIFY2(XN.outputBinary() == o[index], "OR2");
	}
}

void XNeuronTestTest::Plus()
{
	//QList<QList<double>> i;
	//QList<double> o;
	//XNeuron XN;
	//XN.setWeight(0, 0, 0);

	//i.append(QList<double>() << 1.0 << 1.5 << 2.0);
	//i.append(QList<double>() << 3.5 << 2.8 << 3.25);
	//i.append(QList<double>() << 15.25 << 14 << 100);
	//o.append(1.0 + 1.5 + 2.0);
	//o.append(3.5 + 2.8 + 3.25);
	//o.append(15.25 + 14 + 100);

	//XN.train(i, o);

	//for (QList<double>& elem : i)
	//{
	//	XN.setInput(elem);
	//	int index = i.indexOf(elem);
	//	QVERIFY2(XN.outputBinary() == o[index], "Plus");
	//}
}

QTEST_APPLESS_MAIN(XNeuronTestTest)

#include "tst_xneurontesttest.moc"
