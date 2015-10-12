#include<iostream>
#include<cmath>
#include<iomanip>
#include<fstream>
using namespace std;

class Field
{
 private:
  double* data;
  int n;   //local copy of size
  int index(int x, int y) const {return x+(n+1)*y;} // private member function

 public:
  Field (int ngrid) : n(ngrid) // constructor : constructor list (ngrid = n)
  {
    data = new double [(ngrid+1)*(ngrid+1)];
    int l = (ngrid+1)*(ngrid+1);
    for(int i = 0; i<l; i++) data[i] = 0.0;
  }

  ~Field()
    {
      delete[] data;
    }

  double& operator() (int x, int y)       {return data[index(x,y)];}
  double  operator() (int x, int y) const {return data[index(x,y)];}

  int size() const {return n;}
};
/*-------------------------------------
Program Starts Here
-------------------------------------*/
double SOR(int x, int y, Field& Phi, double omega)
{
  //double phi_new = 0.25*(Phi(x-1,y) + Phi(x+1,y) + Phi(x,y-1) + Phi(x,y+1));
  //return (1+omega)*phi_new - omega*Phi(x,y); // use omega between 0 and 1
  return (1-omega)*Phi(x,y) + (omega/4.0)*(Phi(x-1,y) + Phi(x+1,y) + Phi(x,y-1) + Phi(x,y+1));   //use omega between 1 and 2
}

int main()
{
  ofstream f("TvsOmega.dat");
  int size = 300; // Size of grid
  Field Phi(size);
  int t = 0, time = 100000; // Number of time steps
  double precision = 1E-10;
  double omega; // Over-Relaxation Parameter
  double diff;   //Used for storing value for convergence check
  int x, y; // loop variables

  for(omega = 1.01; omega < 2.0; omega += 0.01)
    {
      for(x=1; x<size; x++) // set to 0 for new omega
        {
          for(y=1; y<size; y++)
            Phi(x,y)=0.0;
            }
      for(t=0; t<time; t++) // Loop over time
        {
          diff = 0.0;
          for(x=1; x<size; x++)
            {
              for(y=1; y<size; y++)
                {

                  double Phi_old = Phi(x,y); // Convergence Parameter

                  if (y>=0.1*size && y<=0.4*size && x>=0.1*size && x<=0.2*size) // A
                    Phi(x,y) = 1;

                  else if (y>=0.7*size && y<=0.8*size && x>=0.2*size && x<=0.8*size) // B
                    Phi(x,y) = -1;

                  else
                    Phi(x,y) = SOR(x, y, Phi, omega);


                   diff += (Phi(x,y)-Phi_old) * (Phi(x,y)-Phi_old);

                }//close y

            }//close x

          if(diff< precision)
            {
              f << t << "\t" << omega << "\t" <<  Phi(0.5*size,0.5*size) << endl;
              break;
            }
        }//close time


       }//close omega

  f.close();

  cout <<"For a Field of size " << Phi.size() << ", (0.5,0.5) value is " << setprecision(3) << Phi(0.5*size,0.5*size) << "\n";

  /* -------------
     This section of code was used to print the values of the grid, for a given omega, to a file Matrix.dat
-------------------------------------

   ofstream g("Matrixdiff.dat");

  for(int x=0; x<=size; x++)
      {
      for(int y=0; y<=size; y++)
        {
          g << Phi(x,y) << "\t";
        }
      g << "\n";
      }
      g.close();
  */
}
