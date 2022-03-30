#pragma once
#include <vector>


using namespace std;
const double pi  =3.141592653589793238463;
class matrixOperations
{
public:

	vector<vector<double>> defineMatrix(int dimM, int dimN);
	vector<vector<double>> mmult(vector<vector<double>>mat1, vector<vector<double>>mat2);
	/*vector linespace( float start, float end, int divisions);*/
};

float MultiplyWithOutAMP(float mat1[][10], float mat2[][10], int a, int b, float product[][10]);
int mmul(float** aa, float** ba, int a, int b, int*** Ag);
int MoveP(int num, float *Q, double* pint_d,float IT,float FT, float s_time);
void trajectory(int *pA1, int *pA2, int *pA3,int *pA4,int *pA5);