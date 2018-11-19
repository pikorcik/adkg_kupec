#include "widget.h"
#include "ui_widget.h"

#include <vector>
#include <fstream>
#include <QtGui>


//#include "edge.h"
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

void Widget::on_delaunay_button_clicked()
{
    std::vector<QPoint> points = ui->Canvas->getPoints();

    std::ofstream f ("test.txt");
    for(QPoint p: points)
    {
        f << p.x() << "  " << p.y() <<'\n';
    }
    f.close();

    std::vector<Edge> dt = Algorithms::delaunayTriangulation(points);
    ui->Canvas->setDT(dt);
    repaint();
}

void Widget::on_dmt_button_clicked()
{

}

void Widget::on_clear_button_clicked()
{
    ui->Canvas->clearDT();
    repaint();
}
