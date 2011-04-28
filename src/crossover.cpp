#include "crossover.h"

Crossover::Crossover(int tree_parent1, int tree_parent2, int tree_offspring)
{
    parent1 = tree_parent1;
    parent2 = tree_parent2;
    offspring = tree_offspring;
}

void Crossover::draw(QGraphicsScene *canvas, QPointF origin, int step)
{
    QList<Tree*> trees;
    // TODO: get generations
    Tree::drawMany(canvas, trees, origin, step);
}
