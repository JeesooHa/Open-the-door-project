/***************************************************************************************
                  The Motion Planning library (MP library)
---------------------------------------------------------------------------------------

MP library is made by Dong-Hyung Kim, C&R Lab., Hanyang University.
Since this library is developed for personal use, there is no copyright.
Even so, the author would be thankful to one by informing the source of this library.	

Reference) Steven M. LaValle, 'The Motion Strategy Library (MSL)', University of Illinois, 
           http://msl.cs.uiuc.edu/msl/

The authors may be contacted via:
	Mail:      216, 5th Engineering Building,  Hanyang Univ., 
				   Sa-dong, Ansan-si, Gyeonggi-do, Republic of Korea 
				   426-791
	Phone:     +82-31-400-4743
			   E-mail:    fileman@hanyang.ac.kr
	Web site:  http://www.cnrlab.re.kr
	
	Recent update: 2013.01.04
**************************************************************************************/


#include "stdafx.h"

#define EPSILON 1e-12

using namespace std;

void MPMatrix::flip_rows(int i,int j)
{ MPVector* p = v[i];
  v[i] = v[j];
  v[j] = p;
 }

MPMatrix::MPMatrix(int dim1, int dim2)  
{
  if (dim1<0 || dim2<0) 
  cout<<"MPMatrix: negative dimension."<<endl;

  d1=dim1; 
  d2=dim2; 

  if (d1 > 0) 
  { v = new MPVector*[d1];
    for (int i=0;i<d1;i++) v[i] = new MPVector(d2); 
   }
  else v = NULL;
}


MPMatrix::MPMatrix(const MPMatrix& p)  
{ 
  d1 = p.d1;
  d2 = p.d2;
    
  if (d1 > 0) 
  { v = new MPVector*[d1];
    for (int i=0;i<d1;i++) v[i] = new MPVector(*p.v[i]); 
   }
  else v = NULL;
}

MPMatrix::MPMatrix(int dim1, int dim2, double* D)  
{ d1=dim1; d2=dim2; 
  v = new MPVector*[dim1];
  double* p = D;
  for(int i=0;i<dim1;i++) 
  { v[i] = new MPVector(dim2); 
    for(int j=0;j<dim2;j++) elem(i,j) = *p++;
   }

 }

MPMatrix::~MPMatrix()  
{ if (v) 
  { while(d1--) delete v[d1]; 
    delete[] v;
   }
}


void MPMatrix::check_dimensions(const MPMatrix& mat) const
{ if (d1 != mat.d1 || d2 != mat.d2)
	cout<<"incompatible MPMatrix types."<<endl;
 }

MPMatrix::MPMatrix(const MPVector& vec)
{ d1 = vec.d;
  d2 = 1;
  v = new MPVector*[d1];
  for(int i=0; i<d1; i++)
  { v[i] = new MPVector(1);
    elem(i,0) = vec[i];
   }
    
}

MPMatrix& MPMatrix::operator=(const MPMatrix& mat)
{ register int i,j;

  if (d1 != mat.d1 || d2 != mat.d2)
  { for(i=0;i<d1;i++) delete v[i];
    delete[] v;
    d1 = mat.d1;
    d2 = mat.d2;
    v = new MPVector*[d1];
    for(i=0;i<d1;i++) v[i] = new MPVector(d2);
   }

  for(i=0;i<d1;i++)
    for(j=0;j<d2;j++) elem(i,j) = mat.elem(i,j);

  return *this;
}

int MPMatrix::operator==(const MPMatrix& x) const
{ register int i,j;
  if (d1 != x.d1 || d2 != x.d2) return false;

  for(i=0;i<d1;i++)
    for(j=0;j<d2;j++)
      if (elem(i,j) != x.elem(i,j)) return false;

  return true;
 }


MPVector& MPMatrix::row(int i) const
{ if ( i<0 || i>=d1 )  	cout<<"MPMatrix: row index out of range"<<endl;
   return *v[i];
}


double& MPMatrix::operator()(int i, int j)
{ if ( i<0 || i>=d1 )  cout<<"MPMatrix: row index out of range"<<endl;
  if ( j<0 || j>=d2 )  cout<<"MPMatrix: col index out of range"<<endl;
  return elem(i,j);
}

