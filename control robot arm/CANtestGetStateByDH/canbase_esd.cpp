/*
 * canbase_esd.cpp
 *
 * Implementation of base CAN object to communicate via CAN-bus
 * using ESD USB to CAN converter.
 *
 *  Created on: 2013-06-18
 *      Author: osswald2
 */

#include "stdafx.h"

//#include "canbase.h" // must be included first when using ESD on cygwin
//
//#include <iostream>
//#include <sstream>
//#include <assert.h>
//#include <errno.h>
//#include <stdio.h>
//
//#include "helper.h"

/*
   The Linux version of the ESD ntcan.h file differ a little from the
   Windows (and Cygwin) version.
   So define some macros for mapping/defining names to make the code compile
   on Linux systems too.
*/
#ifdef OSNAME_LINUX

// Linux ntcan.h maps its internal error number to std unix error numbers, so include their definitions:
# include <errno.h>


// Old Linux ntcan.h used HANDLE where Windows ntcan uses NTCAN_HANDLE
// so we had to make the following define:
//# define NTCAN_HANDLE HANDLE
// But newer ESD drivers seem to use NTCAN_HANDLE for both Linux and Windows
// and give deprecation warnings on the use of HANDLE

# ifndef NTCAN_BAUD_1000
   // Some Linux ntcan.h do not define any baudrate codes. The following ones were taken from
   // the Windows version in the hope that they are the same for Linux. Lets see...:
#  define NTCAN_BAUD_1000                 0
#  define NTCAN_BAUD_800                 14
#  define NTCAN_BAUD_500                  2
#  define NTCAN_BAUD_250                  4
#  define NTCAN_BAUD_125                  6
#  define NTCAN_BAUD_100                  7
#  define NTCAN_BAUD_50                   9
#  define NTCAN_BAUD_20                  11
#  define NTCAN_BAUD_10                  13
# endif
#endif


using namespace std;

char const*
ESD_strerror( NTCAN_RESULT rc )
{
    switch (rc)
    {
    DEFINE_TO_CASECOMMAND( NTCAN_SUCCESS );
    DEFINE_TO_CASECOMMAND( NTCAN_RX_TIMEOUT );
    DEFINE_TO_CASECOMMAND( NTCAN_TX_TIMEOUT );
    DEFINE_TO_CASECOMMAND( NTCAN_TX_ERROR );
    DEFINE_TO_CASECOMMAND( NTCAN_CONTR_OFF_BUS );
    DEFINE_TO_CASECOMMAND( NTCAN_CONTR_BUSY );
    DEFINE_TO_CASECOMMAND( NTCAN_CONTR_WARN );
    DEFINE_TO_CASECOMMAND( NTCAN_NO_ID_ENABLED );
    DEFINE_TO_CASECOMMAND( NTCAN_ID_ALREADY_ENABLED );
    DEFINE_TO_CASECOMMAND( NTCAN_ID_NOT_ENABLED );

    DEFINE_TO_CASECOMMAND( NTCAN_INVALID_FIRMWARE );
    DEFINE_TO_CASECOMMAND( NTCAN_MESSAGE_LOST );
    DEFINE_TO_CASECOMMAND( NTCAN_INVALID_HARDWARE );

    DEFINE_TO_CASECOMMAND( NTCAN_PENDING_WRITE );
    DEFINE_TO_CASECOMMAND( NTCAN_PENDING_READ );
    DEFINE_TO_CASECOMMAND( NTCAN_INVALID_DRIVER );

    DEFINE_TO_CASECOMMAND( NTCAN_SOCK_CONN_TIMEOUT );
    DEFINE_TO_CASECOMMAND( NTCAN_SOCK_CMD_TIMEOUT );
    DEFINE_TO_CASECOMMAND( NTCAN_SOCK_HOST_NOT_FOUND );

    DEFINE_TO_CASECOMMAND( NTCAN_INVALID_PARAMETER );
    DEFINE_TO_CASECOMMAND( NTCAN_INVALID_HANDLE );
#ifndef OSNAME_LINUX
// these errors are for Windows only ;-)
    DEFINE_TO_CASECOMMAND( NTCAN_IO_INCOMPLETE );
    DEFINE_TO_CASECOMMAND( NTCAN_IO_PENDING );
    DEFINE_TO_CASECOMMAND( NTCAN_HANDLE_FORCED_CLOSE );
    DEFINE_TO_CASECOMMAND( NTCAN_NOT_IMPLEMENTED );
    DEFINE_TO_CASECOMMAND( NTCAN_NOT_SUPPORTED );
#endif
    DEFINE_TO_CASECOMMAND( NTCAN_NET_NOT_FOUND );
    DEFINE_TO_CASECOMMAND( NTCAN_INSUFFICIENT_RESOURCES );

    DEFINE_TO_CASECOMMAND( NTCAN_OPERATION_ABORTED );
    DEFINE_TO_CASECOMMAND( NTCAN_WRONG_DEVICE_STATE );
    default:
        return "unknown";
    }
}
//-----------------------------------------------------------------

