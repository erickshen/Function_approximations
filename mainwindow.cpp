#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include <fstream>
#include "Definitions.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pictHeight = 600;
    pictWidth = 600;
    step = 0.1;
    scale = 1;
    leftX = -100; rightX = 100;
    leftY = -100; rightY = 100;
    n = 0;
    drawGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::f(double x)
{
    return x;
}

double MainWindow::deltedFunc(double x, double delta)
{
    if(sqrt((x-delta)*(x-delta)) > 1e-15)
        return 0;
    else
        return 1;
}

double MainWindow::f1(double x)
{
    return sin(x);
}

double MainWindow::f2(double x)
{
    return cos(x);
}

double MainWindow::f3(double x)
{
    return sin(1.0/x);
}

void MainWindow::recountPixels()
{
    onePixelX = 600.0/(rightX-leftX);
    onePixelY = 600.0/(rightY-leftY);
    Ox = fabs(leftX); Oy = rightY;
}

void MainWindow::getData()
{
    if(!ui->inputLeftX->text().isEmpty())
         leftX = ui->inputLeftX->text().toDouble();
    if(!ui->inputRightX->text().isEmpty())
         rightX = ui->inputRightX->text().toDouble();
    if(!ui->inputLeftY->text().isEmpty())
         leftY = ui->inputLeftY->text().toDouble();
    if(!ui->inputRightY->text().isEmpty())
         rightY = ui->inputRightY->text().toDouble();
    if(!ui->inputAccuracy->text().isEmpty())
        step = ui->inputAccuracy->text().toDouble();
    if(!ui->inputScale->text().isEmpty())
         scale = ui->inputScale->text().toDouble();
    if(!ui->DerDeg->text().isEmpty())
        n = ui->DerDeg->text().toInt();
    if(!ui->InputDelta->text().isEmpty())
        delta=ui->InputDelta->text().toDouble();

}

void MainWindow::drawGraph(bool notEmpty, double *solutions_vector, int n)
{
    QPixmap graph(600,600);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0,0,600,600);
    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight);
    paint.drawLine(0,Oy*onePixelY,pictWidth,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,3));
    for(double i = leftX;i<=rightX;i+=10.0)
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
    for(double i = leftY;i<=rightY;i+=10.0)
        paint.drawPoint(Ox*onePixelX,(i+Oy)*onePixelY);

   paint.translate(Ox*onePixelX,Oy*onePixelY);
   paint.scale(scale, scale);
    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path,p[5];
    bool first[6] = {1,1,1,1,1,1};

    for(double i = (double)leftX+step;i<=(double)rightX;i+=step)
        {
        Newton_derivatives(n, i, points, values, NumberOfPoints, solutions_vector, support_values, support_points); // Вычисляем по методу 1 всё шо надо
        if(!isnan(solutions_vector[n])) {
            if(first[0]) {
               path.moveTo((i)*onePixelX,(-solutions_vector[n])*onePixelY); //Рисуем выбранную производную
                first[0] = false;
            }
            else
               path.lineTo((i)*onePixelX,(-solutions_vector[n])*onePixelY);
        }
        if(!isnan(f1(i))) {
            if(first[1]) {
                p[0].moveTo((i)*onePixelX,(-f1(i))*onePixelY);
                first[1] = false;
            }
            else
                p[0].lineTo((i)*onePixelX,(-f1(i))*onePixelY);
        }
        if(!isnan(f2(i))) {
            if(first[2]) {
                p[1].moveTo((i)*onePixelX,(-f2(i))*onePixelY);
                first[2] = false;
            }
            else
                p[1].lineTo((i)*onePixelX,(-f2(i))*onePixelY);
        }
        if(!isnan(f3(i))) {
            if(first[3]) {
                p[2].moveTo((i)*onePixelX,(-f3(i))*onePixelY);
                first[3] = false;
            }
            else
                p[2].lineTo((i)*onePixelX,(-f3(i))*onePixelY);
        }
        if(!isnan(Evaluate_Function(i, NumberOfPoints, D, values, points, coefficients_vector)))
        {
            if(first[4]){
                p[3].moveTo((i)*onePixelX, -Evaluate_Function(i, NumberOfPoints, D, values, points, coefficients_vector)*onePixelY);
                first[4] = false;
            }
            else
                p[3].lineTo((i)*onePixelX,-Evaluate_Function(i, NumberOfPoints, D, values, points, coefficients_vector)*onePixelY);
        }
        if(!isnan(Evaluate_Function_34(i, NumberOfPoints, D, values, points, coefficients_vector_34, Newton_result_vector, Newton_support_values, Newton_support_points)))
        {
            if(first[5]){
                p[4].moveTo((i)*onePixelX, -Evaluate_Function_34(i, NumberOfPoints, D, values, points, coefficients_vector_34, Newton_result_vector, Newton_support_values, Newton_support_points)*onePixelY);
                first[5] = false;
            }
            else
                p[4].lineTo((i)*onePixelX,-Evaluate_Function_34(i, NumberOfPoints, D, values, points, coefficients_vector_34, Newton_result_vector, Newton_support_values, Newton_support_points)*onePixelY);
        }
    }
    if(ui->main->isChecked()) {
        paint.setPen(QPen(Qt::blue,1,Qt::SolidLine));
        paint.drawPath(path);
    }
    if(ui->sin->isChecked()) {
        paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
        paint.drawPath(p[0]);
    }
    if(ui->cos->isChecked()) {
        paint.setPen(QPen(Qt::red,1,Qt::SolidLine));
        paint.drawPath(p[1]);
    }
    if(ui->tg->isChecked()) {
        paint.setPen(QPen(Qt::darkMagenta,1,Qt::SolidLine));
        paint.drawPath(p[2]);
    }
    if(ui->Bessel->isChecked())
    {
        paint.setPen(QPen(Qt::black,1,Qt::SolidLine));
        paint.drawPath(p[3]);
    }
    if(ui->PROGA34->isChecked())
    {
        paint.setPen(QPen(Qt::darkMagenta,1,Qt::SolidLine));
        paint.drawPath(p[4]);

    }
    paint.end();
    ui->outputGraph->setPixmap(graph);
    return;
}


