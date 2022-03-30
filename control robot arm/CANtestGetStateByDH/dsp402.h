/*
 * dsp402.h
 *
 * Same type and data definitions according to CANopen device profile 402, i.e.
 *   IEC 61800-7-201 „Adjustable speed electrical power drive systems ? *                    Part 7-201: Generic interface and use of profiles for
 *                    power drive systems ?Profile type 1 specification
 *   formerly known as CiA DS402 Device Profile for Drives and Motion Control.
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */

#ifndef DSP402_H_
#define DSP402_H_

#include "canbase.h" // must be included first when using ESD on cygwin

#include <list>

//------------------------------------------------------------------------
// Some type definitions for easy access:

//! Simple STL list of node-ids:
typedef std::list<unsigned char> tNodeList;

class cSchunkPDO;
typedef std::list<cSchunkPDO*> tPDOList;
typedef std::list<double> tPositionList;

//------------------------------------------------------------------------

namespace nDSP402
{

    enum eModeOfOperation
    {
        //-1 ... -128 Manufacturer specific modes of operation
        MOO_RESERVED_0  			   = 0,
        MOO_PROFILE_POSITION_MODE      = 1,
        MOO_VELOCITY_MODE   		   = 2,
        MOO_PROFILE_VELOCITY_MODE      = 3,
        MOO_TORQUE_PROFILE_MODE 	   = 4,
        MOO_RESERVED_1  			   = 5,
        MOO_HOMING_MODE 			   = 6,
        MOO_INTERPOLATED_POSITION_MODE = 7,
        MOO_CYCLIC_SYNC_POSITION_MODE  = 8,
        MOO_CYCLIC_SYNC_VELOCITY_MODE  = 9,
        MOO_CYCLIC_SYNC_TORQUE_MODE    = 10
        //11 ... 127 reserved
    };

    //! bit field for DSP 402 6040 controlword,
    struct bControlword
    {
        unsigned short Switch_On				: 1; // 0
        unsigned short Enable_Voltage    		: 1; // 1
        unsigned short Quick_Stop   			: 1; // 2
        unsigned short Enable_Operation 		: 1; // 3
        unsigned short Operation_Mode_Specific0 : 1; // 4
        unsigned short Operation_Mode_Specific1 : 1; // 5
        unsigned short Operation_Mode_Specific2 : 1; // 6
        unsigned short Reset_Fault  			: 1; // 7
        unsigned short Halt 					: 1; // 8
        unsigned short Reserved0				: 1; // 9
        unsigned short Reserved1				: 1; // 10
        unsigned short Manufacturer_Specific0   : 1; // 11
        unsigned short Manufacturer_Specific1   : 1; // 12
        unsigned short Manufacturer_Specific2   : 1; // 13
        unsigned short Manufacturer_Specific3   : 1; // 14
        unsigned short Manufacturer_Specific4   : 1; // 15
    };

    //! data union for access to DSP 402 6040 controlword,
    union uControlword
    {
        bControlword   bit;
        unsigned short all;
    };

    //! bit field for DSP 402 6041 statusword,
    struct bStatusword
    {
        unsigned short Ready_to_Switch_On       : 1; // 0
        unsigned short Switched_On  			: 1; // 1
        unsigned short Operation_Enabled		: 1; // 2
        unsigned short Fault					: 1; // 3
        unsigned short Voltage_Enabled  		: 1; // 4
        unsigned short Quick_Stop   			: 1; // 5
        unsigned short Switch_On_Disabled   	: 1; // 6
        unsigned short Warning  				: 1; // 7
        unsigned short Manufacturer_Specific0   : 1; // 8
        unsigned short Remote   				: 1; // 9
        unsigned short Target_Reached   		: 1; // 10
        unsigned short Internal_Limit_Active	: 1; // 11
        unsigned short Operation_Mode_Specific0 : 1; // 12
        unsigned short Operation_Mode_Specific1 : 1; // 13
        unsigned short Manufacturer_Specific1   : 1; // 14
        unsigned short Manufacturer_Specific2   : 1; // 15
    };

    //! data union for access to DSP 402 6041 statusword,
    union uStatusword
    {
        bStatusword    bit;
        unsigned short all;
    };

    tPDOList GetPDOList( cCANBase& can, tNodeList const& node_list );
    tPDOList InitModeOfOperation( cCANBase& can, tNodeList const& node_list, eModeOfOperation mode_of_operation, double sync_period_s );
    void SyncAll( cCANBase& can, tPDOList& pdo_list, bool force_upload = true );
    void EnterOperationEnabled( cCANBase& can, tPDOList& pdo_list, double sync_period_s );
    void LeaveOperationEnabled( cCANBase& can, tPDOList& pdo_list, double sync_period_s );

    void IPM_MovePos( cCANBase& can, tPDOList& pdo_list, int pos[], int velocity, double sync_period_s );
	void IPM_MovePosInit( cCANBase& can, tPDOList& pdo_list, int pos, int velocity, double sync_period_s );
    void IPM_KeepSynced( cCANBase& can, tPDOList& pdo_list, double t_s, double sync_period_s );

    void HOM_Homing( cCANBase& can, tPDOList& pdo_list, double sync_period_s );

} // end of namespace nDSP402

#endif /* DSP402_H_ */
