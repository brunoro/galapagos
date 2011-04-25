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

    private:
        QList<Tree*> population;
        QSet<Mutation> mutations;
        QSet<Crossover> crossovers;
};

#endif // GENERATION_H
