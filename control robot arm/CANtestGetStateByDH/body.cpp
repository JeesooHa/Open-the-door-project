#pragma once
#include "head.h"
#include "iostream"
#include <vector>
#include <stdlib.h>
#include "stdafx.h"
// #include <math.h>

const double pi  =3.141592653589793238463;

using namespace std;


//vector<vector<double>> matrixOperations::defineMatrix(int dimM, int dimN)
//{
//	vector<vector<double>> matrix;
//	matrix.resize(dimM);
//	for (int i=0; i<dimM; i++)
//		matrix[i].resize(dimN);
//
//	return matrix;
//}
//
//vector<vector<double>> matrixOperations::mmult(vector<vector<double>>mat1, vector<vector<double>>mat2)
//{
//	int dimM = mat1.size();
//	int dimN = mat2[0].size();
//	
//	vector<vector<double>> matrixProduct = defineMatrix(dimM, dimN);
//
//	for(int i=0; i< dimM; i++)
//		for (int j=0; j< dimN; j++)
//		{
//			double sum =0;
//			for (int k=0; k<dimM; k++)
//			{
//				sum += mat1[i][k]*mat2[k][j]; 
//			}
//			matrixProduct[i][j] = sum;
//
//		}
//
//	return matrixProduct;
//}
//

// 
// vector<float> linespace( float start, float end,  int divisions)
// {
// 	float angles[100]={0};
// 	for (int i=0; i<=divisions; i++){
// 		angles[i]=start+(i*(end-start)/divisions);
// 	}
// 	return angles;
// 
// }

// The function to calculate inverse function 
/*int InvMatrix(int n, const double* A, double* b)  // nxn matrix, matrix A is invesered and saved to matrix b
{
	double m;
	register int i, j, k;
	double* a = new double[n*n];
	if(a==NULL)  
		return 0;
	for(i=0; i<n*n; i++)  
		a[i]=A[i];
	for(i=0; i<n; i++)  
	{
		for(j=0; j<n; j++)
		{
			b[i*n+j]=(i==j)?1.:0.;
		}
	}
	for(i=0; i<n; i++)
	{
		if(a[i*n+i]==0.)  
		{
			if(i==n-1)  
			{
				delete[] a;
				return 0;
			}
			for(k=1; i+k<n; k++)
			{
				if(a[i*n+i+k] != 0.)  
					break;
			}
			if(i+k>=n)
			{
				delete[] a;
				return 0;
			}
			for(j=0; j<n; j++)  
			{
				m = a[i*n+j];
				a[i*n+j] = a[(i+k)*n+j];
				a[(i+k)*n+j] = m;
				m = b[i*n+j];
				b[i*n+j] = b[(i+k)*n+j];
				b[(i+k)*n+j] = m; 
			}
		}
		m = a[i*n+i];
		for(j=0; j<n; j++)  
		{
			a[i*n+j]/=m;
			b[i*n+j]/=m;
		}
		for(j=0; j<n; j++)  
		{
			if(i==j)  
				continue;
			m = a[j*n+i];
			for(k=0; k<n; k++)   
			{
				a[j*n+k] -= a[i*n+k]*m;
				b[j*n+k] -= b[i*n+k]*m;
			}
		}
	}
	delete[] a;
	return 1;
}

*/


