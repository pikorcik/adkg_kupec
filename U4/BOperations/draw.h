#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>
#include "qpointfb.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPointFB> polA;
    std::vector<QPointFB> polB;
    std::vector<std::vector<QPointFB> > results;
    bool ab;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void drawPolygon(std::vector<QPointFB> &pol, QPainter &painter);
    void setAB() {ab = !ab;}
    void clearAll();

signals:

public slots:
};

#endif // DRAW_H
