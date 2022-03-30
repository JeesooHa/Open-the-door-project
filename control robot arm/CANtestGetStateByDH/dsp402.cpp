/*
 * ds402.cpp
 *
 * Some DS402 specific stuff.
 *
 * - This implements some common routines for communicating with CANopen slaves that provide the
 *   "interpolated position" movement mode of the device profile
 *   IEC 61800-7-201 "Adjustable speed electrical power drive systems"
 *                    Part 7-201: "Generic interface and use of profiles for
 *                    power drive systems" Profile type 1 specification
 *   formerly known as CiA DS402 Device Profile for Drives and Motion Control.
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

#include "stdafx.h"
#include <iostream>
//#include "canbase.h" // must be included first when using ESD on cygwin
//
//#include <iostream>
//#include <sstream>
//#include <list>
//#include <stdlib.h>
//
//#include "nmt.h"
//#include "sdo.h"
//#include "sync.h"
//#include "schunkpdo.h"
//#include "dsp402.h"
//#include "helper.h"

#define DEBUG_MEASURE_TIME 0
#if DEBUG_MEASURE_TIME
#include "boost/date_time/posix_time/posix_time.hpp"
#endif

using namespace std;
using namespace nDSP402;


//int node_number; //external variable from main
//------------------------------------------------------------------------

//! \return a PDO objects to communicate with nodes
tPDOList
nDSP402::GetPDOList( cCANBase& can, tNodeList const& node_list )
{
    tNodeList::const_iterator node;
    tPDOList pdo_list;
    for (node = node_list.begin(); node != node_list.end(); ++node)
    {
        DBG( cerr << "Node " << int(*node) << " create PDO receiver/sender:\n" );
        pdo_list.push_back(new cSchunkPDO(can, *node));
    }

    return pdo_list;
}
//------------------------------------------------------------------------

/*!
 * Init a mode for all nodes in \a node_list
 *
 * \param can - base CAN object to communicate via CAN-bus
 * \param node_list - list of node IDs to init communication with
 * \param mode_of_operation - the mode of operation to set
 * \param sync_period_s - the SYNC period in seconds
 * \return A tPDOlist with ptrs to cSchunkPDO objects created with new according to \a node_list
 */
tPDOList
nDSP402::InitModeOfOperation( cCANBase& can, tNodeList const& node_list, eModeOfOperation mode_of_operation, double sync_period_s )
{
    cSDO sdo(can);
    cNMT nmt(can);
    tNodeList::const_iterator node;
    double sdo_wait_time_s = 0.020;

    // TODO: configure PDOs, for now rely on default mapping

    // Configure timing parameters:
    // (These _MUST_ (sorry for shouting) be set and 
    //  the value set _MUST_ match the actual sync period used later on. The
    //  internal fine-interpolator of the SCHUNK module depends on it.)
    unsigned char interpolation_time_period_value = (unsigned char) (sync_period_s * 1000.0);  // number of milliseconds
    char          interpolation_time_period_index = -3;                                  // -3 === unit of above is milliseconds
    for (node = node_list.begin(); node != node_list.end(); ++node)
    {
        DBG( cerr << "Node " << int(*node) << " setting timing parameters:\n" );
        sdo.Download(*node, 0x60c2, 0x01, 1,
                     (unsigned char*) &interpolation_time_period_value);
        SleepSec( sdo_wait_time_s );
        sdo.Download(*node, 0x60c2, 0x02, 1,
                     (unsigned char*) &interpolation_time_period_index);
    }
    SleepSec( sdo_wait_time_s );

    // Adjust sync_timeout_factor (needed on windows due to bad timing behavior:
    //   The value sync_timeout_factor determines how many SYNC periods a SYNC
    //   can be delayed before the module considers a SYNC timeout.
    //   On Windows with its non deterministic scheduling you might need fairly
    //   large number (like 120), depending on the load of the machine.
    //   For (near) real-time systems in a real robot controller this value
    //   should be reduced.
    //   A value of 0 means no timeout, which should be used for testing
    //   purposes only!
    unsigned char sync_timeout_factor = 10;
    for (node = node_list.begin(); node != node_list.end(); ++node)
    {
        DBG( cerr << "Node " << int(*node) << " setting sync_timeout_factor:\n" );
        sdo.Download(*node, 0x200e, 0x00, 1,
                     (unsigned char*) &sync_timeout_factor);
    }
    SleepSec( sdo_wait_time_s );

    // set mode of operation
    for (node = node_list.begin(); node != node_list.end(); ++node)
    {
        DBG( cerr << "Node " << int(*node) << " setting mode of operation:\n" );
        sdo.Download(*node, 0x6060, 0x00, 1,
                     (unsigned char*) &mode_of_operation);
    }
    SleepSec(5 * sdo_wait_time_s);

    // switch to operational with NMT
    for (node = node_list.begin(); node != node_list.end(); ++node)
    {
        DBG( cerr << "Node " << int(*node) << " NMT start:\n" );
        nmt.Start(*node);
    }
    SleepSec(sdo_wait_time_s);

    return GetPDOList( can, node_list );
}
//----------------------------------------------------------------------

