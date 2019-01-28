#include "model.h"

using namespace std;

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
    vilifac.resize(N+1);

    ifstream input(filename.c_str());
    int rows = 0;

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

    for(int i=0; i<rows-2; i++)
    {
        string line;
        getline(input, line);
        stringstream ss(line);
        ss >> Ap[i];
        ss >> Kp[i];
        ss >> ep[i];
    }

    string line;
    getline(input, line);
    stringstream ss(line);

    ss >> L;
    L /= 1E6;

    ss >> tau;

    cout << rows << " " << L << " " << tau << endl;

 //   string line;
    getline(input, line);
    stringstream st(line);

    st >> vilifac_pc;
    st >> vilifac_pp;

    cout << vilifac_pc << " " << vilifac_pp << endl;

  //  int tt; cin >> tt;

    rows -= 2;


    for(int i=0; i<=N; i++)
    {
        x[i] = (i+0.0)/N;

        vilifac[i] = vilifac_pc + (vilifac_pp-vilifac_pc)*(i+0.0)/N;


        if (x[i]<1.0/(2.0*rows))
        {
            A[i] = Ap[0];
            K[i] = Kp[0]/vilifac[i]/vilifac[i]*k;
            e[i] = ep[0]*vilifac[i];
            continue;
        }
        if (x[i]>1.0-1.0/(2.0*rows))
        {
            A[i] = Ap[rows-1];
            K[i] = Kp[rows-1]/vilifac[i]/vilifac[i]*k;
            e[i] = ep[rows-1]*vilifac[i];
            continue;
        }


        double M = rows*x[i]-0.5;
        double Mlow = floor(M);
        double interx = M-Mlow;


        A[i] = Ap[Mlow]*(1-interx) + Ap[Mlow+1]*interx;
        K[i] = (Kp[Mlow]*(1-interx) + Kp[Mlow+1]*interx)/vilifac[i]/vilifac[i]*k;
        e[i] = ep[Mlow]*(1-interx) + ep[Mlow+1]*interx*vilifac[i];


    }

 /*   for(int i=0; i<=N; i++)
    {
        cout << A[i] << " " << K[i] << " " << e[i] << endl;

    } */

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

/*void output_vector(const vector<double> & v)
{
    int n = v.size();

    for(int i=0; i<n; i++)
    {
        cout << v[i] << endl;
    }
} */

//printout in file
void Model::printout_results(const string filename)
{
   ofstream file(filename.c_str());
   const int printout_every = 1000;

   for(int i=0; i<=N; i++)
   {
       if (!(i%printout_every)) file << x[i] << " " << w[i]/e[i] << " " << w[i] << " " << p[i] << " " << c[i] << " " << K[i] << " " << A[i] << " " << e[i] << " " << vilifac[i] <<  " " << w[i]*1e6 <<endl;
       //x = distance from center p= pressure w=velocity K= resistance A=Membran density e= volume fraction vilifac= vilifactor c=concentration
   }

}

