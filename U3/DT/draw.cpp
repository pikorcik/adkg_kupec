#include "draw.h"

#include <QtGui>
#include <ctime>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    srand((unsigned)time(0));
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.begin(this);
    painter.setPen(Qt::red);

    //Draw points
    for(int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - 5, points[i].y() - 5, 10, 10);
        painter.drawText(points[i].x() + 10, points[i].y() + 10, QString::number(points[i].getZ()));
    }

    //Draw Delaunay edges
    for(int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getS(), dt[i].getE());
    }

    //Draw contour lines
    painter.setPen(Qt::green);

    for(int i = 0; i < contours.size(); i++)
    {
        painter.drawLine(contours[i].getS(), contours[i].getE());
    }

    //Draw slope
    double c = 255.0/180;
    for(int i = 0; i < dtm.size(); i++)
    {
        //Get triangle and its vertices
        Triangle t = dtm[i];
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get slope and set the brush
        int c_slope = c * t.getSlope();
        painter.setBrush(QColor(c_slope, c_slope, c_slope));

        //Create polygon
        QPolygon triangle;
        triangle.append(QPoint(p1.x(), p1.y()));
        triangle.append(QPoint(p2.x(), p2.y()));
        triangle.append(QPoint(p3.x(), p3.y()));

        //Draw polygon
        painter.drawPolygon(triangle);
    }

    painter.end();
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    //Add new point
    int z = (rand()%101);
    QPoint3D p(e->x(), e->y(), z);
    points.push_back(p);

    repaint();
}

void Draw::clearDT()
{
    //Clear all
    points.clear();
    dt.clear();
    dtm.clear();
    contours.clear();
}

