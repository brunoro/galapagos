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

int GPVis::newTreeId()
{
    return trees.count();
}

void GPVis::addTree(Tree *tree)
{
    trees.append(tree);
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

    QTextStream fileStream(fileFile);
    QString ops, vars, terms;
    
    QString fileBuffer = fileStream.readLine();

    if(fileBuffer.contains(QRegExp("definition:*.")))
    {
        qDebug() << "GPVis::readLogFile found definition"; 
        while(!fileStream.atEnd())
        {
            fileBuffer = fileStream.readLine();

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
                break;
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

void GPVis::openFileDialog()
{
    fileField->setText(QFileDialog::getOpenFileName(this,
                                                    tr("Open log"),
                                                    QDir::currentPath(),
                                                    tr("All files (*.*)")));
}

QHash<int, QColor> GPVis::getColorPalette(QList<int> ids)
{
    QHash<int, QColor> palette;
    int h = 190,
        s = 190,
        v = 0;
    float ratio = 255/ids.length();
    for(int i = 0; i < ids.length(); i++)
    {
        QColor color;
        v += ratio;
        palette[i] = color.fromHsv(h, s, v);
    }
    return palette;
}

void GPVis::test()
{
    fileField->setText("tinygp/test/problem.log");
    QTest::mouseClick(fileOpen, Qt::LeftButton);

    Tree::test(scene);
    /*
    QString line = QString("-  x^2  /  -  x^0  *  x^0  x^1  /  x^2  x^2");
    Tree *test_tree = new Tree(line, -1, float(0.22222));
    qDebug() << "Tree::test : " << test_tree->index;
    */
}
