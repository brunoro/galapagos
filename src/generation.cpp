#include "generation.h"

Generation::Generation()
{
}

void Generation::addIndividual(QString individual, float fitness)
{
    population_tree.append(individual);
    population_fit.append(fitness);
}

void Generation::addMutation(Mutation mutation)
{
    mutations.append(mutation);
}

void Generation::addCrossover(Crossover crossover)
{
    crossovers.append(crossover);
}

Tree* Generation::getIndividual(int id)
{
    return new Tree(population_tree[id], id, population_fit[id]);
}
