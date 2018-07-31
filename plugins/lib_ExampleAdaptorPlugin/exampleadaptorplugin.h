#ifndef EXAMPLEADAPTORPLUGIN_H
#define EXAMPLEADAPTORPLUGIN_H

#include <QObject>
//#include <adaptorinterface.h>
#include "adaptorplugin.h"
#include <example.h>

class ExampleAdaptorPlugin :
        public QObject,
        public AdaptorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID AdaptorInterface_iid)
    Q_INTERFACES(AdaptorInterface)
  public:
    // constructor
    ExampleAdaptorPlugin(QObject* parent = 0);
//    ~ExampleAdaptorPlugin(void);
    // methods
    int call(const QVector<double>&, double&);
  protected:
    // data members
    int (*fn)(int*, double*, int*, double*);
};

#endif // EXAMPLEADAPTORPLUGIN_H
