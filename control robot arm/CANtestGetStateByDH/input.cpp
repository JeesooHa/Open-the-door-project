/*
 * get_state.cpp
 *
 * Main program to show status of CANopen axes.
 *
 * - This implements a very basic display of the data sent from some CANopen nodes.
 *
 * \remark
 * - The terms (CANopen-) slave, (CANopen-) node and axis are used
 *   interchangeably here.
 *
 * \warning
 * - This program has _VERY_POOR_ error handling capabilities.
 *   E.g. timeouts due to communication errors/missing or wrongly addressed
 *   nodes might lead to an indefinite "hang" of the program.
 * - It should be used as a very first starting point only.
 * - It _MUST_NOT_ be used in productive environments without modification.
 * - BEFORE YOU RUN THE PROGRAM YOU MUST MAKE SURE THAT THE SELECTED AXES
 *   ARE ABLE TO MOVE IN THE SPECIFIED WAY!
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */
//#include "canbase.h" // must be included first when using ESD on cygwin
//
//#include <iostream>
//#include <stdlib.h>
//#include <assert.h>
//
//#include "input.h"

#include "stdafx.h"


using namespace std;
using namespace nDSP402;

//------------------------------------------------------------------------

/*!
 * Fixed SYNC period in seconds:
 *
 * \remark
 *   On non real-time operating systems (like windows) you can reach a
 *   minimum period of approximately 15ms only.
 *   Linux is usually much better, even on my ancient 500MHz Intel Pentium III
 *   a period of 8ms is not a problem (@ less than 3% CPU load).
 */
//#if defined( OSNAME_LINUX )
//double period_s = 0.008;
//#else
//double period_s = 0.020;
//#endif

//double period_s = 0.05;

double period_s = 0.02;

//------------------------------------------------------------------------
/*!
 * Fill \a nodes with the CANopen node numbers to operate on
 */
void GetNodes( tNodeList& nodes )
{
     cout << "Select nodes to operate on:\n"
               << "  Enter 'd' to use default 3-8\n"
               << "  Enter a single number to add that node number\n"
               << "  Enter 'e' to end input\n";
	 //**********************************************************************
	  int sum (0);	
	 nodes.clear(); //clear this for repetative use (remove previous nodes to create space for new)
	 while (!nodes.empty())
	 {
		sum += nodes.front();
		nodes.pop_front();
	 }
	 std::cout << "total: " << sum << '\n';
	 //************************************************************************
     do
     {
		
//         string input;
//        // cin >> input;
// 		input='d';
//         if ( input == string( "d" ) )
//         {
            //   To work with other/fewer/more default axes just comment/uncomment/change the following lines:
            nodes.push_back( 3 );  // node 3 is usually the first axis in the "foot" of the robot
            nodes.push_back( 4 );  // node 4 is usually the second axis of the robot
            nodes.push_back( 5 );  // node 5 is usually the third axis of the robot
            nodes.push_back( 6 );  // node 6 is usually the fourth axis of the robot
            nodes.push_back( 7 );  // node 7 is usually the fifth axis of the robot
            nodes.push_back( 8 );  // node 8 is usually the sixth and last axis of the robot, just before the tool/gripper
//         }
//         else if ( input == string( "e" ) )
//         {
            break;
//         }
//         else
//         {
//             int n = atoi(input.c_str());
//             assert( n >= 1 );
//             assert( n < 128 );
//             nodes.push_back( n );
//         }
     }
     while (true);

     tNodeList::const_iterator node;
     cout << "OK, accessing nodes: ";
     for (node = nodes.begin(); node != nodes.end(); ++node)
     {
         cout << int(*node) << " ";
     }
     cout << "\n";
}
//------------------------------------------------------------------------
