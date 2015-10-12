# include <iostream> // input output streams
# include <fstream>  // file input output streams
# include <iomanip>  // manipulation of input and output streams
# include <stdlib.h> // standard library
# include <cmath>    // math functions

using namespace std;

//
// function prototypes
//

void Numerov(double *PsiLeft,double *PsiRight, int N, double *KSquared, double l, int MatchPoint);
void UpdateKSquared(double *KSqared, double *Potential, double gamma_sq, double E, int N);
void InitPotential(double *Potential, int N);
void FindEigenstate(double& E, int N, double *KSquared, double l, int TurnPoint, double *Psi, double gamma_sq);
void OutputData(double *Psi, int N);
void Simpsons(double l, int N, double *Psi);
void SimpsonsProb(double l, int N, double *Psi);


// main program
int main() 
{
        // variable declarations

        int N;                                  // number of space points
        N=1000;
        double l;                               // step size = 1/(N-1)
        l=1.0/(N-1);
        double E;                               // Energy 
        double gamma_sq;                // Dimensionless parameter = 2*mass*L^2*V0/hbar^2
        int MatchPoint;                 // The point at which the left and right wavefunctions are matched

        double *Psi;
        double *Potential;
        Psi = new double [N];                   // Wavefunction 
        Potential = new double [N];     // Potential energy inside the well


        // initialize parameters

        gamma_sq=100.0;                                 // gamma includes all the relevant physical constants
        MatchPoint=N/8;                                 // Matchpoint (=N/2 corresponds to the middle of the well)

        cout << "Trial Energy: ";               // ask user for trial energy
        cin >> E; 

        InitPotential(Potential,N);     // initialize potential within the infinite well

        // Find Eigenstate
        // in this template, the function FindEigenstate only computes Psi for the trial energy

        FindEigenstate(E, N, Potential, l, MatchPoint, Psi, gamma_sq);

        Simpsons(l, N, Psi);

        // output the wavefunction to a ASCII file named Psi.dat
        OutputData(Psi,N);

        // Output the corresponding energy to the console (12 digit precision after the decimal point)
        cout << setiosflags(ios::fixed) << setprecision(12)<< "Energy: " << E;
        cout << "\n" ;


        SimpsonsProb(l, N, Psi);

        // free the memory 
        delete[] Psi;
        delete[] Potential;

}
// this function computes Psi for a given energy
// 
void FindEigenstate(double& E, int N, double *Potential, double l, int MatchPoint, double *Psi, double gamma_sq)
{
        double scale;
        int i; // dummy variable
        double *PsiLeft;
        double *PsiRight;
        double *KSquared;

        PsiLeft = new double [N]; 
        PsiRight = new double [N];
        KSquared = new double [N];

        // update KSquared for trial energy E
        UpdateKSquared(KSquared, Potential, gamma_sq, E, N); 
        // Integrate from both sides to the matching point
        Numerov(PsiLeft, PsiRight, N, KSquared, l, MatchPoint); 

        //////////////////////////////////////////////////////
        double leftder;
        double rightder;
        double diffder;

        leftder = (PsiLeft[MatchPoint] - PsiLeft[MatchPoint - 1])/l;
        rightder = (PsiRight[MatchPoint] - PsiRight[MatchPoint -1])/l;

        diffder = leftder - rightder;

        //cout << "Difference in slopes is: ";
        //cout << diffder;
        //cout << "\n" ;
        //////////////////////////////////////////////////////

        double deltaE;
        double diffderdelta;


        deltaE = .001;

        while(abs(deltaE) > .0000005)
        {
                E=E+deltaE;
                // update KSquared for trial energy E
                UpdateKSquared(KSquared, Potential, gamma_sq, E, N); 
                // Integrate from both sides to the matching point
                Numerov(PsiLeft, PsiRight, N, KSquared, l, MatchPoint); 



                leftder = (PsiLeft[MatchPoint] - PsiLeft[MatchPoint - 1])/l;
                rightder = (PsiRight[MatchPoint] - PsiRight[MatchPoint -1])/l;
                diffderdelta = leftder - rightder;

                if (diffder*diffderdelta < 0.0)
                        deltaE = -deltaE / 2.0; 

                diffder = diffderdelta;

        }
/////////////////////////////////////////////////////////////

        /*cout << "Difference in slopes is: ";
        cout << diffder;
        cout << "\n" ;*/



        /*scale = PsiLeft[MatchPoint]/PsiRight[MatchPoint];
//      cout << "scale = "<<scale<< endl;
                for(i=0;i<N;i++)
                        {
                        PsiRight[i] = scale*PsiRight[i];
                        } 
*/


        // merge the left and right integration into Psi
        for (i=0;i<N;i++)
        {
                if (i<MatchPoint)
                {
                        Psi[i]=PsiLeft[i];
                } else
                {
                        Psi[i]=PsiRight[i];
                }
        }

        delete[] PsiRight;
        delete[] PsiLeft;
        delete[] KSquared;
}

