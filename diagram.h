#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QtWidgets>

class Diagram : public QWidget
{
public:
    Diagram(QList<double>, QList<double>);

    void draw(QList<double>, QList<double>, int sizefac = 10);

private:
    QTabWidget* holder;
    QGraphicsView* _parent;
    QGraphicsScene* _scene;
    QList<double> _data1;
    QList<double> _data2;
};


#endif // DIAGRAM_H
