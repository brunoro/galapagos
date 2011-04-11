#ifndef TREE_H
#define TREE_H

#include "def.h"
#include "node.h"

#include <cmath>

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

        void draw(QGraphicsScene *canvas, QPointF coord, int step);
        void addParent(Tree *parent);
        void addOffspring(Tree *offspring);

        int getId();
        float getFitness();
        QList<Node*> getIndex();
        QList<Tree*> getParents();
        QList<Tree*> getOffspring();
        
        static void test(QGraphicsScene *canvas);

    private:
        int id;
        float fitness;
        Node *root;
        QList<Node*> index;
        QList<Tree*> parents;
        QList<Tree*> offspring;
        
        void recursiveDraw(QGraphicsScene *canvas, Node *node, 
                           QPointF coord, int step, 
                           float areaAngle, float refAngle);
        Node *parseTree(QStringList nodes, int pos);
};

#endif
