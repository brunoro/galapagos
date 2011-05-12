#ifndef EDGE_H
#define EDGE_H

#include "node.h"
#include "style.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QLineF>
#include <QPen>

class Node;
class Edge
{
    public:
        Edge(Node *from, Node *to, QPen style);
        void draw(QGraphicsScene *canvas);
        void drawOffset(QGraphicsScene *canvas, float offset);
        void update();

    private:
        QPen style;
        QGraphicsLineItem *line;
        Node *dest, *dir;
};

#endif
