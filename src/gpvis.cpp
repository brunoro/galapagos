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
    int HEIGHT = 768;
    int SCENE_WIDTH = 550;
    int SCENE_HEIGTH = 550;

    resize(WIDTH, HEIGHT);
    center(this, WIDTH, HEIGHT);

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
    viewCross = new QRadioButton("Crossovers", this);
    viewCross->setEnabled(false);
    viewMut = new QRadioButton("Mutations", this);
    viewMut->setEnabled(false);

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

    genLine = new QBoxLayout(QBoxLayout::LeftToRight);
    genLine->addWidget(genSlider);
    genLine->addWidget(genSpin);

    viewLine = new QBoxLayout(QBoxLayout::LeftToRight);
    viewLine->addWidget(viewInd);
    viewLine->addWidget(viewCross);
    viewLine->addWidget(viewMut);   

    grid = new QGridLayout(this);
    grid->addWidget(preview, 0, 0, -1, 1);
    grid->addLayout(fileLine, 0, 1);
    grid->addWidget(genLabel, 1, 1);
    grid->addLayout(genLine, 2, 1);
    grid->addLayout(viewLine, 3, 1);
    grid->addWidget(tableView, 4, 1);
 
    setLayout(grid);
    
    connect(fileSelect, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(readLogFile()));

    connect(genSlider, SIGNAL(valueChanged(int)), genSpin, SLOT(setValue(int)));
    connect(genSpin, SIGNAL(valueChanged(int)), genSlider, SLOT(setValue(int)));
    connect(genSlider, SIGNAL(valueChanged(int)), this, SLOT(showGeneration(int)));

    connect(viewInd, SIGNAL(toggled(bool)), this, SLOT(showIndTable()));
    connect(viewCross, SIGNAL(toggled(bool)), this, SLOT(showCrossTable()));
    connect(viewMut, SIGNAL(toggled(bool)), this, SLOT(showMutTable()));

    individuals = new QStandardItemModel(this);
    crossovers = new QStandardItemModel(this);
    mutations = new QStandardItemModel(this);
    
    individualsHeader = QStringList();
    individualsHeader << "id" << "fitness" << "tree";
    crossoversHeader = QStringList();
    crossoversHeader << "parent 1" << "parent 2" << "offspring";
    mutationsHeader = QStringList();
    mutationsHeader << "parent" << "offspring";

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
            if(fileBuffer.contains(QRegExp("\tops:*.")))
            {
                ops = fileBuffer.remove(QRegExp("\tops:\\s*"));
                //qDebug() << "GPVis::readLogFile found ops " << ops;
                continue;
            }

            /* vars */
            if(fileBuffer.contains(QRegExp("\tvars:*.")))
            {
                vars = fileBuffer.remove(QRegExp("\tvars:\\s*"));
                //qDebug() << "GPVis::readLogFile found vars" << vars;
                continue;
            }
            
            /* terms */
            if(fileBuffer.contains(QRegExp("\tterms:*.")))
            {
                terms = fileBuffer.remove(QRegExp("\tterms:\\s*"));
                //qDebug() << "GPVis::readLogFile found terms" << terms;

                definition = new Def(ops, vars, terms); // TODO: change this

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
    viewCross->setEnabled(true);
    viewMut->setEnabled(true);
    
    /* define first generation read */
    showGeneration(0);
    
    /* set individuals as default view */
    viewInd->setChecked(true);
    showIndTable();
    
    /* individual 0 is the first one shown */
    selectedRow = 0;
    tableView->selectRow(selectedRow);
}

/* builds model from a generation */
void GPVis::showGeneration(int gen)
{
    /* cleanup last viewed */
    individuals->clear();
    crossovers->clear();
    mutations->clear();

    //qDebug() << "GPVis::showGeneration " << gen;
    Generation *actual = generations[gen];

    /* population */

    individuals->setHorizontalHeaderLabels(individualsHeader);
    for(int i = 0; i < actual->population_tree.length(); i++)
    {
        individuals->setItem(i, 0, new QStandardItem(QString::number(i)));
        individuals->setItem(i, 1, new QStandardItem(QString::number(actual->population_fit[i])));
        individuals->setItem(i, 2, new QStandardItem(actual->population_tree[i]));
    }

    connect(viewMut, SIGNAL(toggled(bool)), this, SLOT(showMutTable()));
    connect(viewMut, SIGNAL(toggled(bool)), this, SLOT(showMutTable()));

    /* crossovers and mutations */
    crossovers->setHorizontalHeaderLabels(crossoversHeader);
    mutations->setHorizontalHeaderLabels(mutationsHeader);

    /* if it is not last generation, get next */
    if(gen < generations.length() - 1)
    {
        //Generation *next = generations[gen + 1];
        for(int i = 0; i < actual->crossovers.length(); i++)
        {
            crossovers->setItem(i, 0, new QStandardItem(QString::number(actual->crossovers[i].parent1)));
            crossovers->setItem(i, 1, new QStandardItem(QString::number(actual->crossovers[i].parent2)));
            crossovers->setItem(i, 2, new QStandardItem(QString::number(actual->crossovers[i].offspring)));
        }
        for(int i = 0; i < actual->mutations.length(); i++)
        {
            mutations->setItem(i, 0, new QStandardItem(QString::number(actual->mutations[i].parent)));
            mutations->setItem(i, 1, new QStandardItem(QString::number(actual->mutations[i].offspring)));
        }
        /* set radios */
        viewCross->setEnabled(true);
        viewMut->setEnabled(true);
    }
    else
    {
        /* unset radios */
        viewCross->setEnabled(false);
        viewMut->setEnabled(false);
    }

    /* select right view */
    switch(selectedView)
    {
        case INDIVIDUALS:
            showIndTable();
            break;
        case CROSSOVERS:
            showMutTable();
            break;
        case MUTATIONS:
            showCrossTable();
            break;
    }
    
    /* keep same row selected */
    tableView->selectRow(selectedRow);
}

