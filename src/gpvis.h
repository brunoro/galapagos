#ifndef GPVIS_H 
#define GPVIS_H

#include "tree.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

class GPVis : public QWidget
{
    Q_OBJECT

    public:
        GPVis(QWidget *parent = 0);

        int newTreeId();
        void addTree(Tree *tree);

    private:
        QList<Tree*> trees;

        QGridLayout *grid;
        QSlider *genslider;
        QSpinBox *genspin;
        QGraphicsView *preview;
        QGraphicsScene *scene;
};

#endif
