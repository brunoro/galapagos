#ifndef GENERATION_H
#define GENERATION_H

#include "tree.h"
#include "reproduction.h"

#include <QList>
#include <QSet>

class Generation
{

    public:
        Generation();

        void addIndividual(QString individual, float fitness);
        void addReproduction(Reproduction rep);

        Tree* getIndividual(int id);

        QList<QString> population_tree;
        QList<float> population_fit;
        QList<Reproduction> reproductions;
};

#endif // GENERATION_H
