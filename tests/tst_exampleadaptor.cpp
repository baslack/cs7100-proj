#include "../app/exampleadaptor.h"
#include <QtTest>

class ExampleAdaptorTest : public QObject
{
    Q_OBJECT

public:
    ExampleAdaptorTest();

private Q_SLOTS:
    void testCall();
};

ExampleAdaptorTest::ExampleAdaptorTest()
{
}

void ExampleAdaptorTest::testCall()
{
    ExampleAdaptor temp;
    QVector<double> params;
    double result;
    params.push_back(1.4);
    params.push_back(1.3);
    temp.call(params, result);
    bool test = (result - (1.0/2.001) < 1.0e-6);
    QVERIFY2(test, "ExampleAdaptor Call Test Failed!");
}

QTEST_APPLESS_MAIN(ExampleAdaptorTest)

#include "tst_exampleadaptor.moc"