double MPMatrix::operator()(int i, int j) const
{ if ( i<0 || i>=d1 )  cout<<"MPMatrix: row index out of range"<<endl;
  if ( j<0 || j>=d2 )  cout<<"MPMatrix: col index out of range"<<endl;
  return elem(i,j);
}

MPVector MPMatrix::col(int i)  const
{ if ( i<0 || i>=d2 )  cout<<"MPMatrix: col index out of range"<<endl;
  MPVector result(d1);
  int j = d1;
  while (j--) result.v[j] = elem(j,i);
  return result;
}

MPMatrix::operator MPVector() const
{ if (d2!=1) 
   cout<<"error: cannot make MPVector from MPMatrix\n"<<endl;
  return col(0);
}

MPMatrix MPMatrix::operator+(const MPMatrix& mat) const
{ register int i,j;
  check_dimensions(mat);
  MPMatrix result(d1,d2);
  for(i=0;i<d1;i++)
   for(j=0;j<d2;j++)
      result.elem(i,j) = elem(i,j) + mat.elem(i,j);
  return result;
}

MPMatrix& MPMatrix::operator+=(const MPMatrix& mat) 
{ register int i,j;
  check_dimensions(mat);
  for(i=0;i<d1;i++)
   for(j=0;j<d2;j++)
      elem(i,j) += mat.elem(i,j);
  return *this;
}

MPMatrix& MPMatrix::operator-=(const MPMatrix& mat) 
{ register int i,j;
  check_dimensions(mat);
  for(i=0;i<d1;i++)
   for(j=0;j<d2;j++)
      elem(i,j) -= mat.elem(i,j);
  return *this;
}


MPMatrix MPMatrix::operator-(const MPMatrix& mat) const
{ register int i,j;
  check_dimensions(mat);
  MPMatrix result(d1,d2);
  for(i=0;i<d1;i++)
   for(j=0;j<d2;j++)
      result.elem(i,j) = elem(i,j) - mat.elem(i,j);
  return result;
}


MPMatrix MPMatrix::operator-()  const
{ register int i,j;
  MPMatrix result(d1,d2);
  for(i=0;i<d1;i++)
   for(j=0;j<d2;j++)
      result.elem(i,j) = -elem(i,j);
  return result;
}


MPMatrix MPMatrix::operator*(double f) const
{ register int i,j;
  MPMatrix result(d1,d2);
  for(i=0;i<d1;i++)
   for(j=0;j<d2;j++)
      result.elem(i,j) = elem(i,j) *f;
  return result;
}

MPMatrix MPMatrix::operator*(const MPMatrix& mat) const
{ if (d2!=mat.d1)
     cout<<"MPMatrix multiplication: incompatible MPMatrix types\n"<<endl;
  
  MPMatrix result(d1, mat.d2);
  register int i,j;

  for (i=0;i<mat.d2;i++)
  for (j=0;j<d1;j++) result.elem(j,i) = *v[j] * mat.col(i);

 return result;

}

double MPMatrix::det() const
{
 if (d1!=d2)  
   cout<<"MPMatrix::det: MPMatrix not quadratic.\n"<<endl;

 int n = d1;

 MPMatrix M(n,1);

 int flips;

 double** A = triang(M,flips);

 if (A == NULL)  return 0;

 double Det = 1;

 int i;
 for(i=0;i<n;i++) Det *= A[i][i];
 for(i=0;i<n;i++) delete[] A[i];
 delete[] A;

 return (flips % 2) ? -Det : Det;

}


