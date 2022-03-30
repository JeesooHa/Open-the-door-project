#include "stdafx.h"
#include <iostream>
#include "dsp402.h"
#include <math.h>
#include "head.h"

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



//extern int node_number; //global variable defined in stdafx.h
using namespace std;
using namespace nDSP402;

//const double pi  =3.141592653589793238463;
//------------------------------------------------------------------------


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMemoryMap;
	LPBYTE pMemoryMap;
	


	cout << "start\n"; cout.flush();
	try
	{
		int StopOrNot;
		// Create base CAN object to communicate via CAN-bus:
		cCANBase can;
		// Simple STL list of node-ids
		tNodeList nodes;	
		tPDOList pdos;

		//while(StopOrNot!='y')
		for (int atman_count=0; atman_count<2;atman_count++)
		{
			

			//-----
			// Select the CANopen nodes to operate on
			GetNodes( nodes );

			// get input from shared memory in form of simple true or false

			/*int n_angle[41][6]={{	-71001.06055	,	-5277.177199	,	84586.57872	,	90772.34562	,	70606.7978	,	975.4922049	}	,
{	-70073.2911	,	-2668.79809	,	87470.25705	,	90871.34954	,	69722.92691	,	670.9750116	}	,
{	-69092.65337	,	-180.6575538	,	90091.34516	,	90927.12652	,	68896.70607	,	515.8313828	}	,
{	-68079.41549	,	2141.957643	,	92422.3643	,	90937.93596	,	68208.57216	,	494.8856165	}	,
{	-67056.29941	,	4261.461429	,	94450.96164	,	90907.04546	,	67730.78753	,	584.3771178	}	,
{	-66046.34551	,	6146.782281	,	96175.51757	,	90843.20216	,	67525.81027	,	754.6143378	}	,
{	-65069.89512	,	7772.523213	,	97601.49718	,	90759.77636	,	67643.70394	,	972.6952681	}	,
{	-64141.055	,	9118.388091	,	98738.49105	,	90672.69947	,	68119.02948	,	1205.37732	}	,
{	-63264.20862	,	10168.8732	,	99597.93442	,	90597.61043	,	68967.86279	,	1422.004498	}	,
{	-62431.31117	,	10913.25867	,	100191.5011	,	90546.88423	,	70185.71364	,	1597.206557	}	,
{	-61620.7528	,	11345.90602	,	100530.146	,	90527.32553	,	71747.10978	,	1712.970875	}	,
{	-60798.39675	,	11466.78317	,	100623.7314	,	90539.16987	,	73607.38652	,	1759.738612	}	,
{	-59920.95198	,	11282.03361	,	100481.1335	,	90576.6467	,	75706.76821	,	1736.392455	}	,
{	-58941.2103	,	10804.34103	,	100110.695	,	90629.86589	,	77976.23329	,	1649.277855	}	,
{	-57814.09582	,	10052.85971	,	99520.879	,	90687.40487	,	80344.12648	,	1510.591992	}	,
{	-56502.21677	,	9052.592556	,	98720.98492	,	90738.84963	,	82742.26138	,	1336.511232	}	,
{	-54979.81482	,	7833.258468	,	97721.8117	,	90776.69191	,	85110.4621	,	1145.34087	}	,
{	-53234.57051	,	6427.828947	,	96536.19051	,	90797.28946	,	87399.02504	,	955.8480226	}	,
{	-51267.36045	,	4870.977984	,	95179.34707	,	90800.90465	,	89569.18119	,	785.8463821	}	,
{	-49090.50794	,	3197.669938	,	93669.08897	,	90791.042	,	91592.06372	,	651.0552443	}	,
{	-46725.21789	,	1442.035613	,	92025.83813	,	90773.38015	,	93446.83584	,	564.2366776	}	,
{	-44198.79732	,	-363.4025323	,	90272.54116	,	90754.56988	,	95118.56267	,	534.6035294	}	,
{	-41542.06312	,	-2188.169305	,	88434.49235	,	90741.09783	,	96596.23123	,	567.479441	}	,
{	-38787.13767	,	-4004.000671	,	86539.09599	,	90738.33837	,	97871.13656	,	664.1825487	}	,
{	-35965.68537	,	-5784.815835	,	84615.58064	,	90749.85523	,	98935.70895	,	822.1017048	}	,
{	-33107.55926	,	-7506.58589	,	82694.65946	,	90776.97622	,	99782.76875	,	1034.939526	}	,
{	-30239.79158	,	-9147.183169	,	80808.11064	,	90818.6445	,	100405.1514	,	1293.108225	}	,
{	-27385.85806	,	-10686.28778	,	78988.23338	,	90871.53968	,	100795.6298	,	1584.277291	}	,
{	-24565.15661	,	-12105.41945	,	77267.11955	,	90930.45437	,	100947.0644	,	1894.081131	}	,
{	-21792.65793	,	-13388.15379	,	75675.67408	,	90988.89886	,	100852.7211	,	2206.994819	}	,
{	-19078.70405	,	-14520.57095	,	74242.32286	,	91039.88579	,	100506.7167	,	2507.373693	}	,
{	-16428.94856	,	-15491.96714	,	72991.37123	,	91076.81793	,	99904.5735	,	2780.626921	}	,
{	-13844.44961	,	-16295.83444	,	71941.02264	,	91094.3694	,	99043.89372	,	3014.45934	}	,
{	-11321.94493	,	-16931.07909	,	71101.13457	,	91089.22467	,	97925.19152	,	3200.077462	}	,
{	-8854.354917	,	-17403.40795	,	70470.86582	,	91060.53081	,	96552.95157	,	3333.227421	}	,
{	-6431.576693	,	-17726.77419	,	70036.43523	,	91009.94012	,	94937.00089	,	3414.930319	}	,
{	-4041.642335	,	-17924.74701	,	69769.23772	,	90941.17623	,	93094.2869	,	3451.81814	}	,
{	-1672.31208	,	-18031.66735	,	69624.52509	,	90859.14271	,	91051.13725	,	3456.056645	}	,
{	686.8564791	,	-18093.47478	,	69540.74448	,	90768.68752	,	88846.03021	,	3444.962652	}	,
{	3041.998043	,	-18168.14207	,	69439.44384	,	90673.2139	,	86532.81719	,	3440.561287	}	,
{	5392.166014	,	-18325.73666	,	69225.41099	,	90573.36231	,	84184.19667	,	3469.459248	}	

};
*/

			//task1~3
			int task123[103][6] = {0};
			int *p123;
			//task4
			int task4[22][6] = {0};
			int *p4;
			//task5
			int task5[40][6] = {0};
			int *p5;
			//task6
			int task6[40][6] = {0};
			int *p6;
			//task7
			int task7[40][6] = {0};
			int *p7;

			p123 = *task123;
			p4 = *task4;
			p5 = *task5;
			p6 = *task6;
			p7 = *task7;

			IPM_MovePos(can, pdos, task123[0], 1500, period_s);

			int map = 0;
			while (true)//Staying for receive data
			{
				hMemoryMap = OpenFileMapping(FILE_MAP_READ, NULL, _T("fork_server"));
				if (!hMemoryMap)
				{
					if (map == 0) { printf("공유메모리를 생성 실패\n"); map++; }
					//return FALSE;
					continue;
				}

				pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_READ, 0, 0, 0);
				if (!pMemoryMap)
				{
					CloseHandle(hMemoryMap);
					if (map == 0) { printf("공유메모리를 열수 없습니다\n"); map++; }
					//return FALSE;
					continue;
				}

				Sspace *pSpace = (Sspace*)pMemoryMap;
				
				cout<<pSpace->toM<<endl;
				if (pSpace->toM) break;
			}

			Sspace *pSpace = (Sspace*)pMemoryMap;

			cout << "I'll show it " <<endl;

			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);



			trajectory(p123,p4,p5,p6,p7);


	//for(int h =0; h<40+2;h++){
	//	cout<<h<<" : ";
 //		for(int k = 0; k<6;k++)
 // 			  cout<<task5[h][k]<<" ";
 // 		cout<<endl;
	//	}


			int n_velocity=200;
