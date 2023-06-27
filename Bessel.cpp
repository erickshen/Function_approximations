#include <iostream>
#include <cmath>
using namespace std;
void Evaluate_D_Coefficients(int n, double *values, double *points, double *D);
void Evaluate_C_Coefficients(double *D, double *values, double *points, double *Coeff_Vector);
double Evaluate_Function(double x, int n, double *D, double *values, double *points, double *Coeff_Vector);

double Evaluate_Function(double x, int n, double *D, double *values, double *points, double *Coeff_Vector)
{
    if(x < points[0] || x > points[n-1])
        return 0;
   Evaluate_D_Coefficients(n, values, points, D);
    for(int i = 0; i < n-1; i++)
    {
        if(x >= points[i] && x < points[i+1])
        {
            Evaluate_C_Coefficients(D+i, values+i, points+i, Coeff_Vector);
            return Coeff_Vector[0] + Coeff_Vector[1]*(x-points[i]) + Coeff_Vector[2]*(x-points[i])*(x-points[i]) + Coeff_Vector[3]*(x-points[i])*(x-points[i])*(x-points[i]);
        }
        else continue;
    }
    return 0;
}

void Evaluate_D_Coefficients(int n, double *values, double *points, double *D)
{
    for(int i = 1; i < n-1; i++)
        D[i] = ((points[i+1] - points[i])*(values[i-1]-values[i])/(points[i-1]-points[i]) + (points[i]-points[i-1])*(values[i+1]-values[i])/(points[i+1]-points[i]))/(points[i+1]-points[i-1]);

    D[0] = 1.5*(values[1]-values[0])/(points[1]-points[0]) - 0.5*D[1];
    D[n-1] = 1.5*(values[n-1]-values[n-2])/(points[n-1]-points[n-2]) - 0.5*D[n-2];

}

void Evaluate_C_Coefficients(double *D, double *values, double *points, double *Coeff_Vector)
{
    Coeff_Vector[0] = values[0];
    Coeff_Vector[1] = D[0];
    Coeff_Vector[2] = (3.0*(values[1]-values[0])/(points[1]-points[0]) - 2.0*D[0]-D[1])/(points[1]-points[0]);
    Coeff_Vector[3] = (D[0]+D[1] - 2.0*(values[1]-values[0])/(points[1]-points[0]))/(points[1]-points[0])/(points[1]-points[0]);
}
