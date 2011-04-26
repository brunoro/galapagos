#include "mutation.h"

Mutation::Mutation(Tree *tree_parent, Tree *tree_offspring)
{
    parent = tree_parent;
    offspring = tree_offspring;
    gain = offspring->getFitness() - parent->getFitness();
}

Mutation::draw(QGraphicsScene *canvas, QPointF origin, int step)
{
    QList<Tree*> trees;
    trees.append(parent);
    trees.append(offspring);
    Tree::drawMany(canvas, trees, origin, step);
}