void MainWindow::Delted_drawGraph(bool notEmpty, double *solutions_vector, int n, double delta)
{
    QPixmap graph(600,600);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0,0,600,600);
    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight);
    paint.drawLine(0,Oy*onePixelY,pictWidth,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,3));
    for(double i = leftX;i<=rightX;i+=10.0)
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
    for(double i = leftY;i<=rightY;i+=10.0)
        paint.drawPoint(Ox*onePixelX,(i+Oy)*onePixelY);

   paint.translate(Ox*onePixelX,Oy*onePixelY);
   paint.scale(scale, scale);
    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path,p[5];
    bool first[6] = {1,1,1,1,1,1};

    for(double i = (double)leftX+step;i<=(double)rightX;i+=step)
        {
        Newton_derivatives(n, i, points, values, NumberOfPoints, solutions_vector, support_values, support_points); // Вычисляем по методу 1 всё шо надо
        if(!isnan(solutions_vector[n])) {
            if(first[0]) {
               path.moveTo((i)*onePixelX,(-solutions_vector[n])*onePixelY); //Рисуем выбранную производную
                first[0] = false;
            }
            else
               path.lineTo((i)*onePixelX,(-solutions_vector[n])*onePixelY);
        }
        if(!isnan(f1(i))) {
            if(first[1]) {
                p[0].moveTo((i)*onePixelX,(-f1(i)-deltedFunc(i, delta))*onePixelY);
                first[1] = false;
            }
            else
                p[0].lineTo((i)*onePixelX,(-f1(i)-deltedFunc(i, delta))*onePixelY);
        }
        if(!isnan(f2(i))) {
            if(first[2]) {
                p[1].moveTo((i)*onePixelX,(-f2(i)-deltedFunc(i, delta))*onePixelY);
                first[2] = false;
            }
            else
                p[1].lineTo((i)*onePixelX,(-f2(i)-deltedFunc(i, delta))*onePixelY);
        }
        if(!isnan(f3(i))) {
            if(first[3]) {
                p[2].moveTo((i)*onePixelX,(-f3(i)-deltedFunc(i, delta))*onePixelY);
                first[3] = false;
            }
            else
                p[2].lineTo((i)*onePixelX,(-f3(i)-deltedFunc(i, delta))*onePixelY);
        }
        if(!isnan(Evaluate_Function(i, NumberOfPoints, D, values, points, coefficients_vector)))
        {
            if(first[4]){
                p[3].moveTo((i)*onePixelX, -Evaluate_Function(i, NumberOfPoints, D, values, points, coefficients_vector)*onePixelY);
                first[4] = false;
            }
            else
                p[3].lineTo((i)*onePixelX,-Evaluate_Function(i, NumberOfPoints, D, values, points, coefficients_vector)*onePixelY);
        }
        if(!isnan(Evaluate_Function_34(i, NumberOfPoints, D, values, points, coefficients_vector_34, Newton_result_vector, Newton_support_values, Newton_support_points)))
        {
            if(first[5]){
                p[4].moveTo((i)*onePixelX, -Evaluate_Function_34(i, NumberOfPoints, D, values, points, coefficients_vector_34, Newton_result_vector, Newton_support_values, Newton_support_points)*onePixelY);
                first[5] = false;
            }
            else
                p[4].lineTo((i)*onePixelX,-Evaluate_Function_34(i, NumberOfPoints, D, values, points, coefficients_vector_34, Newton_result_vector, Newton_support_values, Newton_support_points)*onePixelY);
        }
    }
    if(ui->main->isChecked()) {
        paint.setPen(QPen(Qt::blue,1,Qt::SolidLine));
        paint.drawPath(path);
    }
    if(ui->sin->isChecked()) {
        paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
        paint.drawPath(p[0]);
    }
    if(ui->cos->isChecked()) {
        paint.setPen(QPen(Qt::red,1,Qt::SolidLine));
        paint.drawPath(p[1]);
    }
    if(ui->tg->isChecked()) {
        paint.setPen(QPen(Qt::darkMagenta,1,Qt::SolidLine));
        paint.drawPath(p[2]);
    }
    if(ui->Bessel->isChecked())
    {
        paint.setPen(QPen(Qt::black,1,Qt::SolidLine));
        paint.drawPath(p[3]);
    }
    if(ui->PROGA34->isChecked())
    {
        paint.setPen(QPen(Qt::darkMagenta,1,Qt::SolidLine));
        paint.drawPath(p[4]);

    }
    paint.end();
    ui->outputGraph->setPixmap(graph);
    return;
}



