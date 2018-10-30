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
    std::vector<QPoint> points = ui->Canvas->getPoints();
    QPolygon ch = Algorithms::CHJarvis(points);
    ui->Canvas->setCH(ch);
    repaint();
}
