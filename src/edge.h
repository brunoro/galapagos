#ifndef EDGE_H
#define EDGE_H

#include "node.h"

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QLineF>

class Node;
class Edge
{
    public:
        Edge(Node *from, Node *to);
        void draw(QGraphicsScene *canvas);
        void update();

    private:
        QGraphicsLineItem *line;
        Node *dest, *dir;
};

#endif
