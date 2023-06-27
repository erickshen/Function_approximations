#include <iostream>
#include <fstream>
#include <cmath>
void divided_difference(int k, double *points, double *values);
double Gorner_scheme(int k, double *div_diff, double *points, double x);
void Newton_derivatives(int deg, double y, double*points, double* values, int k, double* result_vector);
double eps();
int factorial(int n);
using namespace std;

void Newton_derivatives(int deg, double y, double*points, double* values, int k, double* result_vector, double *support_values, double *support_points)
{
    for(int i = 0; i < k; i++)
    {
        support_values[i] = values[i];
        support_points[i] = points[i];
    }
    divided_difference(k, support_points, support_values);
    result_vector[0] = Gorner_scheme(k , support_values, support_points, y);

            for(int t = k-1; t > 0; t--)
                    {
                        support_values[t] = support_values[t-1];
                        support_points[t] = support_points[t-1];
                    }
        support_values[0] = result_vector[0];
        support_points[0] = y;

    for(int i = 0; i < deg; i++)
    {
        for(int j = 1; j <= k-1; j++)
        {


                support_values[j] = (support_values[j] - support_values[j-1])/(support_points[j]-y);

        }
       result_vector[i+1] = factorial(i+1)*Gorner_scheme(k-i-1, support_values+1, support_points+1, y);
        support_values[0] = result_vector[i+1]/factorial(i+1);
    }

}



void divided_difference(int k, double *points, double *values)
{
  for(int i = k-1; i >=1; i--)
  {
      for(int j = k-1; j >= k-i; j--)
          values[j] = (values[j] - values[j-1])/(points[j]-points[j-(k-i)]);

  }
}

double Gorner_scheme(int k, double *div_diff, double *points, double x)
{
    if(k >= 2)
    {

    double value = (x-points[k-2])*div_diff[k-1] + div_diff[k-2];
    for(int i = k-2; i >= 1; i--)
            value = (x-points[i-1])*value + div_diff[i-1];
    return value;
    }
    else if (k < 2 && k > 0)
        return div_diff[0];
    else if(k == 0)
        return 0;
   return 0;
}

int factorial(int n)
{
    if(n == 1 || n == 0)
        return 1;
    else if(n < 0)
        return 0;
    else if(n > 1)
      return n*factorial(n-1);
    return 0;
}

double eps()
{
    double eps = 1.0;
    while(eps + 1.0 > 1.0)
        eps = eps/2;
    return eps;
}