unsigned int ESD_BaudrateToBaudrateCode( unsigned long baudrate )
{
    switch (baudrate)
    {
    case 1000000: return NTCAN_BAUD_1000;
    case 800000: return NTCAN_BAUD_800;
    case 500000: return NTCAN_BAUD_500;
    case 250000: return NTCAN_BAUD_250;
    case 125000: return NTCAN_BAUD_125;
    case 100000: return NTCAN_BAUD_100;
    case 50000: return NTCAN_BAUD_50;
    case 20000: return NTCAN_BAUD_20;
    case 10000: return NTCAN_BAUD_10;
    }

    stringstream msg;
    msg << "Invalid baudrate " << baudrate << " for ESD";
    throw cCANBaseException( msg.str() );
}
//----------------------------------------------------------------------

cCANBase::cCANBase( void ):
    ntcan_handle( NTCAN_HANDLE(NTCAN_INVALID_HANDLE) )
{
    cout << "canbase_esd: Opening ESD CAN device on net " << CAN_ESD_NET << " with baudrate " << CAN_BAUDRATE << "\n";
    rc = canOpen( CAN_ESD_NET,
                  0,                           // flags
                  CAN_ESD_TXQUEUESIZE,         // txquesize
                  CAN_ESD_RXQUEUESIZE,         // rxquesize
                  CAN_ESD_TIMEOUT_MS,
                  CAN_ESD_TIMEOUT_MS,
                  &(ntcan_handle) );

    if (rc != NTCAN_SUCCESS)
    {
        // open failed, so ensure that ntcan_handle is invalid
        ntcan_handle = NTCAN_HANDLE(NTCAN_INVALID_HANDLE);
        stringstream msg;
        msg << "Could not open ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
		cout<<"error no 2"<<endl;
        throw cCANBaseException( msg.str() );
    }

    rc = canSetBaudrate( ntcan_handle, ESD_BaudrateToBaudrateCode( CAN_BAUDRATE ) );
    if (rc != NTCAN_SUCCESS)
    {
        stringstream msg;
        msg << "Could not set baudrate to " << CAN_BAUDRATE << " on ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
        throw cCANBaseException( msg.str() );
    }


    // (re)init member data:
    M_CMSG_MSG().len = 0;

    for (int i = 0; i < 2048; ++i)
    {
        M_CMSG_MSG_LAST(i).id = 0;
    }
}
//----------------------------------------------------------------------

char const*
cCANBase::GetErrorMessage( DWORD rc )
{
    static char return_msg[512];

    //snprintf(return_msg, 511, "error 0x%x = %d = \"%s\"", (int) rc, (int) rc,
    //         ESD_strerror( NTCAN_RESULT(rc) ) );
    return return_msg;
}
//----------------------------------------------------------------------

void
cCANBase::WriteData( DWORD id, int dlen, unsigned char const* ptr )
{
    assert( ntcan_handle != NTCAN_HANDLE(NTCAN_INVALID_HANDLE) );
    assert( dlen >= 0);
    assert( dlen <= 8);

    //---------------------
    CMSG cmsg;
    cmsg.id = int32_t(id);
    cmsg.len = dlen;
    for (int i = 0; i < dlen; ++i)
    {
        // prepare message to send:
        cmsg.data[i] = *(ptr++);
    }

    //-----
    // now send the cmsgs and check return values
    int32_t len_cmsgs = 1;
    rc = canWrite( ntcan_handle, &cmsg, &len_cmsgs, NULL );
    if (rc != NTCAN_SUCCESS)
    {
        stringstream msg;
        msg << "Could not write 1 CMSGs with dlen " << dlen << " on ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
        throw cCANBaseException( msg.str() );
    }

    if ( len_cmsgs != 1 )
    {
        stringstream msg;
        msg << "Could only write " << int(len_cmsgs) << "/1 CMSGs with dlen " << dlen << " on ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
        throw cCANBaseException( msg.str() );
    }

    DBG( cerr << "cCANBaseESD::WriteData wrote CAN frame ID:0x" << setfill('0') << setw(3) << hex << cmsg.id << " LEN=" << int(cmsg.len) << " DATA (hex):" << cHexByteString( (char const*) cmsg.data, cmsg.len ) << "\n" );

}
//----------------------------------------------------------------------

