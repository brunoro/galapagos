#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "tree.h"

#include <QGraphicsScene>
#include <QPointF>

class Crossover
{
    public:
        Crossover(Tree *parent1, Tree *parent2, int offspring_id);
        void draw(QGraphicsScene *canvas, QPointF origin, int step);
        void setOffspring(Tree *offspring);
        int getOffspringId();

    private:
        Tree *parent1,
             *parent2,
             *offspring;
        float gain;
        int offspring_id;
};

#endif // CROSSOVER_H
