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

#ifndef DEF_H 
#define DEF_H

#include <QDebug>
#include <QHash>
#include <QHashIterator>
#include <QRegExp>
#include <QStringList>

class Def
{
    public:
        Def();
        virtual ~Def(){}

        void addOperators(QString opstr);
        void addTerms(QString terms);

        int isOp(QString str);
        bool isTerm(QString str);

        QHashIterator<QString, int> getOpIterator();

    private:
        QHash<QString, int> ops;
        QStringList terms;
};

#endif
