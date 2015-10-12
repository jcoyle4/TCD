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