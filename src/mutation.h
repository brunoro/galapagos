#ifndef MUTATION_H
#define MUTATION_H

#include "tree.h"

#include <QGraphicsScene>
#include <QPointF>

class Mutation
{
    public:
        Mutation(Tree *parent, Tree *offspring);
        void draw(QGraphicsScene *canvas, QPointF origin, int step);

    private:
        Tree *parent,
             *offspring;
        float gain;
};

#endif // MUTATION_H
