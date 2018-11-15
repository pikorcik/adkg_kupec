#ifndef EDGE_H
#define EDGE_H
#include <QPoint>


class Edge
{
private:
    QPoint s, e;
public:
    Edge(QPoint &start, QPoint &end):s(start),e(end){}
    QPoint getS() const {return s;}
    QPoint getE() const {return e;}
    void switchOrientation()
    {
        QPoint temp = s;
        s = e;
        e = temp;
    }

    bool operator == (const Edge &edge) const
    {
        return (s == edge.getS() && e == edge.getE());
    }

};

#endif // EDGE_H
