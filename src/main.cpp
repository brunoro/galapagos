#include "gpvis.h"

/* global problem definition */
Def *definition;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    GPVis window;

    window.setWindowTitle("GPVis");
    window.show();

    return app.exec();
}