/*!
 * Perform one SYNC step:
 * # send all PDOs from \a pdo_list to the CANopen slaves
 *   (=> Download RPDOs (Receive PDO as seen by the CANopen slave))
 * # send CANopen SYNC object
 * # receive all pending PDOs from the CANopen slaves into \a pdo_list
 *   (=> Upload TPDOs (Transmit PDO as seen by the CANopen slave))
 *   Will not block if not enough TPDOs are available!
 *
 * @param can - base CAN object to communicate via CAN-bus
 * @param pdo_list - tPDOlist with ptrs to cSchunkPDO objects
 * @param force_upload - flag, if true then the upload of the TPDO data will be fored. This might block the process!
 */
void
nDSP402::SyncAll( cCANBase& can, tPDOList& pdo_list, bool force_upload )
{
    tPDOList::iterator pdo;

    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        //
        (*pdo)->Download();
    }
	
    SendSync(can);
	
    // We would have to do busy waiting here to read all TPDOs, since
    // the "PCAN-Light" API from PEAK does not offer any timeout or event
    // mechanism.
    // (The newer "PCAN-Basic" API does offer events but requires
    //  threads to handle these, which is over the top of this
    //  demo program. And PCAN-Basic seems to be available for Windows only)
    // This might block the process for an arbitrary long time which leads to
    // timeouts in the CANopen modules.
    //
    // Therefore we just read everything that is available right now without
    // blocking.
#if DEBUG_MEASURE_TIME
    boost::posix_time::ptime t1(boost::posix_time::microsec_clock::local_time());
#endif
    can.ReadAllPending();
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        // try to upload:
        if ( force_upload )
            while ( (*pdo)->Upload() == 0 )
                ;
        else
            (*pdo)->Upload();
		
        if ((*pdo)->tpdo_data.status_word.bit.Fault)
        {
            stringstream msg;
            msg << "Node " << int((*pdo)->GetNode()) << " reported fault!\n"
                << "  " << GetErrorInfo( can, (*pdo)->GetNode(), "  " );
			cout<<"error no 1"<<endl;
            throw cCANBaseException(msg.str());
        }
    }
#if DEBUG_MEASURE_TIME
    boost::posix_time::ptime t2(boost::posix_time::microsec_clock::local_time());

    boost::posix_time::time_duration  dt = t2 - t1;
    if ( dt.total_microseconds() > 0 )
        cerr << "Reading TPDOs took " << dt.total_microseconds() << "us\n";
#endif
}
//----------------------------------------------------------------------

/*!
 * Bring all nodes in \a node_list into state "Operation enabled".
 * - Change state of internal state machine of all selected CANopen slaves.
 * - State machine is according to CANopen "device profile 402", i.e.
 *   - IEC 61800-7-201 "Adjustable speed electrical power drive systems"
 *                     Part 7-201: "Generic interface and use of profiles for
 *                     power drive systems" Profile type 1 specification
 *     formerly known as CiA DS402 Device Profile for Drives and Motion Control
 *
 * @param can - base CAN object to communicate via CAN-bus
 * @param pdo_list - tPDOlist with ptrs to cSchunkPDO objects
 * @param sync_period_s - the SYNC period in seconds
 */
