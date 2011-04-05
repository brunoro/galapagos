#include "tree.h"

Tree::Tree(QString line, int tree_id)
{
    id = tree_id;
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
    Node *turn = NULL;

    if (definition.isOp(nodes[pos]))
    {
        turn = new Node(OP, nodes[pos]);
        turn->addSon(parseTree(nodes, ++pos));
        turn->addSon(parseTree(nodes, ++pos));
    }
    else if (definition.isVar(nodes[pos]))
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
        qDebug() << "invalid node: " << nodes[pos];
        exit(1);
    }
    
    index.append(turn);
    
    return turn;
}
