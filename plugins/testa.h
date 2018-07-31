#ifndef TESTA_H
#define TESTA_H

#include <QObject>
#include <QtPlugin>
#include "test_interface.h"

class TestA : public QObject, public TestInterface
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "test.TestInterface" FILE "_TestA.json")
    Q_INTERFACES(TestInterface)
public:
    void doA(void) Q_DECL_OVERRIDE;
//    virtual void doB(void) Q_DECL_OVERRIDE;
};

#endif // TESTA_H
