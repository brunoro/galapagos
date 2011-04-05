#ifndef TREE_H
#define TREE_H

#include "node.h"

#include <QList>
#include <QPointF>
#include <QString>
#include <QVector>

class Tree
{
    public:
        Tree(QString line, int tree_id, QObject *parent);

        void draw(QPointF coord, int radius);
        void addParent(Tree *parent);
        void addOffspring(Tree *offspring);

    private:
        int id;
        Node *root;
        QVector<Node*> index;
        QList<Tree*> parents;
        QList<Tree*> offspring;
};

#endif
