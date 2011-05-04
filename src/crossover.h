#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "tree.h"

#include <QGraphicsScene>
#include <QPointF>

class Crossover
{
    public:
        Crossover(int parent1, int parent2, int offspring);
        int parent1,
            parent2,
            offspring;
};

#endif // CROSSOVER_H
