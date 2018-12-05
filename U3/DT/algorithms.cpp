#include "algorithms.h"
#include <list>
#include "sortbyxasc.h"

Algorithms::Algorithms()
{

}

TPosition Algorithms::getPointLinePosition(QPoint3D &q, QPoint3D &a, QPoint3D &b)
{
    //Point and line position
    double eps = 1.0e-6;

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

double Algorithms::getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c)
{
    //Get radius of the circle passing through p1, p2, p3
    double x1 = p1.x();
    double x2 = p2.x();
    double x3 = p3.x();

    double y1 = p1.y();
    double y2 = p2.y();
    double y3 = p3.y();

    //Coefficients k1-k12
    double k1 = x1*x1 + y1*y1;
    double k2 = x2*x2 + y2*y2;
    double k3 = x3*x3 + y3*y3;

    double k4 = y1 - y2;
    double k5 = y1 - y3;
    double k6 = y2 - y3;

    double k7 = x1 - x2;
    double k8 = x1 - x3;
    double k9 = x2 - x3;

    double k10 = x1 * x1;
    double k11 = x2 * x2;
    double k12 = x3 * x3;

    //Compute center
    double m = 0.5 * (k12 * (-k4) + k11 * k5 - (k10 + k4 * k5) * k6)/
                      (x3 * (-k4) + x2 * k5 + x1 * (-k6));
    double n = 0.5 * ((k1 * (-k9) + k2 * k8 + k3 * (-k7))/
                      (y1 * (-k9) + y2 * k8 + y3 * (-k7)));

    c.setX(m);
    c.setY(n);

    //Compute radius
    return sqrt((x1-m) * (x1-m) + (y1-n)*(y1-n));
}

int Algorithms::getNearestPoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    //Find the nearest point
    double dist_min = getDistance(p, points[1]);
    int i_min = 1;

    for(unsigned int i = 2; i < points.size(); i++)
    {
        //Compute the distance
        double dist = getDistance(p,points[i]);

        //Assign the minimum
        if(dist < dist_min)
        {
            dist_min = dist;
            i_min = i;
        }
    }

    return i_min;
}

double Algorithms::getDistance(QPoint3D &p1, QPoint3D &p2)
{
    //Distance between 2 points
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    return sqrt(dx*dx + dy*dy);
}

int Algorithms::getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points)
{
    //Find the Delaunay point
    double rad_min = 10e10;
    int i_min = -1;

    //Process all points...
    for(unsigned int i = 0; i < points.size(); i++)
    {
        //...except the analogous to start, end
        if ((points[i] != s) && (points[i] != e))
        {
            //Is the point in the left half-plane?
            if(getPointLinePosition(points[i], s, e) == LEFT)
            {
                QPoint3D c;
                double rad = getCircleRadius(points[i], s, e, c);

                //Point in the right half-plane is preferred
                if(getPointLinePosition(c,s,e) == RIGHT)
                    rad = - rad;

                //Test min value
                if(rad < rad_min)
                {
                    rad_min = rad;
                    i_min = i;
                }
            }
        }
    }

    return i_min;
}

