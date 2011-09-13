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

#include "gpvis.h"

Style *style;

GPVis::GPVis(QWidget *parent)
    : QWidget(parent), fileFile(NULL), fileStream(NULL), consensusDepthValue(CONSENSUS_INITIAL_DEPTH), drawnTree(NULL)
{
    /* initializing ::style */
    ::style = new Style();

    /* setting scene up */
    resize(::style->windowWidth, ::style->windowHeight);

    scene = new QGraphicsScene(0, 0, ::style->sceneWidth, ::style->sceneHeight, this);
    sceneCenter = new QPointF(::style->sceneWidth/2, ::style->sceneHeight/2);
    viewport = new Viewport(scene, this);
    viewport->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    viewport->setDragMode(QGraphicsView::ScrollHandDrag);
    scaleFactor = 1.0;

    /* UI */
    fileField = new QLineEdit(this);
    fileSelect = new QPushButton("Select file", this);
    fileOpen = new QPushButton("Read file", this);

    genLabel = new QLabel("Generation");

    genSpin = new QSpinBox(this);

    genSlider = new QSlider(this);
    genSlider->setOrientation(Qt::Horizontal);
    genSlider->setTickPosition(QSlider::TicksBelow);

    viewInd = new QRadioButton("Population", this);
    viewInd->setEnabled(false);
    viewBreed = new QRadioButton("Breeding", this);
    viewBreed->setEnabled(false);
    viewFit = new QRadioButton("Fitness space", this);
    viewFit->setEnabled(false);

    consensusUse = new QCheckBox("Non-terminals with fixed positions", this);
    consensusUse->setChecked(true);
    collisionUse = new QCheckBox("Address collisions", this);
    collisionUse->setChecked(false);
    consensusDepth = new QSpinBox(this);
    consensusDepth->setValue(CONSENSUS_INITIAL_DEPTH);
    consensusDepth->setRange(CONSENSUS_MIN_DEPTH, CONSENSUS_MAX_DEPTH);

    tableView = new QTableView(this);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->hide();
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    tableView->setAlternatingRowColors(true);

    turnEverythingOff();

    fileLine = new QBoxLayout(QBoxLayout::LeftToRight);
    fileLine->addWidget(fileField);
    fileLine->addWidget(fileSelect);
    fileLine->addWidget(fileOpen);

    conLine = new QBoxLayout(QBoxLayout::LeftToRight);
    conLine->addWidget(consensusUse);
    conLine->addWidget(consensusDepth);
    consensusDepth->hide();
    conLine->addWidget(collisionUse);

    genLine = new QBoxLayout(QBoxLayout::LeftToRight);
    genLine->addWidget(genSlider);
    genLine->addWidget(genSpin);

    viewLine = new QBoxLayout(QBoxLayout::LeftToRight);
    viewLine->addWidget(viewInd);
    viewLine->addWidget(viewBreed);   
    viewLine->addWidget(viewFit);

    fitnessScene = new Histogram(0, 0, ::style->histogramWidth + 2 * ::style->histogramPadding, 
                                       ::style->histogramHeight + 2 * ::style->histogramPadding, this);
    fitnessView = new QGraphicsView(fitnessScene);
    fitnessView->setRenderHint(QPainter::Antialiasing);
    fitnessSelectedSlice = DEFAULT_SLICE;

    grid = new QGridLayout(this);
    grid->addWidget(viewport, 0, 0, -1, 1);
    grid->addLayout(fileLine, 0, 1);
    grid->addLayout(conLine, 1, 1);
    grid->addWidget(genLabel, 2, 1);
    grid->addLayout(genLine, 3, 1);
    grid->addLayout(viewLine, 4, 1);
    grid->addWidget(tableView, 5, 1);
 
    setLayout(grid);
    
    /* initial point for refBox */
    refPos = QPoint(10, 10);
    ref = NULL;

    /* connections */
    connect(fileSelect, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(readLogFile()));

    connect(genSlider, SIGNAL(valueChanged(int)), genSpin, SLOT(setValue(int)));
    connect(genSpin, SIGNAL(valueChanged(int)), genSlider, SLOT(setValue(int)));
    connect(genSlider, SIGNAL(valueChanged(int)), this, SLOT(showGeneration(int)));
    
    connect(consensusUse, SIGNAL(toggled(bool)), consensusDepth, SLOT(setEnabled(bool)));
    /* temporary, so it won't loop forever */
    connect(consensusUse, SIGNAL(toggled(bool)), collisionUse, SLOT(setChecked(bool)));
    connect(consensusUse, SIGNAL(toggled(bool)), collisionUse, SLOT(setEnabled(bool)));

    /* keep these connections in this order! */
    connect(consensusUse, SIGNAL(toggled(bool)), this, SLOT(redrawTree()));
    connect(consensusDepth, SIGNAL(valueChanged(int)), this, SLOT(redrawTree()));
    connect(collisionUse, SIGNAL(toggled(bool)), this, SLOT(redrawTree()));

    connect(viewInd, SIGNAL(toggled(bool)), this, SLOT(showIndTable()));
    connect(viewBreed, SIGNAL(toggled(bool)), this, SLOT(showBreedTable()));
    connect(viewFit, SIGNAL(toggled(bool)), this, SLOT(showFitView()));

    /* change nodes and edges size when zooming viewport */
    connect(viewport, SIGNAL(scaleView(qreal)), this, SLOT(scaleView(qreal)));

    /* models */
    individuals = new QStandardItemModel(this);
    breedings = new QStandardItemModel(this);
    
    /* table headers*/
    individualsHeader = QStringList();
    individualsHeader << "Id" << "Fitness" << "Tree";
    breedingsHeader = QStringList();
    breedingsHeader << "Fitness gain" << "Offspring" << "Parent" ;

    /* Setting tool tips */
    genSlider->setToolTip("Choose generation");
    genSpin->setToolTip("Navigate through the generations");

    viewInd->setToolTip("View individuals from the chosen generation");
    viewBreed->setToolTip("View breeding from the chosen generation");  

    fileOpen->setToolTip("Read chosen file");
    fileSelect->setToolTip("Choose another file to analyse");

    collisionUse->setToolTip("Collision treatment could be used to improve individual visualization");
    consensusUse->setToolTip("Using non-terminals with fixed positions on space can help comparing individuals");

    consensusDepth->setToolTip("Maximum level to use consensus");
}

