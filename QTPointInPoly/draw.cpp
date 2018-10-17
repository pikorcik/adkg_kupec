#include "draw.h"
#include <QtGui>
using namespace std;

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    loadPolygon(); //konstruktor na predani cesty k souboru???
    draw_point = false; //constructor
    q.setX(-5); // set the initial point of the canvas resulting in removing the ugly fragments of point in the corner
    q.setY(-5);
}

void Draw::loadPolygon(){
    ifstream data; //promenna, kam se nahraji polygony a jejich souradnice
    data.open("..\\polygon.txt");  //relativni cesta k souboru


    if (!data.is_open()) {
        qDebug() << "Unable to open file polygon.txt";
    }

    int vypis;
    while(!data.eof()){
        data >> vypis;
        qDebug() << vypis; //na konci souboru vypisuje o jedno cislo navic
    }

    data.close();

    QPolygon poly; // polygon, do ktereho budeme davat body polygonu
    //int numOfPoly = vypis[0]; // nemuzu najit, jak dostat prvni element z file --> nefunguje
    //qDebug() << numOfPoly;

   /* for (int i = 0; i < numOfPoly; i++) {

    }*/
    //mineno pro nacitani jednotlivych polygonu
    //pri vykreslovani si ohlidat si pocet bodu a skoncit cyklus, az budeme mit vsechny polygony

}

void Draw::paintEvent(QPaintEvent *e)
{
    //Draw polygons and point
    QPainter painter(this);                         //this ukazuje na ten volající objekt?


    //Start drawing
    painter.begin(this);

    //Create polygon
    QPolygon p;

    //Add points to the polygon
    for(int i=0; i<pol.size(); i++)
        p.append(pol[i]);

    //Draw polygon
    painter.drawPolygon(p);

    //Draw points
    for(int i=0; i<pol.size(); i++)
        painter.drawEllipse(pol[i].x()-5, pol[i].y()-5, 10, 10);

    //Draw q
    painter.drawEllipse(q.x()-2.5, q.y()-2.5, 5, 5);

    painter.end();
}



void Draw::mousePressEvent(QMouseEvent *e)
{
    //
    if(draw_point)
    {
        q.setX(e->x());
        q.setY(e->y());
    }
    else
    {
        QPoint point_click(e->x(), e->y());
        pol.push_back(point_click);
    }

    repaint();

}


void Draw::clearCanvas()
{
    //Delete polygon a clean canvas
    pol.clear();
    q.setX(-5);
    q.setY(-5);
    repaint();
}


void Draw::setDrawPoint()
{
    //switching between drawing point and polygon
    draw_point = !draw_point;

}
