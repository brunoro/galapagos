#include "tree.h"

Tree::Tree(QString line, int tree_id, float tree_fitness)
{
    id = tree_id;
    fitness = tree_fitness;
    root = parseTree(line.split(QRegExp("\\s+")), 0);
}

void Tree::draw(QPointF coord, int radius)
{
    return;
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
        qDebug() << "Tree::parseTree : invalid node " << nodes[pos];
        exit(1);
    }
    
    index.append(turn);
    
    return turn;
}

void Tree::test()
{
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));
    qDebug() << "Tree::test : " << test_tree->index;
}
