#ifndef EDGE_H
#define EDGE_H

#include "node.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QLineF>
#include <QPen>

class Node;
class Edge
{
    public:
        Edge(Node *from, Node *to, QColor edge_color);
        void draw(QGraphicsScene *canvas);
        void update();

    private:
        QColor color;
        QGraphicsLineItem line;
        Node *dest, *dir;
};

#endif
