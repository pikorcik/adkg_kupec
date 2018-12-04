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

    qDebug() << "No of points: " << points.size();

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
    painter.setPen(Qt::red);

    //Draw points
    for(int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - 2.5, points[i].y() - 2.55, 5, 5);
        //painter.drawText(points[i].x() + 10, points[i].y() + 10, QString::number(points[i].getZ()));
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

void Draw::clearDT()
{
    //Clear all
    points.clear();
    dt.clear();
    dtm.clear();
    contours.clear();
}

