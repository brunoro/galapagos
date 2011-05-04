#ifndef GPVIS_H 
#define GPVIS_H

#include "tree.h"
#include "generation.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHash>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QSlider>
#include <QSpinBox>
#include <QStandardItemModel>
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
        QStandardItemModel *individuals,
                           *crossovers,
                           *mutations;

        /* ui */
        QGridLayout *grid;

        QPushButton *fileOpen,
                    *fileSelect;
        QLineEdit *fileField;

        QLabel *genLabel;
        QSlider *genSlider;
        QSpinBox *genSpin;
        QRadioButton *viewInd,
                     *viewCross,
                     *viewMut;
        QTableView *tableView;
        QGraphicsView *preview;
        QGraphicsScene *scene;
        
        int selected;

        /* file reading */
        QFile *fileFile;
        QTextStream *fileStream;

        void readGeneration();
        
    private slots:
        void renderGeneration(int gen);
        /* change tableView */
        void showIndTable();
        void showCrossTable();
        void showMutTable();
        /* file reading */
        void openFileDialog();
        void readLogFile();

};

#endif
