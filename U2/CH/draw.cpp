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
    //Generate set of chosen shape
    //Proportions of set according to the size of canvas

    //Generate random set
    if(shape_index == 0)
    {
        for (int i = 0; i < num_of_points; i++)
        {
            int x =  std::rand() % canvas_width + 5;
            int y =  std::rand() % canvas_height + 5;
            points.push_back(QPoint(x, y));
        }
    }

    //Generate raster set
    else if(shape_index == 1)
    {
        //Number of points on one raster edge
        double edge = ceil(sqrt(num_of_points));

        //Step between point coordinates
        double x_size = canvas_width/edge;
        double y_size = canvas_height/edge;

        //Coordinate variables
        int x;
        int y;

        //Number of points to be added to raster
        int num = num_of_points;

        //Raster rows
        for(int i = 0; i < edge; i++)
        {
            y = i*y_size;

            //Raster columns
            for(int j = 0; j < edge; j++)
            {
                x = j*x_size;

                points.push_back(QPoint(x, y));
            }
        }
    }

    //Generate points in circle
    else
    {

    }

    return points;
}

void Draw::clearAll()
{
    points.clear();
    ch.clear();
}
