#ifndef GENERATION_H
#define GENERATION_H

#include "tree.h"
#include "mutation.h"
#include "crossover.h"

#include <QList>
#include <QSet>

class Generation
{

    public:
        Generation(int popSize);

        void addIndividual(Tree *individual);
        void addMutation(Tree *parent, Tree *offspring);
        void addCrossover(Tree *parent1, Tree *parent2, Tree *offspring);

        Tree* getIndividual(int id);

    private:
        int num;
        QList<Tree*> population;
        QSet<Mutation> mutations;
        QSet<Crossover> crossovers;
};

#endif // GENERATION_H
