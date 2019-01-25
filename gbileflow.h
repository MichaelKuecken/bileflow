#ifndef GBILEFLOW_H
#define GBILEFLOW_H

#include "model.h"
#include "tabcontainer.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QtWidgets>
#include <QList>

namespace Ui {
class Gbileflow;
}

/**
 * @brief The Gbileflow class
 * Store and control informations about the GUI
 */
class Gbileflow : public QMainWindow
{
    Q_OBJECT

public:

    explicit Gbileflow(QWidget *parent = nullptr);
    ///the actual calculation
    void calculate(QTableWidget*);
    void keyPressEvent(QKeyEvent*);
    ~Gbileflow();

private slots:
    ///start calculation
    void get_started();
    void closeTab();
    void open();
    void settings();
    void save();

private:
    Ui::Gbileflow *ui;
    /// TabWidget every file, that get opened, opens one tab
    QTabWidget* qtw_main;

    ///all Actions
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *cTabAction;
    QAction *startAction;
    QAction *setAction;

    QMenu *fileMenu;

    ///store the sub-GUI for every tab
    QList<Tabcontainer*> tlist;
    QList<bool> calculated_tabs;
    ///stor the calculation-class for every tab
    QList<Model*> mlist;
};

class Settings : public QMainWindow
{

};
#endif // GBILEFLOW_H
