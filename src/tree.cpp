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
    /* draw rings */
    recursiveDraw(canvas, root, coord, step, 180, 360);
    return;
}

/* draw all sons from a node */
void Tree::recursiveDraw(QGraphicsScene *canvas, Node *node, 
                         QPointF coord, int step, 
                         float areaAngle, float refAngle)
{
    /* draw node */
    node->draw(canvas, coord);

    QList<Node*> sons = node->getSons();
    float hstep = areaAngle / sons.length();
    for(int i = 0; i < sons.length(); i++ )
    {
        int pointIter = i - (sons.length() + 2) / 2 + 1; 
        /* get son position */
        QPointF sonCoord(coord.x() + step * cos(refAngle + pointIter * hstep),
                         coord.y() + step * sin(refAngle + pointIter * hstep));
        /* draw son */
        recursiveDraw(canvas, sons[i], sonCoord, step, hstep, refAngle + pointIter * areaAngle);
        qDebug() << "Tree::recursiveDraw" << i << pointIter << areaAngle << refAngle;
        /* connect son */
        canvas->addLine(QLineF(coord, sonCoord)); 
    }
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

    test_tree->draw(canvas, center, 40);
}
