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

void readLogFile()
{
    extern Def *definition;
}
