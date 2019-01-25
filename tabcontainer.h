#ifndef TABCONTAINER_H
#define TABCONTAINER_H

#include "diagram.h"
#include <QFile>
#include <QtWidgets>
#include <QPainterPath>

class Tabcontainer
{
public:
    Tabcontainer(QString, QWidget*);

    QString get_file();

    QTableWidget* get_table();

    void add(QList<double>,QList<double>);

    void makePoint(QKeyEvent*);

private:
    QString _filename;
    QList<Diagram*> dList;
    QTableWidget* qtw_table;
    QTabWidget* chart_holder;
    QGraphicsView* qgv_chart;
    Diagram* dia;
};

#endif // TABCONTAINER_H
