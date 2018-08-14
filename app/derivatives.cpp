#include "derivatives.h"

/*
 * Performs a fourth derivative on the given call
 * function for use in the adaptive mesh class. Based
 * on the five point stencil technique described here:
 *
 * https://en.wikipedia.org/wiki/Five-point_stencil
 *
*/

double fourth_derivative(
    void (*fn)(const QVector<double>&, double&),
    const QVector<double>& orig_x,
    int axis_index,
    double h) {
    // int return value
    double retVal = 0;
    // setup a vector and term holder for scratch work
    QVector<double> scratch_x(orig_x);
    scratch_x[axis_index] = orig_x[axis_index] + 2 * h;
    double term = 0;
    // calculate each term and total it up
    fn(scratch_x, term);
    retVal += term;
    scratch_x[axis_index] = orig_x[axis_index] + h;
    fn(scratch_x, term);
    retVal -= (4 * term);
    scratch_x[axis_index] = orig_x[axis_index];
    fn(scratch_x, term);
    retVal += (6 * term);
    scratch_x[axis_index] = orig_x[axis_index] - h;
    fn(scratch_x, term);
    retVal -= (4 * term);
    scratch_x[axis_index] = orig_x[axis_index] - 2 * h;
    fn(scratch_x, term);
    retVal += term;
    // divide out by offset to the fourth power
    retVal /= (h * h * h * h);
    return retVal;
}
