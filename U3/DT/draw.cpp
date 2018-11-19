#include "draw.h"

#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{

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
        painter.drawText(points[i].x() + 10, points[i].y() + 10, QString::number(i));
    }

    //Draw Delaunay edges
    for(int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getS(), dt[i].getE());
    }

    painter.end();
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPoint p(e->x(), e->y());

    points.push_back(p);

    repaint();

}
