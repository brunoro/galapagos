#ifndef GPVIS_H 
#define GPVIS_H

#include "tree.h"
#include "generation.h"

#include <QApplication>
#include <QBoxLayout>
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

enum Views{INDIVIDUALS, REPRODUCTIONS};

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
                           *reproductions;

        QStringList individualsHeader,
                    reproductionsHeader;

        /* ui */
        QGridLayout *grid;

        QBoxLayout *fileLine;
        QPushButton *fileOpen,
                    *fileSelect;
        QLineEdit *fileField;

        QBoxLayout *genLine;
        QLabel *genLabel;
        QSlider *genSlider;
        QSpinBox *genSpin;
        
        QBoxLayout *viewLine;
        QRadioButton *viewInd,
                     *viewRep;
        QTableView *tableView;

        QGraphicsView *preview;
        QGraphicsScene *scene;
        QPointF *sceneCenter;
        
        Views selectedView;
        int selectedRow;

        /* file reading */
        QFile *fileFile;
        QTextStream *fileStream;

        void readGeneration();
        
        /* render stuff on canvas */
        void renderIndividual(int gen, int individual);
        void renderReproduction(int gen, QList<int> parent, int offspring);

    private slots:
        /* show data */
        void showGeneration(int gen);
        void individualFromTable();
        void reproductionFromTable();

        /* change tableView */
        void showIndTable();
        void showRepTable();

        /* file reading */
        void openFileDialog();
        void readLogFile();

};

#endif
