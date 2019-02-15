#include "gbileflow.h"
#include "ui_gbileflow.h"
using namespace std;

Gbileflow::Gbileflow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gbileflow)
{
    ///GUI setup
    ui->setupUi(this);
    this->setFixedSize(1000, 600);
    qtw_main = new QTabWidget();
    openAction = new QAction(tr("&Open"), this);
    saveAction = new QAction(tr("&Save"), this);
    saveAsAction = new QAction(tr("&Save as..."), this);
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
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(cTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));
    connect(startAction, SIGNAL(triggered()), this, SLOT(get_started()));
    connect(setAction, SIGNAL(triggered()), this, SLOT(settings()));

    ///setup filemenu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    saveAsAction->setDisabled(true);
    saveAction->setDisabled(true);
    fileMenu->addAction(cTabAction);
    fileMenu->addAction(setAction);
    setAction->setDisabled(true);
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

void Gbileflow::calculate()
{
    /*const string workdir = "/home/cedric/Documents/Qt_Testfiles/";
    const string identifier = "6724";

    const string inputfile = workdir + "input_" + identifier + ".dat";
    const string outputfile =  workdir + "output_" + identifier + ".dat";*/
    QThread* calc = new QThread(this);
    QThread* control = new Timeout(calc);
    Model* model = mlist[qtw_main->currentIndex()];
    model->setVal(slist[qtw_main->currentIndex()]);
    qRegisterMetaType<Qt::Orientation>();
    connect(calc,SIGNAL(started()), model, SLOT(run()));
    connect(model,SIGNAL(ready()), calc, SLOT(quit()));
    connect(model,SIGNAL(ready()), control, SLOT(quit()));
    connect(this,SIGNAL(itsTime2stop()), calc, SLOT(quit()));
    connect(model,SIGNAL(resultReady(QTableWidget*, Tabcontainer*)), this, SLOT(paint(QTableWidget*, Tabcontainer*)));
    connect(control, SIGNAL(miss(QThread*)), this, SLOT(breakThread(QThread*)));
    connect(control, SIGNAL(started()), control, SLOT(test()));

    model->setTable(tlist[qtw_main->currentIndex()]);

    //model->run();

    if(model->thread() == this->thread())
    {
        model->moveToThread(calc);
        control->moveToThread(control);
    }
    calc->start();
    control->start();
}

void Gbileflow::breakThread(QThread* toBreak)
{
    emit itsTime2stop();
    QErrorMessage* errm = new QErrorMessage();
    errm->showMessage("Timeout");
}

void Gbileflow::paint(QTableWidget* qtw, Tabcontainer* tabcon)
{
    for(int i = 1; i < 10; ++i)
    {
       tabcon->add(makeDataset(0,qtw),
               makeDataset(i,qtw),qtw->horizontalHeaderItem(i)->text());
    }
}

///starting a caluclation is only possible, if any Tab is open
///starting a calculation should be also not possible if it`s already calculated
///else it could cause multiple errors and bugs
void  Gbileflow::get_started()
{
    if(tlist.length() >= 1)
    {
        mlist.replace(qtw_main->currentIndex(), new Model(tlist[qtw_main->currentIndex()]->get_file().toStdString()));
        calculate();
    }
}

//commeted
void Gbileflow::open(QString dat)
{
    ///Open up a file dialoge
    ///it can only find .dat files as datasource
    QString fileName;
    if(dat == "")
    {
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Text Files (*.dat)"));
    }
    else {
        fileName = dat;
    }

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

        int already = -1;
        for(int i = 0; i < qtw_main->count(); ++i)
        {
            if(qtw_main->tabText(i) == tmp)
            {
                already = i;
            }
        }
        if(already == -1){
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
            slist.push_back({1000000,100,3E-10,1.7E-3,20E-6});
            ///enable every action that is disabled while there are 0 tabs
            if(tlist.length() == 1)
            {
                saveAction->setDisabled(false);
                cTabAction->setDisabled(false);
                startAction->setDisabled(false);
                saveAsAction->setDisabled(false);
                setAction->setDisabled(false);
            }
        }else{
            qtw_main->removeTab(already);
            qtw_main->insertTab(already,new QWidget(), tmp);
            Tabcontainer* tabcon = new Tabcontainer(fileName, qtw_main->widget(already));
            tlist[already] = tabcon;
            Model* tmp = mlist[already];
            mlist[already] = new Model(fileName.toStdString());
            delete tmp;
            //slist[already] = {1000000,100,3E-10,1.7E-3,20E-6};
            qtw_main->setCurrentIndex(already);
        }
    }
}

