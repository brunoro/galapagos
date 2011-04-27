#include "mutation.h"

Mutation::Mutation(Tree *tree_parent, int tree_offspring)
{
    parent = tree_parent;
    offspring_id = tree_offspring;
    gain = offspring->getFitness() - parent->getFitness();
}

void Mutation::draw(QGraphicsScene *canvas, QPointF origin, int step)
{
    QList<Tree*> trees;
    trees.append(parent);
    trees.append(offspring);
    Tree::drawMany(canvas, trees, origin, step);
}

void Mutation::setOffspring(Tree *offspring_tree)
{
    offspring = offspring_tree;
}

int Mutation::getOffspringId()
{
    return offspring_id;
}
