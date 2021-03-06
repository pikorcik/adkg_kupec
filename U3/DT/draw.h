#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>
#include <iostream>
#include <QDebug>

#include "edge.h"
#include "qpoint3d.h"
#include "triangle.h"

class Draw : public QWidget
{
    Q_OBJECT
    private:
        std::vector<QPoint3D> points;     //List of points
        std::vector<Edge> dt;             //List of Delaunay edges
        std::vector<Edge> contours;       //List of contours
        std::vector<double> z_contours;   //List of contours heights
        int dz;                           //Step of contours
        std::vector<Triangle> dtm;        //List of triangles
        bool flag_slope;
        bool flag_aspect;

    public:
        explicit Draw(QWidget *parent = nullptr);
        void paintEvent(QPaintEvent *e);
        void clearDT();
        std::vector<QPoint3D> & getPoints(){return points;}
        std::vector<Edge> & getDT(){return dt;}
        void setDT(std::vector<Edge> &dt_){dt = dt_;}
        void setContours(std::vector<Edge> &contours_, std::vector<double> &z_contours_, double dz_);
        void setDTM(std::vector<Triangle> &dtm_){dtm = dtm_;}
        QString loadDTM(const char* path, int width, int height, double &z_min, double &z_max);
        void drawSlope();
        void drawAspect();

    signals:

    public slots:

};

#endif // DRAW_H
