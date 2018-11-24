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
    //Create convex hull using the Jarvis Scan procedure

    double eps = 10e-6;

    QPolygon ch;

    //Find pivot q
    std::sort(points.begin(), points.end(), sortByYAsc());
    QPoint q = points[0];

    std::sort(points.begin(), points.end(), SortByXAsc());
    QPoint s = points[0];

    //Change s.X if points s and q are identical (solves problem with counting angle later)
    if(fabs(q.x()-s.x()) < eps)
        s.setX(s.x()+10);

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
        double dist_min = 10e10;

        //Find pi = arg max angle(pi, pj, pjj)
        for(unsigned int i = 0; i < points.size(); i++)
        {
            //Get angle betwwen 2 segments
            double fi = get2LinesAngle(pj, pjj, pj, points[i]);

            //Check whether there are collinear points
            if(fabs(fi-fi_max) < eps)
            {
                //If so, get the closest point to pj
                double d_pj_pi = getDistance(pj,points[i]);
                if(dist_min > d_pj_pi)
                {
                    dist_min = d_pj_pi;
                    i_max = i;
                    fi_max = fi;
                }
            }

            //Find maximum
            else if (fi>fi_max)
            {
                i_max = i;
                fi_max = fi;
            }

        }

        //Add the next point to CH
        ch.push_back(points[i_max]);

        //Assign CH verticess
        pjj = pj;
        pj = points[i_max];

    } while (!(pj == q));

    //Create strict convex hull
    strictCH(ch);

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

    //Create strict convex hull
    strictCH(ch);

    return ch;
}


void Algorithms::qh_loc(int s, int e, vector<QPoint> &ss, QPolygon &h)
{
    //Local recursive procedure of QHull
    int i_max = -1;
    double d_max = 0;

    //Browse all points
    for(unsigned int i=2; i<ss.size(); i++)
    {
        //Is the point in the right half-plane?
        if(getPointLinePosition(ss[i], ss[s], ss[e])==RIGHT)
        {
            //Compute the distance between point and the given segment
            double d = getPointLineDistance(ss[i], ss[s], ss[e]);

            //Remember the furthest point and its index
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
        //Process the first interval
        qh_loc(s, i_max, ss, h);

        //Add the point to convex hull
        h.push_back(ss[i_max]);

        //Process the second interval
        qh_loc(i_max, e, ss, h);
    }
}

QPolygon Algorithms::CHSweepLine(vector<QPoint> &points)
{
    //Create convex hull using the sweepline procedure
    QPolygon ch;
    double eps = 10e-10;

    //Sort points by x
    std::sort(points.begin(), points.end(), SortByXAsc());

    //Remove duplicit points
    std::vector<QPoint> points_clean;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        //Add last point (unable to compare)
        if(i == points.size()-1)
        {
            points_clean.push_back(points[i]);
            break;
        }

        //Add point if not duplicit with following
        if(getDistance(points[i], points[i+1]) > eps)
            points_clean.push_back(points[i]);
    }

    points = points_clean;

    //Create list of predecessors
    std::vector<int> p(points.size());

    //Create list of successors
    std::vector<int> n(points.size());

    /*
    //Create triangle (0, 1, 2) from the first 3 points
    //Change i to 3 in the next for loop
    if(getPointLinePosition(points[2], points[0], points[1])==LEFT)
    {
         n[0] = 1; n[1] = 2; n[2] = 0;
         p[0] = 2; p[1] = 0; p[2] = 1;
    }

    //Create triangle (0, 2, 1) from the first 3 points
    else
    {
        n[0] = 2; n[2] = 1; n[1] = 0;
        p[0] = 1; p[2] = 0; p[1] = 2;
    }
    */

    //Create an initial biangle (0,1)
    //Change i to 2 in the next for loop
    n[0] = 1;
    n[1] = 0;

    p[0] = 1;
    p[1] = 0;

    for(unsigned int i = 2; i < points.size(); i++)
    {
        //Point in the upper half-plane
        //Link i with predcessor/successor
        if(points[i].y() > points[i-1].y())
        {
            p[i] = i-1;
            n[i] = n[i-1];
        }

        //Point in the lower half-plane
        else
        {
            n[i] = i-1;
            p[i] = p[i-1];
        }

        //Link predecessor/successor with i-th point
        n[p[i]] = i;
        p[n[i]] = i;

        //Fix the upper tangent
        while(getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == RIGHT)
        {
            p[n[n[i]]] = i;
            n[i] = n[n[i]];
        }

        //Fix the lower tangent
        while(getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == LEFT)
        {
            n[p[p[i]]] = i;
            p[i] = p[p[i]];
        }
    }

    //Convert to the polygon
    ch.push_back(points[0]);
    int index = n[0];

    while(index != 0)
    {
        ch.push_back(points[index]);
        index = n[index];

    }


    //Create strict convex hull
    strictCH(ch);

    return ch;
}

void Algorithms::strictCH(QPolygon &ch)
{
    //Create strict convex hull
    for(int i = 0; i < ch.size()-2; i++)
    {
        //Delete point in between if on the same line
        if(getPointLinePosition(ch[i+2],ch[i],ch[i+1]) == ON)
        {
            ch.remove(i+1);
            i--;
        }
    }
}

double Algorithms::getDistance(QPoint &p1, QPoint &p2)
{
    double dx = p1.x()-p2.x();
    double dy = p1.y()-p2.y();
    return sqrt(dx*dx + dy*dy);
}
