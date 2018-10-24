#include "algorithms.h"
#include <cmath>

Algorithms::Algorithms()
{

}


int Algorithms::getPositionRay(QPoint q, std::vector<QPoint> pol)
{
    //Analyze point and polygon position
    int k = 0;
    int n = pol.size();

    //Initialize the coordinates
    double xir = pol[0].x()-q.x();
    double yir = pol[0].y()-q.y();

    for(int i = 1;i<n+1;i++)
    {
        //Reduce the coordinates
        double xiir = pol[i%n].x()-q.x(); //yiir =yi+1
        double yiir = pol[i%n].y()-q.y();

        //Upper half-plane?
        if(((yiir>0)&&(yir<=0))||((yir>0)&&(yiir<=0)))
        {
            //Right half-plane?
            double xm = (xiir*yir-xir*yiir)/(yiir-yir);

            //Increment intersection
            if(xm > 0)
                k++;
        }

        //Assign coordinates
        xir = xiir;
        yir = yiir;
    }

    return k%2;
}


int Algorithms::getPositionWinding(QPoint q, std::vector<QPoint> pol)
{
    int n = pol.size();
    double sum_fi = 0.0;
    double eps = 1.0e-3;

    double xir = pol[0].x();
    double yir = pol[0].y();

    for (int i = 1;i<n+1;i++)
    {
        //Reduce the coordinates
        double xiir = pol[i%n].x(); //yiir = yi+1
        double yiir = pol[i%n].y();

        //Get angle
        double fi = getTwoVectorsAngle(q.x(), q.y(), xir, yir, q.x(), q.y(), xiir, yiir);

        //Get position of q
        int t = getPointLinePosition(q.x(), q.y(), xir, yir, xiir, yiir);

        //Point q in the left half-plane
        if(t > 0)
            sum_fi += fi;

        //Point in the right half-plane
        else
            sum_fi -= fi;

        //Assign coordinates
        xir = xiir;
        yir = yiir;
    }

    //Point q inside P
    if (fabs(fabs(sum_fi)-360)< eps)
        return 1;

    //Point q outside P
    if (fabs(sum_fi) < eps)
        return 0;

    //Point q on the boundary
    else
        return -1;

}


int Algorithms::getPointLinePosition(double xq, double yq, double x1, double y1, double x2, double y2)
{
    //Point and line position
    double eps = 1.0e-3;

    //Get vectors
    double ux = x2-x1;
    double uy = y2-y1;
    double vx = xq-x1;
    double vy = yq-y1;

    //Cross product
    double t = ux*vy-vx*uy;

    //Point in the left half-plane
    if(t > eps)
        return 1;

    //Point in the right half-plane
    if(t < -eps)
        return 0;

    //Point on the line
    else
        return -1;
}


double Algorithms::getTwoVectorsAngle(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    //Get vectors
    double ux = x2-x1;
    double uy = y2-y1;
    double vx = x4-x3;
    double vy = y4-y3;

    //Dot product
    double dot = ux*vx+uy*vy;

    //Norms
    double nu = sqrt(ux*ux+uy*uy);
    double nv = sqrt(vx*vx+vy*vy);

    return acos(dot/(nu*nv))*180/M_PI;
}

