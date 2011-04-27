#ifndef MUTATION_H
#define MUTATION_H

#include "tree.h"

#include <QGraphicsScene>
#include <QPointF>

class Mutation
{
    public:
        Mutation(Tree *parent, int offspring_id);
        void draw(QGraphicsScene *canvas, QPointF origin, int step);

        void setOffspring(Tree *offspring);
        int getOffspringId();

    private:
        Tree *parent,
             *offspring;
        float gain;
        int offspring_id;
};

#endif // MUTATION_H
