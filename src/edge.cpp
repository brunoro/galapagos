#include "edge.h"

Edge::Edge(Node *from, Node *to, QColor edge_color)
{
    color = edge_color;
    dest = from;
    dir = to;
}

void Edge::draw(QGraphicsScene *canvas)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(dest->getCoord(), dir->getCoord()));
    line->setPen(QPen(color));
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::update()
{
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
}
