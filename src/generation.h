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

        void addIndividual(QString individual, float fitness);
        void addMutation(Mutation mutation);
        void addCrossover(Crossover crossover);

        Tree* getIndividual(int id);

    private:
        QList<QString> population_tree;
        QList<float> population_fit;
        QList<Mutation> mutations;
        QList<Crossover> crossovers;
};

#endif // GENERATION_H
