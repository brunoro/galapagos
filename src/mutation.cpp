#include "mutation.h"

Mutation::Mutation(int mut_parent, int mut_offspring)
{
    parent = mut_parent;
    offspring = mut_offspring;
}

void Mutation::draw(QGraphicsScene *canvas, QPointF origin, int step)
{
    QList<Tree*> trees;
    // TODO: get generations 
    Tree::drawMany(canvas, trees, origin, step);
}
