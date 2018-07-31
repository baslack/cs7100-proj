#ifndef TESTB_H
#define TESTB_H
#include <testa.h>
//#include <test_interface.h>
class TestB : public TestA
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "test.TestInterface" FILE "_TestB.json")
    Q_INTERFACES(TestInterface)

public:
    void doB(void) Q_DECL_OVERRIDE;
};

#endif // TESTB_H
