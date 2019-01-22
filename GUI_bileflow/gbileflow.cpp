#include "gbileflow.h"
#include "ui_gbileflow.h"
using namespace std;

Gbileflow::Gbileflow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gbileflow)
{
    ui->setupUi(this);
    qT_debuger = centralWidget()->findChild<QTableWidget*>("tW_debug");
    qpb_start = centralWidget()->findChild<QPushButton*>("pB_start");
    QObject::connect(qpb_start, SIGNAL(clicked(bool)),this, SLOT(get_started()));
}

void Gbileflow::calculate(QTableWidget* qtw)
{
    const string workdir = "/home/cedric/Documents/Qt_Testfiles/";
    const string identifier = "6724";

    const string inputfile = workdir + "input_" + identifier + ".dat";
    const string outputfile =  workdir + "output_" + identifier + ".dat";

    Model model(inputfile);

    double pout = model.shooting();

    model.single_run(pout, true);

    //string filename = "/home/michael/bileflow/real_values/output.dat";

    model.printout_results(qtw);

}

void  Gbileflow::get_started()
{
    calculate(qT_debuger);
}

Gbileflow::~Gbileflow()
{
    delete ui;
}
