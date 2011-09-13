#include "tree.h"

extern Style* style;

Def* Tree::definition = NULL;

Tree::Tree(QString line, int tree_id, float tree_fitness)
{
    id = tree_id;
    fitness = tree_fitness;
    QStringList str_list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    //qDebug() << "Tree::Tree" << str_list;
    int *pos = new int;
    *pos = 0;
    root = parseTree(str_list.replaceInStrings(QRegExp("\\s+"), " "), pos, id);
}

Tree::Tree(int tree_id, float tree_fitness)
{
    id = tree_id;
    fitness = tree_fitness;
}

Tree::~Tree()
{
    delete root;
    root = NULL;

    while (!parents.isEmpty())
        delete parents.takeFirst();

    while (!offspring.isEmpty())
        delete offspring.takeFirst();

    if(definition){
        delete definition;
        definition = NULL;
    }
}

Node *Tree::parseTree(QStringList nodes, int *pos, int id)
{
    Node *turn = NULL;

    int degree = definition->nodeDegree(nodes[*pos]);
    /* operator */
    if (degree > 0)
    {
        turn = new Node(OP, nodes[*pos]);
        *pos += 1;
        for(int i = 0; i < degree; i++)
            turn->addSon(parseTree(nodes, pos, id));
    }
    /* terminal */
    else if (degree == 0)
    {
        turn = new Node(TERM, nodes[*pos]);
        *pos += 1;
    }

    /* TODO: raise exception */
    else
    {
        //qDebug() << "Tree::parseTree invalid node" << nodes[*pos] << degree;
        exit(1);
    }

    QSet<int> tree_id;
    tree_id.insert(id);
    turn->setTreeId(tree_id);
    
    return turn;
}

/* draw multiple trees */
Tree* Tree::drawMany(QGraphicsScene *canvas, QList<Tree*>& trees, QPointF coord, int step, bool treatCollisions, int consensusDepth)
{
    canvas->clear();

    float pi = Style::pi;
    /* start from trees root */
    QList<Node*> nodes;
    QList<int> ids;
    bool needRoot = false;
    foreach(Tree *tree, trees)
    {
        if(tree->getId() == CONSENSUS_ID)
            nodes.append(tree->root->getSons());
        else
        {
           nodes.append(tree->getRoot());
           ids.append(tree->getId());
        }

        if(trees.first()->getId() == CONSENSUS_ID)
        {
            needRoot = true;
            continue;
        }
        foreach(Node *node, nodes)
        {
            if(!((*tree->getRoot()) == (*node)))
                needRoot = true;
        }
    }

    /* set styles adjusting ids to be unique */
    QHash<int, QPen> styles;
    QList<QColor> colors = style->getColorPalette(ids.length());
    for(int i = 0; i < ids.length(); i++)
        styles[ids[i]] = QPen(QBrush(colors[i]), style->edgeWeight);

    /* make new tree */
    Tree *merged = new Tree(GENERATED_TREE_ID, GENERATED_TREE_FITNESS);

    /* draw common root if needed */
    int startLevel;
    if(needRoot)
    {
        startLevel = 1;
        Node *root = new Node(ROOT, "");
        root->draw(canvas, coord);
        merged->setRoot(root);
    }
    else
        startLevel = 0;

    QList<Node*> sons = Node::recursiveDrawMany(canvas, nodes, coord, coord, step, startLevel, 2 * pi, pi/2, styles, consensusDepth);

    if(!needRoot && (sons.length() > 0) )
        merged->setRoot(sons[0]);
    foreach(Node *son, sons)
    {
        merged->getRoot()->addSon(son);
        foreach(int id, son->getTreeId())
        {
            /* avoid drawing edge to consensus */
            if(id == CONSENSUS_ID)
                continue;
            merged->getRoot()->addEdge(son, styles.value(id))->draw(canvas);
            //qDebug() << "Tree::drawMany " << styles.value(id) << id;
        }
    }
    if(treatCollisions)
    {
        merged->correctCollisions(coord, step);
        merged->getRoot()->recursiveUpdateEdges();
    }
    else
        merged->getRoot()->updateEdges();

    Tree::drawRings(canvas, coord, MAX_DEPTH, step);
    
    return merged;
}

/* calls recursiveDraw */
void Tree::draw(QGraphicsScene *canvas, QPointF coord, int step)
{
    canvas->clear();

    float pi = Style::pi;
    int depth = root->recursiveDraw(canvas, coord, coord, step, 1, 2 * pi, pi/2);
    //qDebug() << "Tree::draw depth" << depth;

    Tree::drawRings(canvas, coord, MAX_DEPTH, step);

}

Tree *Tree::opsConsensusTree(int consensusDepth)
{
    Tree *opCon = new Tree(CONSENSUS_ID, GENERATED_TREE_FITNESS);

    Node *root = new Node(ROOT, "");
    opCon->setRoot(root);

    QSet<int> conid;
    conid.insert(CONSENSUS_ID);
    root->setTreeId(conid);

    root->opsConsensus(consensusDepth, Tree::definition);

    return opCon;
}

void Tree::correctCollisions(QPointF origin, int step)
{
    /* use BFS to get nodes */
    QList<Node*> thisLevel = root->getSons();
    QList<Node*> nextLevel;
    int level = 1;
    do
    {
        for(int i = 0; i < thisLevel.length(); i++)
        {
            /* avoid consensus */
            if(thisLevel[i]->getTreeId().contains(CONSENSUS_ID) && (thisLevel[i]->getTreeId().size() <= 1))
                continue;

            for(int j = i + 1; j < thisLevel.length(); j++)
            {
                /* avoid consensus */
                if(thisLevel[j]->getTreeId().contains(CONSENSUS_ID) && (thisLevel[j]->getTreeId().size() <= 1))
                    continue;

                /* check if nodes i and j collide and adjust */
                if(thisLevel[i]->collidesWith(thisLevel[j]))
                    thisLevel[j]->adjustPosition(origin, thisLevel[i], level, step);
            }
            /* get next level */
            nextLevel += thisLevel[i]->getSons();
        }
        thisLevel = nextLevel;
        nextLevel.clear();
        level++;
    } while(!thisLevel.isEmpty());
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
        ellipse->setPen(style->ringColor);
        ellipse->setZValue(0);
        canvas->addItem(ellipse);
    }
}

void Tree::scale(qreal factor)
{
    /* iterates on all nodes and edges reducing their sizes */ 
    root->scale(factor);
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

void Tree::setId(int tree_id)
{
    id = tree_id;
    QSet<int> id_set;
    id_set.insert(tree_id);
    root->recursiveSetTreeId(id_set);
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

QList<Tree*> Tree::getParents()
{
    return parents;
}

QList<Tree*> Tree::getOffspring()
{
    return offspring;
}

void Tree::test(QGraphicsScene *canvas, int consensusDepth)
{
    QString line1 = QString("-  x^0  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    QString line2 = QString("-  x^0  /  -  x^0  +  x^0  x^1  /  x^2  x^2");
    Tree *test_tree1 = new Tree(line1, 10, float(0.22222));
    Tree *test_tree2 = new Tree(line2, 20, float(0.22222));

    QPointF center = canvas->sceneRect().center();
    //qDebug() << "Tree::test center" << center;

    //test_tree1->draw(canvas, center, 60);
    QList<Tree*> trees;
    trees.append(test_tree1);
    trees.append(test_tree2);
    Tree *merged = Tree::drawMany(canvas, trees, center, 60, true, consensusDepth);
}
