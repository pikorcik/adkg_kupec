#include "algorithms.h"

Algorithms::Algorithms()
{

}

int Algorithms::getPositionRay(QPoint q, std::vector<QPoint> pol)
{
    int k = 0;

    //Add point to polygon
    pol.push_back(pol[0]);

    for(int i=0; i<pol.size()-1; i++)
    {
        //Reduce coordinates
        double xi_red = pol[i].x()-q.x();
        double yi_red = pol[i].y()-q.y();
        double xii_red = pol[i+1].x()-q.x();
        double yii_red = pol[i+1].y()-q.y();

        //Upper halfplane
        if((yii_red>0 && yi_red<=0)||(yi_red>0 && yii_red<=0))
        {
            //Right halfplane
            double xm = (xii_red*yi_red-xi_red*yii_red)/(yii_red-yi_red);

            if(xm>0)
                k++;
        }
    }

    return k%2;



}
