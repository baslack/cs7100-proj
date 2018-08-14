#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <QVector>

double fourth_derivative(
        void (*)(const QVector<double>&, double&),
        const QVector<double>&,
        int,
        double);

#endif // DERIVATIVES_H
