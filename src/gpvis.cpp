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
    : QWidget(parent)
{
    int WIDTH = 1024;
    int HEIGHT = 800;
    int SCENE_WIDTH = 600;
    int SCENE_HEIGTH = 500;

    resize(WIDTH, HEIGHT);
    //center(this, WIDTH, HEIGHT);

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGTH, this);
    sceneCenter = new QPointF(SCENE_WIDTH/2, SCENE_HEIGTH/2);
    preview = new QGraphicsView(scene);
    preview->setRenderHint(QPainter::Antialiasing);

    fileField = new QLineEdit(this);
    fileSelect = new QPushButton("Select file", this);
    fileOpen = new QPushButton("Read file", this);

    genLabel = new QLabel("Generation");

    genSpin = new QSpinBox(this);
    genSpin->setEnabled(false);

    genSlider = new QSlider(this);
    genSlider->setEnabled(false);
    genSlider->setOrientation(Qt::Horizontal);
    genSlider->setTickPosition(QSlider::TicksBelow);

    viewInd = new QRadioButton("Individuals", this);
    viewInd->setEnabled(false);
    viewRep = new QRadioButton("Reproductions", this);
    viewRep->setEnabled(false);

    consensusUse = new QCheckBox("Use consensus", this);
    consensusUse->setChecked(false);
    consensusUse->setEnabled(false);
    consensusPush = new QPushButton("View Consensus", this);
    consensusPush->setEnabled(false);

    tableView = new QTableView(this);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->hide();
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    tableView->setAlternatingRowColors(true);
    tableView->setEnabled(false);

    fileLine = new QBoxLayout(QBoxLayout::LeftToRight);
    fileLine->addWidget(fileField);
    fileLine->addWidget(fileSelect);
    fileLine->addWidget(fileOpen);

    conLine = new QBoxLayout(QBoxLayout::LeftToRight);
    conLine->addWidget(consensusUse);
    conLine->addWidget(consensusPush);

    genLine = new QBoxLayout(QBoxLayout::LeftToRight);
    genLine->addWidget(genSlider);
    genLine->addWidget(genSpin);

    viewLine = new QBoxLayout(QBoxLayout::LeftToRight);
    viewLine->addWidget(viewInd);
    viewLine->addWidget(viewRep);   

    grid = new QGridLayout(this);
    grid->addWidget(preview, 0, 0, -1, 1);
    grid->addLayout(fileLine, 0, 1);
    grid->addLayout(conLine, 1, 1);
    grid->addWidget(genLabel, 2, 1);
    grid->addLayout(genLine, 3, 1);
    grid->addLayout(viewLine, 4, 1);
    grid->addWidget(tableView, 5, 1);
 
    setLayout(grid);
    
    connect(fileSelect, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(readLogFile()));

    connect(genSlider, SIGNAL(valueChanged(int)), genSpin, SLOT(setValue(int)));
    connect(genSpin, SIGNAL(valueChanged(int)), genSlider, SLOT(setValue(int)));
    connect(genSlider, SIGNAL(valueChanged(int)), this, SLOT(showGeneration(int)));

    connect(viewInd, SIGNAL(toggled(bool)), this, SLOT(showIndTable()));
    connect(viewRep, SIGNAL(toggled(bool)), this, SLOT(showRepTable()));

    individuals = new QStandardItemModel(this);
    reproductions = new QStandardItemModel(this);
    
    individualsHeader = QStringList();
    individualsHeader << "id" << "fitness" << "tree";
    reproductionsHeader = QStringList();
    reproductionsHeader << "fitness gain" << "offspring" << "parent" ;

}

void GPVis::readLogFile()
{
    fileFile = new QFile(fileField->text());
    //qDebug() << "GPVis::readLogFile using file" << fileField->text();
    ////qDebug() << fileField->text();
    
    /* check if file and can be read */
    if(!fileFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //TODO: implement exception if file doesnt exist
        return;
    }

    fileStream = new QTextStream(fileFile);
    QString ops, vars, terms;
    
    QString fileBuffer = fileStream->readLine();

    extern Def *definition;

    /* delete everything from generations */
    generations.clear();

    /* if other definition was there */
    if(definition != NULL) delete definition;

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
                continue;
            }

            /* terms */
            if(fileBuffer.contains(QRegExp("\\s*terms:*.")))
            {
                terms = fileBuffer.remove(QRegExp("\\s*terms:\\s*"));
                //qDebug() << "GPVis::readLogFile found terms" << terms;

                definition = new Def(ops, terms); // TODO: change this
                consensusTree = Tree::opsConsensusTree();

                continue;
            }

            /* end of definition */
            if(fileBuffer.contains(QRegExp("generation*.")))
            {
                readGeneration();
            }
        }
    }
    else
    {
        // TODO: definition reading exception
        return;
    }

    /* set up the generation stuff */
    genSpin->setRange(0, generations.length() - 1);
    genSpin->setEnabled(true);
    
    genSlider->setRange(0, generations.length() - 1);
    genSlider->setEnabled(true);

    /* enable viewer */
    tableView->setEnabled(true);
    viewInd->setEnabled(true);
    viewRep->setEnabled(true);
    consensusUse->setEnabled(true);
    consensusPush->setEnabled(true);
    
    /* define first generation read */
    showGeneration(0);
    
    /* set individuals as default view */
    viewInd->setChecked(true);
    showIndTable();
    
    /* individual 0 is the first one shown */
    selectedRow = 0;
    tableView->selectRow(selectedRow);
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
            gen->addIndividual(tokens[2], tokens[1].toFloat());
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

