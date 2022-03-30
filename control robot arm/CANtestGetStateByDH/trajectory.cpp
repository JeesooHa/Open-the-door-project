#include "iostream"
#include "head.h"
#include "stdafx.h"	//for multiple Matrix or Vector												//change
#include <fstream>		//to save the data to txt file
using namespace std;

int MoveP(int num, float *Q, double* pint_d,float IT,float FT, float s_time);					//change

const double pi  =3.141592653589793238463;
#define max_motion 245


struct Sspace
{
	bool toK, toA, toM, OK;
	int doorX, doorY;
	int Kmode;

	Sspace()
	{
		toA = false; toK = false; toM = false; OK = false;
		doorX = 0; doorY = 0;
		Kmode = 0;
	}

};


HANDLE hMemoryMap;
LPBYTE pMemoryMap;

void trajectory(int *pA1, int *pA2, int *pA3,int *pA4,int *pA5)																				//change
{
	/////////////////////////////
	//input Handle coordination//
	/////////////////////////////


	
	//0.45 -> x input
	//0.34 -> y input
	//0.355 -> z input


	hMemoryMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Sspace), _T("fork_server"));
	if (!hMemoryMap)
	{
		printf("공유메모리 생성 실패\n");
		//return FALSE;
	}

	pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!pMemoryMap)
	{
		CloseHandle(hMemoryMap);
		printf("공유메모리를 열수 없습니다\n");
		//return FALSE;
	}

	Sspace *pSpace = (Sspace*)pMemoryMap;


	double x_input= 440/1000.0;
	double y_input = pSpace->doorX/1000.0;
	double z_input = pSpace->doorY/1000.0;

	pSpace->toA = true;
	pSpace->toM = false;



	if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
	if (hMemoryMap) CloseHandle(hMemoryMap);


	// time setting
	int IT=0;
	int FT=2;
	float time=0;
	const float s_time=0.05;
	
	// initial joint values
	float q1 = 0.01;
	float q2 = pi/4;
	float q3 = pi/3;
	float q4 = 0.01;
	float q5 = pi*3/4-pi/3;
	float q6 = 0.01;

	//to save all motion
	float *pQs;
	float Qs_save[max_motion][6] ={0};

	//save initial joint angles to Qs_save
	Qs_save[0][0] = q1;
	Qs_save[0][1] = q2;
	Qs_save[0][2] = q3;
	Qs_save[0][3] = q4;
	Qs_save[0][4] = q5;
	Qs_save[0][5] = q6;

	pQs = Qs_save[0];	


	/*
	//to check Qs_save's row values - OKAY
	 	cout<<*(pQs+0)<<endl;	//to access row
	
	 	cout<<endl;
	 	cout<<*(pQs+1)<<endl;	//to access row
	 	cout<<endl;
	 	cout<<*(pQs+2)<<endl;	//to access row
	 	cout<<endl;
	 	cout<<*(pQs+3)<<endl;	//to access row
	 	cout<<endl;
	 	cout<<*(pQs+4)<<endl;	//to access row
	 	cout<<endl;
	 	cout<<*(pQs+5)<<endl;	//to access row
	 	cout<<endl;
	*/

	//to check Qs_save's col values - OKAY
	/*
	 	cout<<*(pQs+0)<<endl;	//to access col
	 	cout<<*(pQs+6*1+1)<<endl;	//to access col
	 	cout<<*(pQs+6*2+1)<<endl;	//to access col
	 	cout<<*(pQs+6*3+1)<<endl;	//to access col
	 	cout<<*(pQs+6*4+1)<<endl;	//to access col
	 	cout<<*(pQs+6*5+1)<<endl;	//to access col
	 	cout<<*(pQs+6*6+1)<<endl;	//to access col
	 	cout<<*(pQs+6*7+1)<<endl;	//to access col
	 	cout<<endl;
	*/

	//Desired workspace configurations (position velocity acceleration)(inputs)
	double home[] = {-0.1515,0,0.5421}; //coordination		//Change xyz
	double home_d[] = {0,0,0};        //vel.
	double home_dd[] = {0,0,0};       //accel.
	double home_A[] = {0,0,0};        //angle coordination
	double home_Ad[] = {0,0,0};
	double home_Add[] = {0,0,0};

	double task1[] = {-x_input+0.01,y_input,z_input}; // for task 1 //Change xyz
	double task1_d[] = {0,0,0};
	double task1_dd[] = {0,0,0};
	double task1_A[] = {-pi/2,0,0};
	double task1_Ad[] = {0,0,0};
	double task1_Add[] = {0,0,0};

	double task2[] = {-x_input+0.01,y_input+0.05,z_input}; // for task2  //Change xyz
	double task2_d[] = {0,0,0};
	double task2_dd[] = {0,0,0};
	double task2_A[] = {-pi/2,0,0};
	double task2_Ad[] = {0,0,0};
	double task2_Add[] = {0,0,0};

	double open[] = {-x_input+0.01,y_input+0.05,z_input}; // for open   //Change xyz
	double open_d[] = {0,0,0};
	double open_dd[] = {0,0,0};
	double open_A[] = {-pi/2,0,0};
	double open_Ad[] = {0,0,0};
	double open_Add[] = {0,0,0};

	double E_open[] = {-x_input+0.01,-y_input+0.05,z_input}; // for E_open  //Change xyz
	double E_open_d[] = {0,0,0};
	double E_open_dd[] = {0,0,0};
	double E_open_A[] = {-pi/2,0,pi/2};
	double E_open_Ad[] = {0,0,0};
	double E_open_Add[] = {0,0,0};

	double B_open[] = {-x_input+0.30+0.01,-y_input+0.05,z_input};  //Change xyz
	double B_open_d[] = {0,0,0};
	double B_open_dd[] = {0,0,0};
	double B_open_A[] = {-pi/2,0,pi/2};
	double B_open_Ad[] = {0,0,0};
	double B_open_Add[] = {0,0,0};

	// to move circular motion - open, E_open motion
	float angle[25]={0};
	for(int i=0; i<25; i++){
		angle[i]=0+(i*(pi*3.7/18-0)/25);
	}

	float angle_b[25]={0};
	for(int i=0; i<25; i++){
		angle_b[i]=pi*3.7/18+(i*(0-pi*3.7/18)/25);
	}

	//to check angle array's values - OKAY
	/*
	 	cout<<"<<<angle array>>>"<<endl;
	 	for(int j=0;j<=25;j++)
	 		cout<<angle[j]<<" "<<endl;
	 	cout<<endl;
	 
	 	cout<<"<<<angle_b array>>>"<<endl;
	 	for(int j=0;j<=25;j++)
	 			cout<<angle_b[j]<<" "<<endl;
			cout<<endl;
	*/


	//for task3 -open
	float p2top3[25][3]={0};
	float a2toa3[25][3]={0};

	for (int i=0; i<25; i++){ //Change xyz
		p2top3[i][0] = -x_input+0.01 ;  
		p2top3[i][1] = y_input+0.05-0.115*cos(angle[i]);
		p2top3[i][2] =  z_input-0.115*sin(angle[i]);

		a2toa3[i][0] = (pi/2)+angle[i] ;
		a2toa3[i][1] = 0;
		a2toa3[i][2] = 0;	
	}

	//for task4 - E_open
	float p3top4[25][3]={0};
	float a3toa4[25][3]={0};

	for (int i=0; i<25; i++){  //Change xyz
		p3top4[i][0] = -x_input+0.01 ;  
		p3top4[i][1] = y_input+0.05-0.115*cos(angle_b[i]);
		p3top4[i][2] = z_input-0.115*sin(angle_b[i]);

		a3toa4[i][0] = (pi/2)+angle_b[i] ;
		a3toa4[i][1] = 0;
		a3toa4[i][2] = 0;	
	}


	double *htot1 = home;
	double *htot1_d = home_d;
	double *htot1_dd = home_dd;
	double *htot1_A = home_A;
	double *htot1_Ad = home_Ad;
	double *htot1_Add = home_Add;

	double *t1tot2 = task1;
	double *t1tot2_d = task1_d;
	double *t1tot2_dd = task1_dd;
	double *t1tot2_A = task1_A;
	double *t1tot2_Ad = task1_Ad;
	double *t1tot2_Add = task1_Add;

	double *t2tot3 = task2;
	double *t2tot3_d = task2_d;
	double *t2tot3_dd = task2_dd;

	double *otoE = open;
	double *otoE_d = open_d;
	double *otoE_dd = open_dd;
	double *otoE_A = open_A;
	double *otoE_Ad = open_Ad;
	double *otoE_Add = open_Add;

	double *EtoB = E_open;
	double *EtoB_d = E_open_d;
	double *EtoB_dd = E_open_dd;
	double *EtoB_A = E_open_A;
	double *EtoB_Ad = E_open_Ad;
	double *EtoB_Add = E_open_Add;

	double *Btoh = B_open;
	double *Btoh_d = B_open_d;
	double *Btoh_dd = B_open_dd;
	double *Btoh_A = B_open_A;
	double *Btoh_Ad = B_open_Ad;
	double *Btoh_Add = B_open_Add;


	//Quintic polynomial trajectories
	double Qnt[6][6] = {{1, IT, IT*IT, IT*IT*IT, IT*IT*IT*IT, IT*IT*IT*IT*IT},                  //representing equation in matrix
						{0, 1,  2*IT, 3*IT*IT, 4*IT*IT*IT, 5*IT*IT*IT*IT},            //inital and termial time
						{0, 0, 2, 6*IT, 12*IT*IT, 20*IT*IT*IT},
						{1, FT, FT*FT, FT*FT*FT, FT*FT*FT*FT, FT*FT*FT*FT*FT},                  //representing equation in matrix
						{0, 1,  2*FT, 3*FT*FT, 4*FT*FT*FT, 5*FT*FT*FT*FT},            //inital and termial time
						{0, 0, 2, 6*FT, 12*FT*FT, 20*FT*FT*FT}
		};

	//to check Qnt  values- OKAY
	/*
		cout<<"<<<This is Qnt  Matrix>>>"<<endl;
	 	for(int i=0;i<6;i++){
	 		for(int j=0;j<6;j++)
	 			cout<<Qnt[i][j]<<" ";
	 		cout<<endl;
	 	}
	 	cout<<endl;
	*/


	//change the form for Multiple Matrix : Qnt->Qt
	MPMatrix Qt(6,6);
	for (int i=0;i<6;i++)
		for(int j=0;j<6;j++)
			Qt(i,j) = Qnt[i][j];

	//to check Qt  values- OKAY

	/*
	cout<<"<<<This is Qnt  Matrix>>>"<<endl;
	Qt.print();
	cout<<endl;
	*/


	//using  Dong-Hyung Kim's code is better.
	//define Qt inverse 
 	MPMatrix QntInv(6,6);
 	QntInv = Qt.inv();


 	//to check Qnt  values	- OKAY
	/*
	 	cout<<"<<<This is Qnt  Matrix>>>"<<endl;
	 	for(int i=0;i<6;i++){
	 		for(int j=0;j<6;j++)
	 			cout<<QntI[i][j]<<" ";
	 		cout<<endl;
	 	}
	 	cout<<endl;
	*/

	//to check Qnt invers values- OKAY
	/*
		cout<<"<<<This is Qnt Inverse Matrix>>>"<<endl;
		Qt.inv().print();
		cout<<endl;
	*/


	//the constants to input trajectory function
	// ex) const_h2t1 =[ home (3x3), home_A(3x3);
	//				     task1(3x3), task1_A(3x3)]
	//define a matrix which will multiple to QI
	// home -> task1
	MPMatrix RMTh1(6,6);
	for(int j = 0;j<3 ;j++){
		//RMTh1 = left 6x3 
		RMTh1(0,j) = home[j];
		RMTh1(1,j) = home_d[j];
		RMTh1(2,j) = home_dd[j];
		RMTh1(3,j) = task1[j];
		RMTh1(4,j) = task1_d[j];
		RMTh1(5,j) = task1_dd[j];

		//RMTh1 = right 6x3 
		RMTh1(0,j+3) = home_A[j];
		RMTh1(1,j+3) = home_Ad[j];
		RMTh1(2,j+3) = home_Add[j];
		RMTh1(3,j+3) = task1_A[j];
		RMTh1(4,j+3) = task1_Ad[j];
		RMTh1(5,j+3) = task1_Add[j];
	}

	MPMatrix cnst_h2t1(6,6);
	cnst_h2t1 = QntInv*RMTh1;


	//task1 -> task2
	MPMatrix RMT12(6,6);
	for(int j = 0;j<3 ;j++){
		//RMT12 = left 6x3 
		RMT12(0,j) = task1[j];
		RMT12(1,j) = task1_d[j];
		RMT12(2,j) = task1_dd[j];
		RMT12(3,j) = task2[j];
		RMT12(4,j) = task2_d[j];
		RMT12(5,j) = task2_dd[j];

		//RMT12 = right 6x3 
		RMT12(0,j+3) = task1_A[j];
		RMT12(1,j+3) = task1_Ad[j];
		RMT12(2,j+3) = task1_Add[j];
		RMT12(3,j+3) = task2_A[j];
		RMT12(4,j+3) = task2_Ad[j];
		RMT12(5,j+3) = task2_Add[j];
	}
	MPMatrix cnst_t12t2(6,6);
	cnst_t12t2 = QntInv*RMT12;


	//task2 -> task3
	MPMatrix RMToE(6,6);
	for(int j = 0;j<3 ;j++){
		//RMToE = left 6x3 
		RMToE(0,j) = open[j];
		RMToE(1,j) = open_d[j];
		RMToE(2,j) = open_dd[j];
		RMToE(3,j) = E_open[j];
		RMToE(4,j) = E_open_d[j];
		RMToE(5,j) = E_open_dd[j];

		//RMToE = right 6x3 
		RMToE(0,j+3) = open_A[j];
		RMToE(1,j+3) = open_Ad[j];
		RMToE(2,j+3) = open_Add[j];
		RMToE(3,j+3) = E_open_A[j];
		RMToE(4,j+3) = E_open_Ad[j];
		RMToE(5,j+3) = E_open_Add[j];
	}
	MPMatrix cnst_o2E(6,6);
	cnst_o2E = QntInv*RMToE;

	//task5 -> task6
	MPMatrix RMTEB(6,6);
	for(int j = 0;j<3 ;j++){
		//RMTEB = left 6x3 
		RMTEB(0,j) = E_open[j];
		RMTEB(1,j) = E_open_d[j];
		RMTEB(2,j) = E_open_dd[j];
		RMTEB(3,j) = B_open[j];
		RMTEB(4,j) = B_open_d[j];
		RMTEB(5,j) = B_open_dd[j];

		//RMTEB = right 6x3 
		RMTEB(0,j+3) = E_open_A[j];
		RMTEB(1,j+3) = E_open_Ad[j];
		RMTEB(2,j+3) = E_open_Add[j];
		RMTEB(3,j+3) = B_open_A[j];
		RMTEB(4,j+3) = B_open_Ad[j];
		RMTEB(5,j+3) = B_open_Add[j];
	}
	MPMatrix cnst_E2B(6,6);
	cnst_E2B = QntInv*RMTEB;

	//task6 -> task7
	MPMatrix RMTBh(6,6);
	for(int j = 0;j<3 ;j++){
		//RMTBH = left 6x3 
		RMTBh(0,j) = B_open[j];
		RMTBh(1,j) = B_open_d[j];
		RMTBh(2,j) = B_open_dd[j];
		RMTBh(3,j) = open[j];
		RMTBh(4,j) = open_d[j];
		RMTBh(5,j) = open_dd[j];

		//RMTBH = right 6x3 
		RMTBh(0,j+3) = B_open_A[j];
		RMTBh(1,j+3) = B_open_Ad[j];
		RMTBh(2,j+3) = B_open_Add[j];
		RMTBh(3,j+3) = open_A[j];
		RMTBh(4,j+3) = open_Ad[j];
		RMTBh(5,j+3) = open_Add[j];
	}
	MPMatrix cnst_B2h(6,6);
	cnst_B2h = QntInv*RMTBh;

	/*
	//to check the upper constants values- OKAY
	 	cout<<endl;
	 	cout<<"<<<Right Matrix to use for calculate cnst_h2t1>>>"<<endl;
	 	RMTh1.print();
	 	cout<<endl;
	 	cout<<"<<<Right Matrix to use for calculate cnst_t12t2>>>"<<endl;
	 	RMT12.print();
	 	cout<<endl;
	 	cout<<"<<<cnst_h2t1 matrix>>>"<<endl;
	 	cnst_h2t1.print();
	 	cout<<endl;
	 	cout<<"<<<cnst_t12t2 matrix>>>"<<endl;
	 	cnst_t12t2.print();
	*/

	//to save data to n-th element of a array
	int n=0;
	

	//******* the number 41 is the value of num to save data which is getted under loop
	// to save x,y,z and 3 angles
	//for task1
	double htot1_save[41][6] = {0};
	double htot1_d_save[41][6] = {0};
	//for task2
	double t1tot2_save[41][6] = {0};
	double t1tot2_d_save[41][6] = {0};
	//for task5
	double otoE_save[41][6] = {0};
	double otoE_d_save[41][6] = {0};
	//for task6
	double EtoB_save[41][6] = {0};
	double EtoB_d_save[41][6] = {0};
	//for task7
	double Btoh_save[41][6] = {0};
	double Btoh_d_save[41][6] = {0};


