/*
 * sdo.cpp
 *
 * Simple, incomplete CANopen SDO (Service Data Object) implementation.
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */

#include "stdafx.h"

//#include "canbase.h" // must be included first when using ESD on cygwin
//
//#include <assert.h>
//#include <iostream>
//#include <sstream>
//
//#include "sdo.h"
//#include "helper.h"

using namespace std;

bool cSDO::first_time = true;
unsigned char cSDO::nodes_added[ NB_NODES ];

char const*
cSDO::SDOErrorToString( unsigned int rc )
{
    switch (rc)
    {
    case 0x00000000:
        return "Error Reset or no Error";
    case 0x05030000:
        return "Toggle bit not alternated";
    case 0x05040000:
        return "SDO protocol timed out";
    case 0x05040001:
        return "Client/server command specifier not valid or unknown";
    case 0x05040002:
        return "Invalid block size (block mode only)";
    case 0x05040003:
        return "Invalid sequence number (block mode only)";
    case 0x05040004:
        return "CRC error (block mode only)";
    case 0x05040005:
        return "Out of memory";
    case 0x06010000:
        return "Unsupported access to an object";
    case 0x06010001:
        return "Attempt to read a write only object";
    case 0x06010002:
        return "Attempt to write a read only object";
    case 0x06020000:
        return "Object does not exist in the object dictionary";
    case 0x06040041:
        return "Object cannot be mapped to the PDO";
    case 0x06040042:
        return "The number and length of the objects to be mapped would exceed PDO length";
    case 0x06040043:
        return "General parameter incompatibility reason";
    case 0x06040047:
        return "General internal incompatibility in the device";
    case 0x06060000:
        return "Access failed due to an hardware error";
    case 0x06070010:
        return "Data type does not match, length of service parameter does not match";
    case 0x06070012:
        return "Data type does not match, length of service parameter too high";
    case 0x06070013:
        return "Data type does not match, length of service parameter too low";
    case 0x06090011:
        return "Sub-index does not exist";
    case 0x06090030:
        return "Value range of parameter exceeded (only for write access)";
    case 0x06090031:
        return "Value of parameter written too high";
    case 0x06090032:
        return "Value of parameter written too low";
    case 0x06090036:
        return "Maximum value is less than minimum value";
    case 0x08000000:
        return "general error";
    case 0x08000020:
        return "Data cannot be transferred or stored to the application";
    case 0x08000021:
        return "Data cannot be transferred or stored to the application because of local control";
    case 0x08000022:
        return "Data cannot be transferred or stored to the application because of the present device state";
    case 0x08000023:
        return "Object dictionary dynamic generation fails or no object dictionary is present (e.g. object dictionary is generated from file and generation fails because of an file error)";
    default:
        return "Unknown error";
    }
}
//-----------------------------------------------------------------

