#include <QString>
#include <QtTest>
#include <../XNeuron/gradienttraining.h>

class XNeuronTestTest : public QObject
{
    Q_OBJECT

public:
    XNeuronTestTest();

    void Check(QList<bool> o, QList<QList<bool>> i, XNeuron XN);
    void Check(QList<double> o, QList<QList<double>> i, XNeuron XN);

private:
    GradientTraining XNAND;
    GradientTraining XNAND2;
    GradientTraining XNOR;
    GradientTraining XNOR2;
    GradientTraining XNADD;

private Q_SLOTS:
    void AND();
    void AND2();
    void OR();
    void OR2();
    void ADD();
};

XNeuronTestTest::XNeuronTestTest()
{

}

void XNeuronTestTest::Check(QList<bool> o, QList<QList<bool>> i, XNeuron XN)
{
    for (QList<bool>& elem : i)
    {
        XN.setInput(elem);
        int index = i.indexOf(elem);
        QStringList str;
        for (bool& e : elem)
        {
            str.append(e?"1":"0");
        }
        QStringList str2;
        str2.append(str.join("x"));
        str2.append(QString().number(XN.output()));
        str2.append(QString().number(XN.bias()));
        str2.append(XN.outputBinary()?"1":"0");
        str2.append(o[index]?"1":"0");
        QVERIFY2(XN.outputBinary() == o[index],str2.join("=").toStdString().c_str());
    }
}

void XNeuronTestTest::Check(QList<double> o, QList<QList<double>> i, XNeuron XN)
{
    for (QList<double>& elem : i)
    {
        XN.setInput(elem);
        int index = i.indexOf(elem);
        QStringList str;
        for (double& e : elem)
        {
            str.append(QString().number(e));
        }
        QStringList str2;
        str2.append(str.join("x"));
        str2.append(QString().number(XN.output()));
        str2.append(QString().number(XN.bias()));
        str2.append(QString().number(XN.output()));
        str2.append(QString().number(o[index]));
        QVERIFY2(XN.output() == o[index],str2.join("=").toStdString().c_str());
    }
}

void XNeuronTestTest::AND()
{
    QList<QList<bool>> i;
    QList<bool> o;
    XNAND.setWeight(0, 0);

    i.append(QList<bool>() << false << false);
    i.append(QList<bool>() << false << true);
    i.append(QList<bool>() << true << false);
    i.append(QList<bool>() << true << true);
    o.append(false);
    o.append(false);
    o.append(false);
    o.append(true);

    XNAND.train(i, o);
    Check(o, i, XNAND);
}

void XNeuronTestTest::AND2()
{
    QList<QList<bool>> i;
    QList<bool> o;
    XNAND2.setWeight(0, 0, 0);

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

    XNAND2.train(i, o);
    Check(o, i, XNAND2);
}


void XNeuronTestTest::OR()
{
    QList<QList<bool>> i;
    QList<bool> o;
    XNOR.setWeight(0, 0);

    i.append(QList<bool>() << false << false);
    i.append(QList<bool>() << false << true);
    i.append(QList<bool>() << true << false);
    i.append(QList<bool>() << true << true);
    o.append(false);
    o.append(true);
    o.append(true);
    o.append(true);

    XNOR.train(i, o);
    Check(o, i, XNOR);
}

void XNeuronTestTest::OR2()
{
    QList<QList<bool>> i;
    QList<bool> o;
    XNOR2.setWeight(0, 0, 0);

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

    XNOR2.train(i, o);
    Check(o, i, XNOR2);
}

void XNeuronTestTest::ADD()
{
    QList<QList<double>> i;
    QList<double> o;
    XNADD.setWeight(1, 1, 1);

    i.append(QList<double>() << 1.0 << 1.5 << 2.0);
    i.append(QList<double>() << 3.5 << 2.8 << 3.25);
    i.append(QList<double>() << 15.25 << 14 << 100);
    o.append(1.0 + 1.5 + 2.0);
    o.append(3.5 + 2.8 + 3.25);
    o.append(15.25 + 14 + 100);

    XNADD.train(i, o, ActivityFunction::Line);
    Check(o, i, XNADD);
}

QTEST_APPLESS_MAIN(XNeuronTestTest)

#include "tst_xneurontesttest.moc"
