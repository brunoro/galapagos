#ifndef MUTATION_H
#define MUTATION_H

#include "tree.h"

#include <QGraphicsScene>
#include <QPointF>

class Mutation
{
    public:
        Mutation(int mut_parent, int mut_offspring);
        int parent,
            offspring;
};

#endif // MUTATION_H
