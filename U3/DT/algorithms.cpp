#include <list>
#include "algorithms.h"
#include "sortbyxasc.h"

Algorithms::Algorithms()
{

}

TPosition Algorithms::getPointLinePosition(QPoint &q, QPoint &a, QPoint &b)
{
    //Point and line position
    double eps = 1.0e-10;

    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = q.x() - a.x();
    double vy = q.y() - a.y();

    //Determinant
    double t = (ux * vy - uy * vx);

    //Point in the left half plane
    if(t > eps)
        return LEFT;

    //Point in the right half plane
    if(t < -eps)
        return RIGHT;

    //Point on the line
    return ON;
}

double Algorithms::getCircleRadius(QPoint &p1, QPoint &p2, QPoint &p3)
{
    //Get radius of the circle passing through p1, p2, p3
    double x1 = p1.x();
    double x2 = p2.x();
    double x3 = p3.x();

    double y1 = p1.y();
    double y2 = p2.y();
    double y3 = p3.y();

    //Coefficients k1-k12
    double k1 = x1*x1+y1*y1;
    double k2 = x2*x2+y2*y2;
    double k3 = x3*x3+y3*y3;

    double k4 = y1 - y2;
    double k5 = y1 - y3;
    double k6 = y2 - y3;

    double k7 = x1 - x2;
    double k8 = x1 - x3;
    double k9 = x2 - x3;

    double k10 = x1 * x1;
    double k11 = x2 * x2;
    double k12 = x3 * x3;

    //Compute radius
    double m = 0.5 *(k12*(-k4)+k11*k5 - (k10+k4*k5)*k6)/(x3*(-k4)+x2*k5+x1*(-k6));
    double n = 0.5*(k1*(-k9)+k2*k8+k3*(-k7))/(y1*(-k9)+y2*k8+y3*(-k7));
    return sqrt((x1-m)*(x1-m)+(y1-n)*(y1-n));
}

int Algorithms::getNearestPoint(QPoint &p, std::vector<QPoint> points)
{
    //Find the nearest point
    double dist_min = getDistance(p, points[0]);
    int i_min = 0;

    for(unsigned int i = 1; i < points.size(); i++)
    {
        //Compute the distance
        double dist = getDistance(p,points[i]);
        if(dist < dist_min)
        {
            dist_min = dist;
            i_min = i;
        }
    }

    return i_min;
}

double Algorithms::getDistance(QPoint &p1, QPoint &p2)
{
    //Distance between 2 points
    double dx = p1.x()-p2.x();
    double dy = p1.y()-p2.y();
    return sqrt(dx*dx + dy*dy);
}

int Algorithms::getDelaunayPoint(QPoint &s, QPoint &e, std::vector<QPoint> points)
{
    //Find the Delaunay point
    double rad_min = 10e10;
    int index = -1;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        //Is the point in the left half-plane?
        if(getPointLinePosition(points[i], s, e) == LEFT)
        {
            double rad = getCircleRadius(points[i], s, e);
            if(rad < rad_min)
            {
                rad_min = rad;
                index = i;
            }
        }
    }

    return index;
}

std::vector<Edge> Algorithms::delaunayTriangulation(std::vector<QPoint> &points)
{
    //Create Delaunay triangulation

    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Sort by X
    sort(points.begin(), points.end(), SortByXAsc());

    //Find pivot
    QPoint q = points[0];

    //Get nearest point to pivot
    int index_n = getNearestPoint(q, points);
    QPoint q_n = points[index_n];

    //Create edge
    Edge e(q,q_n);

    //Find Delaunay point
    int index_d = getDelaunayPoint(q, q_n, points);

    //Delaunay point found
    if(index_d != -1)
    {
        //Delaunay point
        QPoint q_d = points[index_d];

        //Create initial triangle
        Edge e2(q_n, q_d);
        Edge e3(q_d, q);

        //Add first Delaunay triangle
        dt.push_back(e);
        dt.push_back(e2);
        dt.push_back(e3);

        //Add to AEL
        ael.push_back(e);
        ael.push_back(e2);
        ael.push_back(e3);
    }

    //Switch edge orientation
    else
    {
        e.switchOrientation();

        //Find Delaunay point
        int index_d = getDelaunayPoint(q, q_n, points);

        //Delaunay point found
        if(index_d != -1)
        {
            //Delaunay point
            QPoint q_d = points[index_d];

            //Create initial triangle
            Edge e2(q_n, q_d);
            Edge e3(q_d, q);

            //Add first Delaunay triangle
            dt.push_back(e);
            dt.push_back(e2);
            dt.push_back(e3);

            //Add to AEL
            ael.push_back(e);
            ael.push_back(e2);
            ael.push_back(e3);
        }
    }

    //Process until AEL is empty
    while(!ael.empty())
    {
        //Get the edge
        Edge e = ael.back();
        ael.pop_back();

        //Switch orientation
        e.switchOrientation();
        QPoint p1 = e.getS();
        QPoint p2 = e.getE();

        //Find Delaunay point
        int index_d = getDelaunayPoint(p1, p2, points);

        //Delaunay point found
        if(index_d != -1)
        {
            //Delaunay point
            QPoint p_d = points[index_d];

            //Create initial triangle
            Edge e2(p2, p_d);
            Edge e3(p_d, p1);

            //Add Delaunay triangle
            dt.push_back(e);
            dt.push_back(e2);
            dt.push_back(e3);

            //Switch orientation
            e2.switchOrientation();
            e3.switchOrientation();

            //Is e2 in AEL?
            std::list<Edge>::iterator i_e2 = std::find(ael.begin(), ael.end(), e2);

            if(i_e2 != ael.end())
                ael.push_back(e2);
            else
                ael.erase(i_e2);

            //Is e3 in AEL?
            std::list<Edge>::iterator i_e3 = std::find(ael.begin(), ael.end(), e3);

            if(i_e3 != ael.end())
                ael.push_back(e3);
            else
                ael.erase(i_e3);
        }
    }

    return dt;
}
