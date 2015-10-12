#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
using namespace std;

double f1(double y3, double y1, double t)
{
  double y3dot = -t*t*y3 - 4.0*y1;
    return y3dot;
}

double f2(double y3, double t)
{
  double y2dot = y3;
  return y2dot;
}

double f3(double y2, double t)
{
  double y1dot = y2;
  return y1dot;
}

int main()
{
  double t0 = 0.0;
  double t1 = 20.0;
  double h, t;
  int N;
  double y1a = 1.0, y2a = 0.0, y3a = -1.0;
  double y1, y2, y3, ans;

  for(N=10.0; N<1000000000.0; N*=10.0)
    {
      t = t0;
      y1 = y1a;
      y2 = y2a;
      y3 = y3a;

      h = (t1 - t0)/N;

      for(int i=1.0; i<N; i++)
        {
          double k1a = h * f3(y2, t);
          double k1b = h * f2(y3, t);
          double k1c = h * f1(y3, y1, t);
          double k2a = h * f3(y2 + k1a/2.0, t + h/2.0);
          double k2b = h * f2(y3 + k1b/2.0, t + h/2.0);
          double k2c = h * f1(y3 + k1c/2.0, y1 + k1a/2.0,  t + h/2.0);
          double k3a = h * f3(y2 + k2a/2.0, t + h/2.0);
          double k3b = h * f2(y3 + k2b/2.0, t + h/2.0);
          double k3c = h * f1(y3 + k2c/2.0, y1 + k2a/2.0,  t + h/2.0);
          double k4a = h * f3(y2 + k3a, t + h);
          double k4b = h * f2(y3 + k3b, t + h);
          double k4c = h * f1(y3 + k3c, y1 + k3a,  t + h);

          y1 = y1 + (k1a + 2.0*k2a + 2.0*k3a + k4a) / 6.0;
          y2 = y2 + (k1b + 2.0*k2b + 2.0*k3b + k4b) / 6.0;
          y3 = y3 + (k1c + 2.0*k2c + 2.0*k3c + k4c) / 6.0;


          t += h;
        }

      if(fabs(y1 - ans)<1E-3) break;
      else ans = y1;
    }

  cout << "Completed after " << N << " iterations." << "\n";
  cout << "x(20) =  " << y1 << " to 3sf, x(20) = " << setprecision(3) << y1 << "\n";

}
