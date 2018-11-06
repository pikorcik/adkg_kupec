#include <cmath>
#include <vector>
#include "algorithms.h"
#include "sortbyxasc.h"
#include "sortbyyasc.h"

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


double Algorithms::get2LinesAngle(QPoint &p1,QPoint &p2,QPoint &p3, QPoint &p4)
{
    //Angle between two lines
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Dot product
    double dot = ux * vx + uy * vy;

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    return fabs(acos(dot/(nu * nv)))*(180/M_PI);
}

double Algorithms::getPointLineDistance(QPoint &q, QPoint &a, QPoint &b)
{
    //Point and line distance

    double x12 = b.x()-a.x();
    double y12 = b.y()-a.y();

    double y1a = q.y()-a.y();
    double y2a = b.y()-q.y();

    double numerator = -q.x()*y12 + a.x()*y2a + b.x()*y1a;
    double denominator = sqrt(x12*x12+y12*y12);

    return fabs(numerator)/denominator;
}

QPolygon Algorithms::CHJarvis (vector<QPoint> &points)
{
    QPolygon ch;

    //Find pivot q
    std::sort(points.begin(), points.end(), sortByYAsc());
    QPoint q = points[0];

    std::sort(points.begin(), points.end(), SortByXAsc());
    QPoint s = points[0];

    //Create Pjj
    QPoint pjj (s.x(),q.y());
    QPoint pj = q;

    //Add pivot to CH
    ch.push_back(q);

    //Add all points to CH
    do
    {
        int i_max = -1;
        double fi_max = 0;

        //Find pi = arg max angle(pi, pj, pjj)
        for(int i = 0; i<points.size();i++)
        {
            //Get angle betwwen 2 segments
            double fi = get2LinesAngle(pj, pjj, pj, points[i]);

            //Find maximum
            if (fi>fi_max)
            {
                i_max=i;
                fi_max=fi;
            }
        }

        //Add the next point to CH
        ch.push_back(points[i_max]);

        //Assign CH verticess
        pjj = pj;
        pj = points[i_max];

    } while (!(pj == q));

    return ch;
}

QPolygon Algorithms::QHull (vector<QPoint> &points)
{
    //Create convex hull using the qhull procedure
    QPolygon ch;
    vector<QPoint> su, sl;

    //Find q1, q3
    std::sort(points.begin(), points.end(), SortByXAsc());
    QPoint q1 = points[0];
    QPoint q3 = points[points.size()-1];

    //Add to SU, SL
    su.push_back(q1);
    su.push_back(q3);
    sl.push_back(q1);
    sl.push_back(q3);

    //Split into SU, SL
    for(unsigned int i=0; i<points.size(); i++)
    {
        //Add to SU
        if(getPointLinePosition(points[i],q1,q3)==LEFT)
            su.push_back(points[i]);

        // Add to SL
        if(getPointLinePosition(points[i],q1,q3)==RIGHT)
            sl.push_back(points[i]);
    }

    //Add q3 to convex hull
    ch.push_back(q3);

    //Process SU
    qh_loc(1, 0, su, ch);

    //Add q1 to convex hull
    ch.push_back(q1);

    //Process SL
    qh_loc(0, 1, sl, ch);

    return ch;
}


void Algorithms::qh_loc(int s, int e, vector<QPoint> &ss, QPolygon &h)
{
    //Recursive procedure of QHull
    int i_max = -1;
    double d_max = 0;

    //Browse all points
    for(unsigned int i=2; i<ss.size(); i++)
    {
        //Is the point in the right half-plane?
        if(getPointLinePosition(ss[i], ss[s], ss[e])==RIGHT)
        {
            double d = getPointLineDistance(ss[i], ss[s], ss[e]);

            //Remember the farthest point
            if(d>d_max)
            {
                d_max = d;
                i_max = i;
            }
        }
    }

    //Point in the right half-plane exists
    if(i_max > 1)
    {
        //Process first interval
        qh_loc(s, i_max, ss, h);

        //Add the point to convex hull
        h.push_back(ss[i_max]);

        //Process second interval
        qh_loc(i_max, e, ss, h);
    }
}
