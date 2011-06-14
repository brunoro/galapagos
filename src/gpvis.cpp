#include "gpvis.h"

void center(QWidget *widget, int w, int h)
{
    int x, y;
    int screenWidth;
    int screenHeight;

    QDesktopWidget *desktop = QApplication::desktop();

    screenWidth = desktop->width();
    screenHeight = desktop->height();

    x = (screenWidth - w) / 2;
    y = (screenHeight - h) / 2;

    widget->move(x, y);
}

GPVis::GPVis(QWidget *parent)
    : QWidget(parent), fileFile(NULL), fileStream(NULL), consensusDepthValue(CONSENSUS_INITIAL_DEPTH), drawnTree(NULL)
{
    /* setting scene up */
    int WIDTH = 1024;
    int HEIGHT = 800;
    int SCENE_WIDTH = 600;
    int SCENE_HEIGHT = 500;

    resize(WIDTH, HEIGHT);
    //center(this, WIDTH, HEIGHT);

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this);
    sceneCenter = new QPointF(SCENE_WIDTH/2, SCENE_HEIGHT/2);
    preview = new QGraphicsView(scene);
    preview->setRenderHint(QPainter::Antialiasing);

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
    viewRep = new QRadioButton("Breeding", this);
    viewRep->setEnabled(false);
    viewFit = new QRadioButton("Fitness space", this);
    viewFit->setEnabled(false);

    consensusUse = new QCheckBox("Non-terminals with fixed positions", this);
    consensusUse->setChecked(true);
    collisionUse = new QCheckBox("Address collisions", this);
    collisionUse->setChecked(true);
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
    //conLine->addWidget(consensusDepth);
    conLine->addWidget(collisionUse);

    genLine = new QBoxLayout(QBoxLayout::LeftToRight);
    genLine->addWidget(genSlider);
    genLine->addWidget(genSpin);

    viewLine = new QBoxLayout(QBoxLayout::LeftToRight);
    viewLine->addWidget(viewInd);
    viewLine->addWidget(viewRep);   
    viewLine->addWidget(viewFit);

    fitnessScene = new Histogram(0, 0, Style::histogramWidth + 2 * Style::histogramPadding, 
                                       Style::histogramHeight + 2 * Style::histogramPadding, this);
    fitnessView = new QGraphicsView(fitnessScene);
    fitnessView->setRenderHint(QPainter::Antialiasing);
    fitnessSelectedSlice = DEFAULT_SLICE;

    grid = new QGridLayout(this);
    grid->addWidget(preview, 0, 0, -1, 1);
    grid->addLayout(fileLine, 0, 1);
    grid->addLayout(conLine, 1, 1);
    grid->addWidget(genLabel, 2, 1);
    grid->addLayout(genLine, 3, 1);
    grid->addLayout(viewLine, 4, 1);
    grid->addWidget(tableView, 5, 1);
 
    setLayout(grid);
    
    /* initial point for refBox */
    refPos = QPointF(0, 0);
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
    connect(viewRep, SIGNAL(toggled(bool)), this, SLOT(showRepTable()));
    connect(viewFit, SIGNAL(toggled(bool)), this, SLOT(showFitView()));

    /* models */
    individuals = new QStandardItemModel(this);
    reproductions = new QStandardItemModel(this);
    
    /* table headers*/
    individualsHeader = QStringList();
    individualsHeader << "Id" << "Fitness" << "Tree";
    reproductionsHeader = QStringList();
    reproductionsHeader << "Fitness gain" << "Offspring" << "Parent" ;

    /* Setting tool tips */
    genSlider->setToolTip("Choose generation");
    genSpin->setToolTip("Navigate through the generations");

    viewInd->setToolTip("View individuals from the chosen generation");
    viewRep->setToolTip("View reproduction from the chosen generation");  

    fileOpen->setToolTip("Read chosen file");
    fileSelect->setToolTip("Choose another file to analyse");

    collisionUse->setToolTip("Collision treatment could be used to improve individual visualization");
    consensusUse->setToolTip("Consensus is a good tool to compare individuals");

    consensusDepth->setToolTip("Maximum level to use consensus");
}

