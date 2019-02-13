#ifndef GBILEFLOW_H
#define GBILEFLOW_H

#include "settings.h"

#include "model.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QtWidgets>
#include <QList>

namespace Ui {
class Gbileflow;
class Settings;
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
    void calculate();
    void keyPressEvent(QKeyEvent*);
    ~Gbileflow();
signals:
    void itsTime2stop();

public slots:
    ///start calculation
    void paint(QTableWidget*, Tabcontainer*);
    void get_started();
    void closeTab();
    void open(QString dat = "");
    void settings();
    void save();
    void breakThread(QThread*);
    void takeSets(QList<double>);
    void delsets(Settings*);

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
    //QList<bool> calculated_tabs;
    ///stor the calculation-class for every tab
    QList<Model*> mlist;
    QList<QList<double>> slist;
};
#endif // GBILEFLOW_H