/* builds model from a generation */
void GPVis::showGeneration(int gen)
{
    /* cleanup last viewed */
    individuals->clear();
    reproductions->clear();

    //qDebug() << "GPVis::showGeneration " << gen;
    Generation *actual = generations[gen];

    /* population */

    individuals->setHorizontalHeaderLabels(individualsHeader);
    for(int i = 0; i < actual->population.length(); i++)
    {
        individuals->setItem(i, 0, new QStandardItem(QString::number(i)));
        individuals->setItem(i, 1, new QStandardItem(QString::number(actual->population[i].fit)));
        individuals->setItem(i, 2, new QStandardItem(actual->population[i].str));
    }

    connect(viewRep, SIGNAL(toggled(bool)), this, SLOT(showRepTable()));

    /* reproductions */
    reproductions->setHorizontalHeaderLabels(reproductionsHeader);

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
            reproductions->setItem(i, 0, new QStandardItem(QString::number(max_fit - next->population[actual->reproductions[i].offspring].fit)));
            reproductions->setItem(i, 1, new QStandardItem(QString::number(actual->reproductions[i].offspring)));
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
    }
    
    /* keep same row selected */
    tableView->selectRow(selectedRow);
}

void GPVis::individualFromTable()
{
    int row = tableView->selectionModel()->currentIndex().row(),
        ind = tableView->model()->index(row, 0).data().toInt();
    selectedRow = row;
    scene->clear();
    renderIndividual(genSpin->value(), ind);
}

void GPVis::renderIndividual(int gen, int ind)
{
    /* use reference tree */
    if(consensusUse->isChecked())
    {
        QList<Tree*> trees;
        if(consensusTree != NULL)
            trees.append(consensusTree);
        trees.append(generations[gen]->getIndividual(ind));

        Tree::drawMany(scene, trees, *sceneCenter, Style::defaultStep);

        for(int i=1; i < trees.length(); i++)
            delete trees[i];
    }
    else
    {
        Tree *tree;
        tree = generations[gen]->getIndividual(ind);
        tree->draw(scene, *sceneCenter, Style::defaultStep);
        delete tree;
    }
}

void GPVis::reproductionFromTable()
{
    int row = tableView->selectionModel()->currentIndex().row(),
        off_num = tableView->model()->index(row, 1).data().toInt();
    QStringList str_par_num = tableView->model()->index(row, 2).data().toString().split(QRegExp("\\s+"));
    QList<int> par_num;
    // TODO: fix last element of str_par_num getting 0
    for(int i = 0; i < str_par_num.length() - 1; i++)
        par_num << str_par_num[i].toInt();
    selectedRow = row;
    scene->clear();
    renderReproduction(genSpin->value(), par_num, off_num);
}

void GPVis::renderReproduction(int gen, QList<int> parents, int offspring)
{
    //TODO: render tooltip
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

    /* offspring */
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
        trees.append(generations[gen]->getIndividual(parents[i]));
        trees[j]->setId(j);
        QVector<int> pair(2);
        pair[0] = gen;
        pair[1] = parents[i];
        idTable.append(pair);
        j++;
    }

    Tree::drawMany(scene, trees, *sceneCenter, Style::defaultStep);

    // TODO: store last drawn trees
    for(int i=1; i < trees.length(); i++)
        delete trees[i];
}

void GPVis::showIndTable()
{
    selectedView = INDIVIDUALS;
    tableView->setModel(individuals);

    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);

    tableView->selectionModel()->disconnect(this);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(individualFromTable()));

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

    tableView->selectionModel()->disconnect(this);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(reproductionFromTable()));

    genSlider->setMaximum(generations.length() - 2);
    genSpin->setMaximum(generations.length() - 2);
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
    fileField->setText("test/palotti.log");
    QTest::mouseClick(fileOpen, Qt::LeftButton);

    //Tree::test(scene);
    /*
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));
    //qDebug() << "Tree::test : " << test_tree->index;
    */
}
