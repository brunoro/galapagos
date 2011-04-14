#ifndef NODE_H 
#define NODE_H

#include "edge.h"

#include <cmath>

#include <QBrush>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QList>
#include <QObject>
#include <QPointF>
#include <QString>

enum NodeType {OP, TERM, VAR};
class Edge;
class Node
{

    public:
        Node(NodeType type, QString info);

        void addSon(Node *son);
        void draw(QGraphicsScene *canvas, QPointF coord);
        int recursiveDraw(QGraphicsScene *canvas,
                          QPointF origin, QPointF coord,
                          int step, int level,
                          float areaAngle, float refAngle);

        void update(QPointF coord);

        static int recursiveDrawMany(QGraphicsScene *canvas, QList<Node*> nodes, 
                                     QPointF origin, QPointF coord,
                                     int step, int level,
                                     float areaAngle, float refAngle);
        QList<Node*> getSons();
        NodeType getType();
        QString getInfo();
        QPointF getCoord();
        
        inline bool operator==(const Node &other) const
        {
            return (other.type == type) && (other.info == info);
        }

    private:
        NodeType type;
        QString info;
        QList<Node*> sons;
        QList<Edge*> edges;
        QGraphicsItem *text;
        QGraphicsItem *bound;
        QGraphicsItemGroup *nodeView;
};

#endif