//
// this function performs the numerical integration from both sides of the well
//
void Numerov(double *PsiLeft,double *PsiRight, int N, double *KSquared, double l, int MatchPoint)
{
        int i;                          //dummy variable
        double constant;        // =l^2/12

        constant=l*l/12;

        PsiLeft[0]=0;                                   // Initialize left boundary
        PsiLeft[1]=0.0001;

        PsiRight[N-1]=0;
        PsiRight[N-2]=0.0001;                           // Initialize right boundary    

        // Shoot from the left to the Matchpoint

        for (i=2;i<(MatchPoint+1);i++)
        {
                PsiLeft[i]=(2.0*(1.0-(5.0*constant*KSquared[i-1]))*PsiLeft[i-1]-(1.0+constant*KSquared[i-2])*PsiLeft[i-2])/(1.0+constant*KSquared[i]);
        }

        // Shoot from the right to the (Matchpoint-1)



        for (i=N-3;i>(MatchPoint-2);i--)
        {
           PsiRight[i]=(2.0*(1.0-(5.0*constant*KSquared[i+1]))*PsiRight[i+1]-(1.0+constant*KSquared[i+2])*PsiRight[i+2])/(1.0+constant*KSquared[i]);
        }

        double scale;
        scale = PsiLeft[MatchPoint]/PsiRight[MatchPoint];
//      cout << "scale = "<<scale<< endl;
                for(i=0;i<N;i++)
                        {
                        PsiRight[i] = scale*PsiRight[i];
                        }

}
//
// this function is called at the beginning to initialize the potential energy v(x)
//
void InitPotential(double *Potential,int N)
{
        int i;  // dummy variable
        for (i=0;i<N;i++)
        {
                // Infinite square well
                //Potential[i]=-1.0; 

                // Step Potential
                if (i > N/2) {
                        Potential[i]=0;
                } else {
                        Potential[i]=-1;
                }
                // linear Potential
        //Potential[i]=2*float(i)/(N-1)-1;
        }

}
//
// this function computes KSquared for a given trial energy E
//
void UpdateKSquared(double *KSquared, double *Potential, double gamma_sq, double E, int N)
{
        int i;          // dummy variable
        for (i=0;i<N;i++)
        {
                KSquared[i]=gamma_sq*(E-Potential[i]);
        }
}
//
// this function takes a vector of size N and outputs it to an ASCII file named Psi.dat
//
void OutputData(double *Data, int N)
{
        int i;
        fstream Datafile("Psi.dat",ios::out);

        for (i=0;i<N;i++)
        {
                Datafile << Data[i] << "\n";        
        }
        Datafile.close();                                               // close file
}
void Simpsons(double l, int N, double *Psi)
{
        double even_sum, odd_sum, tot_sum;

        int k, m;

        for(k = 1; k < N/2 - 1; k++)
                even_sum = Psi[2*k]*Psi[2*k]+even_sum;

        for(m = 1; m < N/2; m++)
                odd_sum = Psi[2*m-1]*Psi[2*m-1]+odd_sum;

        even_sum = 2 * even_sum;
        odd_sum = 4 * odd_sum;

        tot_sum = Psi[0]*Psi[0] + even_sum + odd_sum + Psi[N]*Psi[N];

        tot_sum = l/3 * tot_sum;

//cout << "Simpsons gives = "<<tot_sum<< endl;

        tot_sum = pow ( tot_sum, 0.5 );

 //cout << "Simpsons gives after Sqrt = "<<tot_sum<< endl;

                for(k=0;k<N;k++)
                        {
                        Psi[k] = Psi[k]/tot_sum;
                        }


}

void SimpsonsProb(double l, int N, double *Psi)
{
        double even_sum, odd_sum, tot_sum;

        int k, m;

        for(k = N/4; k < N/2 - 1; k++)
                even_sum = Psi[2*k]*Psi[2*k]+even_sum;

        for(m = N/4; m < N/2; m++)
                odd_sum = Psi[2*m-1]*Psi[2*m-1]+odd_sum;

        even_sum = 2 * even_sum;
        odd_sum = 4 * odd_sum;

        tot_sum = Psi[0]*Psi[0] + even_sum + odd_sum + Psi[N]*Psi[N];

        tot_sum = l/3 * tot_sum;

        cout << "Probability is = " << tot_sum << endl;

}