//printout in table
void Model::printout_results(QTableWidget* table)
{
    table->setColumnCount(10);
    table->setHorizontalHeaderLabels(QStringList()<<"distance from center"<<"velocity/volume fraction"<<"velocity"<<"pressure"
                                     <<"concentration"<<"resistance"<<"Membran density"<<"volume fraction"<<"vilifactor"<<"velocity * 1e6");
    const int printout_every = 1000;
    for(int i=0; i<=N; i++)
    {
        if (!(i%printout_every))
        {
            table->setRowCount(table->rowCount() + 1);
            table->setItem(table->rowCount() - 1,0,new QTableWidgetItem(QString::number(x[i])));
            table->setItem(table->rowCount() - 1,1,new QTableWidgetItem(QString::number(w[i]/e[i])));
            table->setItem(table->rowCount() - 1,2,new QTableWidgetItem(QString::number(w[i])));
            table->setItem(table->rowCount() - 1,3,new QTableWidgetItem(QString::number(p[i])));
            table->setItem(table->rowCount() - 1,4,new QTableWidgetItem(QString::number(c[i])));
            table->setItem(table->rowCount() - 1,5,new QTableWidgetItem(QString::number(K[i])));
            table->setItem(table->rowCount() - 1,6,new QTableWidgetItem(QString::number(A[i])));
            table->setItem(table->rowCount() - 1,7,new QTableWidgetItem(QString::number(e[i])));
            table->setItem(table->rowCount() - 1,8,new QTableWidgetItem(QString::number(vilifac[i])));
            table->setItem(table->rowCount() - 1,9,new QTableWidgetItem(QString::number(w[i]*1e6)));
        }
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
       pp = kappa*A[0]*L*cpressure/2.0;
       qq = g(0)*kappa*A[0]*L*L*R*T/4.0;
    }
    else
    {
       pp = kappa*A[0]*L*cpressure;
       qq = g(0)*kappa*A[0]*L*L*R*T;
    }

    ws = (-pp/2.0 + sqrt(pp*pp/4.0 + qq));

   // const double mu = 1.0;
    if (central_vein_radius == 0.0)
    {
        c[0] = g(0)/2.0/ws;
    }
    else
    {
        c[0] = g(0)/ws;
    }

    const int substeps = 100;

    const bool adaptive = false;
    const int adaptive_to_N = 10;



    for (int i=1; i<=N; i++)
    {
        double fine_deltax = deltax/substeps;
        double fine_G = 0.0;
        double fine_w = 0.0;
        double fine_p = cpressure;
        double fine_c = 0.0;

        if(adaptive && (i<=adaptive_to_N)) // adaptive code does not work
       {
           // cout << i << endl; int ttt; cin >> ttt;

           for(int j=1; j<=substeps; j++)
           {
               double acx = rel_cvr + (i-1)*deltax + (j-0.5)*fine_deltax;
               fine_G += g(i)*acx*fine_deltax;
               fine_p -= fine_deltax*tau*K[i]*L*fine_w;
               if ((i==1) && (j==1))
               {
                   fine_w += fine_deltax*ws;
               }
               else
               {
                   fine_w += fine_deltax*kappa*A[0]*L*(R*T*fine_G*L/fine_w/acx - fine_p);
               }
               fine_w *= (acx-0.5*fine_deltax)/(acx+0.5*fine_deltax);
               fine_c = fine_G/(acx+0.5*fine_deltax)/fine_w;
               if (verbose)
               {
                   cout << j << " " << fine_p << " " << fine_w << " " << fine_c << " " << fine_G << endl;
               }
           }
           G[i] = fine_G;
           w[i] = fine_w;
           p[i] = fine_p;
           c[i] = fine_c;

           if (verbose) {int tt; cin >> tt;}

       }
       else
       {
           double xavg = (x[i]+x[i-1])/2.0 + rel_cvr;

           //double tau = 1.85;

           G[i] = G[i-1] + (g(i)+g(i-1))/2.0*xavg*deltax;
           p[i] = p[i-1] - deltax*tau*K[i]*L*w[i-1];


           // cout << K[i]*w[i] << " " << K[i] << " " << w[i] << endl;


           if (i==1)
           {
               w[i] = w[i-1] + deltax*ws;
           } else
           {
               double diff = kappa*A[i]*L*(R*T*G[i]*L/w[i-1]/xavg - p[i-1]);

               if (diff<0.0) diff = 0.0;
               double ww = w[i-1] + deltax*diff;
               w[i] = ww*(x[i-1] + rel_cvr)/(x[i]+ rel_cvr);
           }

           c[i] = G[i]/(x[i]+rel_cvr)/w[i];
       }
    }

    if (verbose)
   {
   //    output_vector(w);
   //     cout << endl;
    //   output_vector(p);
   }


  printout_results("/home/cedric/Documents/Qt_Testfiles/track.dat");
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
            cout << "target : " <<target << endl;
            if(target<0.0) {plower = pact;} else {pupper = pact;}
        }

        cout <<"plower : "<< plower << " pupper : " << pupper << " pact :" << pact << " target :" << target << endl;
   //  int tt; cin >> tt;
    }
    return pact;

}