void MainWindow::RotateDrawGraph(bool notEmpty)
{
    QPixmap graph(600,600);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0,0,600,600);

    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight);
    paint.drawLine(0,Oy*onePixelY,pictWidth,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,5));
    for(double i = leftX;i<=rightX;i+=10.0)
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
    for(double i = leftY;i<=rightY;i+=10.0)
       paint.drawPoint(Ox*onePixelX,(i+Oy)*onePixelY);

    paint.translate(Ox*onePixelX,Oy*onePixelY);
   paint.rotate(90);
   paint.scale(scale, scale);
    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path,p[3];
    bool first[4] = {1,1,1,1};

    for(double i = (double)leftX+step;i<=(double)rightX;i+=step) {
        if(!isnan(f(i))) {
            if(first[0]) {
                path.moveTo((i)*onePixelX,(-f(i))*onePixelY);
                first[0] = false;
            }
            else
                path.lineTo((i)*onePixelX,(-f(i))*onePixelY);
        }
        if(!isnan(f1(i))) {
            if(first[1]) {
                p[0].moveTo((i)*onePixelX,(-f1(i))*onePixelY);
                first[1] = false;
            }
            else
                p[0].lineTo((i)*onePixelX,(-f1(i))*onePixelY);
        }
        if(!isnan(f2(i))) {
            if(first[2]) {
                p[1].moveTo((i)*onePixelX,(-f2(i))*onePixelY);
                first[2] = false;
            }
            else
                p[1].lineTo((i)*onePixelX,(-f2(i))*onePixelY);
        }
        if(!isnan(f3(i))) {
            if(first[3]) {
                p[2].moveTo((i)*onePixelX,(-f3(i))*onePixelY);
                first[3] = false;
            }
            else
                p[2].lineTo((i)*onePixelX,(-f3(i))*onePixelY);
        }
    }
    if(ui->main->isChecked()) {
        paint.setPen(QPen(Qt::blue,1,Qt::SolidLine));
        paint.drawPath(path);
    }
    if(ui->sin->isChecked()) {
        paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
        paint.drawPath(p[0]);
    }
    if(ui->cos->isChecked()) {
        paint.setPen(QPen(Qt::red,1,Qt::SolidLine));
        paint.drawPath(p[1]);
    }
    if(ui->tg->isChecked()) {
        paint.setPen(QPen(Qt::darkMagenta,1,Qt::SolidLine));
        paint.drawPath(p[2]);
    }
    paint.end();
    ui->outputGraph->setPixmap(graph);
    return;
}




void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_clear_clicked()
{
    recountPixels();
    drawGraph();
}

