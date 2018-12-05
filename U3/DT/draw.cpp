#include "draw.h"

#include <QtGui>
using namespace std;

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

QString Draw::loadDTM(const char* path, int width, int height, double &z_min, double &z_max)
{
    //Open DTM file
    ifstream input_data;
    input_data.open(path);

    //Check whether file is open
    if (!input_data.is_open())
    {
        input_data.close();
        return "Unable to open file.";
    }

    //Declare variables
    double x, y, z;
    double x_min = std::numeric_limits<double>::max();
    double x_max = std::numeric_limits<double>::min();
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::min();
    z_min = std::numeric_limits<double>::max();
    z_max = std::numeric_limits<double>::min();

    //Get all points
    while(!input_data.eof())
    {
        input_data >> x;
        input_data >> y;
        input_data >> z;
        points.push_back(QPoint3D(x, y, z));

        //Check min and max coordinates
        if(x < x_min)
            x_min = x;
        if(x > x_max)
            x_max = x;
        if(y < y_min)
            y_min = y;
        if(y > y_max)
            y_max = y;
        if(z < z_min)
            z_min = z;
        if(z > z_max)
            z_max = z;
    }

    input_data.close();

    //Check whether number of points is > 0
    if(points.size() < 1)
    {
        return "No points in the file.";
    }

    //Resize data according to canvas size
    double x_resize = width / (x_max - x_min);
    double y_resize = height / (y_max - y_min);
    for(unsigned int i = 0; i < points.size(); i++)
    {
        points[i].setX((points[i].x() - x_min) * x_resize);
        points[i].setY((points[i].y() - y_min) * y_resize);
    }

    return "File successfully open.";
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.begin(this);
    QPen pen_point(Qt::red);
    QPen pen_delaunay(Qt::black);
    QPen pen_contour(Qt::red);


    //Draw points
    painter.setPen(pen_point);
    for(unsigned int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - 1, points[i].y() - 1, 2, 2);
        //painter.drawText(points[i].x() + 10, points[i].y() + 10, QString::number(points[i].getZ()));
    }

    //Draw Delaunay edges
    painter.setPen(pen_delaunay);
    for(unsigned int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getS(), dt[i].getE());
    }

    //Draw contour lines
    painter.setPen(pen_contour);
    for(unsigned int i = 0; i < contours.size(); i++)
    {
        painter.drawLine(contours[i].getS(), contours[i].getE());
    }

    //Draw slope
    painter.setPen(pen_delaunay);
    if(flag_slope)
    {
        double c = 255.0/180;
        for(unsigned int i = 0; i < dtm.size(); i++)
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
    }

    //Draw aspect
    if(flag_aspect)
    {
        for(unsigned int i = 0; i < dtm.size(); i++)
        {
            //Get triangle and its vertices
            Triangle t = dtm[i];
            QPoint3D p1 = t.getP1();
            QPoint3D p2 = t.getP2();
            QPoint3D p3 = t.getP3();

            //Get aspect and set the brush
            int aspect = t.getAspect();
            if(aspect > -180 && aspect <= -157.5)
                painter.setBrush(QColor(192,77,156));
            else if(aspect > -157.5 && aspect <= -112.5)
                painter.setBrush(QColor(231,111,122));
            else if(aspect > -112.5 && aspect <= -67.5)
                painter.setBrush(QColor(226,166,108));
            else if(aspect > -67.5 && aspect <= -22.5)
                painter.setBrush(QColor(214,219,94));
            else if(aspect > -22.5 && aspect <= 22.5)
                painter.setBrush(QColor(141,196,88));
            else if(aspect > 22.5 && aspect <= 67.5)
                painter.setBrush(QColor(61,171,113));
            else if(aspect > 67.5 && aspect <= 112.5)
                painter.setBrush(QColor(80,120,183));
            else if(aspect > 112.5 && aspect <= 157.5)
                painter.setBrush(QColor(119,71,157));
            else if(aspect > 157.5 && aspect <= 180)
                painter.setBrush(QColor(192,77,156));

            //Create polygon
            QPolygon triangle;
            triangle.append(QPoint(p1.x(), p1.y()));
            triangle.append(QPoint(p2.x(), p2.y()));
            triangle.append(QPoint(p3.x(), p3.y()));

            //Draw polygon
            painter.drawPolygon(triangle);
        }
    }

    painter.end();
}

void Draw::drawSlope()
{
    flag_slope = !flag_slope;
    flag_aspect = false;
}

void Draw::drawAspect()
{
    flag_aspect = !flag_aspect;
    flag_slope =  false;
}
void Draw::clearDT()
{
    //Clear all
    points.clear();
    dt.clear();
    dtm.clear();
    contours.clear();
    flag_slope = false;
    flag_aspect = false;
}

