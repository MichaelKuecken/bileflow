#include "tabcontainer.h"

Tabcontainer::Tabcontainer(QString filename, QWidget* parent)
{
    _filename = filename;
    qtw_table = new QTableWidget();
    qtw_table->setGeometry(725, 25, 250, 450);
    qtw_table->setParent(parent);
    qtw_table->show();
}

QString Tabcontainer::get_file()
{
    return _filename;
}

QTableWidget* Tabcontainer::get_table()
{
    return  qtw_table;
}

void Tabcontainer::add(QList<double>list1, QList<double>list2)
{
    new Diagram(list1, list2);
}
