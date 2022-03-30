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

// *********************************************************************
// CLASS:     MPVector class
// 
// *********************************************************************

#include "stdafx.h"

void MPVector::check_dimensions(const MPVector& vec) const
{ 
	if (d!=vec.d)
		cout<<"Error: Vector arguments have different dimensions."<<endl;
}


MPVector::MPVector() 
{
	d = 0; 
	v = 0;
}

MPVector::MPVector(int n) 
{ 
	if (n<0)	cout<<"Error: negative dimension."<<endl;
	d = n; 
	v = 0;
	if (d > 0)
	{ 
		v = new double[d]; 
		for(int i=0; i<d; i++) 
			v[i] = 0.0;
	}
}

MPVector::MPVector(double x, double y) 
{ 
	v = new double[2];
	d = 2;
	v[0] = x;
	v[1] = y;
}

MPVector::MPVector(double x, double y, double z) 
{ 
	v = new double[3];
	d = 3;
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

MPVector::MPVector(const MPVector& p) 
{ 
	d = p.d; 
	v = 0;
	if (d > 0)
	{ 
		v = new double[d];
		for(int i=0; i<d; i++) 
			v[i] = p.v[i];
	}
}

MPVector::~MPVector() 
{   
	if (v) 
	{
		delete[] v; 
		v = 0;
	}
}

void MPVector::print(void)
{
	if(d > 0)
	{
		cout<<"(";
		for(int i=0; i<d; i++)
			cout<<v[i]<<"  ";
		cout<<")"<<endl;
	}

}

MPVector& MPVector::operator=(const MPVector& vec)
{ 
	if (d < vec.d)
	{
		if (v)
		delete(v);

		d = vec.d;

		if (d > 0)
			v = new double[d];
		else
			v = 0;
	}

	for(int i=0; i< vec.dim(); i++) 
		v[i] = vec.v[i];

	return *this;
}

double  MPVector::operator[](int i) const
{ 
	if (i<0 || i>=d)	cout<<"Error: negative dimension."<<endl;
	return v[i]; 
}

double& MPVector::operator[](int i)
{ 
	if (i<0 || i>=d)	cout<<"Error: negative dimension."<<endl;
	return v[i]; 
}

double MPVector::sqr_length() const { return *this * *this; }

double MPVector::length() const { return sqrt(sqr_length()); }

MPVector& MPVector::operator+=(const MPVector& vec)
{
	check_dimensions(vec);
	int n = d;
	while (n--) 
		v[n] += vec.v[n];
	return *this;
}

MPVector& MPVector::operator-=(const MPVector& vec)
{
	check_dimensions(vec);
	int n = d;
	while (n--) 
		v[n] -= vec.v[n];
	return *this;
}

MPVector MPVector::operator+(const MPVector& vec) const
{
	check_dimensions(vec);
	int n = d;
	MPVector result(n);
	while (n--) 
		result.v[n] = v[n]+vec.v[n];
	return result;
}

MPVector MPVector::operator-(const MPVector& vec) const
{ 
	check_dimensions(vec);
	int n = d;
	MPVector result(n);
	while (n--) 
		result.v[n] = v[n]-vec.v[n];
	return result;
}

MPVector MPVector::operator-() const
{ 
	int n = d;
	MPVector result(n);
	while (n--) 
		result.v[n] = -v[n];
	return result;
}


MPVector MPVector::operator*(double x) const
{ 
	int n = d;
	MPVector result(n);
	while (n--) 
		result.v[n] = v[n] * x;
	return result;
}

MPVector MPVector::operator/(double x) const
{ 
	int n = d;
	MPVector result(n);
	while (n--) 
		result.v[n] = v[n] / x;
	return result;
}

double MPVector::operator*(const MPVector& vec) const
{ 
	check_dimensions(vec);
	double result=0;
	int n = d;
	while (n--) 
		result = result+v[n]*vec.v[n];
	return result;
}

bool MPVector::operator==(const MPVector& vec)  const
{
	if (vec.d != d) return false;
	int i = 0;
	while ((i<d) && (v[i]==vec.v[i])) 
		i++;
	return (i==d) ? true : false;
}


