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

#include "edge.h"

extern Style* style;

Edge::Edge(Node *from, Node *to, QPen lineStyle)
{
    edgeStyle = lineStyle;
    dest = from;
    dir = to;
    line = new QGraphicsLineItem();
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
    offset = 0;
}

void Edge::setOffset(float newOffset)
{
    offset = newOffset;
}

float Edge::getOffset()
{
    return offset;
}

void Edge::draw(QGraphicsScene *canvas)
{
    /* set the correct points */
    update();
    
    /* translate edge based on its normal vector */
    QLineF translated = line->line();
    float angleRad = translated.angle() * Style::pi / 180;
    translated.translate(offset * sinf(angleRad), offset * cosf(angleRad));
    line->setLine(translated);

    /* set edgeStyle */
    edgeStyle.setCapStyle(Qt::RoundCap);
    line->setPen(edgeStyle);
    line->setZValue(style->edgeZValue);
    
    /* add it to scene */
    canvas->addItem(line);
}

void Edge::update()
{
    /* translate edge based on its normal vector */
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));

    QLineF translated = line->line();
    float angleRad = translated.angle() * Style::pi / 180;
    translated.translate(offset * sinf(angleRad), offset * cosf(angleRad));
    line->setLine(translated);
}

void Edge::scale(qreal factor)
{
    QPointF oldLineCenter = line->mapToScene(line->boundingRect().center());
    line->scale(factor, factor);
    QTransform lineScale = QTransform::fromScale(1/factor, 1/factor);
    line->setLine(line->line() * lineScale);
    QPointF newLineCenter = line->mapToScene(line->boundingRect().center());
    line->setPos(line->pos() - (newLineCenter - oldLineCenter));
    return;
}
