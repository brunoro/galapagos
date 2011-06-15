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

#ifndef NODE_H 
#define NODE_H

#include "edge.h"
#include "def.h"

#include <cmath>
#include <climits>

#include <QBrush>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QHash>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPen>
#include <QPointF>
#include <QSet>
#include <QString>

#define TO_RADIANS(a) (a) * Style::pi / 180
#define TO_DEGREES(a) (a) / Style::pi * 180
#define CONSENSUS_ID INT_MIN

enum NodeType {OP, TERM, ROOT};

class Edge;
class Node
{

    public:
        Node(NodeType type, QString info);
        virtual ~Node();

        void addSon(Node *son);
        void addEdge(Node *son, QPen style);
        void draw(QGraphicsScene *canvas, QPointF coord);
        void updateEdges(QGraphicsScene *canvas);
        void recursiveUpdateEdges(QGraphicsScene *canvas);
        bool collidesWith(Node *other);
        void adjustPosition(QPointF origin, Node *other, int level, int step);
        void update(QPointF coord);
        void rotateSubtree(QPointF origin, float angle);

        int recursiveDraw(QGraphicsScene *canvas,
                          QPointF origin, QPointF coord,
                          int step, int level,
                          float areaAngle, float refAngle);
        void opsConsensus(int depth, Def* definition);

        static QList<Node*> recursiveDrawMany(QGraphicsScene *canvas, QList<Node*> nodes,
                                              QPointF origin, QPointF coord,
                                              int step, int level,
                                              float areaAngle, float refAngle, QHash<int, QPen> style,
                                              int consensusDepth);
        QList<Node*> getSons();
        NodeType getType();
        QString getInfo();
        QPointF getCoord();
        QSet<int> getTreeId();
        QGraphicsItem *getBound();

        void setSons(QList<Node*> node_sons);
        void setCoord(QPointF node_coord);
        void setTreeId(QSet<int> id);
        void recursiveSetTreeId(QSet<int> id);
        void addTreeId(QSet<int> ids);

        inline bool operator==(const Node &other) const
        {
            return (other.type == type) && (other.info == info);
        }

        inline bool operator<(const Node &other) const
        {
            if (other.type == type)
            {
                if (other.info == info)
                {
                    if (other.pos.x() == pos.x())
                        return other.pos.y() < pos.y();
                    else
                        return other.pos.x() < pos.x();
                }
                else
                    return other.info < info;
            }
            return other.type < type;
        }

    private:
        QSet<int> tree_id;
        NodeType type;
        QString info;
        QList<Node*> sons;
        QMap< Node, QList<Edge*> > edges;
        QPointF pos;
        QGraphicsItem *text;
        QGraphicsItem *bound;
        int tamToEllipse;
};

#endif
