#include "example.h"

int fcn7(int *ndims, double x[], int *nfcns, double funvls[])

{

    //    pi_base_t xx,yy,z,aa = 0.4,bb = 0.3;

    double xx, yy, z, aa = 0.4, bb = 0.3;

    xx = (x[0] - aa);

    yy = (x[1] - bb);

    z = xx * xx + yy * yy + 0.001;

    funvls[0] = 1.0 / z;

    printf("%le %le %le\n", x[0], x[1], funvls[0]);

    return 0;

}
