/*
 * canbase.h
 *
 * Interface of base CAN object to communicate via CAN-bus
 * using PEAK or ESD USB to CAN converter.
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */

#ifndef CANBASE_H_
#define CANBASE_H_

//#ifdef USE_PEAK
//# include "canbase_peak.h"
//#elif defined( USE_ESD )
//# include "canbase_esd.h"
//#else
//# error "Exactly one of USE_PEAK or USE_ESD must be defined"
//#endif

#include "canbase_esd.h"

#include <string>

#if 0  // Set to 1 to enable (lots of) low level debug messages
# define DBG( ... )         \
    do {                    \
        __VA_ARGS__;        \
    } while (0)
#else
# define DBG( ... )
#endif


//======================================================================


/*!
 \brief Derived exception class for low-level CAN PEAK related exceptions.
*/
class cCANBaseException
{
public:
   cCANBaseException( std::string const & _msg )
   {
       msg = _msg;
   }

   std::string msg;
};
//======================================================================

#endif /* CANBASE_H_ */
