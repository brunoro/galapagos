#include "crossover.h"

Crossover::Crossover(Tree *tree_parent1, Tree *tree_parent2, Tree *tree_offspring)
{
    parent1 = tree_parent1;
    parent2 = tree_parent2;
    offspring = tree_offspring;

    float maxFit = parent1->getFitness() > parent2->getFitness() ?
                   parent1->getFitness() : parent2->getFitness();
    gain = offspring->getFitness() - maxFit;
}

Crossover::draw(QGraphicsScene *canvas, QPointF origin, int step)
{
    QList<Tree*> trees;
    trees.append(parent1);
    trees.append(parent2);
    trees.append(offspring);
    Tree::drawMany(canvas, trees, origin, step);
}