void GPVis::readGeneration()
{
    Generation *gen = new Generation();
    QString fileBuffer = fileStream->readLine(),
            individual,
            crossover,
            mutation;
    QStringList tokens;

    while(!(fileBuffer.contains(QRegExp("generation:*."))) &&
          !(fileStream->atEnd()))
    {
        /* individuals */
        if(fileBuffer.contains(QRegExp("ind:*.")))
        {
            individual = fileBuffer.remove(QRegExp("\tind:\\s*"));
            tokens = individual.split("\t", QString::SkipEmptyParts);
            //qDebug() << "GPVis::readLogFile found individual" << tokens;
            gen->addIndividual(tokens[2], tokens[1].toFloat());
            continue;
        }
        /* crossover */
        if(fileBuffer.contains(QRegExp("crossover:*.")))
        {
            crossover = fileBuffer.remove(QRegExp("\tcrossover:\\s*"));
            //qDebug() << "GPVis::readLogFile found crossover" << crossover;
            tokens = crossover.split(" ");
            gen->addCrossover(Crossover(tokens[0].toInt(),
                                        tokens[1].toInt(),
                                        tokens[3].toInt()));
            continue;
        }
        /* mutation */
        if(fileBuffer.contains(QRegExp("mutation:*.")))
        {
            mutation = fileBuffer.remove(QRegExp("\tmutation:\\s*"));
            //qDebug() << "GPVis::readLogFile found mutation" << mutation;
            tokens = mutation.split(" ");
            gen->addMutation(Mutation(tokens[0].toInt(),
                                      tokens[2].toInt()));
            continue;
        }

        fileBuffer = fileStream->readLine();
    }
    generations.append(gen);
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
    Tree *tree;
    tree = generations[gen]->getIndividual(ind);
    tree->draw(scene, *sceneCenter, Style::defaultStep);
    delete tree;
}

void GPVis::crossoverFromTable()
{
    int row = tableView->selectionModel()->currentIndex().row(),
        par1_num = tableView->model()->index(row, 0).data().toInt(),
        par2_num = tableView->model()->index(row, 1).data().toInt(),
        off_num = tableView->model()->index(row, 2).data().toInt();
    selectedRow = row;
    scene->clear();
    renderCrossover(genSpin->value(), par1_num, par2_num, off_num);
}

void GPVis::renderCrossover(int gen, int parent1, int parent2, int offspring)
{
    QList<Tree*> trees;
    trees.append(generations[gen + 1]->getIndividual(offspring));
    trees.append(generations[gen]->getIndividual(parent1));
    trees.append(generations[gen]->getIndividual(parent2));
    Tree::drawMany(scene, trees, *sceneCenter, Style::defaultStep);
    for(int i=0; i < trees.length(); i++)
        delete trees[i];
}

void GPVis::mutationFromTable()
{
    int row = tableView->selectionModel()->currentIndex().row(),
        par_num = tableView->model()->index(row, 0).data().toInt(),
        off_num = tableView->model()->index(row, 1).data().toInt();
    selectedRow = row;
    scene->clear();
    renderMutation(genSpin->value(), par_num, off_num);
}

void GPVis::renderMutation(int gen, int parent, int offspring)
{
    QList<Tree*> trees;
    trees.append(generations[gen + 1]->getIndividual(offspring));
    trees.append(generations[gen]->getIndividual(parent));
    Tree::drawMany(scene, trees, *sceneCenter, Style::defaultStep);
    for(int i=0; i < trees.length(); i++)
        delete trees[i];
}

void GPVis::showIndTable()
{
    selectedView = INDIVIDUALS;
    tableView->setModel(individuals);

    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);
    tableView->resizeColumnToContents(2);

    tableView->selectionModel()->disconnect(this);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(individualFromTable()));

    genSlider->setMaximum(generations.length() - 1);
    genSpin->setMaximum(generations.length() - 1);
}

void GPVis::showCrossTable()
{
    selectedView = CROSSOVERS;
    tableView->setModel(crossovers);

    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);
    tableView->resizeColumnToContents(2);

    tableView->selectionModel()->disconnect(this);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(crossoverFromTable()));

    genSlider->setMaximum(generations.length() - 2);
    genSpin->setMaximum(generations.length() - 2);
}

void GPVis::showMutTable()
{
    selectedView = MUTATIONS;
    tableView->setModel(mutations);

    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);

    tableView->selectionModel()->disconnect(this);
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(mutationFromTable()));

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
    fileField->setText("tinygp/test/problem.log");
    //fileField->setText("../../../tinygp/test/problem.log");
    QTest::mouseClick(fileOpen, Qt::LeftButton);

    //Tree::test(scene);
    /*
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));
    //qDebug() << "Tree::test : " << test_tree->index;
    */
}
