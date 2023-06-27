#include <iostream>
#include <cmath>
void Evaluate_D_Coefficients_34(int n, double *values, double *points, double *D, double* Newton_result_vector, double *Newton_support_values, double *Newton_support_points);
void Evaluate_C_Coefficients_34(double *D, double *values, double *points, double *Coeff_Vector);
void Newton_derivatives(int deg, double y, double*points, double* values, int k, double* result_vector, double *support_values, double *support_points);
double Evaluate_Function_34(double x, int n, double *D, double *values, double *points, double *Coeff_Vector, double* Newton_result_vector, double *Newton_support_values, double *Newton_support_points)
{
    if(x < points[0] || x > points[n-1])
        return 0;
    Evaluate_D_Coefficients_34(n, values, points, D, Newton_result_vector, Newton_support_values, Newton_support_points);
    for(int i = 0; i < n-1; i++)
    {
        if(x >= points[i] && x < points[i+1])
        {
            Evaluate_C_Coefficients_34(D+i, values+i, points+i, Coeff_Vector);
            return Coeff_Vector[0] + Coeff_Vector[1]*(x-points[i]) + Coeff_Vector[2]*(x-points[i])*(x-points[i]) + Coeff_Vector[3]*(x-points[i])*(x-points[i])*(x-points[i]);
        }
        else continue;
    }
    return 0;
}




void Evaluate_D_Coefficients_34(int n, double *values, double *points, double *D, double* Newton_result_vector, double *Newton_support_values, double *Newton_support_points) // n = number of points
{
    for(int i = 1; i < n-1; i++)
    {
        if((values[i-1]-values[i])/(points[i-1]-points[i]) * (values[i]-values[i+1])/(points[i]-points[i+1]) < 0)
            D[i] = 0;
        else if ((values[i]-values[i+1])/(points[i]-points[i+1]) < 0)
            D[i] = -std::min(fabs((values[i]-values[i+1])/(points[i]-points[i+1])), fabs((values[i-1]-values[i])/(points[i-1]-points[i])));
        else
            D[i] = +std::min(fabs((values[i]-values[i+1])/(points[i]-points[i+1])), fabs((values[i-1]-values[i])/(points[i-1]-points[i])));
    }
    Newton_derivatives(1, values[0], points, values, n, Newton_result_vector, Newton_support_values, Newton_support_points);
    D[0] = Newton_result_vector[1];
    Newton_derivatives(1, values[0], points, values, n, Newton_result_vector, Newton_support_values, Newton_support_points);
    D[n-1] = Newton_result_vector[1];



}

void Evaluate_C_Coefficients_34(double *D, double *values, double *points, double *Coeff_Vector)
{
    Coeff_Vector[0] = values[0];
    Coeff_Vector[1] = D[0];
    Coeff_Vector[2] = (3.0*(values[1]-values[0])/(points[1]-points[0]) - 2.0*D[0]-D[1])/(points[1]-points[0]);
    Coeff_Vector[3] = (D[0]+D[1] - 2.0*(values[1]-values[0])/(points[1]-points[0]))/(points[1]-points[0])/(points[1]-points[0]);
}
