#include "algorithms.h"
#include <map>


TPointPolygon Algorithms::getPositionWinding(QPointFB q, std::vector<QPointFB> pol)
{
       int n = pol.size();
       double sum_fi=0.0;
       double eps = 1.0e-10;

       //Process all polygon segments
       for (int i=0; i < n; i++)
       {
           //Get angle
           double fi = get2LinesAngle(pol[i], q, pol[(i+1)%n], q);

           //Get position
           int t = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

           //Point on the boundary
           if (t == COL && fabs(fabs(fi) - 180) < eps)
               return ON;

           //Point q in the left halfplane
           if(t > 0)
               sum_fi += fi;

           //Point in the right halfplane
           else
               sum_fi -= fi;
       }


       //Point q inside P
       if (fabs(fabs(sum_fi)-360)< eps)
           return INSIDE;

       //Point q outside P
       if (fabs(sum_fi) < eps)
           return OUTSIDE;
}


TPointLinePosition Algorithms::getPointLinePosition(QPointFB &q, QPointFB &a, QPointFB &b)
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
    return COL;
}


double Algorithms::get2LinesAngle(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4)
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

T2LinesPosition Algorithms::get2LinesPosition(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4, QPointFB &intersection)
{
    double eps = 10e-10;
    //Get Position of two lines and compute their intersection
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    double wx = p1.x() - p3.x();
    double wy = p1.y() - p3.y();

    double k1 = vx*wy - vy*wx;
    double k2 = ux*wy - uy*wx;
    double k3 = vy*ux - vx*uy;

    // lines are colinear
    if(fabs(k1)< eps && fabs(k2)< eps && fabs(k3)<eps)
    {
        return COLINEAR;
    }

    // lines are parallel
    if (fabs(k1) <eps && fabs(k2)< eps)
    {
        return PARALLEL;
    }

    // lines intersect each other
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
        intersection.setInters(true);

        return INTERSECTING;

    }
    return NONINTERSECTING;
}


void Algorithms::computePolygonIntersections(std::vector<QPointFB> &p1, std::vector<QPointFB> &p2)
{
    //Compute intersections of two polygons and update the list
    for(int i = 0; i<p1.size(); i++)
    {
        // Map of intersection sorted by alpha
        std::map<double, QPointFB> inters;
        for(int j = 0;j<p2.size(); j++)
        {
            //Compute intersection
            QPointFB pi;
            if(get2LinesPosition(p1[i],p1[(i+1)%p1.size()],p2[j],p2[(j+1)%p2.size()], pi) == INTERSECTING)
            {
                //Add for later processing
                inters[pi.getAlfa()] = pi;

                //Process intersection
                processIntersection(pi, pi.getBeta(), p2, j);
            }
        }

        //All intersections (pi, pi+1) with p2
        if(!inters.empty())
        {
            for(auto pi2:inters)
            {
                //Process intersection
                processIntersection(pi2.second, pi2.first, p1, i);

            }
        }
    }
}

void Algorithms::processIntersection(QPointFB &b, double t, std::vector<QPointFB>& poly, int i)
{
    //Process intersection
    double eps = 1e-10;

    //Intersection on p[i]
    if(fabs(t) < eps)
    {
        poly[i].setInters(true);
    }

    //Intersection on pj+1
    else if(fabs(fabs(t)-1) < eps)
    {
        poly[(i+1)%poly.size()].setInters(true);
    }

    //Intersection at current point
    else
    {
        i++;
        poly.insert(poly.begin()+i%poly.size(), b);
    }
}

void Algorithms::setPositions(std::vector<QPointFB> &pol1, std::vector<QPointFB> &pol2)
{
    //Set positions of midpoints of edges

    //Process first polygon
    for(unsigned int i = 0; i < pol1.size(); i++)
    {
        QPointFB midpoint((pol1[i].x()+pol1[(i+1)%pol1.size()].x())*0.5, (pol1[i].y()+pol1[(i+1)%pol1.size()].y())*0.5);
        TPointPolygon loc = getPositionWinding(midpoint, pol2);
        pol1[i].setPosition(loc);
    }

    //Process second polygon
    for(unsigned int i = 0; i < pol2.size(); i++)
    {
        QPointFB midpoint((pol2[i].x()+pol2[(i+1)%pol2.size()].x())*0.5, (pol2[i].y()+pol2[(i+1)%pol2.size()].y())*0.5);
        TPointPolygon loc = getPositionWinding(midpoint, pol1);
        pol2[i].setPosition(loc);
    }


}

