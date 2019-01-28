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
    setAction = new QAction(tr("&Settings..."), this);

    qtw_main->setParent(this);
    qtw_main->setGeometry(0, 50, 1000, 550);
    qtw_main->show();

    ///connect signals and slots
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(cTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));
    connect(startAction, SIGNAL(triggered()), this, SLOT(get_started()));
    connect(setAction, SIGNAL(triggered()), this, SLOT(settings()));

    ///setup filemenu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    saveAction->setDisabled(true);
    fileMenu->addAction(cTabAction);
    fileMenu->addAction(setAction);
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
///starting a calculation should be also not possible if it`s already calculated
///else it could cause multiple errors and bugs
void  Gbileflow::get_started()
{
    if(tlist.length() >= 1)
    {
        calculate(tlist[qtw_main->currentIndex()]->get_table());
    }
}

//commeted
void Gbileflow::open()
{
    ///Open up a file dialoge
    ///it can only find .dat files as datasource
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
    tr("Text Files (*.dat)"));

    ///the tmp QString will store the file name during this function
    QString tmp = "";

    ///check every letter in the filename and attach it to the tmp QString
    ///to get only the name of the file and not fo the directories
    for(auto& sign : fileName)
    {
        ///if the current tmp QString is the name of a directory
        ///tmp gets flushed
        if(sign == '/')
        {
            tmp = "";
        }else{
            tmp += sign;
        }
    }

    ///This check tmp for invalide filenames
    ///like ""
    if (tmp != "") {

        QFile file(fileName);

        ///makes shure, that the file is not opened in readOnly mode
        ///if it is readOnly it throws an error message
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        //at this point should be a test that change the current widget if the
        //file is already open

        ///add a new tab to the QTabWidget qtw_main
        ///and set it as current Index, that will make it easier to
        ///access it for the rest of the function
        ///the new tab will be named after the open file
        qtw_main->setCurrentIndex(qtw_main->addTab(new QWidget(), tmp));

        ///create a new tabcontainer for the new Tab
        /// !!its important to not set it n the addTab() function since it could not
        /// access its parentWidget during constructing!!
        Tabcontainer* tabcon = new Tabcontainer(fileName, qtw_main->currentWidget());

        ///push the new Tabcontainer at the end of tList, where it is easy to access
        ///with tlist[currentTabIndex]
        tlist.push_back(tabcon);

        ///create a new Model with the path to the chosen file
        ///push the new model to the end of mlist to make it easier to access later on
        mlist.push_back(new Model(fileName.toStdString()));

        ///enable every action that is disabled while there are 0 tabs
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
    ///remove all stored information about the tab
    ///so they will not be accessed by mistake
    tlist.removeAt(qtw_main->currentIndex());
    mlist.removeAt(qtw_main->currentIndex());

    ///remove the actual tab
    qtw_main->removeTab(qtw_main->currentIndex());

    ///disable every Actions, that are not supposed to be
    ///available while no Tab is open, if this tab is the only one
    if(tlist.length() <= 0)
    {
        saveAction->setDisabled(true);
        cTabAction->setDisabled(true);
        startAction->setDisabled(true);
    }
}

///if key_up or key_down is pressed
void Gbileflow::keyPressEvent(QKeyEvent *event)
{
    if(tlist.length() > 0 && (event->key() == Qt::Key_Up|| event->key() == Qt::Key_Down))
    {
        tlist[qtw_main->currentIndex()]->makePoint(event);
    }
}

void Gbileflow::save()
{
    //maybe not neccessary since it is only available if there is a tab open
    if(tlist.length() >= 1)
    {
        ///create a .dat file
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Text Files (*.dat)"));

        ///check if the name is invalide
        /// currently only testing for ""
        if (fileName != "") {
            QFile file(fileName);
            ///if it is not possible to write in this file
            ///because it is not WriteOnly throw an errormessage
            if (!file.open(QIODevice::WriteOnly)) {
                // error message
            } else {
                ///Write all information in a file
                ///and close the file
                mlist[qtw_main->currentIndex()]->printout_results(fileName.toStdString() + ".dat");
                file.close();
            }
        }
    }
}

void Gbileflow::settings()
{
    Settings* setval = new Settings();
    setval->show();
}

Gbileflow::~Gbileflow()
{
    delete ui;
}
