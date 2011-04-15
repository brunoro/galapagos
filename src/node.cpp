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

/* draw all sons from a node */
int Node::recursiveDraw(QGraphicsScene *canvas,
                        QPointF origin, QPointF coord,
                        int step, int level,
                        float areaAngle, float refAngle)
{
    /* draw node */
    draw(canvas, coord);
    qDebug() << "Tree::recursiveDraw" << coord << " " <<  areaAngle << " " <<  refAngle;

    int maxLevel = 0,
        sonLevel;

    float hstep = areaAngle / sons.length();
    float sonAngle = refAngle - areaAngle/2 + hstep/2;
    for(int i = 0; i < sons.length(); i++)
    {
        /* get son position */
        QPointF sonCoord(origin.x() + level * step * cosf(sonAngle),
                         origin.y() + level * step * sinf(sonAngle));
        /* draw son */
        sonLevel = sons[i]->recursiveDraw(canvas, origin, sonCoord, 
                                          step, level + 1, hstep, sonAngle);
        sonAngle += hstep;
        
        /* connect son */
        QColor edgecolor(0,0,255);
        Edge *edge = new Edge(this, sons[i], edgecolor);
        edges.append(edge);
        edge->draw(canvas);

        /* get max depth */
        if(sonLevel > maxLevel)
            maxLevel = sonLevel;
    }
    return maxLevel + 1;
}

/* TODO: make joint tree and draw afterwards */
int Node::recursiveDrawMany(QGraphicsScene *canvas, QList<Node*> nodes,
                            QPointF origin, QPointF coord,
                            int step, int level,
                            float areaAngle, float refAngle)
{
    return 0;
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

