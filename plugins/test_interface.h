#ifndef TEST_INTERFACE_H
#define TEST_INTERFACE_H

//#include <QtPlugin>
#include <QString>

class TestInterface
{
public:
    virtual ~TestInterface() = 0;
    virtual void doA(void) = 0;
    virtual void doB(void) = 0;
};


QT_BEGIN_NAMESPACE

#define TestInterface_iid "test.TestInterface"

Q_DECLARE_INTERFACE(TestInterface, TestInterface_iid)

QT_END_NAMESPACE

#endif // TEST_INTERFACE_H
