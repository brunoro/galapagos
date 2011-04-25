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
    line->setPen(QPen(QBrush(color), 2)); // TODO: change this
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::drawOffset(QGraphicsScene *canvas, float offset)
{
    extern float pi;

    update();

    /* translates edge based on its normal vector */
    QLineF translated = line->line();
    double angleRad = (translated.normalVector().angle() * pi) / 180;
    translated.translate(offset * cosf(angleRad), offset * sinf(angleRad));
    line->setLine(translated);

    line->setPen(QPen(QBrush(color), 2)); // TODO: change this
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::update()
{
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
    qDebug() << "Edge::update" << QLineF(dest->getCoord(), dir->getCoord());
}