GPVis::~GPVis(){
    foreach(Generation* gen, generations){
        delete gen; gen = NULL;
    }

    if(individuals){
        delete individuals; individuals = NULL;
    }
    if(reproductions){
        delete reproductions; reproductions = NULL;
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
    if(viewRep){
        delete viewRep; viewRep = NULL;
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
    if(preview){
        delete preview; preview = NULL;
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

            /* ops */
            if(fileBuffer.contains(QRegExp("\\s*ops:*.")))
            {
                ops = fileBuffer.remove(QRegExp("\\s*ops:\\s*"));
                //qDebug() << "GPVis::readLogFile found ops " << ops;

                Tree::definition->addOperators(ops);
                continue;
            }
            /* terms */
            if(fileBuffer.contains(QRegExp("\\s*terms:*.")))
            {
                terms = fileBuffer.remove(QRegExp("\\s*terms:\\s*"));
                //qDebug() << "GPVis::readLogFile found terms" << terms;

                Tree::definition->addTerms(terms);
                continue;
            }
            /* end of definition */
            if(fileBuffer.contains(QRegExp("generation*.")))
            {
                //we must have the operators and terms already defined.
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
            reproduction;
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
        /* reproduction */
        if(fileBuffer.contains(QRegExp("rep:*.")))
        {
            reproduction = fileBuffer;
            reproduction.remove(QRegExp("\\s*rep:\\s*"));
            //qDebug() << "GPVis::readLogFile found reproduction" << reproduction;
            tokens = reproduction.split(QRegExp("\\s+"));

            QList<int> parents;
            int i = 0;
            for(; i < tokens.length(); i++)
            {
                if(tokens[i] == "->")
                    break;
                parents.append(tokens[i].toInt());
            }
            gen->addReproduction(Reproduction(parents,
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
    viewRep->setEnabled(true);
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
    viewRep->setEnabled(false);
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
    reproductions->clear();

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

        individuals->setItem(i, 2, new QStandardItem(actual->population[i].str));
    }

    connect(viewRep, SIGNAL(toggled(bool)), this, SLOT(showRepTable()));

    /* reproductions */
    reproductions->setHorizontalHeaderLabels(reproductionsHeader);
    reproductions->setSortRole(Qt::UserRole);

    /* if it is not last generation, get next */
    if(gen < generations.length() - 1)
    {
        Generation *next = generations[++gen];
        // TODO: divide by columns
        for(int i = 0; i < actual->reproductions.length(); i++)
        {
            QString parents;
            float par_fit,
                  max_fit = FLT_MIN;
            for(int j = 0; j < actual->reproductions[i].parents.length(); j++)
            {
                parents += QString::number(actual->reproductions[i].parents[j]) + " ";
                par_fit = actual->population[actual->reproductions[i].parents[j]].fit;
                if(par_fit > max_fit)
                    max_fit = par_fit;
            }

            QStandardItem *tree_fit = new QStandardItem();
            float fit_dif = max_fit - next->population[actual->reproductions[i].offspring].fit;
            tree_fit->setData(fit_dif, Qt::UserRole);
            tree_fit->setData(QString::number(fit_dif), Qt::DisplayRole);
            reproductions->setItem(i, 0, tree_fit);
        
            QStandardItem *tree_id = new QStandardItem();
            tree_id->setData(actual->reproductions[i].offspring, Qt::UserRole);
            tree_id->setData(QString::number(actual->reproductions[i].offspring), Qt::DisplayRole);
            reproductions->setItem(i, 1, tree_id);

            reproductions->setItem(i, 2, new QStandardItem(parents));

        }
        /* set radios */
        viewRep->setEnabled(true);
    }
    else
    {
        /* unset radios */
        viewRep->setEnabled(false);
    }

    /* select right view */
    switch(selectedView)
    {
        case INDIVIDUALS:
            showIndTable();
            break;
        case REPRODUCTIONS:
            showRepTable();
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
            reproductionFromTable();
            break;
        case FITNESS:
            fitnessFromHistogram();
            break;
    }
}

void GPVis::individualFromTable()
{
    if(ref != NULL) refPos += ref->getPos();

    QList<QModelIndex> rowIndexes = tableView->selectionModel()->selectedRows();
    QList<int> inds;
    foreach(QModelIndex rowIndex, rowIndexes)
        inds.append(tableView->model()->index(rowIndex.row(), 0).data().toInt());

    scene->clear();
    renderIndividual(genSpin->value(), inds);
}

void GPVis::renderIndividual(int gen, QList<int> ind)
{
    if((ind.length() == 1) && !consensusUse->isChecked())
    {
        Tree *tree;
        tree = generations[gen]->getIndividual(ind[0]);
        tree->draw(scene, *sceneCenter, Style::defaultStep);
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

    Tree *drawn = Tree::drawMany(scene, trees, *sceneCenter, Style::defaultStep, collisionUse->isChecked(), consensusDepth->value());
    
    // TODO: delete old refbox
    ref = new Refbox(Style::getColorPalette(ind.length()), refBoxLabel, refPos);
    ref->draw(scene);
    //drawnTree = Tree::drawMany(scene, trees, *sceneCenter, Style::defaultStep, collisionUse->isChecked(), consensusDepth->value());
}

void GPVis::reproductionFromTable()
{
    if(ref != NULL) refPos += ref->getPos();

    int row = tableView->selectionModel()->currentIndex().row();
    int off_num = tableView->model()->index(row, 1).data().toInt();
    QStringList str_par_num = tableView->model()->index(row, 2).data().toString().split(QRegExp("\\s+"));
    QList<int> par_num;
    // TODO: fix last element of str_par_num getting 0
    for(int i = 0; i < str_par_num.length() - 1; i++)
        par_num << str_par_num[i].toInt();

    scene->clear();
    renderReproduction(genSpin->value(), par_num, off_num);
}

void GPVis::renderReproduction(int gen, QList<int> parents, int offspring)
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

    Tree *drawn = Tree::drawMany(scene, trees, *sceneCenter, Style::defaultStep, collisionUse->isChecked(), consensusDepth->value());

    // TODO: delete old ref
    ref = new Refbox(Style::getColorPalette(parents.length() + 1), refBoxLabel, refPos);
    ref->draw(scene);
}

void GPVis::fitnessFromHistogram()
{
    if(ref != NULL) refPos += ref->getPos();

    fitnessSelectedSlice = fitnessScene->getSelectedSlice();
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

void GPVis::showRepTable()
{
    selectedView = REPRODUCTIONS;
    tableView->setModel(reproductions);

    tableView->resizeColumnToContents(1);
    tableView->resizeColumnToContents(2);
    tableView->resizeColumnToContents(0);

    fitnessScene->disconnect(this);
    tableView->selectionModel()->disconnect(this);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(reproductionFromTable()));

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
                                                    tr("All files (*.*)")));
}

void GPVis::test()
{
    //fileField->setText("test/palotti_big.log");
    //QTest::mouseClick(fileOpen, Qt::LeftButton);

    //Tree::test(scene);
    /*
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));
    */
}

