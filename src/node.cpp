#include "node.h"

Node::Node(NodeType nodetype, QString nodeinfo)
{
    type = nodetype;
    info = nodeinfo;

    bound = new QGraphicsEllipseItem();
    text = new QGraphicsSimpleTextItem(info);

    ((QGraphicsEllipseItem*)bound)->setBrush(Qt::white); // TODO: change this

    bound->setPos(0, 0);
    text->setPos(0, 0);
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
        addEdge(sons[i], edgecolor);

        /* get max depth */
        if(sonLevel > maxLevel)
            maxLevel = sonLevel;
    }
    updateEdges(canvas);

    return maxLevel + 1;
}

/* TODO: draw joint tree */
QList<Node*> Node::recursiveDrawMany(QGraphicsScene *canvas, QList<Node*> nodes,
                                     QPointF origin, QPointF coord,
                                     int step, int level,
                                     float areaAngle, float refAngle, QHash<int, QColor> colors)
{
    QList<Node*> merged(nodes);
    QVector<bool> toRemove(merged.length());
    /* iterates on nodes search for duplicates */
    for(int i = 0; i < nodes.length(); i++)
    {
        toRemove[i] = false;
        for(int j = i + 1; j < nodes.length(); j++)
        {
            if(nodes[i]->getTreeId() != nodes[j]->getTreeId())
            {
                qDebug() << "Node::recursiveDrawMany not same treeId " << nodes[i]->getTreeId() << nodes[j]->getTreeId();
                /* if it is duplicate, merge */
                if((*(nodes[i])) == (*(nodes[j])))
                {
                    qDebug() << "Node::recursiveDrawMany nodes are equal " << nodes[i]->getInfo() << nodes[j]->getInfo();
                    merged[j] = new Node(nodes[i]->getType(), nodes[i]->getInfo());
                    merged[j]->setTreeId(nodes[i]->getTreeId());
                    foreach(Node *son, nodes[i]->getSons())
                    {
                        merged[j]->addSon(son);
                    }
                    foreach(Node *son, nodes[j]->getSons())
                    {
                        merged[j]->addSon(son);
                    }
                    /* mark as merged */
                    toRemove[i] = true;
                }

                /* if not, leaves it */
                else
                {
                    foreach(Node *son, nodes[i]->getSons())
                    {
                        merged[i] = new Node(nodes[i]->getType(), nodes[i]->getInfo());
                        merged[i]->addSon(son);
                        merged[i]->setTreeId(nodes[i]->getTreeId());
                    }
                }

            }
        }
    }

    /* remove doubles */
    QList<Node*> merged_new;
    for(int i = 0; i < merged.length(); i++)
    {
        if(!toRemove[i])
            merged_new.append(merged[i]);
    }
    merged = merged_new;

    float hstep = areaAngle / merged.length();
    float sonAngle = refAngle - areaAngle/2 + hstep/2;
    for(int i = 0; i < merged.length(); i++)
    {
        /* get son position */
        QPointF sonCoord(origin.x() + level * step * cosf(sonAngle),
                         origin.y() + level * step * sinf(sonAngle));
        /* draw node and call recursion */
        merged[i]->draw(canvas, sonCoord);
        merged[i]->setSons(Node::recursiveDrawMany(canvas, merged[i]->getSons(), origin, sonCoord,
                           step, level + 1, hstep, sonAngle, colors));
        sonAngle += hstep;

        /* update edges */
        foreach(Node *son, merged[i]->getSons())
        {
            merged[i]->addEdge(son, colors.value(son->getTreeId()));
            qDebug() << "Color" << colors.value(son->getTreeId());
        }
        merged[i]->updateEdges(canvas);

    }
    return merged;
}


void Node::updateEdges(QGraphicsScene *canvas)
{
    foreach(Edge *edge, edges)
    {
        edge->draw(canvas);
    }
}

/* TODO: add brushes and pens */
void Node::draw(QGraphicsScene *canvas, QPointF coord)
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
    bound->setZValue(1);
    canvas->addItem(bound);

    text->setPos(coord - QPointF((bbox.width() - bx)/2, (bbox.height() - by)/2));
    text->setZValue(2);
    canvas->addItem(text);

    pos = coord;
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
    pos = coord;
}

void Node::addEdge(Node *son, QColor color)
{
    /* connect son */
    Edge *edge = new Edge(this, son, color);
    edges.append(edge);
}

QList<Node*> Node::getSons()
{
    return sons;
}

void Node::setSons(QList<Node*> node_sons)
{
    sons = node_sons;
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
    return pos;
}

int Node::getTreeId()
{
    return tree_id;
}

void Node::setCoord(QPointF node_coord)
{
    pos = node_coord;
}

void Node::setTreeId(int id)
{
    tree_id = id;
}