//the function to move one point to the other point.
int MoveP(int num, float *Q, double* pint_d,float IT,float FT, float s_time)
{
	int n = num;	// to access Q array values
	int m=0;		// th access pint_d array values

	MPMatrix T0_1(4,4);
	MPMatrix T1_2(4,4);
	MPMatrix T2_3(4,4);
	MPMatrix T3_4(4,4);
	MPMatrix T4_5(4,4);
	MPMatrix T5_6(4,4);

	MPMatrix Qs(6,1);


	//the joint values
	float q1 = *(Q+n*6+0);
	float q2 = *(Q+n*6+1);
	float q3 = *(Q+n*6+2);
	float q4 = *(Q+n*6+3);
	float q5 = *(Q+n*6+4);
	float q6 = *(Q+n*6+5);

	//save first joint values to use
	Qs(0,0) = q1;
	Qs(1,0) = q2;
	Qs(2,0) = q3;
	Qs(3,0) = q4;
	Qs(4,0) = q5;
	Qs(5,0) = q6;
	

	//cout<<q1;//<<" "<<q2<<" "<<q3<<" "<<q4<<" "<<q5<<" "<<q5<<" "<<q6<<endl;

	for(float i =IT ;i<FT;i=i+s_time)
	{

		//cout<<"/********"<<" "<<n<<" "<<"********/"<<endl;

		// DH parameters 
		T0_1(0,0)= cos(q1);	T0_1(0,1)= 0;	T0_1(0,2)=-sin(q1);	T0_1(0,3)= 0;
		T0_1(1,0)= sin(q1);	T0_1(1,1)= 0;	T0_1(1,2)= cos(q1);	T0_1(1,3)= 0;
		T0_1(2,0)= 0;		T0_1(2,1)= -1;  T0_1(2,2)= 0;		T0_1(2,3)= 0;
		T0_1(3,0)= 0;		T0_1(3,1)= 0;	T0_1(3,2)= 0;		T0_1(3,3)= 1;

		T1_2(0,0)= cos(q2-pi/2);	T1_2(0,1)= sin(q2-pi/2);	T1_2(0,2)= 0;		T1_2(0,3)= 0.35*cos(q2-pi/2);
		T1_2(1,0)= sin(q2-pi/2);	T1_2(1,1)= -cos(q2-pi/2);	T1_2(1,2)= 0;		T1_2(1,3)= 0.35*sin(q2-pi/2);
		T1_2(2,0)= 0;				T1_2(2,1)= 0;				T1_2(2,2)= -1;		T1_2(2,3)= 0;
		T1_2(3,0)= 0;				T1_2(3,1)= 0;				T1_2(3,2)= 0;		T1_2(3,3)= 1;

		T2_3(0,0)= cos(q3-pi/2);	T2_3(0,1)= 0;	T2_3(0,2)= -sin(q3-pi/2);	T2_3(0,3)= 0;
		T2_3(1,0)= sin(q3-pi/2);	T2_3(1,1)= 0;	T2_3(1,2)=  cos(q3-pi/2);	T2_3(1,3)= 0;
		T2_3(2,0)= 0;				T2_3(2,1)= -1;  T2_3(2,2)= 0 ;				T2_3(2,3)= 0;
		T2_3(3,0)= 0;				T2_3(3,1)= 0;	T2_3(3,2)= 0;				T2_3(3,3)= 1;


		T3_4(0,0)= cos(q4);	T3_4(0,1)= 0;	T3_4(0,2)= sin(q4);		T3_4(0,3)= 0;
		T3_4(1,0)= sin(q4);	T3_4(1,1)= 0;	T3_4(1,2)= -cos(q4);	T3_4(1,3)= 0;
		T3_4(2,0)= 0 ;		T3_4(2,1)= 1;   T3_4(2,2)= 0 ;			T3_4(2,3)= 0.305;
		T3_4(3,0)= 0;		T3_4(3,1)= 0;	T3_4(3,2)= 0;			T3_4(3,3)= 1;


		T4_5(0,0)= cos(q5);	T4_5(0,1)= 0;	T4_5(0,2)= -sin(q5);	T4_5(0,3)= 0;
		T4_5(1,0)= sin(q5);	T4_5(1,1)= 0;	T4_5(1,2)= cos(q5);		T4_5(1,3)= 0;
		T4_5(2,0)= 0;		T4_5(2,1)= -1;  T4_5(2,2)= 0 ;			T4_5(2,3)= 0;
		T4_5(3,0)= 0;		T4_5(3,1)= 0;	T4_5(3,2)= 0;			T4_5(3,3)= 1;


		T5_6(0,0)= cos(q6);	T5_6(0,1)= -sin(q5);	T5_6(0,2)= 0;	T5_6(0,3)= 0;
		T5_6(1,0)= sin(q6);	T5_6(1,1)= cos(q5);		T5_6(1,2)= 0;	T5_6(1,3)= 0;
		T5_6(2,0)= 0;		T5_6(2,1)= -0;			T5_6(2,2)= 1;	T5_6(2,3)= 0.320 ;
		T5_6(3,0)= 0;		T5_6(3,1)= 0;			T5_6(3,2)= 0;	T5_6(3,3)= 1;

	
		MPMatrix T0_2(4,4);
		MPMatrix T0_3(4,4);
		MPMatrix T0_4(4,4);
		MPMatrix T0_5(4,4);
		MPMatrix T0_6(4,4);


		 //Forward kinematics
		 T0_2 = T0_1 * T1_2;
		 T0_3 = T0_1 * T1_2 * T2_3;
		 T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
		 T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
		 T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

		 MPMatrix z0(3,1);
		 MPMatrix z1(3,1);
		 MPMatrix z2(3,1);
		 MPMatrix z3(3,1);
		 MPMatrix z4(3,1);
		 MPMatrix z5(3,1);
		 MPMatrix z6(3,1);

		 // to extract the z coordinate
		z0(0,0) = 0; z0(1,0) = 0; z0(2,0) = 1;
		for (int h=0; h<3; h++){
			z1(h,0)=T0_1(h,2);
			z2(h,0)=T0_2(h,2);
			z3(h,0)=T0_3(h,2);
			z4(h,0)=T0_4(h,2);
			z5(h,0)=T0_5(h,2);
			z6(h,0)=T0_6(h,2);
		}

	/*
		// ************** for check z's matrix ************** //
 		cout<<"<<<z1 matix>>>"<<endl;
 		z1.print();
 		cout<<endl;
 		cout<<"<<<z2 matix>>>"<<endl;
		z2.print();
		cout<<endl;
		cout<<"<<<z3 matix>>>"<<endl;
		z3.print();
		cout<<endl;
		cout<<"<<<z4 matix>>>"<<endl;
		z4.print();
		cout<<endl;
		cout<<"<<<z5 matix>>>"<<endl;
		z5.print();
		cout<<endl;
		cout<<"<<<z6 matix>>>"<<endl;
		z6.print();
		cout<<endl;
	*/


		//Jacobian Matrix
		MPMatrix J(6,6);
		
		J(0,0)=z0(1,0)*(T0_6(2,3)-0)-z0(2,0)*(T0_6(1,3)-0);
		J(1,0)=z0(2,0)*(T0_6(0,3)-0)-z0(0,0)*(T0_6(2,3)-0);
		J(2,0)=z0(0,0)*(T0_6(1,3)-0)-z0(1,0)*(T0_6(0,3)-0);
		J(3,0)=z0(0,0);
		J(4,0)=z0(1,0);
		J(5,0)=z0(2,0);

		J(0,1)=z1(1,0)*(T0_6(2,3)-T0_1(2,3))-z1(2,0)*(T0_6(1,3)-T0_1(1,3));
		J(1,1)=z1(2,0)*(T0_6(0,3)-T0_1(0,3))-z1(0,0)*(T0_6(2,3)-T0_1(2,3));
		J(2,1)=z1(0,0)*(T0_6(1,3)-T0_1(1,3))-z1(1,0)*(T0_6(0,3)-T0_1(0,3));
		J(3,1)=z1(0,0);
		J(4,1)=z1(1,0);
		J(5,1)=z1(2,0);

		J(0,2)=z2(1,0)*(T0_6(2,3)-T0_2(2,3))-z2(2,0)*(T0_6(1,3)-T0_2(1,3));
		J(1,2)=z2(2,0)*(T0_6(0,3)-T0_2(0,3))-z2(0,0)*(T0_6(2,3)-T0_2(2,3));
		J(2,2)=z2(0,0)*(T0_6(1,3)-T0_2(1,3))-z2(1,0)*(T0_6(0,3)-T0_2(0,3));
		J(3,2)=z2(0,0);
		J(4,2)=z2(1,0);
		J(5,2)=z2(2,0);

		J(0,3)=z3(1,0)*(T0_6(2,3)-T0_3(2,3))-z3(2,0)*(T0_6(1,3)-T0_3(1,3));
		J(1,3)=z3(2,0)*(T0_6(0,3)-T0_3(0,3))-z3(0,0)*(T0_6(2,3)-T0_3(2,3));
		J(2,3)=z3(0,0)*(T0_6(1,3)-T0_3(1,3))-z3(1,0)*(T0_6(0,3)-T0_3(0,3));
		J(3,3)=z3(0,0);
		J(4,3)=z3(1,0);
		J(5,3)=z3(2,0);

		J(0,4)=z4(1,0)*(T0_6(2,3)-T0_4(2,3))-z4(2,0)*(T0_6(1,3)-T0_4(1,3));
		J(1,4)=z4(2,0)*(T0_6(0,3)-T0_4(0,3))-z4(0,0)*(T0_6(2,3)-T0_4(2,3));
		J(2,4)=z4(0,0)*(T0_6(1,3)-T0_4(1,3))-z4(1,0)*(T0_6(0,3)-T0_4(0,3));
		J(3,4)=z4(0,0);
		J(4,4)=z4(1,0);
		J(5,4)=z4(2,0);

		J(0,5)=z5(1,0)*(T0_6(2,3)-T0_5(2,3))-z5(2,0)*(T0_6(1,3)-T0_5(1,3));
		J(1,5)=z5(2,0)*(T0_6(0,3)-T0_5(0,3))-z5(0,0)*(T0_6(2,3)-T0_5(2,3));
		J(2,5)=z5(0,0)*(T0_6(1,3)-T0_5(1,3))-z5(1,0)*(T0_6(0,3)-T0_5(0,3));
		J(3,5)=z5(0,0);
		J(4,5)=z5(1,0);
		J(5,5)=z5(2,0);


		//*********** for check Jacobian matrix values *************//
		/*
		cout<<"<<<Jacobian Matrix>>>"<<endl;
		J.print();
		cout<<endl;
		cout<<"<<<Jacobian Matrix Det>>>"<<endl;
		cout<<J.det()<<endl;
		cout<<endl;
		*/

		MPMatrix Rmat(6,1);
		//to make the right side matrix to use getting Qs values
 		Rmat(0,0) = *(pint_d+m*6+0);
 		Rmat(1,0) = *(pint_d+m*6+1);
 		Rmat(2,0) = *(pint_d+m*6+2);
 		Rmat(3,0) = *(pint_d+m*6+3);
 		Rmat(4,0) = *(pint_d+m*6+4);
 		Rmat(5,0) = *(pint_d+m*6+5);

	/*
 		cout<<"<<<Rmat Matrix>>>"<<endl;
 		Rmat.print();
 		cout<<endl;
	*/

	/*
		// *********** for check Inverse Jacobian matrix values ************* //
 		cout<<"<<<Inverse Jacobian Matrix>>>"<<endl;
		J.inv().print();
 		cout<<endl;
	*/

		MPMatrix Qst(6,1);
		// Calculate joint values
		Qs = Qs+(J.inv())*Rmat*s_time;

	/*	
		cout<<"<<<Qs Matrix>>>"<<endl;
		Qs.print();
		cout<<endl;
		cout<<endl;
	*/
	
		//for save joint values to Qs_save matrix
		*(Q+n*6+0) = Qs(0,0);
		*(Q+n*6+1) = Qs(1,0);
		*(Q+n*6+2) = Qs(2,0);
		*(Q+n*6+3) = Qs(3,0);
		*(Q+n*6+4) = Qs(4,0);
		*(Q+n*6+5) = Qs(5,0);
		
		//update joint values
		q1 = Qs(0,0);
		q2 = Qs(1,0);
		q3 = Qs(2,0);
		q4 = Qs(3,0);
		q5 = Qs(4,0);
		q6 = Qs(5,0);

// 		cout<<q1<<" "<<q2<<" "<<q3<<" "<<q4<<" "<<q5<<" "<<q6<<endl;

		n = n+1;
		m = m+1;
	}
	
	/*
	cout<< "    "<<n<<"     "<<endl;
	for(int k = 0; k<41;k++)
		{for(int h =0; h<6;h++)
			cout<<Qs_save[k][h]<<" ";
		cout<<endl;
		}
	*/

	return n-1;
}

	