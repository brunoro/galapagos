#ifndef TREE_H
#define TREE_H

#include "def.h"
#include "node.h"
#include "edge.h"

#include <cmath>

#include <QDebug>
#include <QGraphicsLineItem>
#include <QList>
#include <QPen>
#include <QPointF>
#include <QString>
#include <QStringList>
#include <QVector>

const float pi = 3.14159265;

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

        static void drawMany(QGraphicsScene *canvas, QList<Tree*> trees);
        static void test(QGraphicsScene *canvas);

    private:
        int id;
        float fitness;
        Node *root;
        QList<Node*> index;
        QList<Edge*> edges;
        QList<Tree*> parents;
        QList<Tree*> offspring;
        
        int recursiveDraw(QGraphicsScene *canvas, Node *node, 
                          QPointF origin, QPointF coord,
                          int step, int level,
                          float areaAngle, float refAngle);
        Node *parseTree(QStringList nodes, int pos);
};

#endif
