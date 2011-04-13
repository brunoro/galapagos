#include "edge.h"

Edge::Edge(Node *from, Node *to)
{
    dest = from;
    dir = to;
}

void Edge::draw(QGraphicsScene *canvas)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(dest->getCoord(), dir->getCoord())); 
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::update()
{
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
}
