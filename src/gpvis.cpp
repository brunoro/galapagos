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
    int WIDTH = 805;
    int HEIGHT = 605;
    int SCENE_WIDTH = 400;
    int SCENE_HEIGTH = 300;

    resize(WIDTH, HEIGHT);
    center(this, WIDTH, HEIGHT);

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGTH, this);
    preview = new QGraphicsView(scene);
    preview->setRenderHint(QPainter::Antialiasing);

    fileField = new QLineEdit(this);
    fileSelect = new QPushButton("Select file", this);
    fileOpen = new QPushButton("Read file", this);

    grid = new QGridLayout(this);
    grid->addWidget(preview, 0, 0, -1, 1);
    grid->addWidget(fileField, 0, 1, 0, 2);
    grid->addWidget(fileSelect, 1, 1);
    grid->addWidget(fileOpen, 1, 2);
    setLayout(grid);

    connect(fileSelect, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(fileOpen, SIGNAL(clicked()), this, SLOT(readLogFile()));
}

void GPVis::readLogFile()
{
    fileFile = new QFile(fileField->text());
    qDebug() << "GPVis::readLogFile using file" << fileField->text();
    //qDebug() << fileField->text();
    
    /* check if file and can be read */
    if(!fileFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //TODO: implement exception if file doesnt exist
        return;
    }

    fileStream = new QTextStream(fileFile);
    QString ops, vars, terms;
    
    QString fileBuffer = fileStream->readLine();

    if(fileBuffer.contains(QRegExp("definition:*.")))
    {
        qDebug() << "GPVis::readLogFile found definition"; 
        while(!fileStream->atEnd())
        {
            fileBuffer = fileStream->readLine();

            /* ops */
            if(fileBuffer.contains(QRegExp("\tops:*.")))
            {
                ops = fileBuffer.remove(QRegExp("\tops:\\s*"));
                qDebug() << "GPVis::readLogFile found ops " << ops;
                continue;
            }

            /* vars */
            if(fileBuffer.contains(QRegExp("\tvars:*.")))
            {
                vars = fileBuffer.remove(QRegExp("\tvars:\\s*"));
                qDebug() << "GPVis::readLogFile found vars" << vars;
                continue;
            }
            
            /* terms */
            if(fileBuffer.contains(QRegExp("\tterms:*.")))
            {
                terms = fileBuffer.remove(QRegExp("\tterms:\\s*"));
                qDebug() << "GPVis::readLogFile found terms" << terms;
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

    extern Def *definition;
    definition = new Def(ops, vars, terms);
}

void GPVis::readGeneration()
{
    Generation *gen = new Generation();
    QString fileBuffer = fileStream->readLine(),
            individual,
            crossover,
            mutation;
    QStringList tokens;

    while(!(fileBuffer.contains(QRegExp("generation:*."))) ||
          !(fileStream->atEnd()))
    {
        /* individuals */
        if(fileBuffer.contains(QRegExp("ind:*.")))
        {
            individual = fileBuffer.remove(QRegExp("\tind:\\s*"));
            qDebug() << "GPVis::readLogFile found individual" << individual;
            tokens = individual.split("\t", QString::SkipEmptyParts);
            gen->addIndividual(new Tree(tokens[2], tokens[0].toInt(), tokens[1].toFloat()));
            continue;
        }
        /* crossover */
        if(fileBuffer.contains(QRegExp("crossover:*.")))
        {
            crossover = fileBuffer.remove(QRegExp("\tcrossover:\\s*"));
            qDebug() << "GPVis::readLogFile found crossover" << crossover;
            tokens = crossover.split(" ");
            gen->addCrossover(Crossover(gen->getIndividual(tokens[0].toInt()),
                                        gen->getIndividual(tokens[1].toInt()),
                                        tokens[3].toInt()));
            continue;
        }
        /* mutation */
        if(fileBuffer.contains(QRegExp("mutation:*.")))
        {
            mutation = fileBuffer.remove(QRegExp("\tmutation:\\s*"));
            qDebug() << "GPVis::readLogFile found mutation" << mutation;
            tokens = mutation.split(" ");
            gen->addMutation(Mutation(gen->getIndividual(tokens[0].toInt()),
                                      tokens[2].toInt()));
            continue;
        }

        fileBuffer = fileStream->readLine();
    }
    generations.append(gen);
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
    qDebug() << "Tree::test : " << test_tree->index;
    */
}
