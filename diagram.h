#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QtWidgets>

/**
 * @brief The Diagram class
 * Responsible for everything that happens in the chart.
 *
 */
class Diagram
{
public:
    Diagram();

    void draw(QList<double>, QList<double>, int sizefac = 10, QString="X", QString="Y");

    void set_parent(QWidget*);

    double getPosX(double);

    double getPosY(double);

    void single_point(int);

private:

    int edge_left;
    int edge_right;
    int edge_botom;
    int edge_top;

    double sy;
    double ly;
    double sx;
    double lx;

    double rsmallx;

    double frameY;
    double frameX;

    int facX;
    int facY;

    double sceneSizeX;
    double sceneSizeY;

    double chartSizeY;
    double chartSizeX;

    int row;

    QGraphicsLineItem* y_axis;
    QGraphicsLineItem* x_axis;


    QGraphicsLineItem* ypoint = nullptr;
    QGraphicsLineItem* xpoint = nullptr;

    ///parent object
    QWidget* holder;
    ///parent of the chartScene
    QGraphicsView* _parent;
    ///parent of the chartItems
    QGraphicsScene* _scene;
    ///datasets
    QList<double> _data1;
    QList<double> _data2;
};


#endif // DIAGRAM_H
