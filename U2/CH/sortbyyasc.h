#ifndef SORTBYYASC_H
#define SORTBYYASC_H
#include <QtGui>


class sortByYAsc
{
public:
    sortByYAsc();

    bool operator()(QPoint &p1, QPoint &p2)
    {
        return (p1.y()<p2.y())||((p1.y()==p2.y())&&(p1.x()<p2.x()));
    }
};

#endif // SORTBYYASC_H