GPVis::~GPVis(){
    foreach(Generation* gen, generations){
        delete gen; gen = NULL;
    }

    if(individuals){
        delete individuals; individuals = NULL;
    }
    if(breedings){
        delete breedings; breedings = NULL;
    }
    if(consensusTree){
        delete consensusTree; consensusTree = NULL;
    }
    if(fileOpen){
        delete fileOpen;  fileOpen = NULL;
    }
    if(fileSelect){
        delete fileSelect; fileSelect = NULL;
    }
    if(fileField){
        delete fileField; fileField = NULL;
    }
    if(genLabel){
        delete genLabel; genLabel = NULL;
    }
    if(genSlider){
        delete genSlider; genSlider = NULL;
    }
    if(genSpin){
        delete genSpin; genSpin = NULL;
    }
    if(viewInd){
        delete viewInd; viewInd = NULL;
    }
    if(viewBreed){
        delete viewBreed; viewBreed = NULL;
    }
    if(consensusUse){
        delete consensusUse; consensusUse = NULL;
    }
    if(collisionUse){
        delete collisionUse; collisionUse = NULL;
    }
    if(consensusDepth){
        delete consensusDepth; consensusDepth = NULL;
    }
    if(tableView){
        delete tableView; tableView = NULL;
    }
    if(viewport){
        delete viewport; viewport = NULL;
    } 
    if(scene){
        delete scene; scene = NULL;
    } 
    if(sceneCenter){
        delete sceneCenter; sceneCenter = NULL;
    }    
    if(fileFile){
        delete fileFile; fileFile = NULL;
    }
    if(fileStream){
        delete fileStream; fileStream = NULL;
    }    
    if(viewLine){
        delete viewLine; viewLine = NULL;
    }
    if(fileLine){
        delete fileLine;  fileLine = NULL;
    }
    if(genLine){
        delete genLine; genLine = NULL;
    }
    if(conLine){
        delete conLine; conLine = NULL;
    }
 
    //last one because it contains some layouts inside that must be deleted before
    if(grid){
        delete grid;  grid = NULL;
    }
    if(drawnTree){
        delete drawnTree; drawnTree = NULL;
    }

}