void
nDSP402::EnterOperationEnabled( cCANBase& can, tPDOList& pdo_list, double sync_period_s )
{
    tPDOList::iterator pdo;

    cout << "Enabling all selected nodes\n";

    // upload TPDOs at least once to get current position of node
    SendSync(can);
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        while ((*pdo)->Upload() == 0)
            ;
        cout << "Initially node " << int((*pdo)->GetNode()) << " is at pos " << (*pdo)->tpdo_data.position << "\n"; cout.flush();

        // copy current position to desired position
        (*pdo)->rpdo_data.position = (*pdo)->tpdo_data.position;
    }
    SleepSec(sync_period_s);

    // reset halt bit and t15 fault reset, if required
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Halt = 0;

        if ((*pdo)->tpdo_data.status_word.bit.Fault)
        {
            cout << "Node " << int((*pdo)->GetNode()) << " initialy reports error:\n"
                 << GetErrorInfo( can, (*pdo)->GetNode(), "  " ) << "\n";
            (*pdo)->rpdo_data.control_word.bit.Reset_Fault = 1;
        }
    }
    IPM_KeepSynced( can, pdo_list, 3*sync_period_s, sync_period_s );

    // t2 shutdown
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Switch_On = 0;
        (*pdo)->rpdo_data.control_word.bit.Enable_Voltage = 1;
        (*pdo)->rpdo_data.control_word.bit.Quick_Stop = 1;
        (*pdo)->rpdo_data.control_word.bit.Reset_Fault = 0;
    }
    IPM_KeepSynced( can, pdo_list, 3*sync_period_s, sync_period_s );

    // t3 switch on
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Switch_On = 1;
        (*pdo)->rpdo_data.control_word.bit.Enable_Voltage = 1;
        (*pdo)->rpdo_data.control_word.bit.Quick_Stop = 1;
        (*pdo)->rpdo_data.control_word.bit.Enable_Operation = 0;
        (*pdo)->rpdo_data.control_word.bit.Reset_Fault = 0;
    }
    IPM_KeepSynced( can, pdo_list, 3*sync_period_s, sync_period_s );

    // t4 Enable
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Switch_On = 1;
        (*pdo)->rpdo_data.control_word.bit.Enable_Voltage = 1;
        (*pdo)->rpdo_data.control_word.bit.Quick_Stop = 1;
        (*pdo)->rpdo_data.control_word.bit.Enable_Operation = 1;
        (*pdo)->rpdo_data.control_word.bit.Reset_Fault = 0;
    }
    IPM_KeepSynced( can, pdo_list, 3*sync_period_s, sync_period_s );
}
//----------------------------------------------------------------------

/*!
 * Bring all nodes in \a node_list into state "Switch on disabled"
 * - Change state of internal state machine of all selected CANopen slaves.
 * - State machine is according to CANopen "device profile 402", i.e.
 *   - IEC 61800-7-201 "Adjustable speed electrical power drive systems"
 *                     Part 7-201: "Generic interface and use of profiles for
 *                     power drive systems" Profile type 1 specification
 *     formerly known as CiA DS402 Device Profile for Drives and Motion Control
 *
 * @param can - base CAN object to communicate via CAN-bus
 * @param pdo_list - tPDOlist with ptrs to cSchunkPDO objects
 * @param sync_period_s - the SYNC period in seconds
 */
void
nDSP402::LeaveOperationEnabled( cCANBase& can, tPDOList& pdo_list, double sync_period_s )
{
    tPDOList::iterator pdo;

    cout << "Disabling all selected nodes\n";

    // t5 disable operation
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Switch_On = 1;
        (*pdo)->rpdo_data.control_word.bit.Enable_Voltage = 1;
        (*pdo)->rpdo_data.control_word.bit.Quick_Stop = 1;
        (*pdo)->rpdo_data.control_word.bit.Enable_Operation = 0;
        (*pdo)->rpdo_data.control_word.bit.Reset_Fault = 0;
    }
    IPM_KeepSynced( can, pdo_list, 3*sync_period_s, sync_period_s );

    // t6 shutdown
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Switch_On = 0;
        (*pdo)->rpdo_data.control_word.bit.Enable_Voltage = 1;
        (*pdo)->rpdo_data.control_word.bit.Quick_Stop = 1;
        (*pdo)->rpdo_data.control_word.bit.Reset_Fault = 0;
    }
    IPM_KeepSynced( can, pdo_list, 3*sync_period_s, sync_period_s );

    // t7 quick stop
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Enable_Voltage = 1;
        (*pdo)->rpdo_data.control_word.bit.Quick_Stop = 0;
        (*pdo)->rpdo_data.control_word.bit.Reset_Fault = 0;
    }
    IPM_KeepSynced( can, pdo_list, 3*sync_period_s, sync_period_s );
}
//----------------------------------------------------------------------


