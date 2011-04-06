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
    preview = new QGraphicsView(this);
    preview->setScene(scene);
    
    fileField = new QLineEdit(this);
    fileSelect = new QPushButton("Select file", this);
    fileOpen = new QPushButton("Read file", this);

    grid = new QGridLayout(this);
    grid->addWidget(preview, 0, 0, -1, 1);
    grid->addWidget(fileField, 0, 1, 0, 2);
    grid->addWidget(fileSelect, 1, 1);
    grid->addWidget(fileOpen, 1, 2);
    setLayout(grid);
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
    
    /* check if file exists and can be read */
    if(!fileFile->open(QIODevice::WriteOnly) | !fileFile->exists())
    {
        //TODO: implement exception if file doesnt exist
        return;
    }

    QTextStream in(fileFile);
    QString ops, vars, terms;
    
    fileBuffer = in.readLine();

    if(fileBuffer.contains(QRegExp("definition:*.")))
    {
        while(!in.atEnd())
        {
            fileBuffer = in.readLine();

            /* ops */
            if(fileBuffer.contains(QRegExp("\tops:*.")))
            {
                ops = fileBuffer.remove(QRegExp("\tops:\\s"));
                continue;
            }

            /* vars */
            if(fileBuffer.contains(QRegExp("\tvars:*.")))
            {
                vars = fileBuffer.remove(QRegExp("\tvars:\\s"));
                continue;
            }
            
            /* terms */
            if(fileBuffer.contains(QRegExp("\tterms:*.")))
            {
                terms = fileBuffer.remove(QRegExp("\tterms:\\s"));
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
