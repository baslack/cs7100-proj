#include "lib_example2adaptorplugin.h"


Lib_Example2AdaptorPlugin::Lib_Example2AdaptorPlugin(QObject* parent):
    QObject (parent),
    AdaptorPlugin("sqx2y2", "Second example function"),
    fn(&sqx2y2) {
    addParam("X", "X-Axis", 0.0);
    addParam("Y", "Y-Axis", 0.0);
}

int Lib_Example2AdaptorPlugin::call(const QVector<double>& params, double& result) {
    result = fn(params[0], params[1]);
    return SUCCESS;
}
