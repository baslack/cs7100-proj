#ifndef EXAMPLEADAPTOR_H
#define EXAMPLEADAPTOR_H
#include "adaptor.h"
#include "example.h"

class ExampleAdaptor : public Adaptor {
  public:
    ExampleAdaptor();
    ~ExampleAdaptor(void);
    int call(QVector<double> &, double &);
  private:
    int (*fn)(int *, double *, int *, double *);
};

#endif // EXAMPLEADAPTOR_H
