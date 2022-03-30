/*
 * sdo.h
 *
 * Interface of simple, incomplete CANopen SDO (Service Data Object).
 *
 *  Created on: 13.02.2012
 *      Author: osswald2
 */

#ifndef SDO_H_
#define SDO_H_

#include "canbase.h" // must be included first when using ESD on cygwin

#include <assert.h>

/*!
 \brief Derived exception class for SDO related exceptions
*/
class cSDOException : public cCANBaseException
{
public:
   cSDOException( std::string const & _msg )
       : cCANBaseException( _msg )
   {}
};
//======================================================================

class cSDO
{
public:

    /*!
     *  Standard base CAN IDs of SDO for transmit and receive:
     *
     *  Transmit/receive is as seen by the SCHUNK module (as usual for CANopen).
     *  => Transmit is Module to PC
     *  => Receive is PC to Module
     */
    static unsigned short const SDOTX_ID = 0x580; //             # 1408 -> 1409 ... 1535
    static unsigned short const SDORX_ID = 0x600; //             # 1536 -> 1537 ... 1663

    // download
    static unsigned char const SDO_SEG_REQ_INIT_DOWNLOAD_xBYTE = 0x22;
    static unsigned char const SDO_SEG_REQ_INIT_DOWNLOAD_1BYTE = 0x2F;
    static unsigned char const SDO_SEG_REQ_INIT_DOWNLOAD_2BYTE = 0x2B;
    static unsigned char const SDO_SEG_REQ_INIT_DOWNLOAD_3BYTE = 0x27;
    static unsigned char const SDO_SEG_REQ_INIT_DOWNLOAD_4BYTE = 0x23;
    static unsigned char const SDO_SEG_RES_INIT_DOWNLOAD = 0x60;
    // upload
    static unsigned char const SDO_SEG_REQ_INIT_UPLOAD = 0x40;
    static unsigned char const SDO_SEG_RES_INIT_UPLOAD_nBYTE =  0x41;
    static unsigned char const SDO_SEG_RES_INIT_UPLOAD_xBYTE =  0x42;
    static unsigned char const SDO_SEG_RES_INIT_UPLOAD_1BYTE = 0x4F;
    static unsigned char const SDO_SEG_RES_INIT_UPLOAD_2BYTE = 0x4B;
    static unsigned char const SDO_SEG_RES_INIT_UPLOAD_3BYTE = 0x47;
    static unsigned char const SDO_SEG_RES_INIT_UPLOAD_4BYTE = 0x43;
    static unsigned char const SDO_SEG_REQ_UPLOAD0 = 0x60;
    static unsigned char const SDO_SEG_REQ_UPLOAD1 = 0x70;

    static unsigned char const SDO_SEG_ABORT_TRANSFER =0x80;

    // error register encoding according to CiA DS301
    static unsigned char const EMC_k_1001_GENERIC        = 0x01;
    static unsigned char const EMC_k_1001_CURRENT        = 0x02;
    static unsigned char const EMC_k_1001_VOLTAGE        = 0x04;
    static unsigned char const EMC_k_1001_TEMPERATURE    = 0x08;
    static unsigned char const EMC_k_1001_COMMUNICATION  = 0x10;
    static unsigned char const EMC_k_1001_DEV_PROF_SPEC  = 0x20;
    static unsigned char const EMC_k_1001_RESERVED       = 0x40;
    static unsigned char const EMC_k_1001_MANUFACTURER   = 0x80;

    // number of node ids
    static unsigned char const NB_NODES = 0x80;

    //! CTOR for download of data to CANopen nodes via SDO
    cSDO( cCANBase& _can ) :
        can( _can )
    {
        if ( first_time )
        {
            for ( int i=0; i< NB_NODES; ++i )
                nodes_added[i] = false;
            first_time = false;
        }
    }

    //! convert a SDH error code to a human readable string
    char const* SDOErrorToString( unsigned int rc );