/*****************************************************************************/
	
	// Calculate trajecories from home to task1 and from task1 to task2
	for (float i=0; i<=FT; i=i+s_time){

		for (int j=0; j<3; j++){
			//home->task1
			htot1_d[j] = cnst_h2t1(1,j) + 2 * cnst_h2t1(2,j)*time + 3*cnst_h2t1(3,j)*time*time + 4*cnst_h2t1(4,j)*time*time*time+ 5*cnst_h2t1(5,j)*time*time*time*time ;
			htot1_Ad[j] = cnst_h2t1(1,j+3) + 2 * cnst_h2t1(2,j+3)*time + 3*cnst_h2t1(3,j+3)*time*time + 4*cnst_h2t1(4,j+3)*time*time*time + 5*cnst_h2t1(5,j+3)*time*time*time*time ;

			//task1->task2
			t1tot2_d[j] = cnst_t12t2(1,j) + 2 * cnst_t12t2(2,j)*time + 3*cnst_t12t2(3,j)*time*time + 4*cnst_t12t2(4,j)*time*time*time + 5*cnst_t12t2(5,j)*time*time*time*time ;
	 		t1tot2_Ad[j] = cnst_t12t2(1,j+3) + 2 * cnst_t12t2(2,j+3)*time + 3*cnst_t12t2(3,j+3)*time*time + 4*cnst_t12t2(4,j+3)*time*time*time + 5*cnst_t12t2(5,j+3)*time*time*time*time ;
	 	
			//task4->task5
			otoE_d[j] = cnst_o2E(1,j) + 2 * cnst_o2E(2,j)*time + 3*cnst_o2E(3,j)*time*time + 4*cnst_o2E(4,j)*time*time*time + 5*cnst_o2E(5,j)*time*time*time*time ;
	 		otoE_Ad[j] = cnst_o2E(1,j+3) + 2 * cnst_o2E(2,j+3)*time + 3*cnst_o2E(3,j+3)*time*time + 4*cnst_o2E(4,j+3)*time*time*time + 5*cnst_o2E(5,j+3)*time*time*time*time ;
		
			//task5->task6
			EtoB_d[j] = cnst_E2B(1,j) + 2 * cnst_E2B(2,j)*time + 3*cnst_E2B(3,j)*time*time + 4*cnst_E2B(4,j)*time*time*time + 5*cnst_E2B(5,j)*time*time*time*time ;
	 		EtoB_Ad[j] = cnst_E2B(1,j+3) + 2 * cnst_E2B(2,j+3)*time + 3*cnst_E2B(3,j+3)*time*time + 4*cnst_E2B(4,j+3)*time*time*time + 5*cnst_E2B(5,j+3)*time*time*time*time ;
		
			//task6->task7
			Btoh_d[j] = cnst_B2h(1,j) + 2 * cnst_B2h(2,j)*time + 3*cnst_B2h(3,j)*time*time + 4*cnst_B2h(4,j)*time*time*time + 5*cnst_B2h(5,j)*time*time*time*time ;
	 		Btoh_Ad[j] = cnst_B2h(1,j+3) + 2 * cnst_B2h(2,j+3)*time + 3*cnst_B2h(3,j+3)*time*time + 4*cnst_B2h(4,j+3)*time*time*time + 5*cnst_B2h(5,j+3)*time*time*time*time ;
		
		
		}


		/*
		//to check htot1_d trajectory - OKAY
 		cout<<"<<<This is htot1_d  Matrix>>>"<<endl;
		cout<<n<<"  :  "  <<htot1_d[0]<<" "<<htot1_d[1]<<" "<<htot1_d[2]<<endl;
		cout<<endl;

		//to check htot1_Ad trajectory - OKAY
 		cout<<"<<<This is htot1_Ad  Matrix>>>"<<endl;
 		cout<<n<<"  :  "  <<htot1_Ad[0]<<" "<<htot1_Ad[1]<<" "<<htot1_Ad[2]<<endl;
		cout<<endl;

		//to check t1tot2_d trajectory  - OKAY
 		cout<<"<<<This is t1tot2_d Matrix>>>"<<endl;
 		cout<<n<<"  :  "  <<t1tot2_d[0]<<" "<<t1tot2_d[1]<<" "<<t1tot2_d[2]<<endl;
 		cout<<endl;

		//to check t1tot2_Ad trajectory - OKAY
 		cout<<"<<<This is t1tot2_Ad Matrix>>>"<<endl;
 		cout<<n<<"  :  "  <<t1tot2_Ad[0]<<" "<<t1tot2_Ad[1]<<" "<<t1tot2_Ad[2]<<endl;
 		cout<<endl;
		*/
	
		for (int j=0; j<3; j++){
			//home->task1
			htot1[j] = htot1[j] + s_time*htot1_d[j];
			htot1_A[j] = htot1_A[j] + s_time*htot1_Ad[j];

			//task1->task2
			t1tot2[j] = t1tot2[j] + s_time*t1tot2_d[j];
			t1tot2_A[j] = t1tot2_A[j] + s_time*t1tot2_Ad[j];

			//task4->task5
			otoE[j] = otoE[j] + s_time*otoE_d[j];
			otoE_A[j] = otoE_A[j] + s_time*otoE_Ad[j];

			//task5->task6
			EtoB[j] = EtoB[j] + s_time*EtoB_d[j];
			EtoB_A[j] = EtoB_A[j] + s_time*EtoB_Ad[j];

			//task6->task7
			Btoh[j] = Btoh[j] + s_time*Btoh_d[j];
			Btoh_A[j] = Btoh_A[j] + s_time*Btoh_Ad[j];
			}


		/*
			//to check htot1 trajectory - OKAY
			cout<<"<<<This is htot1 Matrix>>>"<<endl;
 			cout<<n<<"  :  "  <<htot1[0]<<" "<<htot1[1]<<" "<<htot1[2]<<endl;
 			cout<<endl;

			//to check htot1_A_Atrajectory - OKAY
 			cout<<"<<<This is htot1_A Matrix>>>"<<endl;
			cout<<n<<"  :  "  <<htot1_A[0]<<" "<<htot1_A[1]<<" "<<htot1_A[2]<<endl;
 			cout<<endl;

			//to check t1tot2 trajectory  - OKAY
			cout<<"<<<This is t1tot2 Matrix>>>"<<endl;
 			cout<<n<<"  :  "  <<t1tot2[0]<<" "<<t1tot2[1]<<" "<<t1tot2[2]<<endl;
 			cout<<endl;

			//to check t1tot2_A trajectory  - OKAY
			cout<<"<<<This is t1tot2_A  Matrix>>>"<<endl;
 			cout<<n<<"  :  "  <<t1tot2_A[0]<<" "<<t1tot2_A[1]<<" "<<t1tot2_A[2]<<endl;
 			cout<<endl;
		*/


		//to save the coordinates during move one position to the other
		//home->task1 x,y,z,ax,ay,az
		htot1_save[n][0]=htot1[0];
		htot1_save[n][1]=htot1[1];
		htot1_save[n][2]=htot1[2];
		htot1_save[n][3]=htot1_A[0];
		htot1_save[n][4]=htot1_A[1];
		htot1_save[n][5]=htot1_A[2];

		//task1->task2 x,y,z,ax,ay,az
		t1tot2_save[n][0]=t1tot2[0];
		t1tot2_save[n][1]=t1tot2[1];
		t1tot2_save[n][2]=t1tot2[2];
		t1tot2_save[n][3]=t1tot2_A[0];
		t1tot2_save[n][4]=t1tot2_A[1];
		t1tot2_save[n][5]=t1tot2_A[2];

		//task4->task5 x,y,z,ax,ay,az
		otoE_save[n][0]=otoE[0];
		otoE_save[n][1]=otoE[1];
		otoE_save[n][2]=otoE[2];
		otoE_save[n][3]=otoE_A[0];
		otoE_save[n][4]=otoE_A[1];
		otoE_save[n][5]=otoE_A[2];

		//task5->task6 x,y,z,ax,ay,az
		EtoB_save[n][0]=EtoB[0];
		EtoB_save[n][1]=EtoB[1];
		EtoB_save[n][2]=EtoB[2];
		EtoB_save[n][3]=EtoB_A[0];
		EtoB_save[n][4]=EtoB_A[1];
		EtoB_save[n][5]=EtoB_A[2];
		
		//task6->task7 x,y,z,ax,ay,az
		Btoh_save[n][0]=Btoh[0];
		Btoh_save[n][1]=Btoh[1];
		Btoh_save[n][2]=Btoh[2];
		Btoh_save[n][3]=Btoh_A[0];
		Btoh_save[n][4]=Btoh_A[1];
		Btoh_save[n][5]=Btoh_A[2];
		
		
		//home->task1 dx,dy,dz,dax,day,daz
		htot1_d_save[n][0]=htot1_d[0];
		htot1_d_save[n][1]=htot1_d[1];
		htot1_d_save[n][2]=htot1_d[2];
		htot1_d_save[n][3]=htot1_Ad[0];
		htot1_d_save[n][4]=htot1_Ad[1];
		htot1_d_save[n][5]=htot1_Ad[2];

		//task1->task2 dx,dy,dz,dax,day,daz
		t1tot2_d_save[n][0]=t1tot2_d[0];
		t1tot2_d_save[n][1]=t1tot2_d[1];
		t1tot2_d_save[n][2]=t1tot2_d[2];
		t1tot2_d_save[n][3]=t1tot2_Ad[0];
		t1tot2_d_save[n][4]=t1tot2_Ad[1];
		t1tot2_d_save[n][5]=t1tot2_Ad[2];

		//task4->task5 dx,dy,dz,dax,day,daz
		otoE_d_save[n][0]=otoE_d[0];
		otoE_d_save[n][1]=-0.68/FT;
		otoE_d_save[n][2]=otoE_d[2];
		otoE_d_save[n][3]=otoE_Ad[0];
		otoE_d_save[n][4]=otoE_Ad[1];
		otoE_d_save[n][5]=otoE_Ad[2];

		//task4->task5 dx,dy,dz,dax,day,daz
		EtoB_d_save[n][0]=0.3/FT;
		EtoB_d_save[n][1]=EtoB_d[1];
		EtoB_d_save[n][2]=EtoB_d[2];
		EtoB_d_save[n][3]=EtoB_Ad[0];
		EtoB_d_save[n][4]=EtoB_Ad[1];
		EtoB_d_save[n][5]=EtoB_Ad[2];

		//task4->task5 dx,dy,dz,dax,day,daz
		Btoh_d_save[n][0]=Btoh_d[0];
		Btoh_d_save[n][1]=Btoh_d[1];
		Btoh_d_save[n][2]=Btoh_d[2];
		Btoh_d_save[n][3]=Btoh_Ad[0];
		Btoh_d_save[n][4]=Btoh_Ad[1];
		Btoh_d_save[n][5]=Btoh_Ad[2];


		n = n+1;
		time = time + s_time;
		}

		/*
		//to check upper elements
 			for(int h=0; h<42;h++)
 				{for(int k = 0;k<6;k++)
 					cout<<htot1_d_save[h][k]<<" ";
 				cout<<endl;
		*/
 	

	// to check t1tot2_A trajectory  - OKAY
	/*
	   	cout<<"<<<This is t1tot2_A  Matrix>>>"<<endl;
	 	cout<<n<<"  :  "  <<t1tot2_A[0]<<" "<<t1tot2_A[1]<<" "<<t1tot2_A[2]<<endl;
	 	cout<<endl;
	*/

