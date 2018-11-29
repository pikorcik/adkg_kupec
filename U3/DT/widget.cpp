#include "widget.h"
#include "ui_widget.h"

#include <vector>
#include <fstream>
#include <QtGui>
#include "qpoint3d.h"
#include "triangle.h"


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
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    std::ofstream f ("test.txt");
    for(QPoint3D p: points)
    {
        f << p.x() << "  " << p.y() <<'\n';
    }
    f.close();

    std::vector<Edge> dt = Algorithms::delaunayTriangulation(points);
    ui->Canvas->setDT(dt);
    repaint();
}


void Widget::on_clear_button_clicked()
{
    ui->Canvas->clearDT();
    repaint();
}

void Widget::on_contours_button_clicked()
{
    //Create contour lines
    std::vector<Edge> dt = ui->Canvas->getDT();
    std::vector<Edge> contours = Algorithms::createContours(dt, 0, 100, 5);
    ui->Canvas->setContours(contours);

    repaint();
}

void Widget::on_dtm_button_clicked()
{
    //Analyze slope and aspect
    std::vector<Edge> dt = ui->Canvas->getDT();
    std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);

    ui->Canvas->setDTM(dtm);

    repaint();
}
