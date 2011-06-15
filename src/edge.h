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

#ifndef EDGE_H
#define EDGE_H

#include "node.h"
#include "style.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QLineF>
#include <QPen>

class Node;
class Edge
{
    public:
        Edge(Node *from, Node *to, QPen style);
        void draw(QGraphicsScene *canvas);
        void drawOffset(QGraphicsScene *canvas, float offset);
        void update();

    private:
        QPen style;
        QGraphicsLineItem *line;
        Node *dest, *dir;
};

#endif
