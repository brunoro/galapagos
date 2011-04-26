#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "tree.h"

#include <QGraphicsScene>
#include <QPointF>

class Crossover
{
    public:
        Crossover(Tree *parent1, Tree *parent2, Tree *offspring);
        draw(QGraphicsScene *canvas, QPointF origin, int step);

    private:
        Tree *parent1,
             *parent2,
             *offspring;
        float gain;
};

#endif // CROSSOVER_H
