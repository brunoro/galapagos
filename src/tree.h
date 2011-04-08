#ifndef TREE_H
#define TREE_H

#include "def.h"
#include "node.h"

#include <QDebug>
#include <QList>
#include <QPointF>
#include <QString>
#include <QStringList>
#include <QVector>

class Tree
{
    public:
        Tree(QString line, int tree_id, float tree_fitness);

        void draw(QPointF coord, int radius);
        void addParent(Tree *parent);
        void addOffspring(Tree *offspring);

        int id;
        float fitness;
        Node *root;
        QList<Node*> index;
        QList<Tree*> parents;
        QList<Tree*> offspring;
        
        Node *parseTree(QStringList nodes, int pos);
        
        static void test();

        /* TODO: make gets/sets */
};

#endif
