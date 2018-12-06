#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.begin(this);

    //Draw polygon A
    painter.setPen(Qt::green);
    drawPolygon(polA, painter);

    //Draw polygon B
    painter.setPen(Qt::blue);
    drawPolygon(polB, painter);

    //Draw result
    painter.setPen(Qt::red);
    for(std::vector<QPointFB> vec: results)
    {
        drawPolygon(vec, painter);
    }
}

void Draw::drawPolygon(std::vector<QPointFB> &pol, QPainter &painter)
{
    //Draw polygon
    QPolygonF pol_q;

    for(QPointFB p: pol)
    {
        QPointFB pb(p.x(), p.y());
        pol_q.append(pb);
    }
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPointFB p(e->x(), e->y());

    //Add to polygon A
    if(ab)
        polA.push_back(p);

    //Add to polygon B
    else
        polB.push_back(p);

    repaint();
}

void Draw::clearAll()
{
    //Clear all data structures
    polA.clear();
    polB.clear();
    results.clear();
}
