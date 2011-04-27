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
        Generation();

        void addIndividual(Tree *individual);
        void addMutation(Mutation mutation);
        void addCrossover(Crossover crossover);

        Tree* getIndividual(int id);

    private:
        int num;
        QList<Tree*> population;
        QList<Mutation> mutations;
        QList<Crossover> crossovers;
};

#endif // GENERATION_H
