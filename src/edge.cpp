#include "edge.h"

Edge::Edge(Node *from, Node *to, QPen edge_style)
{
    style = edge_style;
    dest = from;
    dir = to;
    line = new QGraphicsLineItem();
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
}

void Edge::draw(QGraphicsScene *canvas)
{
    update();
    line->setPen(style);
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::drawOffset(QGraphicsScene *canvas, float offset)
{
    update();

    /* translates edge based on its normal vector */
    QLineF translated = line->line();
    float angleRad = translated.angle() * Style::pi / 180;
    translated.translate(offset * sinf(angleRad), offset * cosf(angleRad));
    line->setLine(translated);

    line->setPen(style);
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::update()
{
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
    //qDebug() << "Edge::update" << QLineF(dest->getCoord(), dir->getCoord());
}
