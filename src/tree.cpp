#include "tree.h"

Tree::Tree(QString line, int tree_id, float tree_fitness)
{
    id = tree_id;
    fitness = tree_fitness;
    QStringList str_list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    qDebug() << "Tree::Tree" << str_list;
    root = parseTree(str_list.replaceInStrings(QRegExp("\\s+"), " "), 0, id);
}

Tree::Tree(int tree_id, float tree_fitness)
{
    id = tree_id;
    fitness = tree_fitness;
}

Node *Tree::parseTree(QStringList nodes, int pos, int id)
{
    extern Def *definition;
    Node *turn = NULL;

    if (definition->isOp(nodes[pos]))
    {
        turn = new Node(OP, nodes[pos]);
        turn->addSon(parseTree(nodes, ++pos, id));
        turn->addSon(parseTree(nodes, ++pos, id));
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

    QSet<int> tree_id;
    tree_id.insert(id);
    turn->setTreeId(tree_id);

    index.append(turn);
    
    return turn;
}

/* draw multiple trees */
Tree* Tree::drawMany(QGraphicsScene *canvas, QList<Tree*> trees, QPointF coord, int step)
{
    float pi = Style::pi;
    /* start from trees root */
    QList<Node*> nodes;
    QList<int> ids;
    bool needRoot = false;
    foreach(Tree *tree, trees)
    {
        nodes.append(tree->getRoot());
        ids.append(tree->getId());
        foreach(Node *node, nodes)
        {
            if(!((*tree->getRoot()) == (*node)))
                needRoot = true;
        }
    }

    /* set styles */
    QHash<int, Qt::PenStyle> styles(Style::getPenStylePalette(ids));

    /* make new tree */
    Tree *merged = new Tree(-1, -1); // TODO: change this

    /* draw common root if needed */
    int startLevel;
    if(needRoot)
    {
        Node *root = new Node(ROOT, "root");
        root->draw(canvas, coord);
        merged->setRoot(root);
        startLevel = 1;
    }
    else
        startLevel = 0;

    QList<Node*> sons = Node::recursiveDrawMany(canvas, nodes, coord, coord, step, startLevel, 2 * pi, pi/2, styles);

    if(!needRoot)
        merged->setRoot(sons[0]);
    foreach(Node *son, sons)
    {
        merged->getRoot()->addSon(son);
        foreach(int id, son->getTreeId())
        {
            merged->getRoot()->addEdge(son, styles.value(id));
            qDebug() << "Tree::drawMany " << styles.value(id) << id;
        }
    }
    merged->getRoot()->updateEdges(canvas);

    Tree::drawRings(canvas, coord, 10, step); // TODO: get depth from merged tree
    return merged;
}

/* calls recursiveDraw */
/* TODO: add pens, brushes and constants */
void Tree::draw(QGraphicsScene *canvas, QPointF coord, int step)
{
    float pi = Style::pi;
    int depth = root->recursiveDraw(canvas, coord, coord, step, 1, 2 * pi, pi/2);
    qDebug() << "Tree::draw depth" << depth;

    Tree::drawRings(canvas, coord, depth, step);

}

void Tree::drawRings(QGraphicsScene *canvas, QPointF coord, int depth, int step)
{
    for(int i = 0; i < depth; i++)
    {
        int radius = i*step;
        QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(coord.x() - radius,
                                                                 coord.y() - radius,
                                                                 radius * 2,
                                                                 radius * 2);
        ellipse->setPen(QPen(Qt::gray)); // TODO: change this
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


void Tree::setRoot(Node *node)
{
    root = node;
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
    QString line1 = QString("-  x^0  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    QString line2 = QString("-  x^0  /  -  x^0  +  x^0  x^1  /  x^2  x^2");
    Tree *test_tree1 = new Tree(line1, 10, float(0.22222));
    Tree *test_tree2 = new Tree(line2, 20, float(0.22222));

    QPointF center = canvas->sceneRect().center();
    qDebug() << "Tree::test center" << center;

    //test_tree1->draw(canvas, center, 60);
    QList<Tree*> trees;
    trees.append(test_tree1);
    trees.append(test_tree2);
    Tree *merged = Tree::drawMany(canvas, trees, center, 60);
}
