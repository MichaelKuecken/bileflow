#include "tabcontainer.h"
#include <iostream>

Tabcontainer::Tabcontainer(QString filename, QWidget* parent)
{
    _filename = filename;
    qtw_table = new QTableWidget();
    qtw_table->setGeometry(725, 25, 250, 450);
    qtw_table->verticalHeader()->setVisible(false);
    qtw_table->setParent(parent);

    chart_holder = new QTabWidget();
    chart_holder->setGeometry(25, 25, 675, 450);
    chart_holder->setParent(parent);

    chart_holder->show();
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

void Tabcontainer::makePoint(QKeyEvent* e)
{
    if(dList.length() > 0)
    {

        if(e->key() == Qt::Key_Down)
        {
            dList[chart_holder->currentIndex()]->single_point(-1);
        }else{
            dList[chart_holder->currentIndex()]->single_point(1);
        }
    }
}

void Tabcontainer::add(QList<double>list1, QList<double>list2)
{
    chart_holder->setCurrentIndex(chart_holder->addTab(new QWidget(), "Start"));
    dList.push_back(new Diagram());
    dList[chart_holder->currentIndex()]->set_parent(chart_holder->currentWidget());
    dList[chart_holder->currentIndex()]->draw(list1, list2);
}
