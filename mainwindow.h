#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph(bool notEmpty = 0, double *solutions_vector = NULL, int n = 0);
    void drawGraph(bool notEmpty, double *solutions_vector, int n,
                   void *Newton_derivatives(double y, double*points, double* values, int k, double* result_vector, double *support_values, double *support_points),
                   double *Evaluate_Function(double x, int n, double *D, double *values, double *points, double *Coeff_Vector),
                   double *f1(double x),
                   double *f2(double x)
                   );
    void Delted_drawGraph(bool notEmpty = 0, double *solutions_vector = NULL, int n = 0,  double delta=0);
    double (MainWindow::*func)(double x);
    void recountPixels();
    void getData();
    void RotateDrawGraph(bool notEmpty = 0);
    double f(double x);
    double deltedFunc(double x, double delta);
    double delta;
    double f1(double x);
    double f2(double x);
    double f3(double x);
    double *points = NULL;
    double *values = NULL;
    double *support_values = NULL;
    double *support_points = NULL;
    double *solutions_vector = NULL;
    double *coefficients_vector = NULL;
    double *D = NULL;

    double *D_34 = NULL;
    double *coefficients_vector_34 = NULL;
    double* Newton_result_vector = NULL;
    double *Newton_support_values = NULL;
    double *Newton_support_points = NULL;
    int NumberOfPoints = 3;
    int n = 0; //Параметр - номер производной, передаваемый в drawGraph.

private slots:
    void on_exit_clicked();

    void on_clear_clicked();

    void on_draw_clicked();

    void on_pushButton_clicked();


    void on_radioButton_clicked();


    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void onTextChanged();

    void RadioClick();


private:
    Ui::MainWindow *ui;
    double leftX,rightX;
    double leftY,rightY;
    int pictWidth,pictHeight;
    double step;
    double onePixelX,onePixelY;
    double Ox,Oy;
    double scale;
};

#endif // MAINWINDOW_H