    /*!
     * Download \a len bytes from \a usrdata into object dictionary of \a node into \a index / \a subindex
     *
     * \warning
     * - Works with expedited SDO download only for now (at most 4 user data bytes allowed)!
     * - Will block for up to a second to read the response of the node.
     */
    void Download( unsigned char node, unsigned short index, unsigned char subindex, unsigned char len, unsigned char const* usrdata );

    /*!
     * Upload \a len bytes from object dictionary of \a node, \a index / \a subindex to \a usrdata
     *
     * \warning
     * - Works with expedited SDO upload only for now (at most 4 user data bytes allowed)!
     * - Will block for up to a second to read the response of the node.
     */
    void Upload( unsigned char node, unsigned short index, unsigned char subindex, unsigned char len, unsigned char* usrdata );

private:
    cCANBase& can;
    static bool first_time;
    static unsigned char nodes_added[ NB_NODES ];
};

typedef enum
{
    ERROR_NONE,                 //0x00
    INFO_TODO,                  //0x01
    INFO_NO_FREE_SPACE,         //0x02
    INFO_NO_RIGHTS,             //0x03
    INFO_UNKNOWN_COMMAND,       //0x04
    INFO_FAILED,                //0x05
    INFO_NOT_REFERENCED,        //0x06
    UNKNOWN_ERROR_07,           //0x07
    UNKNOWN_ERROR_08,           //0x08
    INFO_COMMUNICATION_ERROR,   //0x09
    INFO_PROGRAM_DEBUGDATA,     //0x0A
    INFO_PROGRAM_FINISHED,      //0x0B
    INFO_PROGRAM_ABORTED,       //0x0C
    INFO_PROGRAM_PAUSED,        //0x0D
    INFO_PROGRAM_CONTINUED,     //0x0E
    INFO_SEQUENCE_END,          //0x0F

    INFO_TIMEOUT,               //0x10
    INFO_UNKNOWN_AXIS_INDEX,    //0x11
    UNKNOWN_ERROR_12,           //0x12
    UNKNOWN_ERROR_13,           //0x13
    UNKNOWN_ERROR_14,           //0x14
    UNKNOWN_ERROR_15,           //0x15
    INFO_WRONG_BAUDRATE,        //0x16
    UNKNOWN_ERROR_17,           //0x17
    UNKNOWN_ERROR_18,           //0x18
    INFO_CHECKSUM,              //0x19
    UNKNOWN_ERROR_1A,           //0x1A
    UNKNOWN_ERROR_1B,           //0x1B
    UNKNOWN_ERROR_1C,           //0x1C
    INFO_MESSAGE_LENGTH,        //0x1D
    INFO_WRONG_PARAMETER,       //0x1E
    INFO_PROGRAM_END,           //0x1F

    INFO_BATTERY_LOW,           //0x20
    ERROR_BATTERY_DOWN,         //0x21
    INFO_POSITION_NOT_REACHABLE,         //0x22
    UNKNOWN_ERROR_23,           //0x23
    UNKNOWN_ERROR_24,           //0x24
    UNKNOWN_ERROR_25,           //0x25
    UNKNOWN_ERROR_26,           //0x26
    UNKNOWN_ERROR_27,           //0x27
    UNKNOWN_ERROR_28,           //0x28
    UNKNOWN_ERROR_29,           //0x29
    UNKNOWN_ERROR_2A,           //0x2A
    UNKNOWN_ERROR_2B,           //0x2B
    UNKNOWN_ERROR_2C,           //0x2C
    UNKNOWN_ERROR_2D,           //0x2D
    UNKNOWN_ERROR_2E,           //0x2E
    UNKNOWN_ERROR_2F,           //0x2F

    UNKNOWN_ERROR_30,           //0x30
    UNKNOWN_ERROR_31,           //0x31
    UNKNOWN_ERROR_32,           //0x32
    UNKNOWN_ERROR_33,           //0x33
    UNKNOWN_ERROR_34,           //0x34
    UNKNOWN_ERROR_35,           //0x35
    UNKNOWN_ERROR_36,           //0x36
    UNKNOWN_ERROR_37,           //0x37
    UNKNOWN_ERROR_38,           //0x38
    UNKNOWN_ERROR_39,           //0x39
    UNKNOWN_ERROR_3A,           //0x3A
    UNKNOWN_ERROR_3B,           //0x3B
    UNKNOWN_ERROR_3C,           //0x3C
    UNKNOWN_ERROR_3D,           //0x3D
    UNKNOWN_ERROR_3E,           //0x3E
    UNKNOWN_ERROR_3F,           //0x3F

    INFO_TRIGGER,               //0x40
    INFO_READY,                 //0x41
    INFO_GUI_CONNECTED,         //0x42
    INFO_GUI_DISCONNECTED,      //0x43
    INFO_PROGRAM_CHANGED,       //0x44
    INFO_PROGRAM_CHANGE_FAILED, //0x45
    UNKNOWN_ERROR_46,           //0x46
    UNKNOWN_ERROR_47,           //0x47
    UNKNOWN_ERROR_48,           //0x48
    UNKNOWN_ERROR_49,           //0x49
    UNKNOWN_ERROR_4A,           //0x4A
    UNKNOWN_ERROR_4B,           //0x4B
    UNKNOWN_ERROR_4C,           //0x4C
    UNKNOWN_ERROR_4D,           //0x4D
    UNKNOWN_ERROR_4E,           //0x4E
    UNKNOWN_ERROR_4F,           //0x4F

    UNKNOWN_ERROR_50,           //0x50
    UNKNOWN_ERROR_51,           //0x51
    UNKNOWN_ERROR_52,           //0x52
    UNKNOWN_ERROR_53,           //0x53
    UNKNOWN_ERROR_54,           //0x54
    UNKNOWN_ERROR_55,           //0x55
    UNKNOWN_ERROR_56,           //0x56
    UNKNOWN_ERROR_57,           //0x57
    UNKNOWN_ERROR_58,           //0x58
    UNKNOWN_ERROR_59,           //0x59
    UNKNOWN_ERROR_5A,           //0x5A
    UNKNOWN_ERROR_5B,           //0x5B
    UNKNOWN_ERROR_5C,           //0x5C
    UNKNOWN_ERROR_5D,           //0x5D
    UNKNOWN_ERROR_5E,           //0x5E
    UNKNOWN_ERROR_5F,           //0x5F

    UNKNOWN_ERROR_60,           //0x60
    UNKNOWN_ERROR_61,           //0x61
    UNKNOWN_ERROR_62,           //0x62
    UNKNOWN_ERROR_63,           //0x63
    UNKNOWN_ERROR_64,           //0x64
    UNKNOWN_ERROR_65,           //0x65
    UNKNOWN_ERROR_66,           //0x66
    UNKNOWN_ERROR_67,           //0x67
    UNKNOWN_ERROR_68,           //0x68
    UNKNOWN_ERROR_69,           //0x69
    UNKNOWN_ERROR_6A,           //0x6A
    UNKNOWN_ERROR_6B,           //0x6B
    UNKNOWN_ERROR_6C,           //0x6C
    UNKNOWN_ERROR_6D,           //0x6D
    UNKNOWN_ERROR_6E,           //0x6E
    UNKNOWN_ERROR_6F,           //0x6F

    ERROR_TEMP_LOW,             //0x70
    ERROR_TEMP_HIGH,            //0x71
    ERROR_LOGIC_LOW,            //0x72
    ERROR_LOGIC_HIGH,           //0x73
    ERROR_MOTOR_VOLTAGE_LOW,    //0x74
    ERROR_MOTOR_VOLTAGE_HIGH,   //0x75
    ERROR_CABEL_BREAK,          //0x76
    ERROR_MOTOR_VOLTAGE_SHORT,  //0x77
    ERROR_MOTOR_TEMP,           //0x78
    ERROR_BRAKE_VOLTAGE,        //0x79
    ERROR_LIFE_SIGN,            //0x7A
    ERROR_CUSTOM_DEFINED,       //0x7B
    UNKNOWN_ERROR_7C,           //0x7C
    UNKNOWN_ERROR_7D,           //0x7D
    UNKNOWN_ERROR_7E,           //0x7E
    UNKNOWN_ERROR_7F,           //0x7F

    ERROR_POWER_TEMP,           //0x80
    ERROR_BRAKE_RESISTOR_TEMP,  //0x81
    ERROR_OVERSHOOT,            //0x82
    ERROR_HARDWARE_VERSION,     //0x83
    ERROR_SOFTWARE_VERSION,     //0x84
    UNKNOWN_ERROR_85,           //0x85
    UNKNOWN_ERROR_86,           //0x86
    UNKNOWN_ERROR_87,           //0x87
    UNKNOWN_ERROR_88,           //0x88
    UNKNOWN_ERROR_89,           //0x89
    UNKNOWN_ERROR_8A,           //0x8A
    UNKNOWN_ERROR_8B,           //0x8B
    UNKNOWN_ERROR_8C,           //0x8C
    UNKNOWN_ERROR_8D,           //0x8D
    UNKNOWN_ERROR_8E,           //0x8E
    UNKNOWN_ERROR_8F,           //0x8F

    UNKNOWN_ERROR_90,           //0x90
    UNKNOWN_ERROR_91,           //0x91
    UNKNOWN_ERROR_92,           //0x92
    UNKNOWN_ERROR_93,           //0x93
    UNKNOWN_ERROR_94,           //0x94
    UNKNOWN_ERROR_95,           //0x95
    UNKNOWN_ERROR_96,           //0x96
    UNKNOWN_ERROR_97,           //0x97
    UNKNOWN_ERROR_98,           //0x98
    UNKNOWN_ERROR_99,           //0x99
    UNKNOWN_ERROR_9A,           //0x9A
    UNKNOWN_ERROR_9B,           //0x9B
    UNKNOWN_ERROR_9C,           //0x9C
    UNKNOWN_ERROR_9D,           //0x9D
    UNKNOWN_ERROR_9E,           //0x9E
    UNKNOWN_ERROR_9F,           //0x9F

    UNKNOWN_ERROR_A0,           //0xA0
    UNKNOWN_ERROR_A1,           //0xA1
    UNKNOWN_ERROR_A2,           //0xA2
    UNKNOWN_ERROR_A3,           //0xA3
    UNKNOWN_ERROR_A4,           //0xA4
    UNKNOWN_ERROR_A5,           //0xA5
    UNKNOWN_ERROR_A6,           //0xA6
    UNKNOWN_ERROR_A7,           //0xA7
    UNKNOWN_ERROR_A8,           //0xA8
    UNKNOWN_ERROR_A9,           //0xA9
    UNKNOWN_ERROR_AA,           //0xAA
    UNKNOWN_ERROR_AB,           //0xAB
    UNKNOWN_ERROR_AC,           //0xAC
    UNKNOWN_ERROR_AD,           //0xAD
    UNKNOWN_ERROR_AE,           //0xAE
    UNKNOWN_ERROR_AF,           //0xAF

    ERROR_PROGRAM_ABORTION,     //0xB0
    ERROR_PROGRAM_RUNTIME,      //0xB1
    ERROR_PROGRAM_INVALID,      //0xB2
    ERROR_PROGRAM_CONFLICT,     //0xB3
    UNKNOWN_ERROR_B4,           //0xB4
    UNKNOWN_ERROR_B5,           //0xB5
    UNKNOWN_ERROR_B6,           //0xB6
    UNKNOWN_ERROR_B7,           //0xB7
    UNKNOWN_ERROR_B8,           //0xB8
    UNKNOWN_ERROR_B9,           //0xB9
    UNKNOWN_ERROR_BA,           //0xBA
    UNKNOWN_ERROR_BB,           //0xBB
    UNKNOWN_ERROR_BC,           //0xBC
    UNKNOWN_ERROR_BD,           //0xBD
    UNKNOWN_ERROR_BE,           //0xBE
    UNKNOWN_ERROR_BF,           //0xBF

    UNKNOWN_ERROR_C0,           //0xC0
    UNKNOWN_ERROR_C1,           //0xC1
    UNKNOWN_ERROR_C2,           //0xC2
    UNKNOWN_ERROR_C3,           //0xC3
    UNKNOWN_ERROR_C4,           //0xC4
    UNKNOWN_ERROR_C5,           //0xC5
    UNKNOWN_ERROR_C6,           //0xC6
    UNKNOWN_ERROR_C7,           //0xC7
    ERROR_WRONG_RAMP_TYPE,      //0xC8
    UNKNOWN_ERROR_C9,           //0xC9
    UNKNOWN_ERROR_CA,           //0xCA
    UNKNOWN_ERROR_CB,           //0xCB
    UNKNOWN_ERROR_CC,           //0xCC
    UNKNOWN_ERROR_CD,           //0xCD
    UNKNOWN_ERROR_CE,           //0xCE
    UNKNOWN_ERROR_CF,           //0xCF

    UNKNOWN_ERROR_D0,           //0xD0
    ERROR_WRONG_DIRECTION,      //0xD1
    ERROR_CONFIG_MEMORY,        //0xD2
    ERROR_PROGRAM_MEMORY,       //0xD3
    ERROR_INVALID_PHRASE,       //0xD4
    ERROR_SOFT_LOW,             //0xD5
    ERROR_SOFT_HIGH,            //0xD6
    ERROR_PRESSURE,             //0xD7
    ERROR_SERVICE,              //0xD8
    ERROR_FAST_STOP,            //0xD9
    ERROR_TOW,                  //0xDA
    ERROR_VPC3,                 //0xDB
    ERROR_FRAGMENTATION,        //0xDC
    ERROR_COMMUTATION,          //0xDD
    ERROR_CURRENT,              //0xDE
    ERROR_I2T,                  //0xDF

    ERROR_INITIALIZE,           //0xE0
    ERROR_INTERNAL,             //0xE1
    ERROR_HARD_LOW,             //0xE2
    ERROR_HARD_HIGH,            //0xE3
    ERROR_TOO_FAST,             //0xE4
    ERROR_POS_SYSTEM,           //0xE5
    ERROR_ANALOG_INPUT,         //0xE6
    ERROR_FLASH_MEMORY,         //0xE7
    ERROR_LIMITER_ACTIVE,       //0xE8
    ERROR_STOP_CMD_TIMEOUT,     //0xE9
    ERROR_UNEXPECTED_STOP_DURING_PHRASE, //0xEA
    ERROR_RESOLVER_CHECK_FAILED, //0xEB
    ERROR_MATH,                 //0xEC
    ERROR_LEAVE_MASTER,         //0xED
    UNKNOWN_ERROR_EE,           //0xEE
    UNKNOWN_ERROR_EF,           //0xEF

    UNKNOWN_ERROR_F0,           //0xF0
    UNKNOWN_ERROR_F1,           //0xF1
    UNKNOWN_ERROR_F2,           //0xF2
    UNKNOWN_ERROR_F3,           //0xF3
    UNKNOWN_ERROR_F4,           //0xF4
    UNKNOWN_ERROR_F5,           //0xF5
    UNKNOWN_ERROR_F6,           //0xF6
    UNKNOWN_ERROR_F7,           //0xF7
    UNKNOWN_ERROR_F8,           //0xF8
    UNKNOWN_ERROR_F9,           //0xF9
    UNKNOWN_ERROR_FA,           //0xFA
    UNKNOWN_ERROR_FB,           //0xFB
    UNKNOWN_ERROR_FC,           //0xFC
    UNKNOWN_ERROR_FD,           //0xFD
    UNKNOWN_ERROR_FE,           //0xFE
    UNKNOWN_ERROR_FF,           //0xFF

    INFO_BOOT = 0x0001,
    INFO_SEARCH_SINUS_VECTOR = 0x0007,
    INFO_NO_ERROR = 0x0008
} eSMPErrorCode;

//! return a string with the symbolic name of \a code
char const* eSMPErrorCodeToCStr( eSMPErrorCode code );

#endif /* SDO_H_ */
