#ifndef TREE_H
#define TREE_H

#include "def.h"
#include "node.h"

#include <cfloat>

#include <QDebug>
#include <QGraphicsLineItem>
#include <QHash>
#include <QList>
#include <QPen>
#include <QPointF>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>

class Tree
{
    public:
        Tree(QString line, int tree_id, float tree_fitness);
        Tree(int tree_id, float tree_fitness);
        ~Tree();

        void draw(QGraphicsScene *canvas, QPointF coord, int step);
        void correctCollisions(QGraphicsScene *canvas, QPointF origin);

        void addParent(Tree *parent);
        void addOffspring(Tree *offspring);
        void setRoot(Node *node);

        void setId(int id);
        int getId();
        float getFitness();
        Node *getRoot();
        QList<Tree*> getParents();
        QList<Tree*> getOffspring();

        static Tree* drawMany(QGraphicsScene *canvas, QList<Tree*> trees,
                              QPointF coord, int step);
        static Tree *opsConsensusTree();
        static void drawRings(QGraphicsScene *canvas, QPointF coord, int depth, int step);
        static void test(QGraphicsScene *canvas);

    private:
        int id;
        float fitness;
        Node *root;
        QList<Tree*> parents;
        QList<Tree*> offspring;
        
        Node *parseTree(QStringList nodes, int pos, int id);
};

#endif
