#include "tabcontainer.h"
#include <iostream>

Tabcontainer::Tabcontainer(QString filename, QWidget* parent)
{
    _filename = filename;
    qtw_table = new QTableWidget();
    saveplace = "";
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

///Trigger the crosshair in the chart
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

///make a new Diagram
void Tabcontainer::add(QList<double>list1, QList<double>list2, QString text)
{
    ///test if the chart is already open
    std::cout<<text.toStdString()<<std::endl;
    int already = -1;
    for(int i = 0; i < chart_holder->count(); ++i)
    {
        std::cout<<"    " + chart_holder->tabText(i).toStdString()<<std::endl;
        if(chart_holder->tabText(i) == text)
        {
            already = i;
        }
    }
    std::cout<<already<<std::endl;
    ///if not add a new Diagram
    if(already == -1){
        chart_holder->setCurrentIndex(chart_holder->addTab(new QWidget(), text));
        dList.push_back(new Diagram());

    ///else go to the Diagram and update it
    }else{
        //delete chart_holder->widget(already);
        //chart_holder->removeTab(already);
        //chart_holder->insertTab(already,new QWidget(), text);
        chart_holder->setCurrentIndex(already);
        dList.replace(already, new Diagram());
    }

    ///draw chart
    dList[chart_holder->currentIndex()]->set_parent(chart_holder->widget(chart_holder->currentIndex()));
    dList[chart_holder->currentIndex()]->draw(list1, list2);
    chart_holder->setCurrentIndex(0);
}

Tabcontainer::~Tabcontainer(){
    for(auto &elem : dList){
        delete elem;
    }
}

void Tabcontainer::mkPng(QString directory){
    for(int i=0; i< dList.length(); ++i){
        QString text = chart_holder->tabText(i) + ".png";
        for(auto &elem : text){
            if(elem == ' '){
                elem = '_';
            }else if(elem == '/'){
                elem = '|';
            }
        }
        std::cout<<(text).toStdString()<<std::endl;
        dList[i]->get_scene()->clearSelection();
        dList[i]->get_scene()->setSceneRect(dList[i]->get_scene()->itemsBoundingRect());

        QImage image(dList[i]->get_scene()->sceneRect().size().toSize(), QImage::Format_ARGB32);
        image.fill(Qt::white);

        QPainter painter(&image);
        dList[i]->get_scene()->render(&painter);
        image.save(directory + text);
    }
}
