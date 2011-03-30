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
    int WIDTH = 800;
    int HEIGHT = 600;
    int SCENE_WIDTH = 800;
    int SCENE_HEIGTH = 1200;

    resize(WIDTH, HEIGHT);
    center(this, WIDTH, HEIGHT);

    grid = new QGridLayout(this);

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGTH, this);
    preview = new QGraphicsView(this);
    preview->setScene(scene);
    
    setLayout(grid);
}
