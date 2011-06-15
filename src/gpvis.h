/*
 * Galapagos - a genetic programming algorithms visualizer
 * http://github.com/brunoro/galapagos
 * Copyright (C) <2011>  <Gustavo Brunoro>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GPVIS_H 
#define GPVIS_H

#include "tree.h"
#include "style.h"
#include "refbox.h"
#include "generation.h"
#include "fitnessview.h"

#include <cfloat>

#include <QMessageBox>
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
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

#define DEFAULT_ROW 0
#define DEFAULT_GENERATION 0
#define DEFAULT_SLICE 0
#define CONSENSUS_INITIAL_DEPTH 5
#define CONSENSUS_MIN_DEPTH 2
#define CONSENSUS_MAX_DEPTH 4

enum Views{INDIVIDUALS, REPRODUCTIONS, FITNESS};

class GPVis : public QWidget
{
    Q_OBJECT

    public:
        GPVis(QWidget *parent = 0);
        virtual ~GPVis();

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
        Tree *consensusTree;
        Refbox *ref;
        QPointF refPos;

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
                     *viewRep,
                     *viewFit;

        QBoxLayout *conLine;
        QCheckBox *consensusUse;
        QCheckBox *collisionUse;
        QSpinBox *consensusDepth;

        QTableView *tableView;

        QGraphicsView *fitnessView;
        Histogram *fitnessScene;
        int fitnessSelectedSlice;

        QGraphicsView *preview;
        QGraphicsScene *scene;
        QPointF *sceneCenter;
        
        Views selectedView;

        /* file reading */
        QFile *fileFile;
        QTextStream *fileStream;

        int consensusDepthValue;

        void readGeneration();

        /* render stuff on canvas */
        void renderIndividual(int gen, QList<int> ind);
        void renderReproduction(int gen, QList<int> parent, int offspring);

        /* functions to turn UI on and off */
        void turnEverythingOn();
        void turnEverythingOff();

        Tree* drawnTree;

    private slots:
        /* show data */
        void showGeneration(int gen);
        void individualFromTable();
        void reproductionFromTable();
        void fitnessFromHistogram();
        void redrawTree();

        /* change tableView */
        void showIndTable();
        void showRepTable();
        void showFitView();

        /* file reading */
        void openFileDialog();
        void readLogFile();
};

#endif
