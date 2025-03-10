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

#include <QGraphicsItemGroup>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QList>
#include <QStringList>
#include <QPen>

#include <cfloat>

#include "style.h"
#include "roundrect.h"

class Refbox
{
    //TODO: implement destructor
    public:
        Refbox(QList<QColor> box_lines, QStringList box_labels, QPointF pos);
        void setPos(QPointF npos);
        QPointF getPos();
        void draw(QGraphicsScene *canvas);
        void scale(qreal factor);
        QPointF scenePos();
        QPointF mapToScene(QPointF point);
        QPointF mapFromScene(QPointF point);

    private:
        QPointF pos;
        QList<QColor> lines;
        QStringList labels;
        QGraphicsItemGroup *elements;
};
