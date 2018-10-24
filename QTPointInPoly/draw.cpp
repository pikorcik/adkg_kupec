#include "draw.h"
#include <QtGui>
#include <vector>
using namespace std;

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //Set the initial point of the canvas resulting in removing the ugly fragments of point in the corner
    q.setX(-5);
    q.setY(-5);
}

QString Draw::loadPolygon(const char* path)
{
    //Open polygon file
    ifstream input_data;
    input_data.open(path);

    //Check whether file is open
    if (!input_data.is_open())
    {
        input_data.close();
        return "Unable to open file.";
    }

    //Number of polygons
    int poly_count;
    input_data >> poly_count;

    //Check whether number of polygons is > 0
    if(poly_count < 1)
    {
        input_data.close();
        return "No polygon in the file.";
    }

    //Declare variables
    int num_poly = 1;
    int num_poly_current;
    int num_points = 0;
    double x, y;

    //Declare vector for points in one polygon
    std::vector<QPoint> one_poly;

    while(!input_data.eof())
    {
        input_data >> num_poly_current;
        input_data >> x;
        input_data >> y;

        //Insert point into polygon
        if(num_poly_current == num_poly)
        {
            one_poly.push_back(QPoint(x, y));
            num_points++;
        }

        //Insert polygon into list of polygons and go to another
        else
        {
            //Check for invalid polygons
            if(num_points < 3)
            {
                input_data.close();
                return "Invalid polygon detected!";
            }

            //Insert polygon into list
            poly_list.push_back(one_poly);

            one_poly.clear();

            //Add point to new polygon
            one_poly.push_back(QPoint(x, y));
            num_points = 1;
            num_poly++;
        }
    }

    //Insert last polygon into the list
    poly_list.push_back(one_poly);

    one_poly.clear();

    input_data.close();
    return "File successfully open.";
}

void Draw::paintEvent(QPaintEvent *e)
{
    //Draw the polygon and the point
    QPainter painter(this);

    //Define how lines and outlines of shapes should be painted
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    //Draw polygons from file
    for(int i = 0; i < poly_list.size(); i++)
    {
        //Create the polygon
        QPolygon p;

        //Get one polygon from file
        vector<QPoint> one_poly = poly_list[i];

        //Add points to the polygon
        for(int j = 0; j < one_poly.size(); j++)
        {
            p.append(one_poly[j]);
        }

        //Draw the polygon
        painter.drawPolygon(p);
    }

    //Draw q
    painter.drawEllipse(q.x()-2.5, q.y()-2.5, 5, 5);

    //Stop drawing
    painter.end();
}


void Draw::mousePressEvent(QMouseEvent *e)
{
    //Set point q
    q.setX(e->x());
    q.setY(e->y());

    //Repaint the screen
    repaint();
}


void Draw::clearCanvas()
{
    //Clear the canvas
    poly_list.clear();
    q.setX(-5);
    q.setY(-5);

    repaint();
}
