#include <ctime>
#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QPolygon ch;
    std::vector<QPoint> points = ui->Canvas->getPoints();

    //Start time
    clock_t s = std::clock();

    //Construct convex hull
    if(ui->comboBox->currentIndex()==0)
        ch = Algorithms::CHJarvis(points);
    else if(ui->comboBox->currentIndex()==1)
        ch = Algorithms::QHull(points);
    else
        ch = Algorithms::CHSweepLine(points);

    //End time
    clock_t e = std::clock();

    //Time difference
    clock_t time = float (e-s)/CLOCKS_PER_SEC;

    //Write time
    ui->time_label->setText(QString::number(time));

    //Set and repaint
    ui->Canvas->setCH(ch);
    repaint();
}

void Widget::on_clear_button_clicked()
{
    //Clear canvas
    ui->Canvas->clear();
    repaint();
}