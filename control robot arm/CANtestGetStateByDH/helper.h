/*
 * helper.h
 *
 * Some helper macros, functions, classes.
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "canbase.h" // must be included first when using ESD on cygwin

#include <iostream>
#include <string>
#include <iomanip>
#include <string>
#include <time.h>
#include <math.h>
//#include <sys/time.h>

// from GetErrorInfo.cpp:
extern std::string GetErrorInfo( cCANBase& can, unsigned char node, char const* prefix );

/*!
 Just a macro for the very lazy programmer to convert an enum or a DEFINE macro
 into a case command that returns the name of the macro as string.

 Usage:
 \code
 char const* eSomeEnumType_ToString( eSomeEnumType rc )
 {
   switch (rc)
   {
   DEFINE_TO_CASECOMMAND( AN_ENUM );
   DEFINE_TO_CASECOMMAND( AN_OTHER_ENUM );
   ...
   default:               return "unknown return code";
   }
 }
 \endcode

 \remark You must use the enum or macro directly (not a variable with that value) since CPP-stringification is used.

 See also #DEFINE_TO_CASECOMMAND_MSG
*/
#define DEFINE_TO_CASECOMMAND( _c ) case _c: return (#_c)

//======================================================================



//! dummy class for (debug) stream output of bytes as list of hex values
class cHexByteString
{
    char const* bytes;
    int         len;
public:
    //! ctor: create a cHexByteString with \a _len bytes at \a _bytes
    cHexByteString( char const* _bytes, int _len ) :
        bytes(_bytes),
        len(_len)
        {};

    friend std::ostream &operator<<(std::ostream &stream, cHexByteString const &s);
};

//! output the bytes in \a s to \a stream as a list of space separated hex bytes (without 0x prefix)
inline std::ostream &operator<<(std::ostream &stream, cHexByteString const &s)
{
    //-----
    // print all bytes as hex bytes:
    bool is_all_printable_ascii = true;
    for ( int i = 0; i < s.len; ++i )
    {
        stream << std::hex << std::setw(2) << std::setfill('0') << int( ((unsigned char const*)s.bytes)[i] ) << " ";
        if ( s.bytes[i] < 0x20 || ((unsigned char) s.bytes[i]) >= 0x80 )
            is_all_printable_ascii = false;
    }
    //-----

    //-----
    // if the bytes were all printable ascii codes then print them as string as well:
    if ( is_all_printable_ascii )
        stream << "= \"" << std::string( s.bytes, s.len ) << "\"";
    //-----

    return stream << std::dec;
};
//======================================================================

inline void SleepSec( double t )
{
    //timespec sleeptime;
    //sleeptime.tv_sec  = (time_t) floor( t );
    //sleeptime.tv_nsec = (long)   ((t - floor( t )) * 1E9);

    ////std::cerr << "Sleeping for " << sleeptime.tv_sec << "s and " << sleeptime.tv_nsec << "ns\n";

    //nanosleep( &sleeptime, NULL );


	for(int i=0; i< (int)(20000.0*t); i++)
	{
		for(int j=0; j< (int)(20000.0*t); j++)
		{

		}
	}
}
//-----------------------------------------------------------------


/*!
  \brief Very simple class to measure elapsed time
*/
class cSimpleTime
{
public:
    //! Constructor: store current time ("now") internally.
    cSimpleTime()
    {
        StoreNow();
    }
    //----------------------------------------------------------------------


    //! Store current time internally.
    void StoreNow( void )
    {
        //gettimeofday( &a_time, NULL );
    }
    //----------------------------------------------------------------------

    //! Return time in seconds elapsed between the time stored in the object and now.
    double Elapsed( void ) const
    {
        cSimpleTime now;

        return Elapsed( now );
    }
    //----------------------------------------------------------------------


    //! Return time in micro seconds elapsed between the time stored in the object and now.
    long Elapsed_us( void ) const
    {
        cSimpleTime now;

        return Elapsed_us( now );
    }
    //----------------------------------------------------------------------


    //! Return time in seconds elapsed between the time stored in the object and \a other.
    double Elapsed( cSimpleTime const& other ) const
    {
        double seconds = double( other.a_time.tv_sec - a_time.tv_sec );
        double usec = double( other.a_time.tv_usec - a_time.tv_usec );

        return seconds + usec / 1000000.0;
    }
    //----------------------------------------------------------------------

    //! Return time in micro seconds elapsed between the time stored in the object and \a other.
    long Elapsed_us( cSimpleTime const& other ) const
    {
        long seconds = other.a_time.tv_sec - a_time.tv_sec;
        long usec    = other.a_time.tv_usec - a_time.tv_usec;

        return seconds * 1000000 + usec;
    }
    //----------------------------------------------------------------------


    //! Return the time stored as C struct timeval
    timeval Timeval( void )
    {
        return a_time;
    }

private:
    struct timeval a_time;
};

#endif /* HELPER_H_ */
