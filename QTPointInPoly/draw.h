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

    bool draw_point;
    QPoint q;
    std::vector<QPoint> pol;
public:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    explicit Draw(QWidget *parent = nullptr);
    void clearCanvas();
    void setDrawPoint();
    void loadPolygon();
    QPoint getQ() {return q;}
    std::vector<QPoint> getPolygon() {return pol;}

signals:

public slots:
};

#endif // DRAW_H
