/*
 * get_error_info.cpp
 *
 *  Created on: 14.09.2012
 *      Author: osswald2
 */


#include "stdafx.h"


//#include "canbase.h" // must be included first when using ESD on cygwin
//
////#include <iostream>
//#include <string>
//#include <sstream>
////#include <list>
////#include <stdlib.h>
//
////#include "nmt.h"
//#include "sdo.h"
////#include "sync.h"
////#include "schunkpdo.h"
////#include "dsp402.h"
////#include "helper.h"

using namespace std;

//------------------------------------------------------------------------


/*!
 * Return a string describing the error state of node \a node
 */
string GetErrorInfo( cCANBase& can, unsigned char node, char const* prefix )
{
    unsigned char error_register;
    unsigned int  manufacturer_status_register;

    cSDO sdo(can);

    sdo.Upload( node, 0x1001, 0, sizeof( error_register ), &error_register );

    stringstream result;
    result << "error_register=0x" << hex << int( error_register ) << ", categories:";
    if ( error_register & sdo.EMC_k_1001_GENERIC )
        result << " generic,";
    if ( error_register & sdo.EMC_k_1001_CURRENT)
        result << " current,";
    if ( error_register & sdo.EMC_k_1001_VOLTAGE )
        result << " voltage,";
    if ( error_register & sdo.EMC_k_1001_TEMPERATURE )
        result << " temperature,";
    if ( error_register & sdo.EMC_k_1001_COMMUNICATION )
        result << " communication,";
    if ( error_register & sdo.EMC_k_1001_DEV_PROF_SPEC )
        result << " device profile specific,";
    if ( error_register & sdo.EMC_k_1001_RESERVED )
        result << " reserved,";
    if ( error_register & sdo.EMC_k_1001_MANUFACTURER)
        result << " manufacturer specific";
    result << "\n";

    sdo.Upload( node, 0x1002, 0, sizeof( manufacturer_status_register ), (unsigned char*) &manufacturer_status_register );
    unsigned char code = (manufacturer_status_register & 0x00ff);
    unsigned char classification = ((manufacturer_status_register>>8) & 0x00ff);

    result << prefix << "manufacturer_status_register=0x" << hex << manufacturer_status_register
           << ": code=0x" << hex << int( code ) << " (" << eSMPErrorCodeToCStr( eSMPErrorCode( code ) ) << "),"
           << ", classification=0x" << hex << int( classification ) << dec;
    if ( classification == 0x88 )
        result << " (CMD_ERROR)";
    if ( classification == 0x89 )
        result << " (CMD_WARNING)";
    if ( classification == 0x8a )
        result << " (CMD_INFO)";
    result << "\n";

    float detail;
    sdo.Upload( node, 0x200b, 1, sizeof( detail ), (unsigned char*) &detail );
    unsigned short line;
    sdo.Upload( node, 0x200b, 3, sizeof( line ), (unsigned char*) &line );
    result << "Error detail=" << detail << " line=" << line << "\n";


    return result.str();
}
//----------------------------------------------------------------------