/*!
 * Move all nodes in \a pdo_list to position \a pos
 * with \a velocity
 *
 * @param can - base CAN object to communicate via CAN-bus
 * @param pdo_list - tPDOlist with ptrs to cSchunkPDO objects
 * @param pos - target position in mille degrees
 * @param velocity - target velocity in milli degrees / second
 * @param sync_period_s - the SYNC period in seconds
 */

//**************************************************************************************************************************************************

//void
//	nDSP402::IPM_MovePosInit( cCANBase& can, tPDOList& pdo_list, int pos, int velocity, double sync_period_s )
//{
//	tPDOList::iterator pdo;
//	tPositionList start_positions;
//	tPositionList::iterator sp;
//	//int node_number;
//	cout << "Moving all nodes to pos " << pos << " mdeg with velocity " << velocity << " mdeg/s\n";
//
//	int delta_pos, max_delta_pos;
//	max_delta_pos = 0;
//	for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
//	{
//		// use the previously calculated last target position as new start position:
//		delta_pos = abs(pos - (*pdo)->rpdo_data.position);
//		if (delta_pos > max_delta_pos)
//			max_delta_pos = delta_pos;
//		start_positions.push_back( (*pdo)->rpdo_data.position );
//	}
//	cout<<"reached 1"<<endl;
//	int move_steps = int( (double(max_delta_pos) / double(velocity)) / sync_period_s);
//	int i;
//	for ( i=0; i<move_steps; i++ )
//	{
//		DBG( cerr << "rpdo.pos = " );
//		for ( pdo = pdo_list.begin(), sp = start_positions.begin();
//			pdo != pdo_list.end();
//			++pdo, ++sp )
//		{
//			(*pdo)->rpdo_data.position = int( *sp + ( pos - *sp ) * double(i) / double(move_steps) );
//
//			DBG( cerr << (*pdo)->rpdo_data.position << " " );
//			(*pdo)->rpdo_data.control_word.bit.Operation_Mode_Specific0 = 1;
//			(*pdo)->rpdo_data.control_word.bit.Halt = 0;
//		}
//		DBG( cerr << "\n" );
//		SyncAll(can, pdo_list, false ); // Sync but do not force upload
//		SleepSec(sync_period_s);
//	}
//	cout<<"reached 2"<<endl;
//}


//**************************************************************************************************************************************************



void
nDSP402::IPM_MovePos( cCANBase& can, tPDOList& pdo_list, int pos[], int velocity, double sync_period_s )
{
    tPDOList::iterator pdo;
    tPositionList start_positions;
    tPositionList::iterator sp;
	
//	cout<<"the node number is "<<  node_number <<endl;
    cout << "Moving all nodes to pos " << pos << " mdeg with velocity " << velocity << " mdeg/s\n";

	//cout<<"enter the positions of every node"<<endl;
// 	for(int k=0; k<6; k++)
// 	{
// 		/*cin>>position[k];*/
// 	
// 	}

    int delta_pos, max_delta_pos;
    max_delta_pos = 0;
	int k=0;
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
         //use the previously calculated last target position as new start position:
        delta_pos = abs(pos[k] - (*pdo)->rpdo_data.position);
		//cout<<"the value of delta_pos"<<abs(pos[k] - (*pdo)->rpdo_data.position)<<endl;   // remove this
        if (delta_pos > max_delta_pos)
            max_delta_pos = delta_pos;
        start_positions.push_back( (*pdo)->rpdo_data.position );
		//cout<<"the value of data.pos"<<abs(pos[k] - (*pdo)->rpdo_data.position)<<endl;   // remove this
		k++;
    }
	
    int move_steps = int( (double(max_delta_pos) / double(velocity)) / sync_period_s);
    int i;
    for ( i=0; i<move_steps; i++ )
    {
		k=0;
        DBG( cerr << "rpdo.pos = " );
        for ( pdo = pdo_list.begin(), sp = start_positions.begin();   pdo != pdo_list.end();   ++pdo, ++sp )
        {
			
            (*pdo)->rpdo_data.position = int( *sp + ( pos[k] - *sp ) * double(i) / double(move_steps) );
			//cout<<"the value of new data.position"<<abs(pos[k] - (*pdo)->rpdo_data.position)<<endl;   // remove this	
            DBG( cerr << (*pdo)->rpdo_data.position << " " );
            (*pdo)->rpdo_data.control_word.bit.Operation_Mode_Specific0 = 1;
            (*pdo)->rpdo_data.control_word.bit.Halt = 0;
			k++;
        }
		
        DBG( cerr << "\n" );
		
        SyncAll(can, pdo_list, false ); // Sync but do not force upload
		
        SleepSec(sync_period_s);
		
    }
}
//----------------------------------------------------------------------

