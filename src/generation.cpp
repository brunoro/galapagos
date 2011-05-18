#include "generation.h"

Generation::Generation()
{
}

void Generation::addIndividual(QString individual, float fitness)
{
    population_tree.append(individual);
    population_fit.append(fitness);
}

void Generation::addReproduction(Reproduction rep)
{
    reproductions.append(rep);
}

Tree* Generation::getIndividual(int id)
{
    return new Tree(population_tree[id], id, population_fit[id]);
}
