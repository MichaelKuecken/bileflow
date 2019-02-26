#ifndef MODEL_H
#define MODEL_H

#include "tabcontainer.h"
#include <QTableWidget>
#include <QThread>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Timeout : public QThread
{
    Q_OBJECT
public:
    Timeout(QThread* ctrl)
    {
        control = ctrl;
    }
public slots:
    void test()
    {
        this->sleep(5);
        if(control->isRunning())
        {
            emit miss(control);
        }
        this->quit();
    }
signals:
    void miss(QThread*);
private:
    QThread* control;
};

class Model : public QObject
{
    Q_OBJECT
public:
    //Nutzer möglichkeit zur manuellen Eingabe geben.
    //
    int N = 1000000;
    double L = 4E-4;
    double tau = 1.5; //tortuosity
    //
    double outerpressure = 100;
    const double R = 8.314; //8.314;
    const double T = 310; // 293;
    //
    double kappa = 3E-10;
    //
    double k = 1.0;
    //
    double secretion_rate = 1.7E-3; // 1.7E-3;
    double vilifac_pc = 0.3;
    double vilifac_pp = 0.3;
    //
    double central_vein_radius = 20E-6; // given in meter

    vector<double> p;  // pressure
    vector<double> w;  // velocity
    vector<double> K;  // resistance
    vector<double> A;  // membrane density
    vector<double> e;  // volume fraction
    vector<double> G;
    vector<double> x;  // distance from center
    vector<double> vilifac; //vilifactor
    vector<double> c; // concentration

private:
    QTableWidget* qtw;
    Tabcontainer* tabcon;

public:

    Model();
    Model(const string filename);
    ~Model(){std::cout<<"loesche Model"<<std::endl;}
    void setTable(Tabcontainer*);
    double g(const int i);
    void setVal(QList<double>);
    double single_run(const double cpressure, bool verbose);
    double shooting();
    void printout_results(const string, const string filename);
    void printout_results(QTableWidget*);
public slots:
    void run();
signals:
    void ready();
    void resultReady(QTableWidget*, Tabcontainer*);
};


#endif // MODEL_H
