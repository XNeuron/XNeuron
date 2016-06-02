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
    XNeuron XN;
    bool A=false;
    bool B=false;
    XN.setWeight(0.5,0.5);
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())==0, "0&0");
    A=false;
    B=true;
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())==0, "0&1");
    B=false;
    A=true;
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())==0, "1&0");
    A=true;
    B=true;
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())==1, "1&1");
}

void XNeuronTestTest::OR()
{
    XNeuron XN;
    bool A=false;
    bool B=false;
    XN.setWeight(1,1);
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())==0, "0&0");
    A=false;
    B=true;
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())>=1, "0&1");
    B=false;
    A=true;
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())>=1, "1&0");
    A=true;
    B=true;
    XN.setInput(A,B);
    QVERIFY2(((int)XN.outputReal())>=1, "1&1");
}

QTEST_APPLESS_MAIN(XNeuronTestTest)

#include "tst_xneurontesttest.moc"
