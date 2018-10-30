#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}
void Draw::paintEvent(QPaintEvent *e)
{
   QPainter painter(this);
   painter.begin(this);
   painter.setPen(Qt::red);

   //Draw convex hull
   painter.drawPolygon(ch);

   //Draw points
   for(int i = 0; i < points.size(); i++)
   {
       painter.drawEllipse(points[i].x()-5, points[i].y()-5, 10, 10);
   }


   painter.end();


}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPoint p(e->x(), e->y());

    points.push_back(p);

    repaint();

}
