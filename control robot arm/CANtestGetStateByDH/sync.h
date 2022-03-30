/*
 * sync.h
 *
 * Simple header-only implementation of CANopen SYNC object.
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */

#ifndef SYNC_H_
#define SYNC_H_

#include "canbase.h" // must be included first when using ESD on cygwin

#include <iostream>
#include <assert.h>


using namespace std;

unsigned short const SYNC_ID = 0x080; //

//! send SYNC object now
inline void SendSync( cCANBase& can )
{
#if 0
    // for debugging purposes: send sync with increasing number as parameter
    static unsigned int i = 0;
    can.WriteData( SYNC_ID, 4, (unsigned char*) &++i );
    cerr << "sync " << i << "\n";
#else
    can.WriteData( SYNC_ID, 0, (unsigned char*) NULL );
#endif
}

#endif /* SYNC_H_ */
