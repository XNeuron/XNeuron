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
    void OR();
};

XNeuronTestTest::XNeuronTestTest()
{
}

void XNeuronTestTest::AND()
{
    QList<QList<bool>> i;
    QList<bool> o;
    XNeuron XN;
    XN.setWeight(2.3,-0.15);

    i.append(QList<bool>()<<false<<false);
    i.append(QList<bool>()<<false<<true);
    i.append(QList<bool>()<<true<<false);
    i.append(QList<bool>()<<true<<true);
    o.append(false);
    o.append(false);
    o.append(false);
    o.append(true);

    XN.train(i,o);

    XN.setInput(false,false);
    QVERIFY2(!XN.outputBinary(), "0&0");

    XN.setInput(false,true);
    QVERIFY2(!XN.outputBinary(), "0&1");

    XN.setInput(true,false);
    QVERIFY2(!XN.outputBinary(), "1&0");

    XN.setInput(true,true);
    QVERIFY2(XN.outputBinary(), "1&1");
}

void XNeuronTestTest::OR()
{
    QList<QList<bool>> i;
    QList<bool> o;
    XNeuron XN;
    XN.setWeight(2.3,-0.15);

    i.append(QList<bool>()<<false<<false);
    i.append(QList<bool>()<<false<<true);
    i.append(QList<bool>()<<true<<false);
    i.append(QList<bool>()<<true<<true);
    o.append(false);
    o.append(true);
    o.append(true);
    o.append(true);

    XN.train(i,o);
    XN.setInput(false,false);
    QVERIFY2(XN.outputBinary()==false, "0&0");

    XN.setInput(false,true);
    QVERIFY2(XN.outputBinary()==true, "0&1");

    XN.setInput(true,false);
    QVERIFY2(XN.outputBinary()==true, "1&0");

    XN.setInput(true,true);
    QVERIFY2(XN.outputBinary()==true, "1&1");
}

QTEST_APPLESS_MAIN(XNeuronTestTest)

#include "tst_xneurontesttest.moc"
