#ifndef NODE_H 
#define NODE_H

#include <iostream>
#include <QList>
#include <QPointF>
#include <QString>

enum NodeType {OP, TERM, VAR};

class Node:
    public:
        Node(NodeType type, QString info, QObject *parent);

        void addSon(Node *son);
        void draw(QPointF coord);

    private:
        NodeType type;
        QString info;
        QList<Node*> sons;
        QObject *parentWidget;

#endif
