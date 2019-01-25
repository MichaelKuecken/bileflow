#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QtWidgets>

class Diagram
{
public:
    Diagram();

    void draw(QList<double>, QList<double>, int sizefac = 10);

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

    QWidget* holder;
    QGraphicsView* _parent;
    QGraphicsScene* _scene;
    QList<double> _data1;
    QList<double> _data2;
};


#endif // DIAGRAM_H
