#ifndef GENERATION_H
#define GENERATION_H

#include "tree.h"
#include "reproduction.h"

#include <QList>
#include <QSet>

struct ind
{
    QString str;
    float fit;
};

class Generation
{

    public:
        Generation();

        void addIndividual(QString individual, float fitness);
        void addReproduction(Reproduction rep);

        Tree* getIndividual(int id);

        QList<struct ind> population;
        QList<Reproduction> reproductions;
};

#endif // GENERATION_H
