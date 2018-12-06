#include "algorithms.h"
#include <map>

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


int Algorithms::getPointLinePosition(QPointFB &q, QPointFB &p1, QPointFB &p2)
{
    //Point and line position
    double eps = 1.0e-3;

    //Get vectors
    double ux = p2.x()-p1.x();
    double uy = p2.y()-p1.y();
    double vx = q.x()-p1.x();
    double vy = q.y()-p1.y();

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


double Algorithms::getTwoVectorsAngle(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4)
{
    //Get vectors
    double ux = p2.x()-p1.x();
    double uy = p2.y()-p1.y();
    double vx = p4.x()-p3.x();
    double vy = p4.y()-p3.y();

    //Dot product
    double dot = ux*vx+uy*vy;

    //Norms
    double nu = sqrt(ux*ux+uy*uy);
    double nv = sqrt(vx*vx+vy*vy);

    return acos(dot/(nu*nv))*180/M_PI;
}

T2LinesPosition Algorithms::get2LinesPosition(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4, QPointFB &intersection)
{
    //Get position of two lines and intersection

    double eps = 10e10;

    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();
    double wx = p1.x() - p3.x();
    double wy = p1.y() - p3.y();

    double k1 = vx*wy - vy*wx;
    double k2 = ux*wy - uy*wx;
    double k3 = vy*ux - vx*uy;

    //Lines are colinear
    if(fabs(k1) < eps && fabs(k2) < eps && fabs(k3) < eps)
        return COLINEAR;

    //Lines are parallel
    if(fabs(k1) < eps && fabs(k2) < eps)
        return PARALLEL;

    //Lines intersect each other
    double alfa = k1/k3;
    double beta = k2/k3;
    if(alfa >= 0 && alfa <= 1 && beta >= 0 && beta <= 1)
    {
        //Compute intersection
        double xq = p1.x() + alfa*ux;
        double yq = p1.y() + alfa*uy;

        intersection.setX(xq);
        intersection.setY(yq);
        intersection.setAlfa(alfa);
        intersection.setBeta(beta);
        intersection.setIntersection(true);

        return INTERSECTING;
    }

    return NONINTERSECTING;

}

void Algorithms::computePolygonIntersection(std::vector<QPointFB> &pol1, std::vector<QPointFB> &pol2)
{
    //Compute intersections of two polygons and update the lists
    for(unsigned int i = 0; i < pol1.size(); i++)
    {
        //Map of intersections sorted by alfa
        std::map<double, QPointFB> inters;

        for(unsigned int j = 0; j < pol2.size(), j++)
        {
            //Compute intersection
            QPointFB p_inters;
            if(get2LinesPosition(pol1[i], pol1[(i+1)%pol1.size()], pol2[j], pol2[(j+1)%pol2.size()], p_inters) == INTERSECTING)
            {
                //Add for later processing
                inters[p_inters.getAlfa()] = p_inters;

                //Intersection on pj
                if(p_inters.getBeta() == 0)
                    pol2[j].setIntersection(true);

                //Intersection on pj+1
                else if(p_inters.getBeta() == 1)
                    pol2[(j+1)%pol2.size()].setIntersection(true);

                //Intersection at current point
                else
                {
                    pol2.insert(pol2.begin()+(j+1)%pol2.size(), p_inters);
                    j++;
                }

            }

        }

    }

}
