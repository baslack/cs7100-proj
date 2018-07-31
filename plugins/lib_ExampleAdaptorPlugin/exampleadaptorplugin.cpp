#include "exampleadaptorplugin.h"

ExampleAdaptorPlugin::ExampleAdaptorPlugin(QObject* parent) :
    QObject(parent),
    AdaptorPlugin ("fcn7", "Example function from Dr. Dedoncker."),
    fn(&fcn7) {
    addParam("x", "Parameter 0", 1.0);
    addParam("y", "Parameter 1", 1.0);
}

//ExampleAdaptorPlugin::~ExampleAdaptorPlugin(void){

//}

int ExampleAdaptorPlugin::call( const QVector<double>& params, double& result) {
    int temp = numParams();
    int* ndims = &temp;
    QVector<double> tempx = params;
    double* x = tempx.data();
    int temp2 = 1;
    int* nfcns = &temp2;
    int retval = fn(ndims, x, nfcns, &result);
    return retval;
}


