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

#include "refbox.h"

extern Style* style;

Refbox::Refbox(QList<QColor> box_lines, QStringList box_labels, QPointF box_pos)
{
    lines = box_lines;
    labels = box_labels;
    pos = box_pos;
    elements = NULL;
}

void Refbox::setPos(QPointF npos)
{
    pos = npos;
    if(elements != NULL)
        elements->setPos(pos);
}

QPointF Refbox::getPos()
{
    if(elements != NULL)
        return elements->pos();
    else
        return pos;
}

QPointF Refbox::scenePos()
{
    return elements->pos();
}

QPointF Refbox::mapToScene(QPointF point)
{
    return elements->mapToScene(point);
}

QPointF Refbox::mapFromScene(QPointF point)
{
    return elements->mapFromScene(point);
}

void Refbox::draw(QGraphicsScene *canvas)
{
    QPointF pointPos = QPointF(0, 0);

    elements = new QGraphicsItemGroup();
    QGraphicsRoundRectItem *bound = new QGraphicsRoundRectItem(pointPos.x(), pointPos.y(), 2, 2);
    bound->setRadius(style->refboxBorderRadius);
    bound->setBrush(style->refboxBGColor);
    bound->setPen(style->refboxBorder);
    elements->addToGroup(bound);

    pointPos += QPointF(style->refboxPadding, style->refboxPadding);

    float maxTextLen = FLT_MIN;
    float boxHeight = 0;
    for(int i = 0; i < lines.length(); i++)
    {
        QGraphicsTextItem *text = new QGraphicsTextItem(labels[i]);
        QRectF bbox = text->boundingRect();
        QGraphicsLineItem *line = new QGraphicsLineItem(pointPos.x(),
                                                        pointPos.y() + bbox.height()/2,
                                                        pointPos.x() + style->refboxLineLen,
                                                        pointPos.y() + bbox.height()/2);
        line->setPen(QPen(lines[i], style->edgeWeight));
        text->setPos(QPointF(pointPos.x() + style->refboxLineLen + style->refboxPadding, pointPos.y()));
        if(bbox.width() > maxTextLen)
            maxTextLen = bbox.width();

        elements->addToGroup(text);
        elements->addToGroup(line);

        pointPos += QPointF(0, bbox.height() * 1.5);
        if(i < lines.length() - 1)
            boxHeight += bbox.height() * 1.5;
        else
        {
            boxHeight += bbox.height();
            maxTextLen += style->refboxPadding;
        }
    }

    /* case where no trees are drawn */
    if(lines.length() == 0)
    {
        QGraphicsTextItem *text = new QGraphicsTextItem("No trees drawn");
        QRectF bbox = text->boundingRect();
        text->setPos(QPointF(pointPos.x(), pointPos.y()));
        maxTextLen = bbox.width() - style->refboxLineLen;
        boxHeight = bbox.height();
        elements->addToGroup(text);
    }

    bound->setRect(0, 0, 
                   maxTextLen + style->refboxLineLen + style->refboxPadding * 2,
                   boxHeight + style->refboxPadding * 2);
    elements->setFlags(QGraphicsItem::ItemIsMovable);
    elements->setZValue(style->refboxZValue);
    elements->setToolTip("Click to drag");
    elements->setCursor(Qt::SizeAllCursor);
    canvas->addItem(elements);

    elements->setPos(pos);
}

void Refbox::scale(qreal factor)
{
    /* store position relative to Viewport */
    QPointF old_pos = elements->mapToScene(elements->boundingRect().center());
    elements->scale(factor, factor);
    QPointF new_pos = elements->mapToScene(elements->boundingRect().center());
    elements->setPos(elements->pos() - (new_pos - old_pos));
}