int
cCANBase::ReadData( unsigned short id, unsigned char *data )
{
    assert( ntcan_handle != NTCAN_HANDLE(NTCAN_INVALID_HANDLE) );
    assert( id < 2048);

    int bytes_read = 0;

    // message already received before?
    if (M_CMSG_MSG_LAST(id).id == id)
    {
        // yes, so return old data
        for (bytes_read = 0; bytes_read < M_CMSG_MSG_LAST(id).len; ++bytes_read )
            *data++ = M_CMSG_MSG_LAST(id).data[bytes_read];
        DBG( cerr << "cCANBaseESD::ReadData read old CAN frame ID:0x" << hex << M_CMSG_MSG_LAST(id).id << " LEN=" << int(M_CMSG_MSG_LAST(id).len) << " DATA (hex):" << cHexByteString( (char const*) M_CMSG_MSG_LAST(id).data, M_CMSG_MSG_LAST(id).len ) << "\n" );

        M_CMSG_MSG_LAST(id).id = 0; // mark as invalid
        return bytes_read;
    }
    // no, try to receive new data

    while (1)
    {
        M_CMSG_MSG().len = 0;
        // if necessary read one more CMSGs with blocking call
        int32_t len_cmsgs = 1;


        if ( CAN_ESD_TIMEOUT_MS == 0 )
            rc = canTake( ntcan_handle, &(m_cmsg), &len_cmsgs );
        else
            rc = canRead( ntcan_handle, &(m_cmsg), &len_cmsgs, NULL );

        if ( rc == NTCAN_RX_TIMEOUT || len_cmsgs == 0)
        {
            M_CMSG_MSG().len = 0;
            // no error, just no more data available
            DBG( cerr << "cCANBaseESD::ReadData no more data available\n" );
            return bytes_read;
        }

        if (rc != NTCAN_SUCCESS)
        {
            M_CMSG_MSG().len = 0;
            stringstream msg;
            msg << "Could not read CAN message with ID " << int(id) << " from ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
            throw cCANBaseException( msg.str() );
        }

        // check the actual id of the received message:
        if (M_CMSG_MSG().id == id)
        {
            // matches, so finally copy received bytes to user data:
            for (bytes_read = 0; bytes_read < M_CMSG_MSG().len; ++bytes_read)
                *data++ = M_CMSG_MSG().data[bytes_read];
            DBG( cerr << "cCANBaseESD::ReadData read new CAN frame ID:0x" << hex << M_CMSG_MSG().id << " LEN=" << int(M_CMSG_MSG().len) << " DATA (hex):" << cHexByteString( (char const*) M_CMSG_MSG().data, M_CMSG_MSG().len ) << "\n" );
            return bytes_read;
        }

        // does not match, so record message for other callers
        M_CMSG_MSG_LAST(M_CMSG_MSG().id) = M_CMSG_MSG();
        DBG( cerr << "cCANBase::ReadData read other CAN frame ID:0x" << hex << M_CMSG_MSG().id << " LEN=" << int(M_CMSG_MSG().len) << " DATA (hex):" << cHexByteString( (char const*) M_CMSG_MSG().data, M_CMSG_MSG().len ) << "\n" );

    } // end of while(1)
}
//----------------------------------------------------------------------

/*!
 * Read all pending can frames and return the number of frames read.
 * The frames read can then be queried with cCANBase::ReadData()
 * \return Number of frames read
 */
int
cCANBase::ReadAllPending( void )
{
    assert( ntcan_handle != NTCAN_HANDLE(NTCAN_INVALID_HANDLE) );

    int frames_read = 0;

    while (true)
    {
        M_CMSG_MSG().len = 0;

        int32_t len_cmsgs = 1;
        rc = canTake( ntcan_handle, &(m_cmsg), &len_cmsgs );

        //cerr << "rc=" << rc << "\n";
        //-----
        // check the received message:
        if ( rc == NTCAN_RX_TIMEOUT || len_cmsgs == 0)
            break;

        if (rc != NTCAN_SUCCESS)
        {
            M_CMSG_MSG().len = 0;
            stringstream msg;
            msg << "Could not read CAN message from ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
            throw cCANBaseException( msg.str() );
        }

        // Normal frame received, store for later use:
        M_CMSG_MSG_LAST(M_CMSG_MSG().id) = M_CMSG_MSG();
        ++frames_read;
    }
    DBG( cerr << "cCANBaseESD::ReadAllPending read " << frames_read << " frames\n" );
    return frames_read;
}
//----------------------------------------------------------------------

/*!
 * Add can ID \a id to the list of filtered IDs
 *
 * @param id - CAN ID to add to filter. CAN frames with this CAN message ID will be received.
 *
 * \warning
 *   ANOTE: Due to a bug in the Linux version of the PEAK PCAN-Light driver
 *   it seems that you can only install up to 8 separate filters with
 *   CAN_MsgFilter(). Any further call will be rejected with error code -1.
 *   Since we do not know how man nodes we want to address and how the
 *   IDs of the SDOs ond PDOs are distributed we can not rely on the
 *   the filter mechanism of the driver there.
 *   So we keep all IDs enabled on Linux.
 */
void
cCANBase::AddId( unsigned short id )
{
	canIdDelete( ntcan_handle, id );
    rc = canIdAdd( ntcan_handle, id );
    if (rc != NTCAN_SUCCESS)
    {
        stringstream msg;
        msg << "Could not add CAN ID 0x" << hex << id << dec << " on ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
		cout<<"error no 3"<<endl;
		//error is here
        throw cCANBaseException( msg.str() );
    }
}

void
cCANBase::DeleteId( unsigned short id )
{
	rc=canIdDelete( ntcan_handle, id);
	if (rc != NTCAN_SUCCESS)
	{
		stringstream msg;
		msg << "Could not delete CAN ID 0x" << hex << id << dec << " on ESD CAN net " << CAN_ESD_NET << ": " << GetLastErrorMessage();
		throw cCANBaseException( msg.str() );	

	}

}


//----------------------------------------------------------------------
