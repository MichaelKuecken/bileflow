#include "tabcontainer.h"
#include <iostream>

Tabcontainer::Tabcontainer(QString filename, QWidget* parent)
{
    _filename = filename;
    qtw_table = new QTableWidget();

    chart_holder = new QTabWidget();
    chart_holder->setGeometry(25, 25, 675, 450);
    chart_holder->setParent(parent);

    chart_holder->show();

    _parent = parent;
}

QString Tabcontainer::get_file()
{
    return _filename;
}

QTableWidget* Tabcontainer::get_table()
{
    delete qtw_table;
    qtw_table = new QTableWidget();
    qtw_table->setGeometry(725, 25, 250, 450);
    qtw_table->verticalHeader()->setVisible(false);
    qtw_table->setParent(_parent);
    qtw_table->show();
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

void Tabcontainer::add(QList<double>list1, QList<double>list2, QString text)
{
    int already = -1;
    for(int i = 0; i < chart_holder->count(); ++i)
    {
        if(chart_holder->tabText(i) == text)
        {
            already = i;
        }
    }
    if(already == -1){
        chart_holder->setCurrentIndex(chart_holder->addTab(new QWidget(), text));
        dList.push_back(new Diagram());

    }else{
        chart_holder->removeTab(already);
        chart_holder->insertTab(already,new QWidget(), text);
        chart_holder->setCurrentIndex(already);
        dList[already] = new Diagram();
    }
    dList[chart_holder->currentIndex()]->set_parent(chart_holder->currentWidget());
    dList[chart_holder->currentIndex()]->draw(list1, list2);
    chart_holder->setCurrentIndex(0);
}
