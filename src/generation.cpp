#include "generation.h"

Generation::Generation()
{
}

void Generation::addIndividual(Tree *individual)
{
    population.append(individual);
}

void Generation::addMutation(Tree *parent, Tree *offspring)
{
    mutations.insert(Mutation(parent, offspring));
}

void Generation::addCrossover(Tree *parent1, Tree *parent2, Tree *offspring)
{
    crossovers.insert(Crossover(parent1, parent2, offspring));
}

Tree* Generation::getIndividual(int id)
{
    return population[id];
}
