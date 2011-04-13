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
    ((QGraphicsEllipseItem*)bound)->setBrush(Qt::white); // TODO: change this

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
    bound->setPos(coord - QPointF(bbox.width()/2, bbox.height()/2));
    bound->setZValue(1);
    canvas->addItem(bound);

    text->setPos(coord - QPointF((bbox.width() - bx)/2, (bbox.height() - by)/2));
    text->setZValue(2);
    canvas->addItem(text);

    
    qDebug() << "Node::draw" << coord << " " << bbox << info;
    
    return;
}

void Node::update(QPointF coord)
{   
    /* get bounding box from text */
    QRectF bbox = text->boundingRect();
    
    /* borders TODO: config */
    int bx = 10,
        by = 10;

    bbox.setSize(bbox.size() + QSizeF(bx, by));
    ((QGraphicsEllipseItem*)bound)->setRect(bbox);
 
    /* adjust position to center in terms of bounding box and border */
    bound->setPos(coord - QPointF(bbox.width()/2, bbox.height()/2));
    text->setPos(coord - QPointF((bbox.width() - bx)/2, (bbox.height() - by)/2));
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

QPointF Node::getCoord()
{
    QRectF box = ((QGraphicsEllipseItem*)bound)->rect();
    return bound->pos() + QPointF(box.width()/2, box.height()/2);
}
