#ifndef LIB_EXAMPLE2ADAPTORPLUGIN_H
#define LIB_EXAMPLE2ADAPTORPLUGIN_H
#include <QObject>
#include <adaptorplugin.h>
#include <example2.h>

class Lib_Example2AdaptorPlugin :
    public QObject,
    public AdaptorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID AdaptorInterface_iid)
    Q_INTERFACES(AdaptorInterface)
  public:
    Lib_Example2AdaptorPlugin(QObject* parent = 0);
    int call(const QVector<double>&, double&);
  protected:
    double (*fn)(double, double, double, double);
};

#endif // LIB_EXAMPLE2ADAPTORPLUGIN_H
