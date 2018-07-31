#include "exampleadaptor.h"

ExampleAdaptor::ExampleAdaptor():
    Adaptor("fcn7", "Example function from Dr. Dedoncker."),
    fn(&fcn7) {
    addParam("x", "Parameter 0", 1.0);
    addParam("y", "Parameter 1", 1.0);
}

ExampleAdaptor::~ExampleAdaptor() {
}

int ExampleAdaptor::call(
    QVector<double>& params,
    double& result) {
    int temp = int(NumParams());
    int* ndims = &temp;
    double* x = params.data();
    int temp2 = 1;
    int* nfcns = &temp2;
    double* funvls = &result;
    int retval = fn(ndims, x, nfcns, funvls);
    return retval;
}
