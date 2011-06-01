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

Node::~Node()
{
    for(int i = 0; i < sons.length(); i++)
        delete sons[i];
    delete text;
    delete bound;
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
    //qDebug() << "Tree::recursiveDraw" << coord << " " <<  areaAngle << " " <<  refAngle;

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
        addEdge(sons[i], QPen(Style::edgeColor, Style::edgeWeight));

        /* get max depth */
        if(sonLevel > maxLevel)
            maxLevel = sonLevel;
    }
    updateEdges(canvas);

    return maxLevel + 1;
}

/* draw joint tree */
QList<Node*> Node::recursiveDrawMany(QGraphicsScene *canvas, QList<Node*> nodes,
                                     QPointF origin, QPointF coord,
                                     int step, int level,
                                     float areaAngle, float refAngle, QHash<int, QPen> styles)
{
    QVector<bool> toRemove(nodes.length());
    /* iterates on nodes search for duplicates */
    for(int i = 0; i < nodes.length(); i++)
        toRemove[i] = false;
    for(int i = 0; i < nodes.length(); i++)
    {
        if(toRemove[i])
            continue;
        for(int j = i + 1; j < nodes.length(); j++)
        {
            if(toRemove[j])
                continue;
            QSet<int> intersection = nodes[i]->getTreeId() & nodes[j]->getTreeId();
            /* if it is duplicate, merge */
            if( (*(nodes[j]) == *(nodes[i])) && intersection.isEmpty())
            {
                //qDebug() << "Node::recursiveDrawMany nodes are equal " << nodes[j]->getInfo() << nodes[i]->getInfo();
                Node *old_i = nodes[i];
                nodes[i] = new Node(old_i->getType(), old_i->getInfo());

                nodes[i]->setTreeId(nodes[j]->getTreeId());
                nodes[i]->addTreeId(old_i->getTreeId());
                foreach(Node *son, nodes[j]->getSons())
                    nodes[i]->addSon(son);
                foreach(Node *son, old_i->getSons())
                    nodes[i]->addSon(son);

                /* mark as merged and do not merge this node again */
                toRemove[j] = true; 
            }
        }
    }

    /* remove doubles */
    QList<Node*> merged;
    int drawables = 0;
    for(int i = 0; i < nodes.length(); i++)
    {
        if(!toRemove[i])
        {
            merged.append(nodes[i]);
            if(!(nodes[i]->getTreeId().contains(CONSENSUS_ID) && (nodes[i]->getTreeId().size() <= 1)))
                drawables++;
        }
    }

    float hstep;
    if(level < CONSENSUS_DEPTH)
        hstep = areaAngle / merged.length();
    else
        hstep = areaAngle / drawables;
    float sonAngle = refAngle - areaAngle/2 + hstep/2;

    for(int i = 0; i < merged.length(); i++)
    {
        /* avoid drawing consensus tree */
        if(!(merged[i]->getTreeId().contains(CONSENSUS_ID) && (merged[i]->getTreeId().size() <= 1)))
        {
            /* get son position */
            QPointF sonCoord(origin.x() + level * step * cosf(sonAngle),
                             origin.y() + level * step * sinf(sonAngle));
            /* draw node and call recursion */
            merged[i]->draw(canvas, sonCoord);
            merged[i]->setSons(Node::recursiveDrawMany(canvas, merged[i]->getSons(), origin, sonCoord,
                               step, level + 1, hstep, sonAngle, styles));
            /* update edges */
            foreach(Node *son, merged[i]->getSons())
            {
                foreach(int id, son->getTreeId())
                {
                    if(id == CONSENSUS_ID)
                        continue;
                    merged[i]->addEdge(son, styles.value(id));
                }
            }
            merged[i]->updateEdges(canvas);
            if(level >= CONSENSUS_DEPTH)
            {
                sonAngle += hstep;
                continue;
            }
        }
        if(level < CONSENSUS_DEPTH)
            sonAngle += hstep;
    }
    return merged;
}

void Node::opsConsensus(int depth)
{
    extern Def *definition;

    QSet<int> conid;
    conid.insert(CONSENSUS_ID);
    tree_id = conid;

    if(depth == 0)
        return;
    
    int degree = (type == ROOT) ? 1 : definition->isOp(info);
    /* insert as many times as the degree */
    for(int d = 0; d < degree; d++)
    {
        QHashIterator<QString, int> opIterator = definition->getOpIterator();
        while(opIterator.hasNext())
        {
            opIterator.next();

            /* make new node */
            Node *turn = new Node(OP, opIterator.key());
            turn->opsConsensus(depth - 1);

            addSon(turn);
        }
    }
}

void Node::correctCollisions(QGraphicsScene *canvas)
{
    /* iterates on son nodes 2 x 2 and apply delta d to move them */
    return;
}

void Node::updateEdges(QGraphicsScene *canvas)
{
    // TODO: get offset based on edges from same nodes
    foreach(QList<Edge*> sonEdges, edges)
    {
        if(sonEdges.length() == 0)
              continue;

        // TODO: use distance from style
        float edgeDistance = Style::edgeDistance;
        float offset = - (sonEdges.length() - 1) * edgeDistance / 2;
        for(int i = 0; i < sonEdges.length(); i++)
        {
            sonEdges[i]->drawOffset(canvas, offset);
            offset += edgeDistance;
        }
    }
}

void Node::draw(QGraphicsScene *canvas, QPointF coord)
{
    /* set colors */
    ((QGraphicsEllipseItem*)bound)->setBrush(QBrush(QColor(Style::defaultNodeColor)));
    ((QGraphicsEllipseItem*)bound)->setPen(QPen(QColor(Style::nodePenColor)));
    ((QGraphicsSimpleTextItem*)text)->setBrush(QBrush(QColor(Style::nodeTextColor)));

    /* set position */
    QRectF bbox = text->boundingRect();
    QSizeF size = Style::nodeSize;

    ((QGraphicsEllipseItem*)bound)->setRect(QRectF(bbox.topLeft(), size));
    bound->setPos(coord - QPointF(size.width()/2, size.height()/2));
    bound->setZValue(1);
    canvas->addItem(bound);

    text->setPos(coord - QPointF(bbox.width()/2, bbox.height()/2));
    text->setZValue(1);
    canvas->addItem(text);

    pos = coord;
    //qDebug() << "Node::draw" << coord << " " << bbox << info;
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

void Node::addEdge(Node *son, QPen style)
{
    /* connect son */
    Edge *edge = new Edge(this, son, style);
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

QGraphicsItem *Node::getBound()
{
    return bound;
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

void Node::recursiveSetTreeId(QSet<int> ids)
{
    tree_id = ids;
    for(int i = 0; i < sons.length(); i++)
        sons[i]->recursiveSetTreeId(ids);
}
