#include "node.h"

#include <QString>
#include <QVector>
#include <QPointF>

class Tree:
    public:
        Tree(QString line, QObject *parent);

    private:
        int id;
        Node *root;
        QVector<Node*> index;
        QVector<Tree*> parents;
        QVector<Tree*> offspring;

        void draw(QPointF coord, int radius);
