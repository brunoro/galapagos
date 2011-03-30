#ifndef GPVIS_H 
#define GPVIS_H

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

class GPVis : public QWidget
{
    Q_OBJECT

    public:
        GPVis(QWidget *parent = 0);

    private:
        QGridLayout *grid;
        QSlider *genslider;
        QSpinBox *genspin;
        QGraphicsView *preview;
        QGraphicsScene *scene;
};

#endif
