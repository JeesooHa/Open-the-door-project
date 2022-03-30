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


#ifndef MP_MATRIX_H
#define MP_MATRIX_H

#include "MPVector.h"
#include "mslio.h"

class MPMatrix {

  MPVector** v;
  int  d1;
  int  d2;

  void     flip_rows(int,int);
  void     check_dimensions(const MPMatrix&) const; 
  double&  elem(int i, int j) const { return v[i]->v[j]; }
  double** triang(const MPMatrix&, int&) const;
  
public:

  MPMatrix(int n=0, int m=0);
  MPMatrix(int n, int m, double* D);
  MPMatrix(const MPMatrix&);
  MPMatrix(const MPVector&);
  MPMatrix& operator=(const MPMatrix&);
  ~MPMatrix();

  int     dim1()  const  {  return d1; }
  int     dim2()  const  {  return d2; }
  MPVector& row(int i) const;
  MPVector  col(int i) const;
  MPMatrix  trans() const;
  MPMatrix  inv()   const;
  double  det()   const;
  MPMatrix solve(const MPMatrix&) const;
  MPVector solve(const MPVector& b) const 
    { return MPVector(solve(MPMatrix(b))); }
  operator MPVector() const;
  MPVector& operator[](int i)    const { return row(i); }
  double& operator()(int i, int j);
  double  operator()(int,int) const;
  int     operator==(const MPMatrix&)    const;
  int     operator!=(const MPMatrix& x)  const { return !(*this == x); }
  MPMatrix operator+(const MPMatrix& M1) const;
  MPMatrix operator-(const MPMatrix& M1) const;
  MPMatrix operator-() const;
  MPMatrix& operator-=(const MPMatrix&);
  MPMatrix& operator+=(const MPMatrix&);
  MPMatrix operator*(const MPMatrix& M1) const;
  MPVector operator*(const MPVector& vec) const 
    { return MPVector(*this * MPMatrix(vec)); }
  MPMatrix operator*(double x) const;
  void read(istream& I);
  void read() { read(cin); }

  friend ostream& operator<<(ostream& O, const MPMatrix& M);
  friend istream& operator>>(istream& I, MPMatrix& M);

  void print(void);

  string GetString();
};

ostream& operator<<(ostream& O, const MPMatrix& M);
istream& operator>>(istream& I, MPMatrix& M);


#endif
