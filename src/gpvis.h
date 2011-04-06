#ifndef GPVIS_H 
#define GPVIS_H

#include "tree.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QRegExp>
#include <QSlider>
#include <QSpinBox>
#include <QStringList>
#include <QTextStream>
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

        QPushButton *fileOpen,
                    *fileSelect;
        QLineEdit *fileField;

        QSlider *genslider;
        QSpinBox *genspin;
        QGraphicsView *preview;
        QGraphicsScene *scene;
        
        QFile *fileFile;
        QString fileBuffer;

    private slots:
        void readLogFile();
};

#endif