void MainWindow::on_draw_clicked()
{
    getData();
    recountPixels();
    if(ui->InputDelta->text().isEmpty())
        drawGraph(1, solutions_vector, n);
    else if(!ui->InputDelta->text().isEmpty())
        Delted_drawGraph(1, solutions_vector, n, delta);
}

void MainWindow::on_pushButton_clicked()
{
    getData();
    recountPixels();
    RotateDrawGraph(1);
}




void MainWindow::on_radioButton_clicked() //Н
{
    ifstream fin;
    fin.open("t.txt");
    fin >> NumberOfPoints;
    points = new double[NumberOfPoints];
    values = new double[NumberOfPoints];
    support_values = new double[NumberOfPoints];
    support_points = new double[NumberOfPoints];
    D = new double[NumberOfPoints];
    coefficients_vector   = new double[4];

    D_34 = new double[NumberOfPoints];
    Newton_result_vector = new double[2];
    Newton_support_values = new double[NumberOfPoints];
    Newton_support_points = new double[NumberOfPoints];
    coefficients_vector_34 = new double[4];
    for(int i =0; i < NumberOfPoints; i++)
    {
        fin >> points[i];
        fin >> values[i];
        support_values[i] = values[i];
        support_points[i] = points[i];
    }
    solutions_vector = new double[NumberOfPoints+1];
    for(int i = 0; i < NumberOfPoints; i++)
        solutions_vector[i] = 0;
}

void MainWindow::on_radioButton_2_clicked()
{
   leftX = ui->inputLeftX->text().toDouble();
   rightX = ui->inputRightX->text().toDouble();
   leftY = ui->inputLeftY->text().toDouble();
   rightY = ui->inputRightY->text().toDouble();
   NumberOfPoints = ui->inputNumber->text().toDouble();
   if(!ui->InputDelta->text().isEmpty())
   {
       delta=ui->InputDelta->text().toDouble();
       double shortCut = fabs(rightX-leftX)/NumberOfPoints;
       delete[] points;
       delete[] values;
       delete[] support_values;
       delete[] support_points;
       delete[] D;
       delete[] coefficients_vector;

       delete[] D_34;
       delete[] coefficients_vector_34;
       delete[] Newton_result_vector;
       delete[] Newton_support_values;
       delete[] Newton_support_points;
     points = new double[NumberOfPoints];
     values = new double[NumberOfPoints];
     support_values = new double[NumberOfPoints];
     support_points = new double[NumberOfPoints];
     D = new double[NumberOfPoints];
     coefficients_vector   = new double[4];

     D_34 = new double[NumberOfPoints];
     Newton_result_vector = new double[2];
     Newton_support_values = new double[NumberOfPoints];
     Newton_support_points = new double[NumberOfPoints];
     coefficients_vector_34 = new double[4];
     for(int i =0; i< NumberOfPoints; i++)
     {
        points[i] = leftX + shortCut*i;
        values[i] = f1(points[i])+deltedFunc(points[i], delta);
        support_values[i] = values[i];
        support_points[i] = points[i];
     }
     solutions_vector = new double[NumberOfPoints+1];
     for(int i = 0; i < NumberOfPoints; i++)
         solutions_vector[i] = 0;
   }

   else
   {
       delete[] points;
       delete[] values;
       delete[] support_values;
       delete[] support_points;
       delete[] D;
       delete[] coefficients_vector;

       delete[] D_34;
       delete[] coefficients_vector_34;
       delete[] Newton_result_vector;
       delete[] Newton_support_values;
       delete[] Newton_support_points;
       double shortCut = fabs(rightX-leftX)/NumberOfPoints;
     points = new double[NumberOfPoints];
     values = new double[NumberOfPoints];
     support_values = new double[NumberOfPoints];
     support_points = new double[NumberOfPoints];
     D = new double[NumberOfPoints];
     coefficients_vector   = new double[4];

     D_34 = new double[NumberOfPoints];
     Newton_result_vector = new double[2];
     Newton_support_values = new double[NumberOfPoints];
     Newton_support_points = new double[NumberOfPoints];
     coefficients_vector_34 = new double[4];
     for(int i =0; i< NumberOfPoints; i++)
     {
        points[i] = leftX + shortCut*i;
        values[i] = f1(points[i]);
        support_values[i] = values[i];
        support_points[i] = points[i];
     }
     solutions_vector = new double[NumberOfPoints+1];
     for(int i = 0; i < NumberOfPoints; i++)
         solutions_vector[i] = 0;


   }

}