char const*
eSMPErrorCodeToCStr( eSMPErrorCode code )
{
    switch ((int)code)
    {
    DEFINE_TO_CASECOMMAND( ERROR_NONE );
    DEFINE_TO_CASECOMMAND( INFO_TODO );
    DEFINE_TO_CASECOMMAND( INFO_NO_FREE_SPACE );
    DEFINE_TO_CASECOMMAND( INFO_NO_RIGHTS );
    DEFINE_TO_CASECOMMAND( INFO_UNKNOWN_COMMAND );
    DEFINE_TO_CASECOMMAND( INFO_FAILED );
    DEFINE_TO_CASECOMMAND( INFO_NOT_REFERENCED );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_07 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_08 );
    DEFINE_TO_CASECOMMAND( INFO_COMMUNICATION_ERROR );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_DEBUGDATA );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_FINISHED );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_ABORTED );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_PAUSED );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_CONTINUED );
    DEFINE_TO_CASECOMMAND( INFO_SEQUENCE_END );

    DEFINE_TO_CASECOMMAND( INFO_TIMEOUT );
    DEFINE_TO_CASECOMMAND( INFO_UNKNOWN_AXIS_INDEX );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_12 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_13 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_14 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_15 );
    DEFINE_TO_CASECOMMAND( INFO_WRONG_BAUDRATE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_17 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_18 );
    DEFINE_TO_CASECOMMAND( INFO_CHECKSUM );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_1A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_1B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_1C );
    DEFINE_TO_CASECOMMAND( INFO_MESSAGE_LENGTH );
    DEFINE_TO_CASECOMMAND( INFO_WRONG_PARAMETER );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_END );

    DEFINE_TO_CASECOMMAND( INFO_BATTERY_LOW );
    DEFINE_TO_CASECOMMAND( ERROR_BATTERY_DOWN );
    DEFINE_TO_CASECOMMAND( INFO_POSITION_NOT_REACHABLE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_23 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_24 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_25 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_26 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_27 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_28 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_29 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_2A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_2B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_2C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_2D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_2E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_2F );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_30 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_31 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_32 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_33 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_34 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_35 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_36 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_37 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_38 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_39 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_3A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_3B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_3C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_3D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_3E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_3F );

    DEFINE_TO_CASECOMMAND( INFO_TRIGGER );
    DEFINE_TO_CASECOMMAND( INFO_READY );
    DEFINE_TO_CASECOMMAND( INFO_GUI_CONNECTED );
    DEFINE_TO_CASECOMMAND( INFO_GUI_DISCONNECTED );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_CHANGED );
    DEFINE_TO_CASECOMMAND( INFO_PROGRAM_CHANGE_FAILED );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_46 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_47 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_48 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_49 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_4A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_4B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_4C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_4D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_4E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_4F );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_50 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_51 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_52 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_53 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_54 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_55 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_56 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_57 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_58 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_59 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_5A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_5B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_5C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_5D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_5E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_5F );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_60 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_61 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_62 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_63 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_64 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_65 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_66 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_67 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_68 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_69 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_6A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_6B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_6C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_6D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_6E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_6F );

    DEFINE_TO_CASECOMMAND( ERROR_TEMP_LOW );
    DEFINE_TO_CASECOMMAND( ERROR_TEMP_HIGH );
    DEFINE_TO_CASECOMMAND( ERROR_LOGIC_LOW );
    DEFINE_TO_CASECOMMAND( ERROR_LOGIC_HIGH );
    DEFINE_TO_CASECOMMAND( ERROR_MOTOR_VOLTAGE_LOW );
    DEFINE_TO_CASECOMMAND( ERROR_MOTOR_VOLTAGE_HIGH );
    DEFINE_TO_CASECOMMAND( ERROR_CABEL_BREAK );
    DEFINE_TO_CASECOMMAND( ERROR_MOTOR_VOLTAGE_SHORT );
    DEFINE_TO_CASECOMMAND( ERROR_MOTOR_TEMP );
    DEFINE_TO_CASECOMMAND( ERROR_BRAKE_VOLTAGE );
    DEFINE_TO_CASECOMMAND( ERROR_LIFE_SIGN );
    DEFINE_TO_CASECOMMAND( ERROR_CUSTOM_DEFINED );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_7C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_7D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_7E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_7F );

    DEFINE_TO_CASECOMMAND( ERROR_POWER_TEMP );
    DEFINE_TO_CASECOMMAND( ERROR_BRAKE_RESISTOR_TEMP );
    DEFINE_TO_CASECOMMAND( ERROR_OVERSHOOT );
    DEFINE_TO_CASECOMMAND( ERROR_HARDWARE_VERSION );
    DEFINE_TO_CASECOMMAND( ERROR_SOFTWARE_VERSION );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_85 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_86 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_87 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_88 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_89 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_8A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_8B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_8C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_8D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_8E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_8F );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_90 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_91 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_92 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_93 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_94 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_95 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_96 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_97 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_98 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_99 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_9A );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_9B );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_9C );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_9D );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_9E );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_9F );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A0 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A1 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A2 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A3 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A4 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A5 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A6 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A7 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A8 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_A9 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_AA );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_AB );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_AC );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_AD );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_AE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_AF );

    DEFINE_TO_CASECOMMAND( ERROR_PROGRAM_ABORTION );
    DEFINE_TO_CASECOMMAND( ERROR_PROGRAM_RUNTIME );
    DEFINE_TO_CASECOMMAND( ERROR_PROGRAM_INVALID );
    DEFINE_TO_CASECOMMAND( ERROR_PROGRAM_CONFLICT );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_B4 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_B5 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_B6 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_B7 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_B8 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_B9 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_BA );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_BB );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_BC );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_BD );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_BE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_BF );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C0 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C1 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C2 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C3 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C4 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C5 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C6 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C7 );
    DEFINE_TO_CASECOMMAND( ERROR_WRONG_RAMP_TYPE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_C9 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_CA );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_CB );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_CC );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_CD );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_CE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_CF );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_D0 );
    DEFINE_TO_CASECOMMAND( ERROR_WRONG_DIRECTION );
    DEFINE_TO_CASECOMMAND( ERROR_CONFIG_MEMORY );
    DEFINE_TO_CASECOMMAND( ERROR_PROGRAM_MEMORY );
    DEFINE_TO_CASECOMMAND( ERROR_INVALID_PHRASE );
    DEFINE_TO_CASECOMMAND( ERROR_SOFT_LOW );
    DEFINE_TO_CASECOMMAND( ERROR_SOFT_HIGH );
    DEFINE_TO_CASECOMMAND( ERROR_PRESSURE );
    DEFINE_TO_CASECOMMAND( ERROR_SERVICE );
    DEFINE_TO_CASECOMMAND( ERROR_FAST_STOP );
    DEFINE_TO_CASECOMMAND( ERROR_TOW );
    DEFINE_TO_CASECOMMAND( ERROR_VPC3 );
    DEFINE_TO_CASECOMMAND( ERROR_FRAGMENTATION );
    DEFINE_TO_CASECOMMAND( ERROR_COMMUTATION );
    DEFINE_TO_CASECOMMAND( ERROR_CURRENT );
    DEFINE_TO_CASECOMMAND( ERROR_I2T );

    DEFINE_TO_CASECOMMAND( ERROR_INITIALIZE );
    DEFINE_TO_CASECOMMAND( ERROR_INTERNAL );
    DEFINE_TO_CASECOMMAND( ERROR_HARD_LOW );
    DEFINE_TO_CASECOMMAND( ERROR_HARD_HIGH );
    DEFINE_TO_CASECOMMAND( ERROR_TOO_FAST );
    DEFINE_TO_CASECOMMAND( ERROR_POS_SYSTEM );
    DEFINE_TO_CASECOMMAND( ERROR_ANALOG_INPUT );
    DEFINE_TO_CASECOMMAND( ERROR_FLASH_MEMORY );
    DEFINE_TO_CASECOMMAND( ERROR_LIMITER_ACTIVE );
    DEFINE_TO_CASECOMMAND( ERROR_STOP_CMD_TIMEOUT );
    DEFINE_TO_CASECOMMAND( ERROR_UNEXPECTED_STOP_DURING_PHRASE );
    DEFINE_TO_CASECOMMAND( ERROR_RESOLVER_CHECK_FAILED );
    DEFINE_TO_CASECOMMAND( ERROR_MATH );
    DEFINE_TO_CASECOMMAND( ERROR_LEAVE_MASTER );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_EE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_EF );

    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F0 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F1 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F2 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F3 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F4 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F5 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F6 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F7 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F8 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_F9 );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_FA );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_FB );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_FC );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_FD );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_FE );
    DEFINE_TO_CASECOMMAND( UNKNOWN_ERROR_FF );
    default:
        return "unknown";
    }
}
//-----------------------------------------------------------------

