/*-----------------------------
  Program Starts Here
  ------------------------------*/

double Update(int x, int y, Field& phi, double delta) // proposal step
{
  return phi(x,y) + (delta + delta)*drand48() - delta;
}

double Action(double theta, int x, int y, Field& phi) // Periodic Boundry Conditions
{
  double phi_down =  phi(x,y?(y-1):(phi.size()+1)); // "y?(y-1):(N-1)" means, if i is non-zero return i-1, otherwise return N.
  double phi_up = phi(x,(y+1)%phi.size()+1);
  double phi_left = phi(x?(x-1):(phi.size()+1),y);
  double phi_right = phi((x+1)%phi.size()+1,y);
  return  4 - cos(theta - phi_down) - cos(theta - phi_up) - cos(theta - phi_right) - cos(theta - phi_left);
}

int main()
{
  srand48(1234); //Random number seed
  int accepts = 0, tries = 0; // global variables for acceptance rate
  double pi = M_PI;
  int size = 32;
  Field phi(size);
  int x,y;
  int t = 0, t_steps = 10000;
  double beta = 1.0; // 1/double(Temp);
  double theta_old, theta_new, S_new, S_old, PIjPIi, Acceptance_Rate;
  double delta;

  ofstream f("Q2.dat");

  for(y=0; y<=size; y++) // loop to give every point in grid random value between [-pi,pi]
    {
      for(x=0; x<=size; x++)
        {
          phi(x,y) = (pi + pi)*drand48() - pi;
        }
    }

  for(delta = 1.0; delta <= 2.0; delta += 0.05)
    {
      accepts = 0.0;
      tries = 0.0;
      for(t=0; t<=t_steps; t++)
        {
          for(y=0; y<=size; y++)
            {
              for(x=0; x<=size; x++)
                {
                  theta_old = phi(x,y);
                  theta_new = Update(x, y, phi, delta);
                  S_new = 2*Action(theta_new, x, y, phi);
                  S_old = 2*Action(theta_old, x, y, phi);

                  PIjPIi = exp (-beta*(S_new - S_old)); //Transition Prob

                  if( PIjPIi < 0.0 || PIjPIi > drand48() )
                    {
                      tries += 1;
                      phi(x,y) = theta_new;
                      accepts += 1;
                    }
                  else
                    tries +=1;

                }//close x
            }//close y
        }//close t

      Acceptance_Rate = (double(accepts)/double(tries))*100;

      f << Acceptance_Rate << "\t" << delta << endl;

    }//close delta

  f.close();

}
