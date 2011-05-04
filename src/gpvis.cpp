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
    preview = new QGraphicsView(scene);
    preview->setRenderHint(QPainter::Antialiasing);

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
    tableView->setEnabled(false);

    fileField = new QLineEdit(this);
    fileSelect = new QPushButton("Select file", this);
    fileOpen = new QPushButton("Read file", this);

    grid = new QGridLayout(this);
    grid->addWidget(preview, 0, 0, -1, 1);
    grid->addWidget(fileField, 0, 1);
    grid->addWidget(fileSelect, 0, 2);
    grid->addWidget(fileOpen, 0, 3);
    grid->addWidget(genLabel, 1, 1);
    grid->addWidget(genSlider, 1, 2);
    grid->addWidget(genSpin, 1, 3);
    grid->addWidget(viewInd, 2, 1);
    grid->addWidget(viewCross, 2, 2);
    grid->addWidget(viewMut, 2, 3);
    grid->addWidget(tableView, 3, 1, 3, 3);
    setLayout(grid);

    
    connect(fileSelect, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(readLogFile()));

    connect(genSlider, SIGNAL(valueChanged(int)), genSpin, SLOT(setValue(int)));
    connect(genSpin, SIGNAL(valueChanged(int)), genSlider, SLOT(setValue(int)));
    connect(genSlider, SIGNAL(valueChanged(int)), this, SLOT(renderGeneration(int)));

    connect(viewInd, SIGNAL(toggled(bool)), this, SLOT(showIndTable()));
    connect(viewCross, SIGNAL(toggled(bool)), this, SLOT(showCrossTable()));
    connect(viewMut, SIGNAL(toggled(bool)), this, SLOT(showMutTable()));
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
    
    /* set individuals as default view */
    viewInd->setDown(true);

    /* define first generation read */
    renderGeneration(0);

    
}

/* builds model from a generation */
void GPVis::renderGeneration(int gen)
{
    //qDebug() << "GPVis::renderGeneration " << gen;
    QStandardItemModel *ind, *cross, *mut;

    Generation *actual = generations[gen];
    /* population */
    ind = new QStandardItemModel(actual->population_tree.length(), 2);
    for(int i = 0; i < actual->population_tree.length(); i++)
    {
        ind->setItem(i, 0, new QStandardItem(actual->population_fit[i]));
        ind->setItem(i, 1, new QStandardItem(actual->population_tree[i]));
    }

    /* crossovers and mutations */
    cross = new QStandardItemModel(actual->crossovers.length(), 3);
    mut = new QStandardItemModel(actual->mutations.length(), 2);

    /* if it is not last generation, get next */
    if(gen < generations.length() - 1)
    {
        Generation *next = generations[gen + 1];
        for(int i = 0; i < actual->crossovers.length(); i++)
        {
            cross->setItem(i, 0, new QStandardItem(QString::number(actual->crossovers[i].parent1)));
            cross->setItem(i, 1, new QStandardItem(QString::number(actual->crossovers[i].parent2)));
            cross->setItem(i, 2, new QStandardItem(QString::number(actual->crossovers[i].offspring)));
        }
        for(int i = 0; i < actual->mutations.length(); i++)
        {
            cross->setItem(i, 0, new QStandardItem(QString::number(actual->mutations[i].parent)));
            cross->setItem(i, 1, new QStandardItem(QString::number(actual->mutations[i].offspring)));
        }
    }

    // TODO: make this work
    /* cleanup last viewed
    if(individuals != NULL)
        delete individuals;

    if(crossovers != NULL)
        delete crossovers;

    if(mutations != NULL)
        delete mutations; */

    /* replace */
    individuals = ind;
    crossovers = cross;
    mutations = mut;

    //TODO: change this after putting radio buttons
    tableView->setModel(ind);
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

void GPVis::showIndTable()
{
    tableView->setModel(individuals);
}

void GPVis::showCrossTable()
{
    tableView->setModel(crossovers);
}

void GPVis::showMutTable()
{
    tableView->setModel(mutations);
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

    Tree::test(scene);
    /*
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));
    //qDebug() << "Tree::test : " << test_tree->index;
    */
}