void GPVis::readLogFile()
{
    if(fileFile != NULL){
        delete fileFile;
    }

    fileFile = new QFile(fileField->text());
    //qDebug() << "GPVis::readLogFile using file" << fileField->text();
    //qDebug() << fileField->text();
    
    /* check if file and can be read */
    if(!fileFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox message(this);
        QString error("File \"" + fileField->text() + "\" does not exist. ");
        message.setText(error);
        message.exec();

        return;
    }

    if(fileStream != NULL){
        delete fileStream;
    }

    fileStream = new QTextStream(fileFile);
    QString ops, vars, terms;

    QString fileBuffer = fileStream->readLine();

    /* delete everything from generations */
    if(!generations.isEmpty())
        generations.clear();

    /* if other definition was there */
    if(Tree::definition != NULL) delete Tree::definition;
    Tree::definition = new Def();

    /* search definition in file */
    if(fileBuffer.contains(QRegExp("definition:*.")))
    {
        //qDebug() << "GPVis::readLogFile found definition"; 
        while(!fileStream->atEnd())
        {
            fileBuffer = fileStream->readLine();

            /* nodes */
            if(fileBuffer.contains(QRegExp("\\s*nodes:*.")))
            {
                ops = fileBuffer.remove(QRegExp("\\s*nodes:\\s*"));
                //qDebug() << "GPVis::readLogFile found ops " << ops;

                Tree::definition->addNodes(ops);
                continue;
            }
            /* end of definition */
            if(fileBuffer.contains(QRegExp("generation*.")))
            {
                /* we must have the operators and terms already defined. */
                consensusTree = Tree::opsConsensusTree(CONSENSUS_MAX_DEPTH);

                //qDebug() << "GPVis::reading generation ";
                readGeneration();
            }
        }
    }
    else
    {
        QMessageBox message(this);
        QString error("File \"" + fileField->text() + "\" is not valid. ");
        message.setText(error);
        message.exec();
        turnEverythingOff();
        return;
    }

    /* set up the generation stuff */
    genSpin->setRange(0, generations.length() - 1);

    genSlider->setRange(0, generations.length() - 1);

    turnEverythingOn();

    /* define first generation read */
    showGeneration(DEFAULT_GENERATION);
    
    /* set individuals as default view */
    viewInd->setChecked(true);
    showIndTable();
    
    /* first one shown */
    tableView->selectRow(DEFAULT_ROW);
    tableView->sortByColumn(0, Qt::AscendingOrder);
}

void GPVis::readGeneration()
{
    Generation *gen = new Generation();
    QString fileBuffer,
            individual,
            breeding;
    QStringList tokens;

    do
    {
        fileBuffer = fileStream->readLine();

        /* individuals */
        if(fileBuffer.contains(QRegExp("ind:*.")))
        {
            individual = fileBuffer;
            individual.remove(QRegExp("\\s*ind:\\s*"));
            tokens = individual.split("\t", QString::SkipEmptyParts);
            //qDebug() << "GPVis::readLogFile found individual" << tokens;
            gen->addIndividual(tokens[1], tokens[0].toFloat());
        }
        /* breeding */
        if(fileBuffer.contains(QRegExp("breed:*.")))
        {
            breeding = fileBuffer;
            breeding.remove(QRegExp("\\s*breed:\\s*"));
            //qDebug() << "GPVis::readLogFile found breeding" << breeding;
            tokens = breeding.split(QRegExp("\\s+"));

            QList<int> parents;
            int i = 0;
            for(; i < tokens.length(); i++)
            {
                if(tokens[i] == "->")
                    break;
                parents.append(tokens[i].toInt());
            }
            gen->addBreeding(Breeding(parents,
                                              tokens[++i].toInt()));
        }
    } while(fileBuffer[0] == ('\t') && !(fileStream->atEnd()));

    generations.append(gen);
    
    /* read another generation */
    if(fileBuffer.contains(QRegExp("generation*.")))
        readGeneration();
}


void GPVis::turnEverythingOn(){
    /* enable viewer */
    genSlider->setEnabled(true);
    genSpin->setEnabled(true);
    tableView->setEnabled(true);
    tableView->setSortingEnabled(true);
    viewInd->setEnabled(true);
    viewBreed->setEnabled(true);
    viewFit->setEnabled(true);
    consensusUse->setEnabled(true);
    collisionUse->setEnabled(true);
    consensusDepth->setEnabled(true);
}