/*!
 * Keep all nodes corresponding to \a pdo_list in SYNC
 * for \a t_s seconds, but activate controller.
 *
 * (Useful for Powerball since these have to search their sine vector
 * on the very first start)
 *
 * @param can - base CAN object to communicate via CAN-bus
 * @param pdo_list - tPDOlist with ptrs to cSchunkPDO objects
 * @param t_s - time in seconds to stay in position
 * @param sync_period_s - the SYNC period in seconds
*/
void
nDSP402::IPM_KeepSynced( cCANBase& can, tPDOList& pdo_list, double t_s, double sync_period_s )
{
    int move_steps = int( t_s / sync_period_s );
	//std::cout<<"this one is running with t_s"<<t_s<<" and syncperiod "<<sync_period_s<<endl;
    //double dp;
    for (move_steps ; move_steps > 0; move_steps--)
    {
        SyncAll(can, pdo_list, false); // Sync but do not force upload
        SleepSec(sync_period_s);
    }
	
}
//----------------------------------------------------------------------

/*!
 * Perform homing with all nodes corresponding to \a pdo_list.
 * This will block until all homings are finished or an error occurs
 *
 * @param can - base CAN object to communicate via CAN-bus
 * @param pdo_list - tPDOlist with ptrs to cSchunkPDO objects
 * @param t_s - time in seconds to stay in position
 * @param sync_period_s - the SYNC period in seconds
 */
void
nDSP402::HOM_Homing( cCANBase& can, tPDOList& pdo_list, double sync_period_s )
{
    tPDOList::iterator pdo;

    cout << "Perform homing with all selected nodes\n";

    //-----
    // start homing with all axes:
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Operation_Mode_Specific0 = 1;
        (*pdo)->rpdo_data.control_word.bit.Halt = 0;
    }
    //-----

    //-----
    // wait until homing has started for sure (or is already finished)
    int wait_steps = int( 0.5 / sync_period_s );
    if ( wait_steps < 1 )
        wait_steps = 1;

    for (; wait_steps > 0; wait_steps--)
    {
        SyncAll(can, pdo_list);
        SleepSec(sync_period_s);
    }
    //-----


    //-----
    // Wait until homing is attained or error is signaled:
    tPDOList to_check( pdo_list );
    do
    {
        SyncAll(can, pdo_list);
        SleepSec(sync_period_s);

        for (pdo = to_check.begin(); pdo != to_check.end(); )
        {
            if ( (*pdo)->tpdo_data.status_word.bit.Operation_Mode_Specific0 == 1 )
            {
                cout << "Node " << int((*pdo)->GetNode()) << ": homing attained\n";
                to_check.erase( pdo++ );
                continue;
            }

            if (    (*pdo)->tpdo_data.status_word.bit.Fault == 1
                 || (*pdo)->tpdo_data.status_word.bit.Operation_Mode_Specific1 == 1 )
            {
                cout << "Node " << int((*pdo)->GetNode()) << ": signals error\n";
                to_check.erase( pdo++ );
                continue;
            }
            ++pdo;
        }
    } while ( !to_check.empty() );
    //-----

    //-----
    // stop homing with all axes:
    for (pdo = pdo_list.begin(); pdo != pdo_list.end(); ++pdo)
    {
        (*pdo)->rpdo_data.control_word.bit.Operation_Mode_Specific0 = 0;
    }

    SyncAll(can, pdo_list);
    SleepSec(sync_period_s);
    //-----
}
//----------------------------------------------------------------------
