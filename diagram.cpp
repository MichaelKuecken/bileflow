#include "diagram.h"
#include <QPainterPath>
#include <iostream>
#include <math.h>
#include <QGraphicsItemGroup>

Diagram::Diagram()
{
    row = -1;
    _parent = new QGraphicsView();
    _scene = new QGraphicsScene();

}

//have to be called before drawing!
void Diagram::set_parent(QWidget* parent)
{
    holder = parent;
    _parent->setParent(holder);
    _parent->setGeometry(holder->geometry());
    _scene->setParent(_parent);
}

int getFac(double bereich)
{
    int fac = 0;
    if (bereich >= 1)
    {
        while (bereich > pow(10, fac)*2)
        {
            ++fac;
        }
        --fac;
    }
    else
    {
        while(bereich <= pow(10, fac)*2)
        {
            --fac;
        }
    }
    return fac;
}

double Diagram::getPosX(double value)
{
    return edge_left + chartSizeX * ((value - sx) / frameX);
}

double Diagram::getPosY(double value)
{
    return sceneSizeY - edge_botom - chartSizeY * ((value - sy)/ frameY);
}

void Diagram::draw(QList<double> zahlenx,QList<double> zahleny, int sizeFac)
{
    ///save data
    _data1 = zahlenx;
    _data2 = zahleny;
    if(zahleny.length() && zahleny.length() == zahlenx.length())
    {
        ///set offset
        edge_left = 40;
        edge_right = 20;
        edge_top = 20;
        edge_botom = 20;

        ///get largest and smallest value
        ///set to 0 to guarantee, that the axis stay visible

        sy = 0;
        ly = 0;
        sx = 0;
        lx = 0;

        rsmallx = zahlenx[0];

        ///find smallest/biggest value
        for(int i = 0; i < zahlenx.length(); ++i)
        {
            if(sy > zahleny[i]){sy = zahleny[i];}
            if(ly < zahleny[i]){ly = zahleny[i];}
            if(sx > zahlenx[i]){sx = zahlenx[i];}
            if(lx < zahlenx[i]){lx = zahlenx[i];}
            if(rsmallx < zahlenx[i]){rsmallx = zahlenx[i];}
        }

        ///get the frame of values
        frameY = ly - sy;
        frameX = lx - sx;

        ///get factor
        facY = getFac(frameY);
        facX =getFac(frameX);

        ///get scene size

        if(edge_left < QString::number(pow(10,facY)).length() * 8)
        {
            edge_left = QString::number(pow(10,facY)).length() * 8;
        }

        sceneSizeY = edge_top + edge_botom + (frameY*pow(10, -1*facY))*sizeFac;
        sceneSizeX = edge_left + edge_right + (frameX*pow(10, -1*facX))*sizeFac;

        if(sceneSizeY < holder->height() - 25){sceneSizeY = holder->height() - 25;}
        if(sceneSizeX < holder->width() - 25){sceneSizeX = holder->width() - 25;}
        chartSizeY = sceneSizeY - edge_top - edge_botom;
        chartSizeX = sceneSizeX - edge_left - edge_right;
        _scene->setSceneRect(0,0,sceneSizeX,sceneSizeY);

        ///paint Axis
        y_axis = new QGraphicsLineItem(getPosX(0),edge_top,
                                                          getPosX(0),sceneSizeY - edge_botom);
        x_axis = new QGraphicsLineItem(edge_left,getPosY(0),sceneSizeX - edge_right,
                                                          getPosY(0));
        y_axis->setPen(QPen(Qt::black, 2));
        x_axis->setPen(QPen(Qt::black, 2));
        QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(0), x_axis);
        qgsti_text->setScale(0.80);
        qgsti_text->setPos(getPosX(0) - QString::number(0).length() * 10, getPosY(0));
        _scene->addItem(y_axis);
        _scene->addItem(x_axis);

        ///paint grid
        for(double x = pow(10,facX); x <= lx; x += pow(10,facX))
        {

            QGraphicsLineItem* y_gridline = new QGraphicsLineItem(getPosX(x),edge_top,
                                                              getPosX(x),sceneSizeY - edge_botom);
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(x), y_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(x) - QString::number(x).length() * 8, getPosY(0));
            y_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(y_gridline);
        }

        for(double x = -1 * pow(10,facX); x >= sx; x -= pow(10,facX))
        {

            QGraphicsLineItem* y_gridline = new QGraphicsLineItem(getPosX(x),edge_top,
                                                              getPosX(x),sceneSizeY - edge_botom);
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(x), y_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(x) - QString::number(x).length() * 8, getPosY(0));
            y_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(y_gridline);
        }

        for(double y = pow(10,facY); y <= ly; y += pow(10,facY))
        {

            QGraphicsLineItem* x_gridline = new QGraphicsLineItem(edge_left,getPosY(y),sceneSizeX - edge_right,
                                                                  getPosY(y));
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(y), x_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(0) - QString::number(y).length() * 8, getPosY(y));
            x_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(x_gridline);
        }

        for(double y = -1 * pow(10,facY); y >= sy; y -= pow(10,facY))
        {

            QGraphicsLineItem* x_gridline = new QGraphicsLineItem(edge_left,getPosY(y),sceneSizeX - edge_right,
                                                                  getPosY(y));
            QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString::number(y), x_gridline);
            qgsti_text->setScale(0.80);
            qgsti_text->setPos(getPosX(0) - QString::number(y).length() * 8, getPosY(y));
            x_gridline->setPen(QPen(Qt::gray, 1));
            _scene->addItem(x_gridline);
        }

        ///draw Values
        for(int x = 0; x < zahlenx.length() - 1; ++x )
        {
            QGraphicsLineItem* connect = new QGraphicsLineItem(getPosX(zahlenx[x]),getPosY(zahleny[x]),
                                                              getPosX(zahlenx[x + 1]),getPosY(zahleny[x + 1]));
            connect->setPen(QPen(Qt::blue, 1));
            _scene->addItem(connect);
        }

        //update scene
        _parent->setScene(_scene);
        _parent->centerOn(getPosX(0),getPosY(0));
        _parent->show();
    }
}

void Diagram::single_point(int add)
{
    row += add;
    if(xpoint != nullptr)
    {
        _scene->removeItem(xpoint);
        _scene->removeItem(ypoint);
        xpoint = nullptr;
        ypoint = nullptr;
    }
    if(row >= 0 && row < _data1.length())
    {
        ypoint = new QGraphicsLineItem(getPosX(_data1[row]),edge_top, getPosX(_data1[row]),sceneSizeY - edge_botom);
        xpoint = new QGraphicsLineItem(edge_left,getPosY(_data2[row]),sceneSizeX - edge_right,getPosY(_data2[row]));

        ypoint->setPen(QPen(Qt::red, 1));
        xpoint->setPen(QPen(Qt::red, 1));

        QGraphicsSimpleTextItem* qgsti_text = new QGraphicsSimpleTextItem(QString(), xpoint);
        qgsti_text->setScale(0.80);
        QString txt = "(x: " + QString::number(_data1[row]) + "|y: " + QString::number(_data2[row]) + ")";
        qgsti_text->setText(txt);
        qgsti_text->setPos(getPosX(_data1[row]) - txt.length() * 6, getPosY(_data2[row]) - 20);
        qgsti_text->setBrush(QBrush(Qt::red));

        _scene->addItem(xpoint);
        _scene->addItem(ypoint);
        _parent->show();
    }else if(row < 0){
        row = -1;
    }else{
        row = _data1.length();
    }
}
