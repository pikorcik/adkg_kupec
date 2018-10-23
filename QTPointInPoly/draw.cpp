#include "draw.h"
#include <QtGui>
#include <vector>
using namespace std;

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    loadPolygon(); //konstruktor na predani cesty k souboru???

    //Set the initial values
    draw_point = false; //constructor
    q.setX(-5); // set the initial point of the canvas resulting in removing the ugly fragments of point in the corner
    q.setY(-5);
}

void Draw::loadPolygon()
{
    //Open polygon file
    ifstream input_data; //promenna, kam se nahraji polygony a jejich souradnice
    input_data.open("..\\polygon.txt");  //relativni cesta k souboru

    //Check whether file is open
    if (!input_data.is_open())
    {
        input_data.close();
        qDebug() << "Unable to open file.";
    }

    //Number of polygons
    int poly_count;
    input_data >> poly_count;

    //Check whether number of polygons is > 0
    if(poly_count < 1)
    {
        input_data.close();
        qDebug() << "No polygon in the file.";
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
                qDebug() << "Invalid polygon detected!";
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

    //Insert last polygon into list
    poly_list.push_back(one_poly);

    one_poly.clear();

    qDebug() << "finalni vector" << poly_list;

    input_data.close();
    qDebug() << "File successfully open.";
}

void Draw::paintEvent(QPaintEvent *e)
{
    //Draw the polygon and the point
    QPainter painter(this);                         //this ukazuje na ten volající objekt?

    //Start drawing
    painter.begin(this);

    //Create the polygon
    QPolygon p;

    //Add points to the polygon
    for(int i=0; i<pol.size(); i++)
        p.append(pol[i]);

    //Draw the polygon
    painter.drawPolygon(p);

    //Draw the points
    for(int i=0; i<pol.size(); i++)
        painter.drawEllipse(pol[i].x()-5, pol[i].y()-5, 10, 10);

    //Draw q
    painter.drawEllipse(q.x()-2.5, q.y()-2.5, 5, 5);

    //Stop drawing
    painter.end();
}


void Draw::mousePressEvent(QMouseEvent *e)
{
    //Set point q
    if(draw_point)
    {
        q.setX(e->x());
        q.setY(e->y());
    }

    //Add point to the polygon
    else
    {
        QPoint point_click(e->x(), e->y());
        pol.push_back(point_click);
    }

    //Repaint the screen
    repaint();

}


void Draw::clearCanvas()
{
    //Clear the canvas
    pol.clear();
    q.setX(-5);
    q.setY(-5);

    repaint();
}


void Draw::setDrawPoint()
{
    //Switch, whether to draw a point or a polygon
    draw_point = !draw_point;

}
