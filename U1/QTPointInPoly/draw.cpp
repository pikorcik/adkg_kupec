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
                poly_list.clear();
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

    //Check for invalid polygons
    if(num_points < 3)
    {
        input_data.close();
        poly_list.clear();
        return "Invalid polygon detected!";
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

    //Define how lines should be painted
    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    //Draw polygons from file
    for(unsigned i = 0; i < poly_list.size(); i++)
    {
        //Create the polygon
        QPolygon p;

        //Get one polygon from file
        vector<QPoint> one_poly = poly_list[i];

        //Add points to the polygon
        for(unsigned j = 0; j < one_poly.size(); j++)
        {
            p.append(one_poly[j]);
        }

        //Draw the polygon
        painter.drawPolygon(p);
    }

    //Change how lines should be painted
    QPen pen_fill(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen_fill);

    //Define how to fill polygons containing point q
    QBrush brush(Qt::red, Qt::DiagCrossPattern);
    QPainterPath path;

    //Draw and fill polygons containg point q
    for(unsigned i = 0; i < poly_fill.size(); i++)
    {
        //Create the polygon
        QPolygon p2;

        //Get one polygon from list of polygons to fill
        std::vector<QPoint> one_poly2 = poly_fill[i];

        //Add points to the polygon
        for(unsigned j = 0; j < one_poly2.size(); j++)
        {
            p2.append(one_poly2[j]);
        }

        //Draw the polygon
        painter.drawPolygon(p2);

        //Fill the polygon
        path.addPolygon(p2);
        painter.fillPath(path, brush);
        p2.clear();
    }

    //Draw q
    painter.setPen(pen);
    painter.drawEllipse(q.x()-5, q.y()-5, 10, 10);

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
    poly_fill.clear();
    q.setX(-5);
    q.setY(-5);

    repaint();
}


void Draw::fillPolygon(std::vector<std::vector<QPoint>> poly)
{
    //Fill polygon containing point q
    poly_fill = poly;

    repaint();
}