std::vector<Edge> Algorithms::delaunayTriangulation(std::vector<QPoint3D> &points)
{
    //Create Delaunay triangulation
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Sort by X
    sort(points.begin(), points.end(), SortByXAsc());

    //Find pivot
    QPoint3D q = points[0];

    //Get nearest point to pivot
    int indexn = getNearestPoint(q, points);
    QPoint3D qn = points[indexn];

    //Create edge
    Edge e(q,qn);

    //Find Delaunay point
    int indexd = getDelaunayPoint(e.getS(), e.getE(), points);

    // Delaunay point not found
    if (indexd == -1)
    {
        //Switch orientation
        e.switchOrientation();

        //Find Delaunay point
        indexd = getDelaunayPoint(e.getS(), e.getE(), points);
    }

    //Delaunay point
    QPoint3D qd = points[indexd];

    //Create initial triangle
    Edge e2(e.getE(), qd);
    Edge e3(qd, e.getS());

    //Add first Delaunay triangle
    dt.push_back(e);
    dt.push_back(e2);
    dt.push_back(e3);

    //Add to AEL
    ael.push_back(e);
    ael.push_back(e2);
    ael.push_back(e3);

    //Process until AEL is empty
    while(!ael.empty())
    {
        //Get the edge
        Edge e = ael.back();
        ael.pop_back();

        //Switch orientation
        e.switchOrientation();

        //Find Delaunay point
        int indexd2 = getDelaunayPoint(e.getS(), e.getE(), points);

        //Delaunay point found
        if(indexd2 != -1)
        {
            //Delaunay point
            QPoint3D pd = points[indexd2];

            //Create the triangle
            Edge e2(e.getE(), pd);
            Edge e3(pd, e.getS());

            //Add Delaunay triangle
            dt.push_back(e);
            dt.push_back(e2);
            dt.push_back(e3);

            //Switch orientation
            e2.switchOrientation();
            e3.switchOrientation();

            //Is e2 in AEL?
            std::list<Edge>::iterator i_e2 = std::find(ael.begin(), ael.end(), e2);


            //e2 not in AEL
            if(i_e2 == ael.end())
            {
                //Switch orientation back
                e2.switchOrientation();

                //Add to the list
                ael.push_back(e2);
            }

            //e2 in AEL
            else ael.erase(i_e2);

            //Is e3 in AEL?
            std::list<Edge>::iterator i_e3 = std::find(ael.begin(), ael.end(), e3);


            //e3 nor in AEL
            if(i_e3 == ael.end())
            {
                //Switch orientation back
                e3.switchOrientation();

                //Add to the list
                ael.push_back(e3);
            }

            //e3 in AEL
            else ael.erase(i_e3);
        }
    }

    return dt;
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Return intersection of the edge and a horizontal plane
    double cx = (p2.x() - p1.x()) / (p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.x();
    double cy =(p2.y() - p1.y()) / (p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.y();

    QPoint3D p(cx, cy, z);
    return p;
}

std::vector<Edge> Algorithms::createContours(std::vector<Edge> &dt, double z_min, double z_max, double dz)
{
    //Create contour lines
    std::vector<Edge> contours;

    //Process all triangles
    for(unsigned int i = 0; i < dt.size(); i += 3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getS();
        QPoint3D p2 = dt[i].getE();
        QPoint3D p3 = dt[i+1].getE();

        //Get Z of vertices
        double z1 = p1.getZ();
        double z2 = p2.getZ();
        double z3 = p3.getZ();

        //Find all contour lines
        for(double z = z_min; z <= z_max; z += dz)
        {
            // High differences
            double dz1 = z - z1;
            double dz2 = z - z2;
            double dz3 = z - z3;

            //Is the edge intersected by a plane?
            double dz12 = dz1 * dz2;
            double dz23 = dz2 * dz3;
            double dz31 = dz3 * dz1;

            //Triangle is coplanar
            if(dz1 == 0 && dz2 == 0 && dz3 == 0)
                continue;

            //Edge e12 is coplanar
            else if(dz1 == 0 && dz2 == 0)
                contours.push_back(dt[i]);

            //Edge e23 is colinear
            else if(dz2 == 0 && dz3 == 0)
                contours.push_back(dt[i+1]);

            //Edge e31 is colinear
            else if(dz3 == 0 && dz1 == 0)
                contours.push_back(dt[i+2]);

            //Plane intersects edges e12 and e23
            else if((dz12 <= 0 && dz23 < 0) || (dz12 < 0 && dz23 <= 0))
            {
                //Compute contour line points
                QPoint3D a  = getContourPoint(p1, p2, z);
                QPoint3D b = getContourPoint(p2, p3, z);

                contours.push_back(Edge(a, b));
            }

            //Plane intersects edges e23 and e31
            else if((dz23 <= 0 && dz31 < 0) || (dz23 < 0 && dz31 <= 0))
            {
                //Compute contour line points
                QPoint3D a  = getContourPoint(p2, p3, z);
                QPoint3D b = getContourPoint(p3, p1, z);

                contours.push_back(Edge(a, b));
            }

            //Plane intersects edges e31 and e12
            else if((dz31 <= 0 && dz12 < 0) || (dz31 < 0 && dz12 <= 0))
            {
                //Compute contour line points
                QPoint3D a  = getContourPoint(p3, p1, z);
                QPoint3D b = getContourPoint(p1, p2, z);

                contours.push_back(Edge(a, b));
            }
        }
    }

    return contours;
}

double Algorithms::getSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute slope of the triangle

    //Count vectors
    double ux = p1.x() - p2.x();
    double uy = p1.y() - p2.y();
    double uz = p1.getZ() - p2.getZ();
    double vx = p3.x() - p2.x();
    double vy = p3.y() - p2.y();
    double vz = p3.getZ() - p2.getZ();

    //Cross product
    double nx = uy*vz - uz*vy;
    double ny = -(ux*vz - uz*vx);
    double nz = ux*vy - uy*vx;

    //Count norm
    double norm = sqrt(nx*nx + ny*ny + nz*nz);

    //Count slope
    return acos(nz/norm)*180/M_PI;

}

double Algorithms::getAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Compute aspect of the triangle

    //Count vectors
    double ux = p1.x() - p2.x();
    double uy = p1.y() - p2.y();
    double uz = p1.getZ() - p2.getZ();
    double vx = p3.x() - p2.x();
    double vy = p3.y() - p2.y();
    double vz = p3.getZ() - p2.getZ();

    //Cross product
    double nx = uy*vz - uz*vy;
    double ny = -(ux*vz - uz*vx);

    //Count aspect
    return atan2(nx,ny)*180/M_PI;
}

std::vector<Triangle> Algorithms::analyzeDTM(std::vector<Edge> &dt)
{
    //Convert edges and compute slope and aspect
    std::vector<Triangle> dtm;

    //Process all triangles
    for(unsigned int i = 0; i < dt.size(); i += 3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getS();
        QPoint3D p2 = dt[i].getE();
        QPoint3D p3 = dt[i+1].getE();

        //Compute slope
        double slope = getSlope(p1, p2, p3);

        //Compute aspect
        double aspect = getAspect(p1, p2, p3);

        //Create the triangle
        Triangle t(p1, p2, p3, slope, aspect);
        dtm.push_back(t);
    }

    return dtm;
}
