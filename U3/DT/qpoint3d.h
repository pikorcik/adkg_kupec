#ifndef QPOINT3D_H
#define QPOINT3D_H

#include <QtGui>

class QPoint3D : public QPointF
{

private:
    double z = 0;

public:
    QPoint3D(double x, double y, double z_):QPointF(x, y), z(z_){}
    double getZ() {return z;}
    void setZ(double z_) {z = z_;}

};

#endif // QPOINT3D_H