void
cSDO::Download( unsigned char node, unsigned short index,
                unsigned char subindex, unsigned char len,
                unsigned char const* usrdata )
{
    assert( len <= 4 ); // only expedited transfer supported so far
    unsigned char data[8];
    switch (len)
    {
    case 1:
        data[0] = SDO_SEG_REQ_INIT_DOWNLOAD_1BYTE;
        break;
    case 2:
        data[0] = SDO_SEG_REQ_INIT_DOWNLOAD_2BYTE;
        break;
    case 3:
        data[0] = SDO_SEG_REQ_INIT_DOWNLOAD_3BYTE;
        break;
    case 4:
        data[0] = SDO_SEG_REQ_INIT_DOWNLOAD_4BYTE;
        break;
    default:
        assert( "unexpected case" );
        break;
    }
    data[1] = index & 0xff;
    data[2] = index >> 8;
    data[3] = subindex;
    for (int i = 0; i < 4; ++i)
        if (i < len)
            data[4 + i] = usrdata[i];
        else
            data[4 + i] = 0;

    // we have to read the response from the node
    if ( ! nodes_added[ node ] )
    {
        can.AddId( SDOTX_ID+node );
        nodes_added[node] = true;
    }

    can.WriteData(SDORX_ID + node, 8, data);

    // We have to do busy waiting here since the "PCAN-Light" API from
    // PEAK does not offer any timeout or event mechanism.
    // (The newer "PCAN-Basic" API does offer events but requires
    //  threads to handle these, which is over the top of this
    //  demo program.)
    int dlen;
    int n;
    for ( n=0; n<100; ++n )
    {
        dlen = can.ReadData( SDOTX_ID+node, data );
        if ( dlen == 8 )
            break;
        SleepSec( 0.010 );

        // TODO: In case you want to read SDO responses while _actively_
        //       controlling the nodes you have to replace the simple
        //       SleepSec() above with some proper handling of the SYNC stuff
        //       in order to prevent timeouts within the nodes!
    }
    //cout << "sdo download tried reading " << n << " times\n";

    if ( dlen != 8 )
    {
        std::stringstream msg;
        msg << "cSDO.Download: Unexpected length " << dlen << " of SDO response. Expected 8.";
        throw cSDOException( msg.str() );
    }


    int rdindex = data[1] + (data[2]<<8);
    int rdsubindex = data[3];
    if (rdindex != index || rdsubindex != subindex)
    {
        std::stringstream msg;
        msg << "cSDO.Download: Invalid index/subindex, expected 0x" << std::hex << index << "/" << int(subindex) << ", got 0x" << std::hex << rdindex << "/" << int(rdsubindex);
        throw cSDOException( msg.str() );
    }

    if ( data[0] == SDO_SEG_ABORT_TRANSFER )
    {
        std::stringstream msg;
        msg << "cSDO.Download: SDO transfer aborted: error code " << std::hex << (*((unsigned int*) &(data[4])));
        throw cSDOException( msg.str() );
    }
    else if ( data[0] != SDO_SEG_RES_INIT_DOWNLOAD )
    {
        std::stringstream msg;
        msg << "cSDO.Download: Invalid SDO response, got 0x" << std::hex << int(data[0]) << " expected 0x"  << std::hex << int(SDO_SEG_RES_INIT_DOWNLOAD);
        throw cSDOException( msg.str() );
    }
}