double** MPMatrix::triang(const MPMatrix& M, int& flips)  const
{
 register double **p, **q;
 register double *l, *r, *s;

 register double pivot_el,tmp;

 register int i,j, col, row;

 int n = d1;
 int d = M.d2;
 int m = n+d;

 double** A = new double*[n];

 p = A;

 for(i=0;i<n;i++) 
 { *p = new double[m];
   l = *p++;
   for(j=0;j<n;j++) *l++ = elem(i,j);
   for(j=0;j<d;j++) *l++ = M.elem(i,j);
  }

 flips = 0;

 for (col=0, row=0; row<n; row++, col++)
 { 
   // search for row j with maximal absolute entry in current col
   j = row;
   for (i=row+1; i<n; i++)
     if (fabs(A[j][col]) < fabs(A[i][col])) j = i;

   if (row < j) 
   { double* p = A[j];
     A[j] = A[row];
     A[row] = p;
     flips++;
    }

   tmp = A[row][col];
   q  = &A[row];

   if (fabs(tmp) < EPSILON) // MPMatrix has not full rank
   { p = A;
     for(i=0;i<n;i++) delete A[i];
     delete[] A;
     return NULL;
    }

   for (p = &A[n-1]; p != q; p--)
   { 
     l = *p+col;
     s = *p+m;	
     r = *q+col;

     if (*l != 0.0)
     { pivot_el = *l/tmp;
        while(l < s) *l++ -= *r++ * pivot_el;
      }

    }

  }

 return A;
}

MPMatrix MPMatrix::inv() const
{
 if (d1!=d2)  
     cout<<"MPMatrix::inv: MPMatrix not quadratic."<<endl;
 int n = d1;
 MPMatrix I(n,n);
 for(int i=0; i<n; i++) I(i,i) = 1;
 return solve(I);
}



MPMatrix MPMatrix::solve(const MPMatrix& M) const
{

if (d1 != d2 || d1 != M.d1)
     cout<<"Solve: wrong dimensions"<<endl;

 register double **p, ** q;
 register double *l, *r, *s;

 int      n = d1;
 int      d = M.d2;
 int      m = n+d;
 int      row, col,i;


 double** A = triang(M,i);

 if (A == NULL) 
   cout<<"MPMatrix::solve: MPMatrix has not full rank."<<endl;

 for (col = n-1, p = &A[n-1]; col>=0; p--, col--)
 { 
   s = *p+m;

   double tmp = (*p)[col];

   for(l=*p+n; l < s; l++) *l /=tmp;

   for(q = A; q != p; q++ )
   { tmp = (*q)[col];
     l = *q+n;
     r = *p+n;
     while(r < s)  *l++ -= *r++ * tmp;
    }
                 
  } 

  MPMatrix result(n,d);

  for(row=0; row<n; row++)
  { l = A[row]+n;
    for(col=0; col<d; col++) result.elem(row,col) = *l++;
    delete[] A[row];
   }

  delete[] A;

  return result;
}


MPMatrix MPMatrix::trans() const
{ MPMatrix result(d2,d1);
  for(int i = 0; i < d2; i++)
    for(int j = 0; j < d1; j++)
      result.elem(i,j) = elem(j,i);
  return result;
}


void MPMatrix::read(istream& is)
{ for(int i = 0; i < d1; i++)
    for(int j = 0; j < d2; j++)
        is >> elem(i,j);
 }

void MPMatrix::print(void)
{
	int i,j;
	for(i=0;i<d1;i++)
	{
		cout<<"[";
		for(j=0;j<d2;j++)
			cout<<elem(i,j)<<"  ";
		cout<<"]"<<endl;
	}
}

 string MPMatrix::GetString()
 {
	
	 ostringstream s;
	 s << dim1() << " " << dim2() << endl;
	 for(int i = 0; i < dim1(); i++)
	 { for(int j = 0; j < dim2(); j++) s << " " << elem(i,j);
		s << endl;
	 }
	  string str = s.str ();

	return str;
 }


ostream& operator<<(ostream& os, const MPMatrix& M)
{ os << M.dim1() << " " << M.dim2() << endl;
  for(int i = 0; i < M.dim1(); i++)
  { for(int j = 0; j < M.dim2(); j++) os << " " << M(i,j);
    os << endl;
   }
  return os;
}

istream& operator>>(istream& is, MPMatrix& M)
{ int d1,d2;
  is >> d1 >> d2;
  MPMatrix MM(d1,d2);
  for(int i = 0; i < d1; i++)
    for(int j = 0; j < d2; j++) 
       is >> MM(i,j);
  M = MM;
  return is;
}