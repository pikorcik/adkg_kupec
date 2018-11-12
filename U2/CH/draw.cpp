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
   for(unsigned int i = 0; i < points.size(); i++)
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

std::vector<QPoint> Draw::generateSet(int shape_index, int num_of_points, int canvas_width, int canvas_height)
{
    //Generate random set according to size of canvas
    for (int i = 0; i < num_of_points; i++)
    {
        int x =  std::rand() % canvas_width + 5;
        int y =  std::rand() % canvas_height + 5;
        points.push_back(QPoint(x,	y));
    }

    return points;
}

void Draw::clearAll()
{
    points.clear();
    ch.clear();
}
