#ifndef NODE_H 
#define NODE_H

#include <QBrush>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QList>
#include <QObject>
#include <QPointF>
#include <QString>

enum NodeType {OP, TERM, VAR};

class Node
{
    public:
        Node(NodeType type, QString info);

        void addSon(Node *son);
        void draw(QGraphicsScene *canvas, QPointF coord);

        QList<Node*> getSons();
        NodeType getType();
        QString getInfo();

    private:
        NodeType type;
        QString info;
        QList<Node*> sons;
        
        QGraphicsItem *text;
        QGraphicsItem *bound;
        QGraphicsItemGroup nodeView;
};

#endif
