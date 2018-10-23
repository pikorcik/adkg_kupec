#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>
#include <iostream>
#include <QDebug>
#include <vector>


class Draw : public QWidget
{
    Q_OBJECT

private:
    bool draw_point;                                //Switch, whether to draw a point or a polygon
    QPoint q;                                       //The analyzed point q
    std::vector<std::vector<QPoint>> poly_list;     //Vector of polygons
    std::vector<QPoint> pol;                        //pozdeji zahodit

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void clearCanvas();
    void setDrawPoint();
    QString loadPolygon(const char* path);
    QPoint getPoint() {return q;}
    std::vector<QPoint> getPolygon() {return pol;}

signals:

public slots:
};

#endif // DRAW_H
