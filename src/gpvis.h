#ifndef GPVIS_H 
#define GPVIS_H

#include "tree.h"
#include "generation.h"

#include <QAbstractItemModel>
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
#include <QTableView>
#include <QTest>
#include <QTextStream>
#include <QVector>
#include <QWidget>

class GPVis : public QWidget
{
    Q_OBJECT

    public:
        GPVis(QWidget *parent = 0);

        int newTreeId();
        void addTree(Tree *tree);
        void test();

    private:
        /* data */
        QList<Generation*> generations;
        QAbstractItemModel *individuals,
                           *crossovers,
                           *mutations;

        /* ui */
        QGridLayout *grid;

        QPushButton *fileOpen,
                    *fileSelect;
        QLineEdit *fileField;

        QSlider *genSlider;
        QSpinBox *genSpin;
        QTableView *tableView;
        QGraphicsView *preview;
        QGraphicsScene *scene;
        
        /* file reading */
        QFile *fileFile;
        QTextStream *fileStream;

        void readGeneration();
        
    private slots:
        void renderGeneration(int gen);
        void openFileDialog();
        void readLogFile();
};

#endif
