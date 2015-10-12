#include <iostream>
#include <cmath>
using namespace std;

#include "Planet.h"

int main()
{
  double start_t = 0.0;
  double final_t = 3.0;
  double N=10000.0;
  double h= (final_t - start_t) / N;

  Planet p0(2.0, -0.5, 0.0, -0.94, 0.65);
  Planet p1(1.0, -0.6, -0.2, 1.86, 0.70);
  Planet p2(1.0, 0.5, 0.1, -0.44, -1.4);
  Planet p3(0.4, 0.6, 0.3, 1.15, -1.5);

  Planet p[4] = {p0, p1, p2, p3};
  int i, j; // loop variables
  double x_0_x, x_half_x, x_1_x, x_0_y, x_half_y, x_1_y = 0.0; // step variables all initilized to 0
  double p_0_x, p_1_x, p_0_y, p_1_y = 0.0; // p is velocity

  double Force_x, Force_y, angle;


  for (double t=0.0; t<=final_t; t +=h)
    {
      double F_x_total [4] = {0,0,0,0};
      double F_y_total [4] = {0,0,0,0};

      for(i=0; i<=3; i++)
        {
          p_0_x = p[i].vel(0); //leap frog step 1 X-Direction
          x_0_x = p[i].pos(0);
          x_half_x = x_0_x + (h/2)*p_0_x;
          p[i].pos(0) = x_half_x;

          p_0_y = p[i].vel(1); //leap frog step 1 Y-Direction
          x_0_y = p[i].pos(1);
          x_half_y = x_0_y + (h/2)*p_0_y;
          p[i].pos(1) = x_half_y;

          for (j=0; j<=3; j++)
            {
              double X_Distance = p[i].pos(0)-p[j].pos(0);
              double Y_Distance = p[i].pos(1)-p[j].pos(1);
              double Force = (p[i].mass()*p[j].mass())/(X_Distance*X_Distance+Y_Distance*Y_Distance);
              angle = atan(fabs(Y_Distance/X_Distance));
              Force_x = Force*cos(angle);
              Force_y = Force*sin(angle);

              if(X_Distance > 0) Force_x *= -1;
              if(Y_Distance > 0) Force_y *= -1;
              if(i==j)
                {
                  Force_x = 0;
                  Force_y = 0;
                }

              F_x_total[i] += Force_x;
              F_y_total[i] += Force_y;
            }

          p_1_x = p_0_x + h*F_x_total[i]/p[i].mass(); // leap frog X-Direction Step 2 & 3
          x_1_x = x_half_x + (h/2) * p_1_x;
          p[i].pos(0) = x_1_x; //updating coordinated
          p[i].vel(0) = p_1_x;

          p_1_y = p_0_y + h*F_y_total[i]/p[i].mass(); //leap frog Y-Direction Step 2 & 3
          x_1_y = x_half_y + (h/2) * p_1_y;
          p[i].pos(1) = x_1_y; //updating coordinates
          p[i].vel(1) = p_1_y;
        }
      // cout << p[0].pos(0) << " " << p[0].pos(1) << " " << p[1].pos(0) << " " << p[1].pos(1) << " " << p[2].pos(0) << " " << p[2].pos(1) << " " << p[3].pos(0) << " " << p[3].pos(1) << "\n"; //command for printing to graph for plotting

    }

  cout << p[0].pos(0) << " " << p[0].pos(1) << "\n";
  cout << p[1].pos(0) << " " << p[1].pos(1) << "\n";
  cout << p[2].pos(0) << " " << p[2].pos(1) << "\n";
  cout << p[3].pos(0) << " " << p[3].pos(1) << "\n";

}
