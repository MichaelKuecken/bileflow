#ifndef MODEL_H
#define MODEL_H

#include <QTableWidget>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Model
{
public:
    //Nutzer möglichkeit zur manuellen Eingabe geben.
    const int N = 1000000;
    double L = 4E-4;
    double tau = 1.5; //tortuosity
    const double outerpressure = 100;
    const double R = 8.314; //8.314;
    const double T = 310; // 293;
    const double kappa = 3E-10;
    const double k = 1.0;
    const double secretion_rate = 1.7E-3; // 1.7E-3;
    double vilifac_pc = 0.3;
    double vilifac_pp = 0.3;
    const double central_vein_radius = 20E-6; // given in meter




    vector<double> p;  // pressure
    vector<double> w;  // velocity
    vector<double> K;  // resistance
    vector<double> A;  // membrane density
    vector<double> e;  // volume fraction
    vector<double> G;
    vector<double> x;  // distance from center
    vector<double> vilifac; //vilifactor
    vector<double> c; // concentration

public:

    Model();
    Model(const string filename);

    double g(const int i);
    void setVal(double,double,double,double);
    double single_run(const double cpressure, bool verbose);
    double shooting();
    void printout_results(const string filename);
    void printout_results(QTableWidget*);
};


#endif // MODEL_H
