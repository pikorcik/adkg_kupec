#include "draw.h"
#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    draw_point = false;
    q.setX(-5);
    q.setY(-5);

}

void Draw::paintEvent(QPaintEvent *e)
{
    //Draw polygons and point
    QPainter painter(this);                         //this ukazuje na ten volající objekt?


    //Start drawing
    painter.begin(this);


    //Create polygon
    QPolygon p;

    //Add points to the polygon
    for(int i=0; i<pol.size(); i++)
        p.append(pol[i]);

    //Draw polygon
    painter.drawPolygon(p);

    //Draw points
    for(int i=0; i<pol.size(); i++)
        painter.drawEllipse(pol[i].x()-5, pol[i].y()-5, 10, 10);

    //Draw q
    painter.drawEllipse(q.x()-2.5, q.y()-2.5, 5, 5);

    painter.end();
}



void Draw::mousePressEvent(QMouseEvent *e)
{
    //
    if(draw_point)
    {
        q.setX(e->x());
        q.setY(e->y());
    }
    else
    {
        QPoint point_click(e->x(), e->y());
        pol.push_back(point_click);
    }
    repaint();

}


void Draw::clearCanvas()
{
    pol.clear();
    q.setX(-5);
    q.setY(-5);
    repaint();
}


void Draw::setDrawPoint()
{
    draw_point = !draw_point;

}

















