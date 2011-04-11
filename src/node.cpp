#include "node.h"

Node::Node(NodeType nodetype, QString nodeinfo)
{
    type = nodetype;
    info = nodeinfo;
}

void Node::addSon(Node *son)
{
    sons.append(son);
}

/* TODO: add brushes and pens */
void Node::draw(QGraphicsScene *canvas, QPointF coord)
{
    /* draw bound */
    bound = canvas->addEllipse(0, 0, 0, 0);
    bound->setPos(coord);

    /* draw text */
    text = canvas->addSimpleText(info, QFont("Trebuchet MS"));
    text->setPos(coord);

    /* get bounding box from text and set bound position */
    QRectF bbox = text->boundingRect();
    ((QGraphicsEllipseItem*)bound)->setRect(bbox);

    /* add both to group */
    nodeView.addToGroup(text);
    nodeView.addToGroup(bound);
    
    qDebug() << "Node::draw" << coord << " " << bbox << info;
    
    return;
}

QList<Node*> Node::getSons()
{
    return sons;
}

NodeType Node::getType()
{
    return type;
}

QString Node::getInfo()
{
    return info;
}
