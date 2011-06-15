#include "node.h"

Node::Node(NodeType nodetype, QString nodeinfo): tamToEllipse(4)
{
    type = nodetype;
    info = nodeinfo;

    bound = new QGraphicsEllipseItem();
    if(nodeinfo.size()<=tamToEllipse)
        text = new QGraphicsSimpleTextItem(info);
    else
        text = new QGraphicsSimpleTextItem(nodeinfo.replace(tamToEllipse, nodeinfo.size()-tamToEllipse,".."));

    ((QGraphicsEllipseItem*)bound)->setBrush(Qt::white); // TODO: change this

    bound->setPos(0, 0);
    text->setPos(0, 0);
}

Node::~Node()
{
    foreach(Node* son, sons){
        delete son;
        son = NULL;
    }
    
    foreach(QList<Edge*> edge, edges)
    {
        foreach(Edge *ed, edge)
        {
            delete ed;
            ed = NULL;
        }
    }
    
    delete text;
    text = NULL;

    delete bound;
    bound = NULL;
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

bool Node::collidesWith(Node *other)
{
    QPointF from = getCoord(),
            to = other->getCoord();
    float dist = sqrtf(powf(from.x() - to.x(), 2) + powf(from.y() - to.y(), 2));
    return (dist <= Style::nodeSize.width());
}

void Node::adjustPosition(QPointF origin, Node *other, int level, int step)
{
    /* get how much they collide */
    QPointF from = getCoord(),
            to = other->getCoord();
    float dist = sqrtf(powf(from.x() - to.x(), 2) + powf(from.y() - to.y(), 2));
    float diff = Style::nodeSize.width() - dist;

    /* get new ideal angle */
    float angleDelta = TO_DEGREES(acosf(1 - powf(diff, 2) / (2 * powf(step * level, 2))));
    float angleOrigin = QLineF(origin, from).angle();
    
    /* adjust position of all the subtrees so they won't collide */
    rotateSubtree(origin, angleOrigin - angleDelta);
}

void Node::rotateSubtree(QPointF origin, float angle)
{
    /* rotate ray vector */
    QLineF ray(origin, getCoord());
    ray.setAngle(angle);
    update(ray.p2());

    /* rotate sons */
    foreach(Node *son, sons)
        son->rotateSubtree(origin, angle);
}

/* draw joint tree */
QList<Node*> Node::recursiveDrawMany(QGraphicsScene *canvas, QList<Node*> nodes,
                                     QPointF origin, QPointF coord,
                                     int step, int level,
                                     float areaAngle, float refAngle, QHash<int, QPen> styles, int consensusDepth)
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
                foreach(Node *son, nodes[j]->getSons()){
                    nodes[i]->addSon(son);
                    son = NULL;
                }
                foreach(Node *son, old_i->getSons()){
                    nodes[i]->addSon(son);
                    son = NULL;
                }

                /* mark as merged and do not merge this node again */
                toRemove[j] = true; 
           
                //TODO: we should delete the old_i...but now i am getting segfault here
                //delete old_i;
                old_i = NULL;
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
    if(level < consensusDepth)
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
                               step, level + 1, hstep, sonAngle, styles, consensusDepth));
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
            if(level >= consensusDepth)
            {
                sonAngle += hstep;
                continue;
            }
        }
        if(level < consensusDepth)
            sonAngle += hstep;
    }
    return merged;
}

void Node::opsConsensus(int depth, Def* definition)
{
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
            turn->opsConsensus(depth - 1, definition);

            addSon(turn);
        }
    }
}

void Node::updateEdges(QGraphicsScene *canvas)
{
    // TODO: get offset based on edges from same nodes
    foreach(QList<Edge*> sonEdges, edges)
    {
        if(sonEdges.length() == 0)
              continue;

        // TODO: use distance from style
        /* avoid edges passing nodes bound */
        float edgeDistance;
        if(sonEdges.length() * Style::edgeDistance < Style::nodeSize.width())
            edgeDistance = Style::edgeDistance;
        else
            edgeDistance = Style::nodeSize.width() / sonEdges.length();
        float offset = - (sonEdges.length() - 1) * edgeDistance / 2;
        for(int i = 0; i < sonEdges.length(); i++)
        {
            sonEdges[i]->drawOffset(canvas, offset);
            offset += edgeDistance;
        }
    }
}

void Node::recursiveUpdateEdges(QGraphicsScene *canvas)
{
    updateEdges(canvas);
    foreach(Node *son, sons)
        son->recursiveUpdateEdges(canvas);
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
    bound->setZValue(Style::nodeZValue);
    if(info.size() > tamToEllipse)
        bound->setToolTip(info);
    canvas->addItem(bound);

    text->setPos(coord - QPointF(bbox.width()/2, bbox.height()/2));
    text->setZValue(Style::nodeZValue);
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
