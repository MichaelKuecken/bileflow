#include "gbileflow.h"
#include "ui_gbileflow.h"
using namespace std;

Gbileflow::Gbileflow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gbileflow)
{
    ///GUI setup
    ui->setupUi(this);
    qtw_main = new QTabWidget();
    openAction = new QAction(tr("&Open"), this);
    saveAction = new QAction(tr("&Save"), this);
    exitAction = new QAction(tr("E&xit"), this);
    startAction = new QAction(tr("&Start"), this);
    cTabAction = new QAction(tr("&Close Tab"), this);
    qtw_main->setParent(this);
    qtw_main->setGeometry(0, 50, 1000, 550);
    qtw_main->show();

    ///connect signals and slots
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(cTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));
    connect(startAction, SIGNAL(triggered()), this, SLOT(get_started()));

    ///setup filemenu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    saveAction->setDisabled(true);
    fileMenu->addAction(cTabAction);
    cTabAction->setDisabled(true);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    fileMenu->addAction(startAction);
    startAction->setDisabled(true);
}

QList<double> makeDataset(int column, QTableWidget* datagrid)
{
    QList<double> tmp;
    for(int i = 0; i < datagrid->rowCount(); ++i)
    {
        tmp.append(datagrid->item(i,column)->text().toDouble());
    }
    return tmp;
}

void Gbileflow::calculate(QTableWidget* qtw)
{
    const string workdir = "/home/cedric/Documents/Qt_Testfiles/";
    const string identifier = "6724";

    const string inputfile = workdir + "input_" + identifier + ".dat";
    const string outputfile =  workdir + "output_" + identifier + ".dat";

    Model* model = mlist[qtw_main->currentIndex()];

    double pout = model->shooting();

    model->single_run(pout, true);

    //string filename = "/home/michael/bileflow/real_values/output.dat";

    model->printout_results(qtw);
    for(int i = 1; i < 10; ++i)
    {
        tlist[qtw_main->currentIndex()]->add(makeDataset(0,qtw),makeDataset(i,qtw));
    }
}

///starting a caluclation is only possible, if any Tab is open
///starting a calculation is also not possible if it`s already calculated
void  Gbileflow::get_started()
{
    if(tlist.length() >= 1)
    {
        calculate(tlist[qtw_main->currentIndex()]->get_table());
    }
}

void Gbileflow::open()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
    tr("Text Files (*.dat)"));
    QString tmp = "";
    for(auto& sign : fileName)
    {
        if(sign == '/')
        {
            tmp = "";
        }else{
            tmp += sign;
        }
    }
    if (tmp != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        qtw_main->setCurrentIndex(qtw_main->addTab(new QWidget(), tmp));
        Tabcontainer* tabcon = new Tabcontainer(fileName, qtw_main->currentWidget());
        tlist.push_back(tabcon);
        mlist.push_back(new Model(fileName.toStdString()));
        if(tlist.length() == 1)
        {
            saveAction->setDisabled(false);
            cTabAction->setDisabled(false);
            startAction->setDisabled(false);
        }
    }
}

///closing a Tab is only possible if any tab is open
void Gbileflow::closeTab()
{
    tlist.removeAt(qtw_main->currentIndex());
    mlist.removeAt(qtw_main->currentIndex());
    qtw_main->removeTab(qtw_main->currentIndex());
    if(tlist.length() <= 0)
    {
        saveAction->setDisabled(true);
        cTabAction->setDisabled(true);
        startAction->setDisabled(true);
    }
}

void Gbileflow::save()
{
    if(tlist.length() >= 1)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Text Files (*.dat)"));

        if (fileName != "") {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                // error message
            } else {
                mlist[qtw_main->currentIndex()]->printout_results(fileName.toStdString() + ".dat");
                file.close();
            }
        }
    }
}


Gbileflow::~Gbileflow()
{
    delete ui;
}
