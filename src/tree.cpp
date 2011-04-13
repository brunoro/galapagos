#include "tree.h"

Tree::Tree(QString line, int tree_id, float tree_fitness)
{
    id = tree_id;
    fitness = tree_fitness;
    root = parseTree(line.split(QRegExp("\\s+")).replaceInStrings(QRegExp("\\s+"), " "), 0);
}

/* calls recursiveDraw */
/* TODO: add pens, brushes and constants */
void Tree::draw(QGraphicsScene *canvas, QPointF coord, int step)
{
    int depth = recursiveDraw(canvas, root, coord, coord, step, 1, pi, pi/2);
    qDebug() << "Tree::draw depth" << depth;

    /* draw rings */
    for(int i = 0; i < depth; i++)
    {
        int radius = i*step;
        QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(coord.x() - radius,
                                                                 coord.y() - radius,
                                                                 radius * 2,
                                                                 radius * 2);
        ellipse->setPen(QPen(Qt::gray));
        ellipse->setZValue(0);
        canvas->addItem(ellipse);
    }
}

/* draw all sons from a node */
int Tree::recursiveDraw(QGraphicsScene *canvas, Node *node, 
                        QPointF origin, QPointF coord,
                        int step, int level,
                        float areaAngle, float refAngle)
{
    /* draw node */
    node->draw(canvas, coord);
    qDebug() << "Tree::recursiveDraw" << coord << " " <<  areaAngle << " " <<  refAngle;

    int maxLevel = 0,
        sonLevel;

    QList<Node*> sons = node->getSons();
    float hstep = areaAngle / sons.length();
    float sonAngle = refAngle - areaAngle/2 + hstep/2;
    for(int i = 0; i < sons.length(); i++ )
    {
        /* get son position */
        QPointF sonCoord(origin.x() + level * step * cosf(sonAngle),
                         origin.y() + level * step * sinf(sonAngle));
        /* draw son */
        sonLevel = recursiveDraw(canvas, sons[i], origin, sonCoord, 
                                 step, level + 1,  hstep, sonAngle);
        sonAngle += hstep;
        
        /* connect son */
        Edge *edge = new Edge(node, sons[i]);
        edge->draw(canvas);

        /* get max depth */
        if(sonLevel > maxLevel)
            maxLevel = sonLevel;
    }
    return maxLevel + 1;
}

void Tree::addParent(Tree *parent)
{
    return;
}

void Tree::addOffspring(Tree *offspring)
{
    return;
}

Node *Tree::parseTree(QStringList nodes, int pos)
{
    extern Def *definition;
    Node *turn = NULL;

    if (definition->isOp(nodes[pos]))
    {
        turn = new Node(OP, nodes[pos]);
        turn->addSon(parseTree(nodes, ++pos));
        turn->addSon(parseTree(nodes, ++pos));
    }
    else if (definition->isVar(nodes[pos]))
    {
        turn = new Node(VAR, nodes[pos]);
    }
    else if (definition->isTerm(nodes[pos]))
    {
        turn = new Node(TERM, nodes[pos]);
    }

    /* TODO: raise exception */
    else
    {
        qDebug() << "Tree::parseTree invalid node" << nodes[pos];
        exit(1);
    }
    
    index.append(turn);
    
    return turn;
}

int Tree::getId()
{
    return id;
}

float Tree::getFitness()
{
    return fitness;
}

QList<Node*> Tree::getIndex()
{
    return index;
}

QList<Tree*> Tree::getParents()
{
    return parents;
}

QList<Tree*> Tree::getOffspring()
{
    return offspring;
}

void Tree::test(QGraphicsScene *canvas)
{
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));

    QPointF center = canvas->sceneRect().center();
    qDebug() << "Tree::test center" << center;

    test_tree->draw(canvas, center, 60);
}
