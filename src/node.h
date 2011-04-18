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
#include <QHash>
#include <QList>
#include <QObject>
#include <QPointF>
#include <QString>

enum NodeType {OP, TERM, VAR, ROOT};
class Edge;
class Node
{

    public:
        Node(NodeType type, QString info);

        void addSon(Node *son);
        void addEdge(Node *son, QColor color);
        void draw(QGraphicsScene *canvas, QPointF coord);
        void updateEdges(QGraphicsScene *canvas);
        int recursiveDraw(QGraphicsScene *canvas,
                          QPointF origin, QPointF coord,
                          int step, int level,
                          float areaAngle, float refAngle);

        void update(QPointF coord);

        static QList<Node*> recursiveDrawMany(QGraphicsScene *canvas, QList<Node*> nodes,
                                              QPointF origin, QPointF coord,
                                              int step, int level,
                                              float areaAngle, float refAngle, QHash<int, QColor> colors);
        QList<Node*> getSons();
        NodeType getType();
        QString getInfo();
        QPointF getCoord();
        int getTreeId();

        void setCoord(QPointF node_coord);
        void setTreeId(int id);
        
        inline bool operator==(const Node &other) const
        {
            return (other.type == type) && (other.info == info);
        }

    private:
        int tree_id;
        NodeType type;
        QString info;
        QList<Node*> sons;
        QList<Edge*> edges;
        QGraphicsItem *text;
        QGraphicsItem *bound;
        QGraphicsItemGroup *nodeView;
};

#endif