/*****************************************************************************************/
	 
	//the motion start	
	//to point n-th Qs_save position
	n = 0;
	pQs = Qs_save[0];

	/***************************************************************/
	/********************* for home to task1 ***********************/
	/***************************************************************/
	

	double* pd1;
	pd1 = htot1_d_save[0];

	// Calculate joint values from home to task1
	n = MoveP(n, pQs,pd1,IT,FT,s_time);		//n = 41

	//for check element
	/*
	cout<<endl;
	cout<<n<<endl;
	cout<<endl;
	for(int h =0; h<41;h++){
		cout<<h<<" : ";
		for(int k = 0; k<6;k++)
  		cout<<Qs_save[h][k]<<" ";
  		cout<<endl;
		}
	*/

	/****************************************************************/
	/********************* for task1 to task2 ***********************/ 
	/****************************************************************/
	// Calculate joint values from task1 to task2
	
	double* pd2;
	pd2 = *t1tot2_d_save;

	n = MoveP(n, pQs,pd2, IT,FT,s_time);

	/*
	//for check element
		cout<<endl;
		cout<<n<<endl;
		cout<<endl;
		for(int h =41; h<81;h++){
			cout<<h<<" : ";
			for(int k = 0; k<6;k++)
  				cout<<Qs_save[h][k]<<" ";
  		cout<<endl;}
	*/


	/****************************************************************/
	/********************* for task2 to task3 ***********************/	//Opening the handle
	/****************************************************************/
	// Calculate joint values from task2 to task3
	
	double p2top3_save[24][6] = {0};

	for (int i=0; i<24; i++){
		p2top3_save[i][0]=p2top3[i+1][0]-p2top3[i][0];
		p2top3_save[i][1]=p2top3[i+1][1]-p2top3[i][1];
		p2top3_save[i][2]=p2top3[i+1][2]-p2top3[i][2];
		p2top3_save[i][3]=a2toa3[i+1][0]-a2toa3[i][0];
		p2top3_save[i][4]=a2toa3[i+1][1]-a2toa3[i][1];
		p2top3_save[i][5]=a2toa3[i+1][2]-a2toa3[i][2];
	}

	double* pd3;
	pd3 = *p2top3_save;
	
	n = MoveP(n,pQs,pd3,0,23,1);

	
	/*
	//for check element
		cout<<endl;
		cout<<n<<endl;
		cout<<endl;
		 for(int h =81; h<103;h++){
			cout<<h<<" : ";
			for(int k = 0; k<6;k++)
  			cout<<Qs_save[h][k]<<" ";
  		cout<<endl;
		}
	
	*/


	/****************************************************************/
	/********************* for task3 to task4 ***********************/	//Opening the handle
	/****************************************************************/
	// Calculate joint values from task3 to task4
	
	double p3top4_save[24][6] = {0};

	for (int i=0; i<24; i++){
		p3top4_save[i][0]=p3top4[i+1][0]-p3top4[i][0];
		p3top4_save[i][1]=p3top4[i+1][1]-p3top4[i][1];
		p3top4_save[i][2]=p3top4[i+1][2]-p3top4[i][2];
		p3top4_save[i][3]=a3toa4[i+1][0]-a3toa4[i][0];
		p3top4_save[i][4]=a3toa4[i+1][1]-a3toa4[i][1];
		p3top4_save[i][5]=a3toa4[i+1][2]-a3toa4[i][2];
	}

	double* pd4;
	pd4 = *p3top4_save;

	n = MoveP(n,pQs,pd4,0,23,1);


	/*
	cout<<endl;
	cout<<n<<endl;
	cout<<endl;
	//for check element
	for(int h =103; h<125;h++){
		cout<<h<<" : ";
		for(int k = 0; k<6;k++)
  			cout<<Qs_save[h][k]<<" ";
  		cout<<endl;
		}
	*/


	/****************************************************************/
	/********************* for task4 to task5 ***********************/ 
	/****************************************************************/
	// Calculate joint values from task2 to task3

	double* pd5;
	pd5 = *otoE_d_save;

	n = MoveP(n,pQs,pd5,IT,FT,s_time);
	/*
		cout<<endl;
		cout<<n<<endl;
		cout<<endl;
		//for check element
		for(int h =125; h<165;h++){
			cout<<h<<" : ";
			for(int k = 0; k<6;k++)
  				cout<<Qs_save[h][k]<<" ";
  			cout<<endl;
		}
	*/



	/****************************************************************/
	/********************* for task5 to task6 ***********************/ 
	/****************************************************************/
	// Calculate joint values from task2 to task3
	

	double* pd6;
	pd6 = *EtoB_d_save;

	n = MoveP(n,pQs,pd6,IT,FT,s_time);