///closing a Tab is only possible if any tab is open
void Gbileflow::closeTab()
{
    int  pos = qtw_main->currentIndex();
    ///remove all stored information about the tab
    ///so they will not be accessed by mistake
    delete tlist[pos];
    tlist.removeAt(pos);
    delete mlist[pos];
    mlist.removeAt(pos);
    slist.removeAt(pos);
    ///remove the actual tab
    qtw_main->removeTab(pos);

    ///disable every Actions, that are not supposed to be
    ///available while no Tab is open, if this tab is the only one
    if(tlist.length() <= 0)
    {
        saveAction->setDisabled(true);
        saveAction->setDisabled(true);
        cTabAction->setDisabled(true);
        startAction->setDisabled(true);
        setAction->setDisabled(true);
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

void Gbileflow::save(){
    QString fileName = "";
    std::cout<<tlist[qtw_main->currentIndex()]->get_savePlace().toStdString()<<std::endl;
    if(tlist[qtw_main->currentIndex()]->get_savePlace() == ""){
        fileName  = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
        std::cout<<fileName.toStdString()<<std::endl;
        tlist[qtw_main->currentIndex()]->set_savePlace(fileName);
    }else{
        fileName = tlist[qtw_main->currentIndex()]->get_savePlace();
    }
    if(fileName != ""){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Clear", "Clear content of " + fileName + "?",
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QDir(fileName).removeRecursively();
            QDir().mkdir(fileName);
        }
        mlist[qtw_main->currentIndex()]->printout_results((fileName+"/").toStdString(), "table_" + qtw_main->tabText(qtw_main->currentIndex()).toStdString());
        QFile file(tlist[qtw_main->currentIndex()]->get_file());
        file.copy(fileName+"/origin.dat");
        QDir(fileName).mkdir("pictures");
        tlist[qtw_main->currentIndex()]->mkPng(fileName+"/pictures"+"/");
    }
}

void Gbileflow::saveAs()
{
    //maybe not neccessary since it is only available if there is a tab open
    if(tlist.length() >= 1)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Text Files"));

        ///check if the name is invalide
        /// currently only testing for ""
        if (fileName != "") {
            QDir().mkdir(fileName);
            QDir(fileName).mkdir("pictures");
            //QFile file(fileName);
            QFile file(tlist[qtw_main->currentIndex()]->get_file());
            file.copy(fileName+"/origin.dat");
            mlist[qtw_main->currentIndex()]->printout_results((fileName+"/").toStdString(),"table_" + qtw_main->tabText(qtw_main->currentIndex()).toStdString());
            tlist[qtw_main->currentIndex()]->mkPng(fileName+"/pictures"+"/");
            tlist[qtw_main->currentIndex()]->set_savePlace(fileName);

        }
    }
}

void Gbileflow::settings()
{
    this->setDisabled(true);
    Settings* setval = new Settings(slist[qtw_main->currentIndex()],this);
    connect(setval, SIGNAL(getSets(QList<double>)), this, SLOT(takeSets(QList<double>)));
    setval->show();
}

void Gbileflow::takeSets(QList<double> tmp)
{
    slist[qtw_main->currentIndex()] = tmp;
}

Gbileflow::~Gbileflow()
{
    delete ui;
}

void Gbileflow::delsets(Settings* set){
    delete set;
}
