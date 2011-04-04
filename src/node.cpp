#include "node.h"

Node::Node(NodeType nodetype, QString nodeinfo, QObject *parent)
{
    type = nodetype;
    info = nodeinfo;
    parentWidget = parent;

    if(type == OP)
        sons = new QList();
}

void Node::addSon(Node *son)
{
    sons.append(son);
}

void Node::draw(QPointF coord)
{
    return
}
