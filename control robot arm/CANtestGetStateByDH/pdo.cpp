/*
 * pdo.cpp
 *
 * Simple CANopen PDO (Process Data Object) implementation.
 * (Unused since all is done in the header file for now)
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */

#include "stdafx.h"

//#include "canbase.h" // must be included first when using ESD on cygwin
//
//#include <assert.h>
//#include <iostream>
//#include <stdint.h>
//#include <sstream>
//
//#include "pdo.h"

using namespace std;

unsigned short const cPDO::PDOxRX_ID[] = { 0x0, cPDO::PDO1RX_ID, cPDO::PDO2RX_ID, cPDO::PDO3RX_ID, cPDO::PDO4RX_ID };
unsigned short const cPDO::PDOxTX_ID[] = { 0x0, cPDO::PDO1TX_ID, cPDO::PDO2TX_ID, cPDO::PDO3TX_ID, cPDO::PDO4TX_ID };

#if 0

not tested yet, so use on your own risk:

/*!
 * Change the mode and mapping of a transmit PDO.
 *
 * \param sdo - ptr to a cSDO object to use for communication. If NULL an internal one will be created.
 * \param node_no - node number
 * \param direction - direction of PDO either TRANSMIT or RECEIVE
 * \param pdo_no - pdo number (1..4)
 * \param transmission_type - PDO transmission type
 * \param mapping_list - list of mapping parameters
 *                       containing (index,subindex,len) with the CANopen
 *                       object dictionary objects to map
 */
void cPDO::MapPDO( cSDO& sdo,
                   unsigned char node_no,
                   eDirection direction,
                   unsigned char pdo_no,
                   eTransmissionType transmission_type,
                   tMappingParameterList const& mapping_list )
{
    char const* mode;
    char const* direction_s;

    assert( 1 <= node_no );
    assert( node_no <= 0x7f );
    assert( 1 <= pdo_no );
    assert( pdo_no <= 4 );

    if ( transmission_type == RTR_ONLY_SYNCHRONOUS )
        mode = "synchronous, acyclic";
    else if ( transmission_type == RTR_ONLY_EVENT_DRIVEN )
        mode = "RTR only, synchronous";
    else if ( transmission_type == EVENT_DRIVEN_MANUFACTURER_SPECIFIC )
        mode = "RTR only, synchronous";
    else if ( transmission_type == EVENT_DRIVEN_PROFILE_SPECIFIC )
        mode = "RTR only, synchronous";
    else if ( transmission_type == RTR_ONLY_SYNCHRONOUS )
        mode = "RTR only, synchronous";
    else
        mode = "synchronous, cyclic every x SYNCx";
    if ( direction == RECEIVE )
        direction_s = "R";
    else if ( direction == TRANSMIT)
        direction_s = "T";
    else
    {
        stringstream msg;
        msg << "Unexpected direction " << int(direction) << " for PDO mapping, expected 0 or 1.";
        throw cPDOException( msg.str() );
    }

    cerr << "MapPDO: new " << mode
         << " mapping for node " << node_no << ", "
         << direction_s << "PDO" << pdo_no << ":\n";

    tMappingParameterListIterator it;
    for (it = mapping_list.begin(); it != mapping_list.end(); ++it)
        cerr << "  OV-Index/Subindex: 0x" << hex << it->index << "/" << int(it->subindex) << " length " << int(it->bitlength) << " bits\n";

    unsigned short pdo_communication_parameter;
    unsigned short pdo_mapping_parameter;
    unsigned short pdo_id;
    if ( direction == RECEIVE )
    {
        pdo_communication_parameter = 0x1400 + pdo_no - 1;
        pdo_mapping_parameter       = 0x1600 + pdo_no - 1;
        pdo_id                      = PDOxRX_ID[ pdo_no ] + node_no;
    }
    else
    {
        pdo_communication_parameter = 0x1800 + pdo_no - 1;
        pdo_mapping_parameter       = 0x1a00 + pdo_no - 1;
        pdo_id                      = PDOxTX_ID[ pdo_no ] + node_no;
    }

    // according to CiA DS 301 page 142:
    //1.   Destroy  RPDO/TPDO  by  setting  bit  valid  to  1 b   of  sub-index  01 h   of  the  according  RPDO/TPDO communication parameter.
    uint32_t data32;
    uint8_t data8;
    data32 = 0x80000000 | uint32_t( pdo_id );
    sdo.Download( node_no, pdo_communication_parameter, 0x01, 4, (unsigned char const*) &data32 );

    //2.   Disable mapping by setting sub-index 00 h  to 00 h .
    data8 = 0x00;
    sdo.Download( node_no, pdo_mapping_parameter, 0x00, 1, &data8 );

    //3.   Modify mapping by changing the values of the corresponding sub-indices.
    tMappingParameterListIterator mpi;
    int i;
    for (mpi = mapping_list.begin(), i = 0; mpi != mapping_list.end(); ++mpi, ++i)
    {
        data32 = ((((*mpi).index)&0xffff) << 16) | ((((*mpi).subindex)&0xff) << 8) | (((*mpi).bitlength)&0xff);
        sdo.Download( node_no, pdo_mapping_parameter, 0x01+i, 4, (unsigned char const*) &data32 );
    }

    //4.   Enable mapping by setting sub-index 00 h  to the number mapped objects.
    data8 = uint8_t( mapping_list.size() );
    sdo.Download( node_no, pdo_mapping_parameter, 0x00, 1, &data8 );

    //4.5  set transmission type
    data8 = uint8_t( transmission_type );
    sdo.Download( node_no, pdo_communication_parameter, 0x02, 1, (unsigned char const*) &data8 );

    //5.   Create  pdo  by  setting  bit  valid  to  0 b   of  sub-index  01 h   of  the  according  RPDO/TPDO communication parameter.
    data32 = uint32_t( pdo_id );
    sdo.Download( node_no, pdo_communication_parameter, 0x01, 4, (unsigned char const*) &data32 );
}
#endif
