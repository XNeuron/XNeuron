#include <QString>
#include <QtTest>

class XNeuronTestTest : public QObject
{
    Q_OBJECT

public:
    XNeuronTestTest();

private Q_SLOTS:
    void AND();
};

XNeuronTestTest::XNeuronTestTest()
{
}

void XNeuronTestTest::AND()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(XNeuronTestTest)

#include "tst_xneurontesttest.moc"
