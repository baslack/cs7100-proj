#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <QVector>
#include "adaptorinterface.h"

double fourth_derivative(
        AdaptorInterface*,
        const QVector<double>&,
        int,
        double);

#endif // DERIVATIVES_H
