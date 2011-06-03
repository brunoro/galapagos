#ifndef REPRODUCTION_H
#define REPRODUCTION_H

#include "tree.h"

#include <QGraphicsScene>
#include <QPointF>

class Reproduction
{
    public:
        Reproduction(QList<int> rep_parent, int rep_offspring);
        virtual ~Reproduction(){}
        QList<int> parents;
        int offspring;
};

#endif // REPRODUCTION_H
