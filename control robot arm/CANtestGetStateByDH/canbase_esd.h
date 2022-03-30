/*
 * canbase.h
 *
 * Interface of base CAN object to communicate via CAN-bus
 * using ESD USB to CAN converter.
 *
 *  Created on: 2013-06-18
 *      Author: osswald2
 */

#ifndef CANBASE_ESD_H_
#define CANBASE_ESD_H_

//! transmit queue size for CAN frames
#define CAN_ESD_TXQUEUESIZE 32

//! receive queue size for CAN frames
#define CAN_ESD_RXQUEUESIZE 512

//! Timeout to use in ms
#define CAN_ESD_TIMEOUT_MS 1000

#ifdef OSNAME_LINUX
# include <stdint.h>
typedef uint32_t DWORD;
#endif

// on cygwin ntcan.h includes windef.h which defines macros named max/min which the compiler confuses with max/min templates
// but defining NOMINMAX prevents those evil macros from being defined
// The braindead ntcan.h defines typedefs like uin32_t and int32_t without defining
// the __uint32_t_defined or __nt8_t_defined macros needed by stdint.h on linux
// => we have to include ntcan.h first and then do some moc up as if stdint.h had
//    been included...
#define NOMINMAX
#define __uint32_t_defined
#define __int8_t_defined
#include "ntcan.h"


class cCANBase
{
public:
    //! CTOR: open connection to peak driver
    cCANBase(void);

    char const* GetErrorMessage( DWORD dw );

    //! return the last error message as string. The string returned will be overwritten by the next call to the function
    char const* GetLastErrorMessage( void )
    {
        return GetErrorMessage( rc );
    }

    void WriteData( DWORD id, int dlen, unsigned char const* ptr );

    int ReadData( unsigned short id, unsigned char *data );

    int ReadAllPending( void );
	//error is here
    void AddId( unsigned short id );

	void DeleteId( unsigned short id );
private:
    //! the internal handle to the driver
    NTCAN_HANDLE ntcan_handle;   /*  remark: if you get a compiler error here
                                     (e.g. "error: NTCAN_HANDLE does not name a type")
                                     then please consider updating your ESD CAN
                                     driver (and hence ntcan.h). See also the
                                     comment on NTCAN_HANDLE on the beginning
                                     of this file.
     */
    NTCAN_RESULT rc;

    CMSG m_cmsg;
    CMSG m_cmsg_last[2048];
#   define M_CMSG_MSG() m_cmsg
#   define M_CMSG_MSG_LAST( _id ) m_cmsg_last[ _id ]

};
//======================================================================


#endif /* CANBASE_ESD_H_ */