void
cSDO::Upload( unsigned char node, unsigned short index,
              unsigned char subindex, unsigned char len,
              unsigned char* usrdata )
{
    assert( len <= 4 ); // only expedited transfer supported so far
    assert( node < NB_NODES );

    if ( ! nodes_added[ node ] )
    {
        can.AddId( SDOTX_ID+node );
        nodes_added[node] = true;
    }

    unsigned char data[8] = { SDO_SEG_REQ_INIT_UPLOAD, (unsigned char)( index & 0xFF ), (unsigned char)( (index & 0xFF00)>>8 ), subindex, 0,0,0,0 };

    int dlen;
    can.WriteData( SDORX_ID+node, 8, data );

    // We have to do busy waiting here since the "PCAN-Light" API from
    // PEAK does not offer any timeout or event mechanism.
    // (The newer "PCAN-Basic" API does offer events but requires
    //  threads to handle these, which is over the top of this
    //  demo program.)
    int n;
    for ( n=0; n<100; ++n )
    {
        dlen = can.ReadData( SDOTX_ID+node, data );
        if ( dlen == 8 )
            break;
        SleepSec( 0.010 );

        // TODO: In case you want to read SDO responses while _actively_
        //       controlling the nodes you have to replace the simple
        //       SleepSec() above with some proper handling of the SYNC stuff
        //       in order to prevent timeouts within the nodes!
    }
    //cout << "sdo upload tried reading " << n << " times\n";

    if ( dlen != 8 )
    {
        std::stringstream msg;
        msg << "cSDO.Upload: Unexpected length " << dlen << " of SDO response. Expected 8.";
        throw cSDOException( msg.str() );
    }

    int rdindex = data[1] + (data[2]<<8);
    int rdsubindex = data[3];
    if (rdindex != index || rdsubindex != subindex)
    {
        std::stringstream msg;
        msg << "cSDO.Upload: Invalid index/subindex, expected 0x" << std::hex << index << "/" << int(subindex) << ", got 0x" << std::hex << rdindex << "/" << int(rdsubindex);
        throw cSDOException( msg.str() );
    }

    if ( data[0] == SDO_SEG_ABORT_TRANSFER )
    {
        std::stringstream msg;
        msg << "cSDO.Upload: SDO transfer aborted: error code " << std::hex << (*((unsigned int*) &(data[4])));
        throw cSDOException( msg.str() );
    }
    else if ( data[0] == SDO_SEG_RES_INIT_UPLOAD_1BYTE )
    {
        if ( len != 1 )
        {
            std::stringstream msg;
            msg << "cSDO.Upload: Unexpected upload length 1, expected " << int(len);
            throw cSDOException( msg.str() );
        }
        *usrdata = data[4];
    }
    else if ( data[0] == SDO_SEG_RES_INIT_UPLOAD_2BYTE )
    {
        if ( len != 2 )
        {
            std::stringstream msg;
            msg << "cSDO.Upload: Unexpected upload length 2, expected " << int(len);
            throw cSDOException( msg.str() );
        }
        *usrdata++ = data[4];
        *usrdata++ = data[5];
    }
    else if ( data[0] == SDO_SEG_RES_INIT_UPLOAD_3BYTE )
    {
        if ( len != 3 )
        {
            std::stringstream msg;
            msg << "cSDO.Upload: Unexpected upload length 3, expected " << int(len);
            throw cSDOException( msg.str() );
        }
        *usrdata++ = data[4];
        *usrdata++ = data[5];
        *usrdata++ = data[6];
    }
    else if ( data[0] == SDO_SEG_RES_INIT_UPLOAD_4BYTE )
    {
        if ( len != 4 )
        {
            std::stringstream msg;
            msg << "cSDO.Upload: Unexpected upload length 4, expected " << int(len);
            throw cSDOException( msg.str() );
        }
        *usrdata++ = data[4];
        *usrdata++ = data[5];
        *usrdata++ = data[6];
        *usrdata++ = data[7];
    }
    else
    {
        std::stringstream msg;
        msg << "cSDO.Upload: Unknown SDO response code 0x" << std::hex << int(data[0]);
        throw cSDOException( msg.str() );
    }
}
