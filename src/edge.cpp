#include "edge.h"

Edge::Edge(Node *from, Node *to, QColor edge_color)
{
    color = edge_color;
    dest = from;
    dir = to;
    line = new QGraphicsLineItem();
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
}

void Edge::draw(QGraphicsScene *canvas)
{
    update();
    line->setPen(QPen(color));
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::update()
{
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
}
