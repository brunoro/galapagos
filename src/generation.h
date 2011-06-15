/*
 * Galapagos - a genetic programming algorithms visualizer
 * http://github.com/brunoro/galapagos
 * Copyright (C) <2011>  <Gustavo Brunoro>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        virtual ~Generation(){}

        void addIndividual(QString individual, float fitness);
        void addReproduction(Reproduction rep);

        Tree* getIndividual(int id);

        QList<struct ind> population;
        QList<Reproduction> reproductions;
};

#endif // GENERATION_H
