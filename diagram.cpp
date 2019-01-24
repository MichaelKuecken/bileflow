#include "diagram.h"
#include <QPainterPath>
#include <iostream>
#include <math.h>

Diagram::Diagram(QList<double> dataset1, QList<double> dataset2)
{
    _parent = new QGraphicsView();
    //_parent->setParent(holder);
    _scene = new QGraphicsScene(QRect(0,0,1000,1000));
    _scene->setParent(_parent);
    _data1 = dataset1;
    _data2 = dataset2;
    draw(dataset1, dataset2);
}

int getFac(double bereich)
{
    int fac = 0;
    if (bereich >= 1)
    {
        while (bereich > pow(10, fac))
        {
            ++fac;
        }
        --fac;
    }
    else
    {
        while(bereich <= pow(10, fac))
        {
            --fac;
        }
    }
    return fac;
}

double getPosX(int leftOffset, double chartsize, double value,double min, double frame)
{
    return leftOffset + chartsize * ((value - min) / frame);
}

double getPosY(int bottomOffset, double sceneSize,double chartsize, double value,double min, double frame)
{
    return sceneSize - bottomOffset - chartsize * ((value - min)/ frame);
}

void Diagram::draw(QList<double> zahlenx,QList<double> zahleny, int sizeFac)
{
    _parent->setGeometry(this->geometry());
    if(zahleny.length() && zahleny.length() == zahlenx.length())
    {
        //set offset
        int edge_left = 40;
        int edge_right = 20;
        int edge_top = 20;
        int edge_botom = 20;

        //get largest and smallest value
        //set to 0 to guarantee, that the axis stay visible

        double sy = 0;
        double ly = 0;
        double sx = 0;
        double lx = 0;

        double rsmallx = zahlenx[0];

        //find smallest/biggest value
        for(int i = 0; i < zahlenx.length(); ++i)
        {
            if(sy > zahleny[i]){sy = zahleny[i];}
            if(ly < zahleny[i]){ly = zahleny[i];}
            if(sx > zahlenx[i]){sx = zahlenx[i];}
            if(lx < zahlenx[i]){lx = zahlenx[i];}
            if(rsmallx < zahlenx[i]){rsmallx = zahlenx[i];}
        }

        //get the frame of values
        double frameY = ly - sy;
        double frameX = lx - sx;

        //get factor
        int facY = getFac(frameY);
        int facX =getFac(frameX);

        //get scene size

        if(edge_left < QString::number(pow(10,facY)).length() * 8)
        {
            edge_left = QString::number(pow(10,facY)).length() * 8;
        }

        double sceneSizeY = edge_top + edge_botom + (frameY*pow(10, -1*facY))*sizeFac;
        double sceneSizeX = edge_left + edge_right + (frameX*pow(10, -1*facX))*sizeFac;

        if(sceneSizeY < 400){sceneSizeY = 425;}
        if(sceneSizeX < 400){sceneSizeX = 700;}
        double chartSizeY = sceneSizeY - edge_top - edge_botom;
        double chartSizeX = sceneSizeX - edge_left - edge_right;
        _scene->setSceneRect(0,0,sceneSizeX,sceneSizeY);

        //paint Axis
        QGraphicsLineItem* y_axis = new QGraphicsLineItem(getPosX(edge_left, chartSizeX, 0, sx, frameX),edge_top,
                                                          getPosX(edge_left, chartSizeX, 0, sx, frameX),sceneSizeY - edge_botom);
        QGraphicsLineItem* x_axis = new QGraphicsLineItem(edge_left,getPosY(edge_botom,sceneSizeY,chartSizeY,0,sy,frameY),sceneSizeX - edge_right,
                                                          getPosY(edge_botom,sceneSizeY,chartSizeY,0,sy,frameY));
        y_axis->setPen(QPen(Qt::black, 2));
        x_axis->setPen(QPen(Qt::black, 2));
        QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(0), x_axis);
        qgsti_text->setScale(0.80);
        qgsti_text->setPos(getPosX(edge_left, chartSizeX, 0, sx, frameX) - QString::number(0).length() * 10, getPosY(edge_botom,sceneSizeY,chartSizeY,0,sy,frameY));
        _scene->addItem(y_axis);
        _scene->addItem(x_axis);

        //paint grid
        for(double x = pow(10,facX); x <= lx; x += pow(10,facX))
        {

            QGraphicsLineItem* y_gridline = new QGraphicsLineItem(getPosX(edge_left, chartSizeX, x, sx, frameX),edge_top,
                                                              getPosX(edge_left, chartSizeX, x, sx, frameX),sceneSizeY - edge_botom);
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(x), y_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(edge_left, chartSizeX, x, sx, frameX) - QString::number(x).length() * 8, getPosY(edge_botom,sceneSizeY,chartSizeY,0,sy,frameY));
            y_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(y_gridline);
        }

        for(double x = -1 * pow(10,facX); x >= sx; x -= pow(10,facX))
        {

            QGraphicsLineItem* y_gridline = new QGraphicsLineItem(getPosX(edge_left, chartSizeX, x, sx, frameX),edge_top,
                                                              getPosX(edge_left, chartSizeX, x, sx, frameX),sceneSizeY - edge_botom);
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(x), y_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(edge_left, chartSizeX, x, sx, frameX) - QString::number(x).length() * 8, getPosY(edge_botom,sceneSizeY,chartSizeY,0,sy,frameY));
            y_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(y_gridline);
        }

        for(double y = pow(10,facY); y <= ly; y += pow(10,facY))
        {

            QGraphicsLineItem* x_gridline = new QGraphicsLineItem(edge_left,getPosY(edge_botom,sceneSizeY,chartSizeY,y,sy,frameY),sceneSizeX - edge_right,
                                                                  getPosY(edge_botom,sceneSizeY,chartSizeY,y,sy,frameY));
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(y), x_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(edge_left, chartSizeX, 0, sx, frameX) - QString::number(y).length() * 8, getPosY(edge_botom,sceneSizeY,chartSizeY,y,sy,frameY));
            x_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(x_gridline);
        }

        for(double y = -1 * pow(10,facY); y >= sy; y -= pow(10,facY))
        {

            QGraphicsLineItem* x_gridline = new QGraphicsLineItem(edge_left,getPosY(edge_botom,sceneSizeY,chartSizeY,y,sy,frameY),sceneSizeX - edge_right,
                                                                  getPosY(edge_botom,sceneSizeY,chartSizeY,y,sy,frameY));
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(y), x_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(edge_left, chartSizeX, 0, sx, frameX) - QString::number(y).length() * 8, getPosY(edge_botom,sceneSizeY,chartSizeY,y,sy,frameY));
            x_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(x_gridline);
        }

        //draw Values
        for(int x = 0; x < zahlenx.length() - 1; ++x )
        {
            QGraphicsLineItem* connect = new QGraphicsLineItem(getPosX(edge_left, chartSizeX, zahlenx[x], sx, frameX),getPosY(edge_botom,sceneSizeY,chartSizeY,zahleny[x],sy,frameY),
                                                              getPosX(edge_left, chartSizeX, zahlenx[x + 1], sx, frameX),getPosY(edge_botom,sceneSizeY,chartSizeY,zahleny[x + 1],sy,frameY));
            connect->setPen(QPen(Qt::blue, 1));
            _scene->addItem(connect);
        }

        //update scene
        _parent->setScene(_scene);
        _parent->centerOn(getPosX(edge_left, chartSizeX, 0, sx, frameX),getPosY(edge_botom,sceneSizeY,chartSizeY,0,sy,frameY));
        _parent->show();
    }
}
