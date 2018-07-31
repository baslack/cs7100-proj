#include <QString>
#include <QtTest>

class Attempt2_Test : public QObject
{
    Q_OBJECT

public:
    Attempt2_Test();

private Q_SLOTS:
    void testCase1();
};

Attempt2_Test::Attempt2_Test()
{
}

void Attempt2_Test::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Attempt2_Test)

#include "tst_attempt2_test.moc"
