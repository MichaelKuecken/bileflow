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
private:
    QString _filename;
    QTableWidget* qtw_table;
    QGraphicsView* qgv_chart;
    Diagram* dia;
};

#endif // TABCONTAINER_H
