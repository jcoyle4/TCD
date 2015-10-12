#include <iostream>
#include <math.h>
#include <iomanip>
#include <stdlib.h>
using namespace std;

double f1(double x, double y, double z)
{
  double xdot = y*(z - 1.0 + x*x) + 0.01*x;
    return xdot;
}

double f2(double x, double y, double z)
{
  double ydot = x*(3.0*z + 1.0 - x*x) + 0.01*y;
  return ydot;
}

double f3(double x, double y, double z)
{
  double zdot = -2.0*z*(0.03 + x*y);
  return zdot;
}

int main()
{
  double t0 = 0.0;
  double t1 = 90.0;
  double h, t;
  int N;
  double x0 = -0.5, y0 = 0.0, z0 = 0.5;
  double x_ans, y_ans, z_ans, x, y, z;

  for(N=10.0; N<=1000000000.0; N *= 10.0)
    {
      t = t0;
      x = x0;
      y = y0;
      z = z0;

      h = (t1 - t0)/N;

      for(int i=1.0; i<N; i++)
        {
          double k1a = h * f1(x, y, z);
          double k1b = h * f2(x, y, z);
          double k1c = h * f3(x, y, z);
          double k2a = h * f1(x + k1a/2.0, y + k1b/2.0, z + k1c/2.0);
          double k2b = h * f2(x + k1a/2.0, y + k1b/2.0, z + k1c/2.0);
          double k2c = h * f3(x + k1a/2.0, y + k1b/2.0, z + k1c/2.0);
          double k3a = h * f1(x + k2a/2.0, y + k2b/2.0, z + k2c/2.0);
          double k3b = h * f2(x + k2a/2.0, y + k2b/2.0, z + k2c/2.0);
          double k3c = h * f3(x + k2a/2.0, y + k2b/2.0, z + k2c/2.0);
          double k4a = h * f1(x + k3a, y + k3b, z + k3c);
          double k4b = h * f2(x + k3a, y + k3b, z + k3c);
          double k4c = h * f3(x + k3a, y + k3c, z + k3c);

          x = x + (k1a + 2.0*k2a + 2.0*k3a + k4a) / 6.0;
          y = y + (k1b + 2.0*k2b + 2.0*k3b + k4b) / 6.0;
          z = z + (k1c + 2.0*k2c + 2.0*k3c + k4c) / 6.0;
        }

     if(fabs(x - x_ans)<1E-3 && fabs(y - y_ans)<1E-3 && fabs(z - z_ans)<1E-3) break;

     else
        {
          x_ans = x;
          y_ans = y;
          z_ans = z;
        }
    }


  cout << "Completed after " << N << " iterations." << "\n";
  cout << "x(90) = " << x << " To 3sf, x(90) = " << setprecision(3) << x << "\n";
  cout << "y(90) = " << y << " To 3sf, y(90) = " << setprecision(3) << y << "\n";
  cout << "z(90) = " << z << " To 3sf, z(90) = " << setprecision(3) << z << "\n";

return 0;

}
