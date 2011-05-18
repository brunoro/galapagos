#include "generation.h"

Generation::Generation()
{
}

void Generation::addIndividual(QString individual, float fitness)
{
    struct ind tree;
    tree.str = individual;
    tree.fit = fitness;
    population.append(tree);
}

void Generation::addReproduction(Reproduction rep)
{
    reproductions.append(rep);
}

Tree* Generation::getIndividual(int id)
{
    return new Tree(population[id].str, id, population[id].fit);
}
