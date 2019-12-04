#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "dirent.h"

using namespace std;

class Model
{

    const int N = 10000;
    double L = 4E-4;
    double tau = 1.5; //tortuosity
    const double outerpressure = 1000; // 100
    const double R = 8.314; //8.314;
    const double T = 310; // 293;
    const double kappa = 4.7E-10; // 3E-10
    const double k = 92.0/80.0;   // changing viscosity
    const double reff = 0.2;
    const double secretion_rate = 1.0E-2; // 1.7E-3;
    double vilifac_pc = 0.3;
    double vilifac_pp = 0.3;
    const double central_vein_radius = 20E-6; // given in meter
    const double D = 0E-11; // 0E-9; // in m*m/sec

    const double c0 = 302.25;

    const double ch = 300.0;



    vector<double> p;  // pressure
    vector<double> w;  // velocity
    vector<double> K;  // resistance
    vector<double> A;  // membrane density
    vector<double> e;  // volume fraction
    vector<double> G;
    vector<double> x;  // distance from center
    vector<double> vilifac; //vilifactor
    vector<double> c; // concentration
    vector<double> connect; // concentration

public:

    Model();
    Model(const string filename);

    double g(const int i);

    double single_run(const double cpressure, bool verbose);
    double shooting();
    void printout_results(const string filename);

};

Model::Model()
{

    p.resize(N+1);
    w.resize(N+1);
    K.resize(N+1);
    A.resize(N+1);
    G.resize(N+1);
    x.resize(N+1);
    c.resize(N+1);
    vilifac.resize(N+1);

    for(int i=0; i<=N; i++)
    {
        x[i] = (i+0.0)/N;
        K[i] = 1.0;
        A[i] = 1.0;
        e[i] = 1.0;

    }
}

Model::Model(const string filename)
{
    p.resize(N+1);
    w.resize(N+1);
    K.resize(N+1);
    A.resize(N+1);
    e.resize(N+1);
    G.resize(N+1);
    x.resize(N+1);
    c.resize(N+1);
    connect.resize(N+1);
    vilifac.resize(N+1);



    ifstream input(filename.c_str());
    int rows = 0;

  //  cout << filename << endl;
    for(;;)
    {
       string line;
       getline(input, line);
       stringstream ss(line);
       if (line=="") break;
       rows++;
    }
    cout << rows << endl;
    input.clear();
    input.seekg(0, ios::beg);

    vector<double> Ap(rows);
    vector<double> Kp(rows);
    vector<double> ep(rows);
    vector<double> connectp(rows);

    for(int i=0; i<rows-2; i++)
    {
        string line;
        getline(input, line);
        stringstream ss(line);
        ss >> Ap[i];
        ss >> Kp[i];
        ss >> ep[i];
        ss >> connectp[i];
    }

    string line;
    getline(input, line);
    stringstream ss(line);

    ss >> L;
    L /= 1E6;

    ss >> tau;

    tau = tau*tau*2.5;  

    cout << rows << " " << L << " " << tau << endl;

    getline(input, line);
    stringstream st(line);

    st >> vilifac_pc;
    st >> vilifac_pp;

    cout << vilifac_pc << " " << vilifac_pp << endl;

    rows -= 2;

    for(int i=0; i<=N; i++)
    {
        p[i] = 0.0;
        w[i] = 0.0;
        G[i] = 0.0;
        c[i] = 0.0;

        x[i] = (i+0.0)/N;

        vilifac[i] = vilifac_pc + (vilifac_pp-vilifac_pc)*(i+0.0)/N;

        double focusstart = 0.4*N;
        double focusing;
        if (i<focusstart)
        {
            focusing = 1.0;
        }
        else
        {
            focusing = 1-0.9/(N-focusstart)*(i-focusstart);
//            focusing = 1-0.9/(N-focusstart)/(N-focusstart)*(i-focusstart)*(i-focusstart);
        }

        if (x[i]<1.0/(2.0*rows))
        {
            A[i] = Ap[0];
            K[i] = Kp[0]/vilifac[i]*k/focusing/reff/reff;
            e[i] = ep[0]*vilifac[i]*focusing;
            connect[i] = connectp[0];
            continue;
        }
        if (x[i]>1.0-1.0/(2.0*rows))
        {
            A[i] = Ap[rows-1];
            K[i] = Kp[rows-1]/vilifac[i]*k/focusing/reff/reff;
            e[i] = ep[rows-1]*vilifac[i]*focusing;
            connect[i] = connectp[rows-1];
            continue;
        }


        double M = rows*x[i]-0.5;
        double Mlow = floor(M);
        double interx = M-Mlow;


        A[i] = Ap[Mlow]*(1-interx) + Ap[Mlow+1]*interx;
        K[i] = (Kp[Mlow]*(1-interx) + Kp[Mlow+1]*interx)/vilifac[i]*k/focusing/reff/reff;
        e[i] = (ep[Mlow]*(1-interx) + ep[Mlow+1]*interx)*vilifac[i]*focusing;
        connect[i] = (connectp[Mlow]*(1-interx) + connectp[Mlow+1]*interx);

    }

}

