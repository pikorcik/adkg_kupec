#include "draw.h"
#include <fstream>
using namespace std;

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
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

    //Declare variables
    double num_of_points;
    double x, y;

    while(!input_data.eof())
    {
        input_data >> num_of_points;

        //Check for invalid polygons
        if(num_of_points < 3)
        {
            polA.clear();
            polB.clear();
            input_data.close();
            return "Invalid polygon detected!";
        }

        //Insert point into polygon
        for(int i = 0; i < num_of_points; i++)
        {
            input_data >> x;
            input_data >> y;
            QPointFB p(x, y);

            //Add to polA
            if(ab)
            {
                polA.push_back(p);
            }

            //Add to polB
            else
            {
                polB.push_back(p);
            }
        }

        setAB();
    }

    input_data.close();

    repaint();

    return "File successfully open.";
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter (this);

    painter.begin(this);
    QPen penA(Qt::green, 1);
    QPen penB(Qt::blue, 1);
    QPen pen_res(Qt::red, 2);

    //Draw polygon A
    painter.setPen(penA);
    drawPol(polA, painter);

    //Draw polygon B
    painter.setPen(penB);
    drawPol(polB, painter);

    //Draw result
    painter.setPen(pen_res);
    for(std::vector<QPointFB> vec: res)
    {
        drawPol(vec, painter);
    }

    //Draw offset
    painter.setPen(Qt::cyan);
    for(std::vector<QPointFB> b: buff)
    {
        drawPol(b, painter);
    }
}

void Draw::drawPol(std::vector<QPointFB> &pol, QPainter &painter)
{
    QPolygon pol_q;

    for(QPointFB p: pol)
    {
        QPoint pb (p.x(), p.y());
        pol_q.append(pb);
    }

    painter.drawPolygon(pol_q);
}

void Draw::clearAll()
{
    //Clear all in canvas
    polA.clear();
    polB.clear();
    res.clear();
}

void Draw::clearResults()
{
    //Clear results in canvas
    res.clear();
}
