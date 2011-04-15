#include "tree.h"

Tree::Tree(QString line, int tree_id, float tree_fitness)
{
    id = tree_id;
    fitness = tree_fitness;
    root = parseTree(line.split(QRegExp("\\s+")).replaceInStrings(QRegExp("\\s+"), " "), 0);
}

/* draw multiple trees */
void Tree::drawMany(QGraphicsScene *canvas, QList<Tree*> trees, QPointF coord, int step)
{
    int turnDepth = 1,
        maxDepth = 1;
    /* start from trees root */
    QList<Node*> nodes;
    foreach(Tree *tree, trees)
    {
        nodes.append(tree->getRoot());
    }

    /* make new tree */
    Tree *merged;
    int depth = Node::recursiveDrawMany(canvas, nodes, coord, coord, step, 1, pi, pi/2);
}

/* calls recursiveDraw */
/* TODO: add pens, brushes and constants */
void Tree::draw(QGraphicsScene *canvas, QPointF coord, int step)
{
    int depth = root->recursiveDraw(canvas, coord, coord, step, 1, pi, pi/2);
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

Node* Tree::getRoot()
{
    return root;
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
    QString line1 = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    QString line2 = QString("-  x^2  /  -  x^0  +  x^0  x^1  /  x^2  x^2");
    Tree *test_tree1 = new Tree(line1, -1, float(0.22222));
    Tree *test_tree2 = new Tree(line2, -2, float(0.22222));

    QPointF center = canvas->sceneRect().center();
    qDebug() << "Tree::test center" << center;

    test_tree1->draw(canvas, center, 60);
    //QList<Tree*> trees;
    //trees.append(test_tree1);
    //trees.append(test_tree2);
    //Tree::drawMany(canvas, trees, center, 60);
}
