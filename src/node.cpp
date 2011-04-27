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
    QVector<bool> toRemove(nodes.length());
    /* iterates on nodes search for duplicates */
    for(int i = 0; i < nodes.length(); i++)
    {
        toRemove[i] = false;
        for(int j = i + 1; j < nodes.length(); j++)
        {
            QSet<int> intersection = nodes[i]->getTreeId() & nodes[j]->getTreeId();
            /* if it is duplicate, merge */
            if( (*(nodes[i]) == *(nodes[j])) && intersection.isEmpty())
            {
                qDebug() << "Node::recursiveDrawMany nodes are equal " << nodes[i]->getInfo() << nodes[j]->getInfo();
                Node *old_j = nodes[j];
                nodes[j] = new Node(old_j->getType(), old_j->getInfo());

                nodes[j]->addTreeId(nodes[i]->getTreeId());
                nodes[j]->addTreeId(old_j->getTreeId());
                foreach(Node *son, nodes[i]->getSons())
                    nodes[j]->addSon(son);
                foreach(Node *son, old_j->getSons())
                    nodes[j]->addSon(son);

                /* mark as merged */
                toRemove[i] = true;
            }
        }
    }

    /* remove doubles */
    QList<Node*> merged;
    for(int i = 0; i < nodes.length(); i++)
    {
        if(!toRemove[i])
            merged.append(nodes[i]);
    }

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
            foreach(int id, son->getTreeId())
                merged[i]->addEdge(son, colors.value(id));
        }
        merged[i]->updateEdges(canvas);

    }
    return merged;
}


void Node::updateEdges(QGraphicsScene *canvas)
{
    // TODO: get offset based on edges from same nodes
    foreach(QList<Edge*> sonEdges, edges)
    {
      if(sonEdges.length() == 0)
            continue;

        // TODO: use distance from style
        int edgeDistance = Style::edgeDistance;
        float offset = - sonEdges.length() * edgeDistance / 2;
        for(int i = 0; i < sonEdges.length(); i++)
        {
            offset += edgeDistance;
            sonEdges[i]->drawOffset(canvas, offset);
        }
    }
}

/* TODO: add brushes and pens */
void Node::draw(QGraphicsScene *canvas, QPointF coord)
{
    QRectF bbox = text->boundingRect();
    QSizeF size = Style::nodeSize;

    /* centering */
    ((QGraphicsEllipseItem*)bound)->setRect(QRectF(bbox.topLeft(), size));

    bound->setPos(coord - QPointF(size.width()/2, size.height()/2));
    bound->setZValue(1);
    canvas->addItem(bound);

    text->setPos(coord - QPointF(bbox.width()/2, bbox.height()/2));
    text->setZValue(2);
    canvas->addItem(text);

    pos = coord;
    qDebug() << "Node::draw" << coord << " " << bbox << info;
}

void Node::update(QPointF coord)
{   
    QRectF bbox = text->boundingRect();
    QSizeF size = Style::nodeSize;

    /* centering */
    ((QGraphicsEllipseItem*)bound)->setRect(QRectF(bbox.topLeft(), size));

    bound->setPos(coord - QPointF(size.width()/2, size.height()/2));
    text->setPos(coord - QPointF(bbox.width()/2, bbox.height()/2));

    pos = coord;
}

void Node::addEdge(Node *son, QColor color)
{
    /* connect son */
    Edge *edge = new Edge(this, son, color);
    edges[*son].append(edge);
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

QSet<int> Node::getTreeId()
{
    return tree_id;
}

void Node::setCoord(QPointF node_coord)
{
    pos = node_coord;
}

void Node::addTreeId(QSet<int> ids)
{
    tree_id += ids;
}

void Node::setTreeId(QSet<int> ids)
{
    tree_id = ids;
}