void MainWindow::on_radioButton_3_clicked()
{
   leftX = ui->inputLeftX->text().toDouble();
   rightX = ui->inputRightX->text().toDouble();
   leftY = ui->inputLeftY->text().toDouble();
   rightY = ui->inputRightY->text().toDouble();
   NumberOfPoints = ui->inputNumber->text().toDouble();
   delete[] points;
   delete[] values;
   delete[] support_values;
   delete[] support_points;
   delete[] D;
   delete[] coefficients_vector;
   delete[] D_34;
   delete[] coefficients_vector_34;
   delete[] Newton_result_vector;
   delete[] Newton_support_values;
   delete[] Newton_support_points;
   if(!ui->InputDelta->text().isEmpty())
   {
       delta=ui->InputDelta->text().toDouble();
       double shortCut = fabs(rightX-leftX)/NumberOfPoints;
     points = new double[NumberOfPoints];
     values = new double[NumberOfPoints];
     support_values = new double[NumberOfPoints];
     support_points = new double[NumberOfPoints];
     D = new double[NumberOfPoints];
     coefficients_vector   = new double[4];

     D_34 = new double[NumberOfPoints];
     Newton_result_vector = new double[2];
     Newton_support_values = new double[NumberOfPoints];
     Newton_support_points = new double[NumberOfPoints];
     coefficients_vector_34 = new double[4];

     for(int i =0; i< NumberOfPoints; i++)
     {
        points[i] = leftX + shortCut*i;
        values[i] = f2(points[i]) +deltedFunc(points[i], delta);
        support_values[i] = values[i];
        support_points[i] = points[i];
     }
     solutions_vector = new double[NumberOfPoints+1];
     for(int i = 0; i < NumberOfPoints; i++)
         solutions_vector[i] = 0;
   }

   else
   {
       double shortCut = fabs(rightX-leftX)/NumberOfPoints;
     points = new double[NumberOfPoints];
     values = new double[NumberOfPoints];
     support_values = new double[NumberOfPoints];
     support_points = new double[NumberOfPoints];
     D = new double[NumberOfPoints];
     coefficients_vector   = new double[4];

     D_34 = new double[NumberOfPoints];
     Newton_result_vector = new double[2];
     Newton_support_values = new double[NumberOfPoints];
     Newton_support_points = new double[NumberOfPoints];
     coefficients_vector_34 = new double[4];
     for(int i =0; i< NumberOfPoints; i++)
     {
        points[i] = leftX + shortCut*i;
        values[i] = f2(points[i]);
        support_values[i] = values[i];
        support_points[i] = points[i];
     }
     solutions_vector = new double[NumberOfPoints+1];
     for(int i = 0; i < NumberOfPoints; i++)
         solutions_vector[i] = 0;


   }

}


void MainWindow::onTextChanged()
{
    if(ui->inputLeftX->text().isEmpty() ||
            ui->inputRightX->text().isEmpty() ||
            ui->inputLeftY->text().isEmpty() ||
            ui->inputRightY->text().isEmpty() ||
            ui->inputAccuracy->text().isEmpty() ||
            ui->inputScale->text().isEmpty() ||
            ui->DerDeg->text().isEmpty() ||
            ui->inputNumber->text().isEmpty()
)
    {
        ui->draw->setEnabled(false);
        ui->radioButton->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);

       }
    else if (ui->inputRightX->text().toDouble() < ui->inputLeftX->text().toDouble() ||
             ui->inputRightY->text().toDouble() < ui->inputLeftY->text().toDouble() ||
             ui->inputAccuracy->text().toDouble() <= 0 ||
             ui->inputScale->text().toDouble() <= 0 ||
             ui->inputNumber->text().toDouble() <= 0 ||
             ui->DerDeg->text().toInt() <= -1)
    {
        ui->draw->setEnabled(false);
        ui->radioButton->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
    }
    else
    {

        ui->radioButton->setEnabled(true);
        ui->radioButton_2->setEnabled(true);
        ui->radioButton_3->setEnabled(true);

        if(ui->radioButton->isChecked() || ui->radioButton_2->isChecked() || ui->radioButton_3->isChecked())
            ui->draw->setEnabled(true);
    }
}

void MainWindow::RadioClick()
{
    if(ui->radioButton->isChecked() || ui->radioButton_2->isChecked() || ui->radioButton_3->isChecked())
        ui->draw->setEnabled(true);
}