double Model::g(const int  i)
{
    return secretion_rate;
}

void output_vector(const vector<double> & v)
{
    int n = v.size();

    for(int i=0; i<n; i++)
    {
        cout << v[i] << endl;
    }
}

void Model::printout_results(const string filename)
{
   ofstream file(filename.c_str());
   const int printout_every = 10;

   const double deltax = 1.0/N;
   const double diffusion = 1E-11;

   for(int i=0; i<=N; i++)
   {
       double fluxratio; // ratio of diffusive flux vs advective flux
       if ((i==0) || (w[i]==0.0) || (c[i]==0.0))
       {
           fluxratio = 0.0;
       }
       else
       {
           double cderiv = (c[i]-c[i-1])/deltax/L;
           fluxratio = abs(diffusion*cderiv/w[i]*e[i]/c[i]);
       }
       double actx = (x[i]+central_vein_radius/L)*L*1e6;

       if (!(i%printout_every)) file << x[i] << " " << w[i]/e[i] << " " << w[i] << " " << p[i] << " " << setprecision(10) << c[i] << " "
                                     << setprecision(6) << G[i] << " "
                                     << A[i] << " " << fluxratio << " " << connect[i] <<  " "
                                     << w[i]*1e6/e[i] << " " <<  w[i]*1e6/e[i]/connect[i] << " " << w[i]*1e6 << " " << w[i]*1e6*actx*2*3.141592653 << endl;
   }
}

