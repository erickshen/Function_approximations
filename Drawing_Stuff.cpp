#include "mainwindow.h"
void Drawing_Stuff(void (*Newton_derivatives)(double y, double*points, double* values, int k, double* result_vector, double *support_values, double *support_points),
                   double (*Evaluate_Function)(double x, int n, double *D, double *values, double *points, double *Coeff_Vector))
{
    MainWindow w;
    w.show();   
}