// 			cout<<"please enter the speed of rotation in mdegrees/sec"<<endl;
// 			cin>>n_velocity;

			// Create and initialize a simple STL list of CANopen Process Data
			// Object PDO handling objects:
			
			pdos = InitModeOfOperation( can, nodes, MOO_INTERPOLATED_POSITION_MODE, period_s );

			// Enable selected nodes:
			EnterOperationEnabled( can, pdos, period_s );
			//cerr << "EnterOperationEnabled finished\n"; cerr.flush();

			//---
			// For 10s just sync and sleep for SYNC jitter measurement
			if (0)
			{
				int move_steps = int( 10.0 / period_s );

				//double dp;
				cSimpleTime t;
				for (; move_steps > 0; move_steps--)
				{
					t.StoreNow();
					SyncAll(can, pdos);
					SleepSec(period_s);
				}

				// Disable selected axes:
				LeaveOperationEnabled( can, pdos, period_s );

				exit(1);
			}
			//---

			// Activate controllers but keep all selected axes in current
			// position for 5s:
			tPDOList::iterator pdo;

			cout << "Activating controller for all nodes but stay in position for 5s\n";
			for (pdo = pdos.begin(); pdo != pdos.end(); ++pdo)
			{
				(*pdo)->rpdo_data.control_word.bit.Operation_Mode_Specific0 = 1;
				(*pdo)->rpdo_data.control_word.bit.Halt = 0;
			}

			IPM_KeepSynced( can, pdos, 5.0, period_s );

			//--------------------------Waiting for receiving a signal--------------------------//


			//int d[200];
			
			/////////////////////////////
			// output->  work is done  //
			//       go forward        // 
			/////////////////////////////

			//1.done open then go----------------------------------------------------------------------------------------------

			for (int j = 0; j<103; j++){

				//d[j]=((7*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2)))/20 + (8*sin(double(n_angle[j][4]*pi/180000))*(sin(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][3]*pi/180000)) - cos(double(n_angle[j][3]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) + cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*cos(double(n_angle[j][2]*pi/180000 - pi/2)))))/25 - (8*cos(double(n_angle[j][4]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) - cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2))))/25 - (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)))/200 + (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2)))/200);
				IPM_MovePos(can, pdos, task123[j], 1500, period_s);

			}

			hMemoryMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Sspace), _T("fork_server"));
			if (!hMemoryMap)
			{
				printf("공유메모리 생성 실패\n");
				return FALSE;
			}

			pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (!pMemoryMap)
			{
				CloseHandle(hMemoryMap);
				printf("공유메모리를 열수 없습니다\n");
				return FALSE;
			}


			pSpace->toA = true;
			pSpace->toM = false;

			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);


			 map = 0;
			while (true)//Staying for receive data
			{
				hMemoryMap = OpenFileMapping(FILE_MAP_READ, NULL, _T("fork_server"));
				if (!hMemoryMap)
				{
					if (map == 0) { printf("공유메모리를 생성 실패\n"); map++; }
					//return FALSE;
					continue;
				}

				pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_READ, 0, 0, 0);
				if (!pMemoryMap)
				{
					CloseHandle(hMemoryMap);
					if (map == 0) { printf("공유메모리를 열수 없습니다\n"); map++; }
					//return FALSE;
					continue;
				}

				Sspace *pSpace = (Sspace*)pMemoryMap;

				if (pSpace->toM) break;
			}


			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);

			//2.up the hand-------------------------------------------------------------------------------------------------------

			//////////////////////////////
			// output->door is unlocked //
			//////////////////////////////      

			for (int j = 0; j<22; j++){

				//d[j]=((7*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2)))/20 + (8*sin(double(n_angle[j][4]*pi/180000))*(sin(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][3]*pi/180000)) - cos(double(n_angle[j][3]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) + cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*cos(double(n_angle[j][2]*pi/180000 - pi/2)))))/25 - (8*cos(double(n_angle[j][4]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) - cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2))))/25 - (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)))/200 + (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2)))/200);
				IPM_MovePos(can, pdos, task4[j], 1000, period_s);

			}

			
			hMemoryMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Sspace), _T("fork_server"));
			if (!hMemoryMap)
			{
				printf("공유메모리 생성 실패\n");
				return FALSE;
			}

			pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (!pMemoryMap)
			{
				CloseHandle(hMemoryMap);
				printf("공유메모리를 열수 없습니다\n");
				return FALSE;
			}


			pSpace->toA = true;
			pSpace->toM = false;
			cout << "I'm done" << endl;

			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);

		

			 map = 0;
			while (true)//Staying for receive data
			{
				hMemoryMap = OpenFileMapping(FILE_MAP_READ, NULL, _T("fork_server"));
				if (!hMemoryMap)
				{
					if (map == 0) { printf("공유메모리를 생성 실패\n"); map++; }
					//return FALSE;
					continue;
				}

				pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_READ, 0, 0, 0);
				if (!pMemoryMap)
				{
					CloseHandle(hMemoryMap);
					if (map == 0) { printf("공유메모리를 열수 없습니다\n"); map++; }
					//return FALSE;
					continue;
				}

				Sspace *pSpace = (Sspace*)pMemoryMap;

				if (pSpace->toM) break;
			}


			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);

			//3. swing the door---------------------------------------------------------------------------------------------------

			for (int j=0; j<40; j++){

				//d[j]=((7*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2)))/20 + (8*sin(double(n_angle[j][4]*pi/180000))*(sin(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][3]*pi/180000)) - cos(double(n_angle[j][3]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) + cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*cos(double(n_angle[j][2]*pi/180000 - pi/2)))))/25 - (8*cos(double(n_angle[j][4]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) - cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2))))/25 - (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)))/200 + (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2)))/200);
	 			IPM_MovePos(can, pdos, task5[j], 400, period_s );
				
			}

			map = 0;
			while (true)//Staying for receive data
			{
				hMemoryMap = OpenFileMapping(FILE_MAP_READ, NULL, _T("fork_server"));
				if (!hMemoryMap)
				{
					if (map == 0) { printf("공유메모리를 생성 실패\n"); map++; }
					//return FALSE;
					continue;
				}

				pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_READ, 0, 0, 0);
				if (!pMemoryMap)
				{
					CloseHandle(hMemoryMap);
					if (map == 0){ printf("공유메모리를 열수 없습니다\n"); map++; }
					//return FALSE;
					continue;
				}

				Sspace *pSpace = (Sspace*)pMemoryMap;

				cout << pSpace->OK << endl;
				if (!pSpace->OK) break;
			}

			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);

			hMemoryMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Sspace), _T("fork_server"));
			if (!hMemoryMap)
			{
				printf("공유메모리 생성 실패\n");
				return FALSE;
			}

			pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (!pMemoryMap)
			{
				CloseHandle(hMemoryMap);
				printf("공유메모리를 열수 없습니다\n");
				return FALSE;
			}


			pSpace->OK = false;	//Me OK, You OK?
			cout << "Yeah" << endl;

			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);

			cout << "Do I have to go? Ha ah..." << endl;
			for (int j=0; j<40; j++){

				//d[j]=((7*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2)))/20 + (8*sin(double(n_angle[j][4]*pi/180000))*(sin(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][3]*pi/180000)) - cos(double(n_angle[j][3]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) + cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*cos(double(n_angle[j][2]*pi/180000 - pi/2)))))/25 - (8*cos(double(n_angle[j][4]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) - cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2))))/25 - (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)))/200 + (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2)))/200);
	 			IPM_MovePos(can, pdos, task6[j], 2000, period_s );
				
			}


			for (int j=0; j<40; j++){

				//d[j]=((7*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2)))/20 + (8*sin(double(n_angle[j][4]*pi/180000))*(sin(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][3]*pi/180000)) - cos(double(n_angle[j][3]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*sin(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) + cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*cos(double(n_angle[j][2]*pi/180000 - pi/2)))))/25 - (8*cos(double(n_angle[j][4]*pi/180000))*(cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)) - cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2))))/25 - (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][1]*pi/180000 - pi/2))*sin(double(n_angle[j][2]*pi/180000 - pi/2)))/200 + (61*cos(double(n_angle[j][0]*pi/180000))*cos(double(n_angle[j][2]*pi/180000 - pi/2))*sin(double(n_angle[j][1]*pi/180000 - pi/2)))/200);
	 			IPM_MovePos(can, pdos, task7[j], 1000, period_s );
				
			}

			hMemoryMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Sspace), _T("fork_server"));
			if (!hMemoryMap)
			{
				printf("공유메모리 생성 실패\n");
				return FALSE;
			}

			pMemoryMap = (LPBYTE)MapViewOfFile(hMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (!pMemoryMap)
			{
				CloseHandle(hMemoryMap);
				printf("공유메모리를 열수 없습니다\n");
				return FALSE;
			}


			pSpace->OK = false;	//Me OK, You OK?

			if (pMemoryMap) UnmapViewOfFile(pMemoryMap);
			if (hMemoryMap) CloseHandle(hMemoryMap);

			/////////////////////////////
			//output->   move is done  //
			//                         //
			//        go forward       //
			/////////////////////////////
 					
			// Keep controllers active and keep all selected axes in current
			// position for another second:
			

			IPM_KeepSynced( can, pdos, 1.0, period_s );

			// Disable selected axes:
			LeaveOperationEnabled( can, pdos, period_s );
			std::cout << "Popping out the elements in mylist:";
			while (!nodes.empty())
			{
				std::cout << ' ' << nodes.front();
				nodes.pop_front();
			}

			std::cout << "\nFinal size of mylist is " << nodes.size() << '\n';
			//cCANBase::DeleteId( handle, id);
		}
			system("pause");


		

			
	}
	catch (cSDOException const& e)
	{
		cerr << "\nCaught cSDOException: " << e.msg << "\n  Giving up.\n";
		//exit(2);
	}
	catch (cCANBaseException const& e)
	{
		cerr << "\nCaught cCANBaseException: " << e.msg << "\n  Giving up.\n";
		//exit(1);
	}

	system("pause");
	return 0;
}

//"search error is here" to find the errors for id problem
//to remove the can id error, i added canIdDelete inside the function canIdAdd! Not a good way, but almost works(the command is not executed the first time but is executed everytime after that for that given id).
