#include "edge.h"

Edge::Edge(Node *from, Node *to, QColor edge_color, Qt::PenStyle edge_style)
{
    color = edge_color;
    style = edge_style;
    dest = from;
    dir = to;
    line = new QGraphicsLineItem();
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
}

void Edge::draw(QGraphicsScene *canvas)
{
    update();
    line->setPen(QPen(QBrush(color), Style::edgeWeight, style));
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::drawOffset(QGraphicsScene *canvas, float offset)
{
    update();

    /* translates edge based on its normal vector */
    QLineF translated = line->line();
    double angleRad = (translated.normalVector().angle() * Style::pi) / 180;
    translated.translate(offset * cosf(angleRad), offset * sinf(angleRad));
    line->setLine(translated);

    line->setPen(QPen(QBrush(color), Style::edgeWeight, style));
    line->setZValue(0);
    canvas->addItem(line);
}

void Edge::update()
{
    line->setLine(QLineF(dest->getCoord(), dir->getCoord()));
    //qDebug() << "Edge::update" << QLineF(dest->getCoord(), dir->getCoord());
}