/*
		
		cout<<endl;
		cout<<n<<endl;
		cout<<endl;
		//for check element
		for(int h =165; h<205;h++){
			cout<<h<<" : ";
			for(int k = 0; k<6;k++)
  				cout<<Qs_save[h][k]<<" ";
  			cout<<endl;
		}
	
*/


	/****************************************************************/
	/********************* for task6 to task7 ***********************/ 
	/****************************************************************/
	// Calculate joint values from task2 to task3

	double* pd7;
	pd7 = *Btoh_d_save;

	n = MoveP(n,pQs,pd7,IT,FT,s_time);

	/*
		cout<<endl;
		cout<<n<<endl;
		cout<<endl;
		//for check element
		for(int h =205; h<245;h++){
			cout<<h<<" : ";
			for(int k = 0; k<6;k++)
  				cout<<Qs_save[h][k]<<" ";
  			cout<<endl;
		}
	*/
	



	
	
	
	
/*****************************************************************************************/

	//to save final consequent 
	float Qs_final[max_motion][6];

	//for transe Qs_save element to mm radian 
 	for(int h =0; h<245;h++)
 		for(int k = 0; k<6;k++)
  			Qs_final[h][k] = Qs_save[h][k]*180/pi*1000;


	//for task123
	for(int t =0; t<103;t++)
		for(int k = 0; k<6;k++){
		*(pA1+t*6+k) = Qs_final[t][k];
		//*(pA+t*6+1) = Qs_final[t][1];
		//*(pA+t*6+2) = Qs_final[t][2];
		//*(pA+t*6+3) = Qs_final[t][3];
		//*(pA+t*6+4) = Qs_final[t][4];
		//*(pA+t*6+5) = Qs_final[t][5];
	}
	//for task4
	for(int t =0; t<22;t++)
		for(int k = 0; k<6;k++)
		*(pA2+t*6+k) = Qs_final[t+103][k];
	//for task5
	for(int t =0; t<40;t++)
		for(int k = 0; k<6;k++)
		*(pA3+t*6+k) = Qs_final[t+125][k];
	//for task6
	for(int t =0; t<40;t++)
		for(int k = 0; k<6;k++)
		*(pA4+t*6+k) = Qs_final[t+165][k];
	//for task7
	for(int t =0; t<40;t++)
		for(int k = 0; k<6;k++)
		*(pA5+t*6+k) = Qs_final[t+205][k];


	//
	////to check Qs_final
 //	for(int h =0; h<245;h++){
	//	cout<<h<<" : ";
 //		for(int k = 0; k<6;k++)
 // 			  cout<<Qs_final[h][k]<<" ";
 // 		cout<<endl;
	//	}
	//


	// to output consequence
	/*
 	ofstream out;
 	out.open("output6.txt");
 		for(int h =0; h<245;h++){
 			//for(int k = 0; k<6;k++)
 			out<<Qs_save[h][5];;
			out<<endl;
		}
 	out.close();
	*/
	

	system("pause");
}