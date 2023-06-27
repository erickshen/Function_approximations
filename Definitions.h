void Newton_derivatives(int deg, double y, double*points, double* values, int k, double* result_vector, double *support_values, double *support_points);
double Evaluate_Function(double x, int n, double *D, double *values, double *points, double *Coeff_Vector);
void Drawing_Stuff(void (*Newton_derivatives)(double y, double*points, double* values, int k, double* result_vector, double *support_values, double *support_points),
                   double (*Evaluate_Function)(double x, int n, double *D, double *values, double *points, double *Coeff_Vector));
double Evaluate_Function_34(double x, int n, double *D, double *values, double *points, double *Coeff_Vector, double* Newton_result_vector, double *Newton_support_values, double *Newton_support_points);

