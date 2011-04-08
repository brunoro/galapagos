#ifndef NODE_H 
#define NODE_H

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
        void draw(QPointF coord);

    private:
        NodeType type;
        QString info;
        QList<Node*> sons;
        
        /* TODO: make gets/sets */
};

#endif