void Algorithms::createFragments(std::vector<QPointFB> &pol, TPointPolygon position, bool rev, std::map<QPointFB, std::vector<QPointFB> > &F)
{
    //Create fragments from points with the same position
    int index = 0;
    while(pol[index].getPosition() != position || !pol[index].getInters())
    {
        index++;
    }

    //Remember index of the first intersection with the given polygon
    int indexS = index;

    //Process all points of polygon
    do
    {
        if(pol[index].getPosition() == position && pol[index].getInters())
        {
            //Intersection found
            std::vector<QPointFB> f;

            //Add all points with the same position
            do
            {
                f.push_back(pol[index]);
                index = (index+1)%pol.size();
            } while(pol[index].getPosition() == position);

            //Add next intersection
            f.push_back(pol[index]);

            //Reverse position (difference)
            if(rev)
                std::reverse(f.begin(), f.end());

            F[f[0]] = f;
        }

        index = (index+1)%pol.size();

    } while(index != indexS);
}

void Algorithms::mergeFragments(std::map<QPointFB, std::vector<QPointFB> > &Fa, std::map<QPointFB, std::vector<QPointFB> > &Fb, std::vector<std::vector<QPointFB> > &C)
{
    //Merge fragments to polygons

    for(auto fa:Fa)
    {
        //Create emtpy polygon
        std::vector<QPointFB> pol;

        //Find start point of the fragment
        QPointFB s = fa.first;

        //Add fragment without the start point
        pol.insert(pol.end(), fa.second.begin()+1, fa.second.end());

        int iter = 1;

        //Merge all connected fragments
        do
        {
            //Reference to Fa or Fb
            std::map<QPointFB, std::vector<QPointFB> > &F = (iter%2?Fa:Fb);

            //Find segment with the same start point as is the end point
            std::vector<QPointFB> segment = F[pol.back()];

            //Add fragment without the start point
            pol.insert(pol.end(), segment.begin()+1, segment.end());

            iter++;

        } while(pol.back() != s);

        //Add polygon to the results
        C.push_back(pol);

    }
}

double Algorithms::getPolygonOrientation(std::vector<QPointFB> &pol)
{
    //Get polygon orientation using LH formula

    double A = 0;

    for(int i = 0; i < pol.size(); i++)
    {
        A += pol[i].x() * (pol[(i+1)%pol.size()].y() - pol[(i-1+pol.size())%pol.size()].y());
    }

    return A*0.5;
}

std::vector<std::vector<QPointFB> > Algorithms::booleanOperation(std::vector<QPointFB> &pol1, std::vector<QPointFB> &pol2, TBooleanOperation oper)
{
    //Perform boolean operation of polygons

    //Maintain CCW orientation of the firts polygon
    if(getPolygonOrientation(pol1) < 0)
        std::reverse(pol1.begin(), pol1.end());

    //Maintain CCW orientation of the firts polygon
    if(getPolygonOrientation(pol2) < 0)
        std::reverse(pol2.begin(), pol2.end());

    //Compute intersection
    computePolygonIntersections(pol1, pol2);

    //Set positions
    setPositions(pol1, pol2);

    //Create map of fragments
    std::map<QPointFB, std::vector<QPointFB> > Fa, Fb;

    //Set type of vertices
    TPointPolygon pos1 = (oper == INTERSECTION ? INSIDE : OUTSIDE);
    TPointPolygon pos2 = (oper == UNION || oper == DIFFAB ? INSIDE : OUTSIDE);

    bool rever = (oper == DIFFAB || oper == DIFFBA ? true : false);

    //Create fragments
    createFragments(pol1, pos1, false, Fa);
    createFragments(pol2, pos2, rever, Fb);

    //Merge fragments
    std::vector<std::vector<QPointFB> > C;

    mergeFragments(Fa, Fb, C);

    return C;
}


