#include "draw.h"

#include <QtGui>
using namespace std;

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

QString Draw::loadDTM(const char* path)
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

    //Number of points
    int point_count;
    input_data >> point_count;
    qDebug() << "No of points written in file: " << point_count;

    //Check whether number of points is > 0
    if(point_count < 1)
    {
        input_data.close();
        return "No points in the file.";
    }

    //Declare variables
    double x, y, z;

    while(!input_data.eof())
    {
        input_data >> x;
        input_data >> y;
        input_data >> z;

        points.push_back(QPoint3D(x, y, z));
    }

    input_data.close();
    qDebug() << "No of points: " << points.size();

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

void Draw::clearDT()
{
    //Clear all
    points.clear();
    dt.clear();
    dtm.clear();
    contours.clear();
}