void GPVis::turnEverythingOff(){
    /* disable viewer */
    genSlider->setEnabled(false);
    genSpin->setEnabled(false);
    tableView->setEnabled(false);
    tableView->setSortingEnabled(false);
    viewInd->setEnabled(false);
    viewBreed->setEnabled(false);
    viewFit->setEnabled(false);
    consensusUse->setEnabled(false);
    collisionUse->setEnabled(false);
    consensusDepth->setEnabled(false);
}


/* builds model from a generation */
void GPVis::showGeneration(int gen)
{
    /* store tableView 'state' */
    int selectedRow = DEFAULT_ROW;
    if(tableView->selectionModel())
        selectedRow = tableView->selectionModel()->currentIndex().row();

    int sortedColumn = tableView->horizontalHeader()->sortIndicatorSection();
    Qt::SortOrder sortedOrder = tableView->horizontalHeader()->sortIndicatorOrder();

    /* save histogram */
    fitnessSelectedSlice = fitnessScene->getSelectedSlice();

    /* cleanup last viewed */
    individuals->clear();
    breedings->clear();

    //qDebug() << "GPVis::showGeneration " << gen;
    Generation *actual = generations[gen];

    /* new fitness space */
    fitnessScene->setData(actual);
    fitnessScene->draw();

    /* population */
    individuals->setHorizontalHeaderLabels(individualsHeader);
    individuals->setSortRole(Qt::UserRole);
    for(int i = 0; i < actual->population.length(); i++)
    {
        QStandardItem *tree_id = new QStandardItem();
        tree_id->setData(i, Qt::UserRole);
        tree_id->setData(QString::number(i), Qt::DisplayRole);
        individuals->setItem(i, 0, tree_id);

        QStandardItem *tree_fit = new QStandardItem();
        tree_fit->setData(actual->population[i].fit, Qt::UserRole);
        tree_fit->setData(QString::number(actual->population[i].fit), Qt::DisplayRole);
        individuals->setItem(i, 1, tree_fit);

        QStandardItem *tree_str = new QStandardItem();
        tree_str->setData(actual->population[i].str, Qt::UserRole);
        tree_str->setData(actual->population[i].str, Qt::DisplayRole);
        individuals->setItem(i, 2, tree_str);
    }

    connect(viewBreed, SIGNAL(toggled(bool)), this, SLOT(showBreedTable()));

    /* breedings */
    breedings->setHorizontalHeaderLabels(breedingsHeader);
    breedings->setSortRole(Qt::UserRole);

    /* if it is not last generation, get next */
    if(gen < generations.length() - 1)
    {
        Generation *next = generations[++gen];
        // TODO: divide by columns
        for(int i = 0; i < actual->breedings.length(); i++)
        {
            QString parents;
            QList<int> parents_sorted = actual->breedings[i].parents;
            qSort(parents_sorted);
            float par_fit,
                  max_fit = FLT_MIN;
            for(int j = 0; j < actual->breedings[i].parents.length(); j++)
            {
                parents += QString::number(parents_sorted[j]) + " ";
                par_fit = actual->population[actual->breedings[i].parents[j]].fit;
                if(par_fit > max_fit)
                    max_fit = par_fit;
            }

            QStandardItem *tree_fit = new QStandardItem();
            float fit_dif = max_fit - next->population[actual->breedings[i].offspring].fit;
            tree_fit->setData(fit_dif, Qt::UserRole);
            tree_fit->setData(QString::number(fit_dif), Qt::DisplayRole);
            breedings->setItem(i, 0, tree_fit);
        
            QStandardItem *tree_id = new QStandardItem();
            tree_id->setData(actual->breedings[i].offspring, Qt::UserRole);
            tree_id->setData(QString::number(actual->breedings[i].offspring), Qt::DisplayRole);
            breedings->setItem(i, 1, tree_id);

            QStandardItem *tree_parents= new QStandardItem();
            tree_parents->setData(parents_sorted[0], Qt::UserRole);
            tree_parents->setData(parents, Qt::DisplayRole);
            breedings->setItem(i, 2, tree_parents);;

        }
        /* set radios */
        viewBreed->setEnabled(true);
    }
    else
    {
        /* unset radios */
        viewBreed->setEnabled(false);
    }

    /* select right view */
    switch(selectedView)
    {
        case INDIVIDUALS:
            showIndTable();
            break;
        case REPRODUCTIONS:
            showBreedTable();
            break;
        case FITNESS:
            showFitView();
            fitnessScene->selectSlice(fitnessSelectedSlice);
            fitnessFromHistogram();
            break;
    }
    
    actual = NULL;
    delete actual;

    /* restore tableView 'state' */
    tableView->sortByColumn(sortedColumn, sortedOrder);
    tableView->selectRow(selectedRow);
}

