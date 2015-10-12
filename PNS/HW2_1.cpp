#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
using namespace std;


double V(double x)
{
  return -100.0*(x - 0.8)*(x - 0.9)*(x - 1.0)*(x - 1.0);
}

double f1(double z1, double z2, double x, double E)
{
  return z2; // dz1/dx
}

double f2(double z1, double z2, double x, double E)
{
  return V(x)*z1 - E*z1;
}

double rk4(double E, bool correct = false)
{

  double x0 = 0.0; //.... y(0)
  double x1 = 1.0; //.....y(1)
  double h, x, y1, y2, ans;
  int N;
  double area=0.0;

  double y[2] = {0.0, 1.0};

  for(N=10; N<10000; N*=10)
  {
      x = x0;
      y1 = y[0];
      y2 = y[1];
      h = (x1 - x0)/double(N);

      for (int i=0; i<N; i++)
        {
          double k1a = h*f1(y1, y2, x, E);
          double k1b = h*f2(y1, y2, x, E);

          double k2a = h*f1(y1 + k1a/2.0, y2 + k1b/2.0, x + h/2.0, E);
          double k2b = h*f2(y1 + k1a/2.0, y2 + k1b/2.0, x + h/2.0, E);

          double k3a = h*f1(y1 + k2a/2.0, y2 + k2b/2.0, x + h/2.0, E);
          double k3b = h*f2(y1 + k2a/2.0, y2 + k2b/2.0, x + h/2.0, E);

          double k4a = h*f1(y1 + k3a, y2 + k3b, x + h, E);
          double k4b = h*f2(y1 + k3a, y2 + k3b, x + h, E);

          y1 = y1 + (k1a + 2.0*k2a + 2.0*k3a + k4a)/6.0;
          y2 = y2 + (k1b + 2.0*k2b + 2.0*k3b + k4b)/6.0;

          x += h;

          if (correct) cout << x << " " << y1 << "\n";      // plotting 'command'
         }

        area += h*y1*y1;



        if (fabs(y1 - ans)<1E-5) break;
        else  ans = y1;

  }

  return (1/sqrt(area))*y1;
}

double shooting(double E_lo, double E_hi)
{
   double b_lo=rk4(E_lo);
   double b_hi=rk4(E_hi);
   double Ei;

  if (b_lo/b_hi>0.0)
    {
      cerr << "Error - Function at end points has same sign!\n";
      return 0.0;
    }

  while (b_lo/b_hi < 0.0 && E_hi-E_lo > 1.0e-5)
    {

      Ei = 0.5 * (E_lo + E_hi);
      double b_E = rk4(Ei);
      if(b_E/b_lo > 0.0)
        {
          E_lo = Ei;
          b_lo = b_E;
        }
      else
        {
          E_hi = Ei;
          b_hi = b_E;
        }
    }

      return 0.5*(E_hi + E_lo);
}

int main()
{
  //for( int i = 0; i<400; i++)                     // test loop to find root ranges
  //    {
  //        cout << i << " " << rk4(i) << "\n";
  //    }
  double E1 =  shooting(0.0,5.0);
  double E2 =  shooting(20.0,40.0);
  double E3 =  shooting(60.0,80.0);
  cout << "E1 = " << E1 << " E2 = " << E2 << " E3 = " << E3 << "\n";

  // rk4(E3, true);                                 // used for plotting

}
