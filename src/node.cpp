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
    bound = new QGraphicsEllipseItem();
    ((QGraphicsEllipseItem*)bound)->setBrush(Qt::white);

    /* draw text */
    text = new QGraphicsSimpleTextItem(info);

    /* get bounding box from text */
    QRectF bbox = text->boundingRect();
    
    /* borders TODO: config */
    int bx = 10,
        by = 10;

    bbox.setSize(bbox.size() + QSizeF(bx, by));
    ((QGraphicsEllipseItem*)bound)->setRect(bbox);
    
    /* adjust position to center in terms of bounding box and border */
    text->setPos(coord - QPointF((bbox.width() - bx)/2, (bbox.height() - by)/2));
    bound->setPos(coord - QPointF(bbox.width()/2, bbox.height()/2));

    /* add both to group */
    nodeView = new QGraphicsItemGroup();
    nodeView->addToGroup(bound);
    nodeView->addToGroup(text);
    nodeView->setZValue(1);

    canvas->addItem(nodeView);
    
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
