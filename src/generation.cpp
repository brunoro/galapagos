#include "generation.h"

Generation::Generation()
{
}

void Generation::addIndividual(Tree *individual)
{
    population.append(individual);
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
    return population[id];
}
