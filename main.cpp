#include <QApplication>
#include "mainwindow.h"
#include "Definitions.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&w, SIGNAL(textChanged(QString)), &w, SLOT(on_MainWindow_textChanged(const QString)));
    w.show();




    /*
    void (*f1)(double y, double*points, double* values, int k, double* result_vector, double *support_values, double *support_points) = NULL;
    double (*f2)(double x, int n, double *D, double *values, double *points, double *Coeff_Vector) = NULL;
    f1 = Newton_derivatives;
    f2 = Evaluate_Function;
    Drawing_Stuff(f1, f2);
*/
    
    return a.exec();
}