void GPVis::redrawTree()
{
    switch(selectedView)
    {
        case INDIVIDUALS:
            individualFromTable();
            break;
        case REPRODUCTIONS:
            breedingFromTable();
            break;
        case FITNESS:
            fitnessFromHistogram();
            break;
    }
}


/* change the scale of nodes, edges and refbox */
void GPVis::scaleView(qreal factor)
{
    /* store the factor */
    scaleFactor *= factor;

    /* scale tree, if drawn */
    if(drawnTree)
        drawnTree->scale(factor);

    /* scale e adjust position of refbox */
    ref->scale(factor);
}

/* restore the scaled view */
void GPVis::restoreScale()
{
    /* scale tree, if drawn */
    if(drawnTree)
        drawnTree->scale(scaleFactor);

    /* scale refbox */
    ref->scale(scaleFactor);
}

void GPVis::individualFromTable()
{
    /* preserve rounded refBox position relative to the viewport */
    if(ref != NULL)
    {
        //qDebug() << "SAVE: viewPos\t" << viewport->mapFromScene(ref->scenePos()) << "\t" << ref->scenePos();
        refPos = viewport->mapFromScene(ref->scenePos());
    }

    /* preserve table selection */
    QList<QModelIndex> rowIndexes = tableView->selectionModel()->selectedRows();
    QList<int> inds;
    foreach(QModelIndex rowIndex, rowIndexes)
        inds.append(tableView->model()->index(rowIndex.row(), 0).data().toInt());

    /* draw individual */
    scene->clear();
    renderIndividual(genSpin->value(), inds);
}

void GPVis::renderIndividual(int gen, QList<int> ind)
{
    if((ind.length() == 1) && !consensusUse->isChecked())
    {
        Tree *tree;
        tree = generations[gen]->getIndividual(ind[0]);
        tree->draw(scene, *sceneCenter, ::style->defaultStep);
        return;
    }

    QList<Tree*> trees;

    /* id table */
    QList<int> idTable;
    int j = 0;
    
    /* use reference tree */
    if(consensusUse->isChecked())
    {
        trees.append(consensusTree);
        j++;
    }

    QStringList refBoxLabel;
    /* append other trees */
    for(int i = 0; i < ind.length(); i++)
    {
        refBoxLabel.append("Individual (gen " + QString::number(gen) + ", id " + QString::number(ind[i]) + ")");
        trees.append(generations[gen]->getIndividual(ind[i]));
        trees[j]->setId(j);
        idTable.append(ind[i]);
        j++;
    }

    Tree *drawn = Tree::drawMany(scene, trees, *sceneCenter, ::style->defaultStep, collisionUse->isChecked(), consensusDepth->value());
    
    // TODO: delete old refbox
    //qDebug() << "DRAW: viewPos\t" << refPos << "\t" << viewport->mapToScene(refPos);
    ref = new Refbox(::style->getColorPalette(ind.length()), refBoxLabel, viewport->mapToScene(refPos));
    ref->draw(scene);
    drawnTree = drawn;
    
    /* restore scale */ 
    restoreScale();
}

void GPVis::breedingFromTable()
{
    /* preserve refBox position relative to the viewport */
    //if(ref != NULL) refPos += ref->getPos();
    if(ref != NULL) refPos = viewport->mapFromScene(ref->mapToScene(ref->getPos()));

    /* preserve table selection */
    int row = tableView->selectionModel()->currentIndex().row();
    int off_num = tableView->model()->index(row, 1).data().toInt();
    QStringList str_par_num = tableView->model()->index(row, 2).data().toString().split(QRegExp("\\s+"));
    QList<int> par_num;
    // TODO: fix last element of str_par_num getting 0
    for(int i = 0; i < str_par_num.length() - 1; i++)
        par_num << str_par_num[i].toInt();

    /* draw invidual */
    scene->clear();
    renderBreeding(genSpin->value(), par_num, off_num);
}

