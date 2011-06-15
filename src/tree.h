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

#ifndef TREE_H
#define TREE_H

#include "def.h"
#include "node.h"

#include <cfloat>
#include <climits>

#include <QDebug>
#include <QGraphicsLineItem>
#include <QHash>
#include <QList>
#include <QPen>
#include <QPointF>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>

#define GENERATED_TREE_ID INT_MAX - 1
#define GENERATED_TREE_FITNESS FLT_MAX - 1
#define MAX_DEPTH 10

class Tree
{
    public:
        Tree(QString line, int tree_id, float tree_fitness);
        Tree(int tree_id, float tree_fitnes);
        virtual ~Tree();

        void draw(QGraphicsScene *canvas, QPointF coord, int step);
        void correctCollisions(QGraphicsScene *canvas, QPointF origin, int step);

        void addParent(Tree *parent);
        void addOffspring(Tree *offspring);
        void setRoot(Node *node);

        void setId(int id);
        int getId();
        float getFitness();
        Node *getRoot();
        QList<Tree*> getParents();
        QList<Tree*> getOffspring();

        static Tree* drawMany(QGraphicsScene *canvas, QList<Tree*>& trees,
                              QPointF coord, int step, bool treatCollisions, int consensusDepth);
        static Tree *opsConsensusTree(int consensusDepth);
        static void drawRings(QGraphicsScene *canvas, QPointF coord, int depth, int step);
        static void test(QGraphicsScene *canvas, int consensusDepth);

        static Def* definition;
        
    private:
        int id;
        float fitness;
        Node *root;
        QList<Tree*> parents;
        QList<Tree*> offspring;

        Node *parseTree(QStringList nodes, int *pos, int id);
};

#endif
