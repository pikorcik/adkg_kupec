#include <ctime>
#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->points_spinBox->setRange(1000, 1000000);
    ui->points_spinBox->setValue(1000);
    ui->points_spinBox->setSingleStep(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_ch_button_clicked()
{
    //Clear canvas
    ui->Canvas->clearAll();

    QPolygon ch;

    //Get chosen set parameters
    int shape_index = ui->shape_comboBox->currentIndex();
    int num_of_points = ui->points_spinBox->value();

    //Get size of canvas
    int canvas_width = ui->Canvas->width()-5;
    int canvas_height = ui->Canvas->height()-5;

    //Generate set of points
    std::vector<QPoint> points = ui->Canvas->generateSet(shape_index, num_of_points, canvas_width, canvas_height);

    //Start time
    clock_t s = std::clock();

    //Construct convex hull
    if(ui->method_comboBox->currentIndex()==0)
        ch = Algorithms::CHJarvis(points);
    else if(ui->method_comboBox->currentIndex()==1)
        ch = Algorithms::QHull(points);
    else
        ch = Algorithms::CHSweepLine(points);

    //End time
    clock_t e = std::clock();

    //Time difference in miliseconds
    clock_t time = e-s;
    ui->time_label->setText(QString::number(time) + " ms");

    //Set and repaint
    ui->Canvas->setCH(ch);
    repaint();
}

void Widget::on_clear_button_clicked()
{
    //Clear canvas
    ui->Canvas->clearAll();
    repaint();
}