void GPVis::renderBreeding(int gen, QList<int> parents, int offspring)
{
    QList<Tree*> trees;

    /* id table */
    QList< QVector<int> > idTable;

    /* reference tree */
    int j = 0;
    if(consensusUse->isChecked())
    {
        trees.append(consensusTree);
        j++;
    }

    QStringList refBoxLabel;
    /* offspring */
    refBoxLabel.append("Offspring (gen " + QString::number(gen + 1) + ", id " + QString::number(offspring) + ")");
    trees.append(generations[gen + 1]->getIndividual(offspring));
    trees[j]->setId(j);
    QVector<int> pair(2);
    pair[0] = gen + 1;
    pair[1] = offspring;
    idTable.append(pair);
    j++;

    /* parents */
    for(int i = 0; i < parents.length(); i++)
    {
        refBoxLabel.append("Parent (gen " + QString::number(gen) + ", id " + QString::number(parents[i]) + ")");
        trees.append(generations[gen]->getIndividual(parents[i]));
        trees[j]->setId(j);
        QVector<int> pair(2);
        pair[0] = gen;
        pair[1] = parents[i];
        idTable.append(pair);
        j++;
    }

    Tree *drawn = Tree::drawMany(scene, trees, *sceneCenter, ::style->defaultStep, collisionUse->isChecked(), consensusDepth->value());

    // TODO: delete old ref
    ref = new Refbox(::style->getColorPalette(parents.length() + 1), refBoxLabel, viewport->mapToScene(refPos));
    ref->draw(scene);
    drawnTree = drawn;
    
    /* restore scale */ 
    restoreScale();
}

void GPVis::fitnessFromHistogram()
{
    /* preserve refbox position */
    if(ref != NULL) refPos = viewport->mapFromScene(ref->mapToScene(ref->getPos()));

    /* get inviduals */
    fitnessSelectedSlice = fitnessScene->getSelectedSlice();
    
    /* draw individuals */
    scene->clear();
    QList<int> slice = fitnessScene->getSlice(fitnessSelectedSlice);
    renderIndividual(genSpin->value(), slice);
}

void GPVis::showIndTable()
{
    selectedView = INDIVIDUALS;
    tableView->setModel(individuals);

    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);
    
    fitnessScene->disconnect(this);
    tableView->selectionModel()->disconnect(this);
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(individualFromTable()));

    fitnessView->hide();
    tableView->show();
    grid->removeWidget(fitnessView);
    grid->addWidget(tableView, 5, 1);

    genSlider->setMaximum(generations.length() - 1);
    genSpin->setMaximum(generations.length() - 1);
}

void GPVis::showBreedTable()
{
    selectedView = REPRODUCTIONS;
    tableView->setModel(breedings);

    tableView->resizeColumnToContents(1);
    tableView->resizeColumnToContents(2);
    tableView->resizeColumnToContents(0);

    fitnessScene->disconnect(this);
    tableView->selectionModel()->disconnect(this);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(breedingFromTable()));

    fitnessView->hide();
    tableView->show();
    grid->removeWidget(fitnessView);
    grid->addWidget(tableView, 5, 1);

    genSlider->setMaximum(generations.length() - 2);
    genSpin->setMaximum(generations.length() - 2);
}

void GPVis::showFitView()
{
    selectedView = FITNESS;

    tableView->selectionModel()->disconnect(this);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(fitnessScene, SIGNAL(clickedBar()),
            this, SLOT(fitnessFromHistogram()));

    tableView->hide();
    fitnessView->show();
    grid->addWidget(fitnessView, 5, 1);
}

void GPVis::openFileDialog()
{
    fileField->setText(QFileDialog::getOpenFileName(this,
                                                    tr("Open log"),
                                                    QDir::currentPath(),
                                                    tr("All files (*)")));
}

void GPVis::test()
{
    fileField->setText("test/test1.gpvis");
    QTest::mouseClick(fileOpen, Qt::LeftButton);

    //Tree::test(scene);
    /*
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));
    */
}

