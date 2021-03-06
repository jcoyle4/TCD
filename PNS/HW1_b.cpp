#include <iostream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
using namespace std;

double f(double x, double t)
{
  double f = 0.01*x/(pow(t - 0.5,2.0) + 0.001);
    return f;
}
double rk4(double h, double x, double t)
{

  double k1 = h * f(x, t);
  double k2 = h * f(x + k1/2.0, t + h/2.0);
  double k3 = h * f(x + k2/2.0, t + h/2.0);
  double k4 = h * f(x + k3, t + h);

  x = x + (k1 + 2.0*k2 + 2.0*k3 + k4) / 6.0;

  return x;
}

int main()
{
  double h, t;
  double x;
  double t1 = 1.0;
  double t0 = 0.0;
  double x0 = 1.0;
  int N;
  double x_ans;

  for(N=10.0; N<1000000.0; N *=10.0)
    {
      t = t0;
      x = x0;
      h = (t1 - t0)/N;

        for(int i=0.0; i<N; i++)
          {
            x = rk4(h, x, t);
            t += h;
          }

        if(fabs(x - x_ans) < 1E-3) break;
        else x_ans = x;

    }
  cout << "Completed after " << N << " iterations." << "\n";
  cout << "x(1) = " << x << " To 3sf, x(1) = " << setprecision(3) << x << "\n";

  return 0;

}
