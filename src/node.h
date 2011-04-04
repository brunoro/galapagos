#include <iostream>
#include <QString>
#include <QVector>
#include <QPointF>

enum NodeType {OP, TERM, VAR};

class Node:
    public:
        Node(NodeType type, QString info, QObject *parent);

    private:
        NodeType type;
        QString info;
        QVector<Node*> sons;
        QObject *parentWidget;
        
        void addSon(Node *son);
        void draw(QPointF coord);
