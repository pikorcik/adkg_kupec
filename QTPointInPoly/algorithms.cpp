#include "algorithms.h"
#include <cmath>

Algorithms::Algorithms()
{


}


int Algorithms::getPositionRay(QPoint q, std::vector <QPoint> pol) {
    int k = 0;
    int n = pol.size();
    double xir = pol[0].x()-q.x();
    double yir = pol[0].y()-q.y();

    for (int i = 1;i<pol.size()+1;i++)
    {
        //Reduce the coordinates
        double xiir = pol[i%n].x()-q.x(); //yiir =yi+1
        double yiir = pol[i%n].y()-q.y();

        //Upper halfplane
        if(((yiir>0)&&(yir<=0))||((yir>0)&&(yiir<=0)))
        {
            //right halfplane
            double xm = (xiir*yir-xir*yiir)/(yiir-yir);
            if(xm > 0);
            k++;
        }
        xir = xiir;
        yir = yiir;
    }
    return k%2;
}

int Algorithms::getPositionWinding(QPoint q, std::vector <QPoint> pol)
{
    double sum_fi = 0;
    double eps = 1.0e-10;

    int n = pol.size();
    double xir = pol[0].x();
    double yir = pol[0].y();

    for (int i = 1;i<pol.size()+1;i++)
    {
        //Reduce the coordinates
        double xiir = pol[i%n].x(); //yiir = yi+1
        double yiir = pol[i%n].y();

        //Compute angle
        double fi = getTwoVectorsAngle(q.x(), q.y(), xir, yir, q.x(), q.y(), xiir, yiir);

        //Position of q
        if(getPointLinePosition(q.x(), q.y(), xir, yir, xiir, yiir)==1)
        {
            sum_fi += fi;
        }
        else
        {
            sum_fi -= fi;
        }

        //Assign coordinates
        xir = xiir;
        yir = yiir;
    }

    //Inside
    if(fabs(sum_fi-360)<eps)
    {
        return 1;
    }

    //Outside
    else if(fabs(sum_fi)<eps)
    {
        return 0;
    }

    //On the boundary
    else return -1;

}


int Algorithms::getPointLinePosition(double xq, double yq, double x1, double y1, double x2, double y2)
{
    double eps = 1.0e-10; //zápis v semilogaritmickém tvaru

    //Get vectors
    double ux = x2-x1;
    double uy = y2-y1;
    double vx = xq-x1;
    double vy = yq-y1;

    //Cross product
    double t = ux*vy-vx*uy;

    //Point on the line
    if(fabs(t)<eps)
    {
        return -1;
    }
    //Point in the left half plane
    if(t>0)
    {
        return 1;
    }
    //Point in the right half plane
    else
    {
        return 0;
    }
}

double Algorithms::getTwoVectorsAngle(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    //Get vectors
    double ux = x2-x1;
    double uy = y2-y1;
    double vx = x4-x3;
    double vy = y4-y3;

    //
    double dot = ux*vx+uy*vy;
    double nu = sqrt(ux*ux+uy*uy);
    double nv = sqrt(vx*vx+vy*vy);

    return acos(dot/(nu*nv))*180/M_PI;
}