double Model::single_run(const double cpressure, bool verbose)
{
    double deltax = 1.0/N;

    double rel_cvr = central_vein_radius/L;

    p[0] = cpressure;
    w[0] = 0.0;
    G[0] = 0.0;

    double pp, qq, ws;
    if (central_vein_radius == 0.0)
    {
       pp = kappa*A[0]*L*(cpressure + R*T*ch)/2.0;
       qq = g(0)*kappa*A[0]*L*L*R*T/4.0;
    }
    else
    {
       pp = kappa*A[0]*L*(cpressure + R*T*ch);
       qq = g(0)*kappa*A[0]*L*L*R*T;
    }


    if (D==0.0)
    {       ws = (-pp/2.0 + sqrt(pp*pp/4.0 + qq));
    }
    else
    {
        ws = kappa*A[0]*L*(R*T*c0-cpressure);
    }
 
    if (central_vein_radius == 0.0)
    {
        c[0] = g(0)/2.0/ws*L;
    }
    else
    {
        c[0] = g(0)/ws*L;
    }

    if(D>0.0) c[0] = c0;

    if (verbose) cout << setprecision(8) <<  c[0] << endl;

    int firstphase = 1;

    for (int i=1; i<=N; i++)
    {
        double xavg = (x[i]+x[i-1])/2.0 + rel_cvr;

        //double tau = 1.85;

        G[i] = ((x[i]+rel_cvr)*(x[i]+rel_cvr)-rel_cvr*rel_cvr)/2.0*g(0);  


        if (i<=firstphase)
        {
            p[i] = p[i-1] - deltax*tau*K[i]*L*w[i-1];
            w[i] = w[i-1] + deltax*ws;
            w[i] = w[i]*(x[i-1] + rel_cvr)/(x[i]+ rel_cvr);

            if (i==1 )
            {
                c[i] = c[i-1];
            }
            else
            {
               c[i] = G[i]/(x[i]+rel_cvr)/w[i]*L;
            }

        } else
        {
            double actx = (x[i]+rel_cvr)*L;  // unnormalized x

            double Gend = ((x[i]+rel_cvr)*(x[i]+rel_cvr)-rel_cvr*rel_cvr)/2.0*g(0)*L*L; // already in unnormalized coordinates
            double wit = w[i-1];
            double pit = p[i-1];
            double relw_iterror = 1.0;
            double relp_iterror = 1.0;


            double exit_crit=1E-10;

            while((relw_iterror>exit_crit) || (relp_iterror>exit_crit))
            {
                double wold = wit;
                double pold = pit;

                double f = wit - deltax*L*(kappa*A[i]*(R*T*(Gend/actx/wit - ch) - pit) - wit/actx) - w[i-1];
                double g = pit + deltax*L*K[i]*tau*wit - p[i-1];

                double dfdw = 1.0 + deltax*L*(kappa*A[i]*R*T*Gend/actx/wit/wit + 1/actx);
                double dfdp = deltax*L*kappa*A[i];
                double dgdw = deltax*L*K[i]*tau;
                double dgdp = 1.0;

                double det = dfdw*dgdp - dfdp*dgdw;

                double wnew = wit - ( dgdp*f - dfdp*g)/det;
                double pnew = pit - (-dgdw*f + dfdw*g)/det;

                wit = wnew;
                pit = pnew;

                relw_iterror = abs((wnew-wold)/wold);
                relp_iterror = abs((pnew-pold)/pold);

            }


            w[i] = wit;
            p[i] = pit;
            c[i] = Gend/w[i]/actx;
        }

    }


    if (verbose)
   {
      output_vector(w);
      cout << endl;
      output_vector(p);
   }


  printout_results("/home/michael/bileflow1/test/track.dat");
  return p[N];

}

double Model::shooting()
{
    Model* m = this;
    double pact = 1.0;
    double plower = 0.0;
    double pupper = 0.0;

    double target = m->single_run(pact, false) - outerpressure;

    if(target>0.0) {pupper=pact;} else {plower=pact;}

    while (abs(target)> 1E-8)
    {
        if(plower == 0.0)
        {
            while(plower == 0.0)
            {
                pact/=10;
                if (m->single_run(pact, false)-outerpressure < 0.0) plower = pact;
  //              cout << plower << " " << pact << " " << pupper << endl;
            }
        }

        if (pupper==0.0)
        {
            while(pupper == 0.0)
            {
                pact*=10;
                if (m->single_run(pact, false)-outerpressure > 0.0) pupper = pact;
            }
        }

        if ((plower>0) && (pupper>0))
        {
            pact = (plower + pupper)/2.0;

            target = m->single_run(pact, false) - outerpressure;
            cout << target << endl;
            if(target<0.0) {plower = pact;} else {pupper = pact;}
        }

        cout << plower << " " << pupper << " " << pact << " " << target << endl;
   //  int tt; cin >> tt;
    }
    return pact;

}

int main()
{
    const string workdir = "/home/michael/bileflow1/simulations_june19/nash/";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (workdir.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            string inputfile = ent->d_name;
            if (inputfile.substr(0,5) != "input") continue;

            string label = inputfile.substr(6,5);
            cout << label << endl;

            inputfile = workdir + inputfile;
            string outputfile = workdir + "output_" + label + ".dat";

            cout << inputfile << endl;
            cout << outputfile << endl;

            Model model(inputfile);


            double pout = model.shooting();


            model.single_run(pout, true);

            model.printout_results(outputfile);


        }
        closedir (dir);
    }
    else
    {

        perror ("");
        return EXIT_FAILURE;
    }

    return 0;
}
