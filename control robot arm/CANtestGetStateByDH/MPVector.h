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

#ifndef MP_VECTOR_H
#define MP_VECTOR_H

using namespace std;

class MPVector
{
	friend class MPMatrix;

	double* v;
	int d;

	void check_dimensions(const MPVector&) const;

public:

	MPVector();
	MPVector(int d); 
	MPVector(double a, double b);
	MPVector(double a, double b, double c);
	MPVector(const MPVector&);
	~MPVector(); 

	int    dim()    const { return d; }
	void print(void);

	MPVector& operator=(const MPVector&);
	double& operator[](int i);
	double  operator[](int) const;

	double sqr_length() const;
	double length() const;
	MPVector norm() const { return *this/length(); }

	MPVector& operator+=(const MPVector&);
	MPVector& operator-=(const MPVector&);
	MPVector  operator+(const MPVector& v1) const;
	MPVector  operator-(const MPVector& v1) const;
	double  operator*(const MPVector& v1) const;	// 두 vector 간에 dot product 이다!
	MPVector  operator*(double r)        const;
	MPVector  operator-() const;
	MPVector  operator/(double)        const;
	bool     operator==(const MPVector& w) const;
	bool     operator!=(const MPVector& w)  const { return !(*this == w); }
	friend MPVector operator*(double f, const MPVector& v) { return v *f; }


};

#endif