#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QObject>

#include <cmath>

#include "style.h"
#include "generation.h"

#define DEFAULT_HISTOGRAM_SIZE 10

struct idfit
{
    int id;
    float fit;
};

bool fitnessLessThan(const idfit &a, const idfit &b);

class HistogramBar: public QGraphicsRectItem
{
    public:
        HistogramBar(qreal x, qreal y, qreal width, qreal height, QList<int> nids, int nnum);
        void select();
        void deselect();
        QList<int> getIds();
        int getNum();
    private:
        int num;
        QList<int> ids;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

};

class Histogram: public QGraphicsScene
{
    Q_OBJECT

    public:
        Histogram(qreal x, qreal y, qreal width, qreal height, QWidget *parent);
        void setSize(int size);
        void setData(Generation *gen);
        void draw();
        QList<int> getSlice(int slice);
        void selectSlice(int slice);
        int getSelectedSlice();
    private:
        int size;
        Generation *data;
        QList<HistogramBar*> bars;
        int selectedSlice;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    signals:
        void clickedBar();
};
