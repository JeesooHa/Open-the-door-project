/*!
 * \file pdo.h
 *
 * Simple header-only implementation of CANopen Process Data Objects
 *
 * \date:   13.02.2012
 * \author: Dirk Osswald  dirk.osswald@de.schunk.com
 */

#ifndef PDO_H_
#define PDO_H_

#include "canbase.h" // must be included first when using ESD on cygwin

#include <assert.h>
#include <list>

#include "sdo.h"


/*!
 \brief Derived exception class for SDO related exceptions
*/
class cPDOException : public cCANBaseException
{
public:
   cPDOException( std::string const & _msg )
       : cCANBaseException( _msg )
   {}
};
//======================================================================

//! Return a node_no node number corresponding to a given CAN ID \a id
inline unsigned char GetNodeNoFromID( unsigned short id )
{
    return id & 0x07f;
}

//! Return a tpdo_no TPDO number corresponding to a given CAN ID \a id
inline unsigned char GetTPDONoFromID( unsigned short id )
{
    return (id & 0x700)>>8;
}

/*!
 * Helper class to hold the mapping parameters of one parameter to map to a pdo
 */
struct cMappingParameter
{
    cMappingParameter( unsigned short index_, unsigned char subindex_, unsigned char bitlength_ )
    :
        index(index_),
        subindex(subindex_),
        bitlength(bitlength_)
    {}
    unsigned short index;
    unsigned char  subindex;
    unsigned char  bitlength;
};

/*!
 * Simple class to Download/Upload PDO data
 *
 */
class cPDO
{
public:

    /*!
     *  Standard base CAN IDs of up to 4 PDOs PDO1,... PDO4 for transmit and receive:
     *
     *  Transmit/receive is as seen by the SCHUNK module (as usual for CANopen).
     *  => Transmit is Module to PC
     *  => Receive is PC to Module
     */
    static unsigned short const PDO1TX_ID = 0x180; //         #  384 ->  385 ... 511
    static unsigned short const PDO1RX_ID = 0x200; //         #  512 ->  513 ... 639
    static unsigned short const PDO2TX_ID = 0x280; //         #  640 ->  641 ... 767
    static unsigned short const PDO2RX_ID = 0x300; //         #  768 ->  769 ... 895
    static unsigned short const PDO3TX_ID = 0x380; //         #  896 ->  897 ... 1023
    static unsigned short const PDO3RX_ID = 0x400; //         # 1024 -> 1025 ... 1151
    static unsigned short const PDO4TX_ID = 0x480; //         # 1152 -> 1153 ... 1279
    static unsigned short const PDO4RX_ID = 0x500; //         # 1280 -> 1281 ... 1407

    // PDOs are numbered from 1 on. Therefore we insert a dummy value of 0x0 at index 0:
    static unsigned short const PDOxRX_ID[];// = { 0x0, PDO1RX_ID, PDO2RX_ID, PDO3RX_ID, PDO4RX_ID };
    static unsigned short const PDOxTX_ID[];// = { 0x0, PDO1TX_ID, PDO2TX_ID, PDO3TX_ID, PDO4TX_ID };


    //! Communication directions of a PDO, needed when mapping PDOs
    enum eDirection
    {
        TRANSMIT = 0, //!< transmit from the perspective of the slave: slave->master
        RECEIVE  = 1, //!< receive from the perspective of the slave:  slave<-master
    };

    //! Transmission types of a PDO, needed when mapping PDOs
    enum eTransmissionType {
        SYNCHRONOUS_ACYCLIC = 0,
        RTR_ONLY_SYNCHRONOUS = 252,
        RTR_ONLY_EVENT_DRIVEN = 253,
        EVENT_DRIVEN_MANUFACTURER_SPECIFIC = 254,
        EVENT_DRIVEN_PROFILE_SPECIFIC = 254,
        SYNCHRONOUS_CYCLIC_1 = 1,
        SYNCHRONOUS_CYCLIC_2 = 2,
        SYNCHRONOUS_CYCLIC_3 = 3,
        SYNCHRONOUS_CYCLIC_4 = 4,
        SYNCHRONOUS_CYCLIC_5 = 5,
        SYNCHRONOUS_CYCLIC_6 = 6,
        SYNCHRONOUS_CYCLIC_7 = 7,
        SYNCHRONOUS_CYCLIC_8 = 8,
        SYNCHRONOUS_CYCLIC_9 = 9,
        SYNCHRONOUS_CYCLIC_10 = 10,
        SYNCHRONOUS_CYCLIC_11 = 11,
        SYNCHRONOUS_CYCLIC_12 = 12,
        SYNCHRONOUS_CYCLIC_13 = 13,
        SYNCHRONOUS_CYCLIC_14 = 14,
        SYNCHRONOUS_CYCLIC_15 = 15,
        SYNCHRONOUS_CYCLIC_16 = 16,
        SYNCHRONOUS_CYCLIC_17 = 17,
        SYNCHRONOUS_CYCLIC_18 = 18,
        SYNCHRONOUS_CYCLIC_19 = 19,
        SYNCHRONOUS_CYCLIC_20 = 20,
        SYNCHRONOUS_CYCLIC_21 = 21,
        SYNCHRONOUS_CYCLIC_22 = 22,
        SYNCHRONOUS_CYCLIC_23 = 23,
        SYNCHRONOUS_CYCLIC_24 = 24,
        SYNCHRONOUS_CYCLIC_25 = 25,
        SYNCHRONOUS_CYCLIC_26 = 26,
        SYNCHRONOUS_CYCLIC_27 = 27,
        SYNCHRONOUS_CYCLIC_28 = 28,
        SYNCHRONOUS_CYCLIC_29 = 29,
        SYNCHRONOUS_CYCLIC_30 = 30,
        SYNCHRONOUS_CYCLIC_31 = 31,
        SYNCHRONOUS_CYCLIC_32 = 32,
        SYNCHRONOUS_CYCLIC_33 = 33,
        SYNCHRONOUS_CYCLIC_34 = 34,
        SYNCHRONOUS_CYCLIC_35 = 35,
        SYNCHRONOUS_CYCLIC_36 = 36,
        SYNCHRONOUS_CYCLIC_37 = 37,
        SYNCHRONOUS_CYCLIC_38 = 38,
        SYNCHRONOUS_CYCLIC_39 = 39,
        SYNCHRONOUS_CYCLIC_40 = 40,
        SYNCHRONOUS_CYCLIC_41 = 41,
        SYNCHRONOUS_CYCLIC_42 = 42,
        SYNCHRONOUS_CYCLIC_43 = 43,
        SYNCHRONOUS_CYCLIC_44 = 44,
        SYNCHRONOUS_CYCLIC_45 = 45,
        SYNCHRONOUS_CYCLIC_46 = 46,
        SYNCHRONOUS_CYCLIC_47 = 47,
        SYNCHRONOUS_CYCLIC_48 = 48,
        SYNCHRONOUS_CYCLIC_49 = 49,
        SYNCHRONOUS_CYCLIC_50 = 50,
        SYNCHRONOUS_CYCLIC_51 = 51,
        SYNCHRONOUS_CYCLIC_52 = 52,
        SYNCHRONOUS_CYCLIC_53 = 53,
        SYNCHRONOUS_CYCLIC_54 = 54,
        SYNCHRONOUS_CYCLIC_55 = 55,
        SYNCHRONOUS_CYCLIC_56 = 56,
        SYNCHRONOUS_CYCLIC_57 = 57,
        SYNCHRONOUS_CYCLIC_58 = 58,
        SYNCHRONOUS_CYCLIC_59 = 59,
        SYNCHRONOUS_CYCLIC_60 = 60,
        SYNCHRONOUS_CYCLIC_61 = 61,
        SYNCHRONOUS_CYCLIC_62 = 62,
        SYNCHRONOUS_CYCLIC_63 = 63,
        SYNCHRONOUS_CYCLIC_64 = 64,
        SYNCHRONOUS_CYCLIC_65 = 65,
        SYNCHRONOUS_CYCLIC_66 = 66,
        SYNCHRONOUS_CYCLIC_67 = 67,
        SYNCHRONOUS_CYCLIC_68 = 68,
        SYNCHRONOUS_CYCLIC_69 = 69,
        SYNCHRONOUS_CYCLIC_70 = 70,
        SYNCHRONOUS_CYCLIC_71 = 71,
        SYNCHRONOUS_CYCLIC_72 = 72,
        SYNCHRONOUS_CYCLIC_73 = 73,
        SYNCHRONOUS_CYCLIC_74 = 74,
        SYNCHRONOUS_CYCLIC_75 = 75,
        SYNCHRONOUS_CYCLIC_76 = 76,
        SYNCHRONOUS_CYCLIC_77 = 77,
        SYNCHRONOUS_CYCLIC_78 = 78,
        SYNCHRONOUS_CYCLIC_79 = 79,
        SYNCHRONOUS_CYCLIC_80 = 80,
        SYNCHRONOUS_CYCLIC_81 = 81,
        SYNCHRONOUS_CYCLIC_82 = 82,
        SYNCHRONOUS_CYCLIC_83 = 83,
        SYNCHRONOUS_CYCLIC_84 = 84,
        SYNCHRONOUS_CYCLIC_85 = 85,
        SYNCHRONOUS_CYCLIC_86 = 86,
        SYNCHRONOUS_CYCLIC_87 = 87,
        SYNCHRONOUS_CYCLIC_88 = 88,
        SYNCHRONOUS_CYCLIC_89 = 89,
        SYNCHRONOUS_CYCLIC_90 = 90,
        SYNCHRONOUS_CYCLIC_91 = 91,
        SYNCHRONOUS_CYCLIC_92 = 92,
        SYNCHRONOUS_CYCLIC_93 = 93,
        SYNCHRONOUS_CYCLIC_94 = 94,
        SYNCHRONOUS_CYCLIC_95 = 95,
        SYNCHRONOUS_CYCLIC_96 = 96,
        SYNCHRONOUS_CYCLIC_97 = 97,
        SYNCHRONOUS_CYCLIC_98 = 98,
        SYNCHRONOUS_CYCLIC_99 = 99,
        SYNCHRONOUS_CYCLIC_100 = 100,
        SYNCHRONOUS_CYCLIC_101 = 101,
        SYNCHRONOUS_CYCLIC_102 = 102,
        SYNCHRONOUS_CYCLIC_103 = 103,
        SYNCHRONOUS_CYCLIC_104 = 104,
        SYNCHRONOUS_CYCLIC_105 = 105,
        SYNCHRONOUS_CYCLIC_106 = 106,
        SYNCHRONOUS_CYCLIC_107 = 107,
        SYNCHRONOUS_CYCLIC_108 = 108,
        SYNCHRONOUS_CYCLIC_109 = 109,
        SYNCHRONOUS_CYCLIC_110 = 110,
        SYNCHRONOUS_CYCLIC_111 = 111,
        SYNCHRONOUS_CYCLIC_112 = 112,
        SYNCHRONOUS_CYCLIC_113 = 113,
        SYNCHRONOUS_CYCLIC_114 = 114,
        SYNCHRONOUS_CYCLIC_115 = 115,
        SYNCHRONOUS_CYCLIC_116 = 116,
        SYNCHRONOUS_CYCLIC_117 = 117,
        SYNCHRONOUS_CYCLIC_118 = 118,
        SYNCHRONOUS_CYCLIC_119 = 119,
        SYNCHRONOUS_CYCLIC_120 = 120,
        SYNCHRONOUS_CYCLIC_121 = 121,
        SYNCHRONOUS_CYCLIC_122 = 122,
        SYNCHRONOUS_CYCLIC_123 = 123,
        SYNCHRONOUS_CYCLIC_124 = 124,
        SYNCHRONOUS_CYCLIC_125 = 125,
        SYNCHRONOUS_CYCLIC_126 = 126,
        SYNCHRONOUS_CYCLIC_127 = 127,
        SYNCHRONOUS_CYCLIC_128 = 128,
        SYNCHRONOUS_CYCLIC_129 = 129,
        SYNCHRONOUS_CYCLIC_130 = 130,
        SYNCHRONOUS_CYCLIC_131 = 131,
        SYNCHRONOUS_CYCLIC_132 = 132,
        SYNCHRONOUS_CYCLIC_133 = 133,
        SYNCHRONOUS_CYCLIC_134 = 134,
        SYNCHRONOUS_CYCLIC_135 = 135,
        SYNCHRONOUS_CYCLIC_136 = 136,
        SYNCHRONOUS_CYCLIC_137 = 137,
        SYNCHRONOUS_CYCLIC_138 = 138,
        SYNCHRONOUS_CYCLIC_139 = 139,
        SYNCHRONOUS_CYCLIC_140 = 140,
        SYNCHRONOUS_CYCLIC_141 = 141,
        SYNCHRONOUS_CYCLIC_142 = 142,
        SYNCHRONOUS_CYCLIC_143 = 143,
        SYNCHRONOUS_CYCLIC_144 = 144,
        SYNCHRONOUS_CYCLIC_145 = 145,
        SYNCHRONOUS_CYCLIC_146 = 146,
        SYNCHRONOUS_CYCLIC_147 = 147,
        SYNCHRONOUS_CYCLIC_148 = 148,
        SYNCHRONOUS_CYCLIC_149 = 149,
        SYNCHRONOUS_CYCLIC_150 = 150,
        SYNCHRONOUS_CYCLIC_151 = 151,
        SYNCHRONOUS_CYCLIC_152 = 152,
        SYNCHRONOUS_CYCLIC_153 = 153,
        SYNCHRONOUS_CYCLIC_154 = 154,
        SYNCHRONOUS_CYCLIC_155 = 155,
        SYNCHRONOUS_CYCLIC_156 = 156,
        SYNCHRONOUS_CYCLIC_157 = 157,
        SYNCHRONOUS_CYCLIC_158 = 158,
        SYNCHRONOUS_CYCLIC_159 = 159,
        SYNCHRONOUS_CYCLIC_160 = 160,
        SYNCHRONOUS_CYCLIC_161 = 161,
        SYNCHRONOUS_CYCLIC_162 = 162,
        SYNCHRONOUS_CYCLIC_163 = 163,
        SYNCHRONOUS_CYCLIC_164 = 164,
        SYNCHRONOUS_CYCLIC_165 = 165,
        SYNCHRONOUS_CYCLIC_166 = 166,
        SYNCHRONOUS_CYCLIC_167 = 167,
        SYNCHRONOUS_CYCLIC_168 = 168,
        SYNCHRONOUS_CYCLIC_169 = 169,
        SYNCHRONOUS_CYCLIC_170 = 170,
        SYNCHRONOUS_CYCLIC_171 = 171,
        SYNCHRONOUS_CYCLIC_172 = 172,
        SYNCHRONOUS_CYCLIC_173 = 173,
        SYNCHRONOUS_CYCLIC_174 = 174,
        SYNCHRONOUS_CYCLIC_175 = 175,
        SYNCHRONOUS_CYCLIC_176 = 176,
        SYNCHRONOUS_CYCLIC_177 = 177,
        SYNCHRONOUS_CYCLIC_178 = 178,
        SYNCHRONOUS_CYCLIC_179 = 179,
        SYNCHRONOUS_CYCLIC_180 = 180,
        SYNCHRONOUS_CYCLIC_181 = 181,
        SYNCHRONOUS_CYCLIC_182 = 182,
        SYNCHRONOUS_CYCLIC_183 = 183,
        SYNCHRONOUS_CYCLIC_184 = 184,
        SYNCHRONOUS_CYCLIC_185 = 185,
        SYNCHRONOUS_CYCLIC_186 = 186,
        SYNCHRONOUS_CYCLIC_187 = 187,
        SYNCHRONOUS_CYCLIC_188 = 188,
        SYNCHRONOUS_CYCLIC_189 = 189,
        SYNCHRONOUS_CYCLIC_190 = 190,
        SYNCHRONOUS_CYCLIC_191 = 191,
        SYNCHRONOUS_CYCLIC_192 = 192,
        SYNCHRONOUS_CYCLIC_193 = 193,
        SYNCHRONOUS_CYCLIC_194 = 194,
        SYNCHRONOUS_CYCLIC_195 = 195,
        SYNCHRONOUS_CYCLIC_196 = 196,
        SYNCHRONOUS_CYCLIC_197 = 197,
        SYNCHRONOUS_CYCLIC_198 = 198,
        SYNCHRONOUS_CYCLIC_199 = 199,
        SYNCHRONOUS_CYCLIC_200 = 200,
        SYNCHRONOUS_CYCLIC_201 = 201,
        SYNCHRONOUS_CYCLIC_202 = 202,
        SYNCHRONOUS_CYCLIC_203 = 203,
        SYNCHRONOUS_CYCLIC_204 = 204,
        SYNCHRONOUS_CYCLIC_205 = 205,
        SYNCHRONOUS_CYCLIC_206 = 206,
        SYNCHRONOUS_CYCLIC_207 = 207,
        SYNCHRONOUS_CYCLIC_208 = 208,
        SYNCHRONOUS_CYCLIC_209 = 209,
        SYNCHRONOUS_CYCLIC_210 = 210,
        SYNCHRONOUS_CYCLIC_211 = 211,
        SYNCHRONOUS_CYCLIC_212 = 212,
        SYNCHRONOUS_CYCLIC_213 = 213,
        SYNCHRONOUS_CYCLIC_214 = 214,
        SYNCHRONOUS_CYCLIC_215 = 215,
        SYNCHRONOUS_CYCLIC_216 = 216,
        SYNCHRONOUS_CYCLIC_217 = 217,
        SYNCHRONOUS_CYCLIC_218 = 218,
        SYNCHRONOUS_CYCLIC_219 = 219,
        SYNCHRONOUS_CYCLIC_220 = 220,
        SYNCHRONOUS_CYCLIC_221 = 221,
        SYNCHRONOUS_CYCLIC_222 = 222,
        SYNCHRONOUS_CYCLIC_223 = 223,
        SYNCHRONOUS_CYCLIC_224 = 224,
        SYNCHRONOUS_CYCLIC_225 = 225,
        SYNCHRONOUS_CYCLIC_226 = 226,
        SYNCHRONOUS_CYCLIC_227 = 227,
        SYNCHRONOUS_CYCLIC_228 = 228,
        SYNCHRONOUS_CYCLIC_229 = 229,
        SYNCHRONOUS_CYCLIC_230 = 230,
        SYNCHRONOUS_CYCLIC_231 = 231,
        SYNCHRONOUS_CYCLIC_232 = 232,
        SYNCHRONOUS_CYCLIC_233 = 233,
        SYNCHRONOUS_CYCLIC_234 = 234,
        SYNCHRONOUS_CYCLIC_235 = 235,
        SYNCHRONOUS_CYCLIC_236 = 236,
        SYNCHRONOUS_CYCLIC_237 = 237,
        SYNCHRONOUS_CYCLIC_238 = 238,
        SYNCHRONOUS_CYCLIC_239 = 239,
        SYNCHRONOUS_CYCLIC_240 = 240,
    };


    //! Simple STL list of node-ids:
    typedef std::list<cMappingParameter> tMappingParameterList;
    typedef tMappingParameterList::const_iterator tMappingParameterListIterator;

    //! CTOR for CANopen PDO communication
    cPDO( cCANBase& _can, unsigned char _node, unsigned char _rpdo_no, unsigned char _rpdo_len, unsigned char _tpdo_no, unsigned char _tpdo_len ) :
        can( _can ),
        node( _node ),
        rpdo_no( _rpdo_no ),
        rpdo_len( _rpdo_len ),
        tpdo_no( _tpdo_no ),
        tpdo_len( _tpdo_len ),
        read_something_else( false )
    {
        assert( rpdo_no > 0  );   // RPDOs are numbered from 1 on in the CiA standard
        assert( rpdo_no <= 2 );   // only 1 RPDOs supported by SCHUNK modules
        assert( rpdo_len <= 8 );  // TPDOs are numbered from 1 on in the CiA standard
        assert( tpdo_no <= 4 );   // only 4 TPDOs supported by SCHUNK modules
        assert( tpdo_len <= 8 );
        rpdo_id = ((unsigned short) rpdo_no+1) * 0x100         + (unsigned short) node;
        tpdo_id = ((unsigned short) tpdo_no)   * 0x100 + 0x080 + (unsigned short) node;
        can.AddId( tpdo_id );
    }

    cCANBase& GetCAN(void) const
    {
        return can;
    }

    unsigned char GetNode(void) const
    {
        return node;
    }

    /*!
     * Download the configured \a _rpdo_len bytes from \a usrdata via RPDO number \a rpdo_no
     */
    void Download( unsigned char const* usrdata )
    {
        can.WriteData( rpdo_id, rpdo_len, usrdata );
    }

    /*!
     * Upload the configured \a _tpdo_len bytes into \a usrdata via TPDO number \a tpdo_no.
     * Will not block if no data is available.
     *
     * \return number of bytes actually received
     */
    int Upload( unsigned char* usrdata )
    {
        return can.ReadData( tpdo_id, usrdata );
    }

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
    void MapPDO( cSDO&                        sdo,
                 unsigned char                node_no,
                 eDirection                   direction,
                 unsigned char                pdo_no,
                 eTransmissionType            transmission_type,
                 tMappingParameterList const& mapping_list );

private:
    cCANBase& can;
    unsigned char  node;
    unsigned char  rpdo_no;
    unsigned char  rpdo_len;
    unsigned short rpdo_id;
    unsigned char  tpdo_no;
    unsigned char  tpdo_len;
    unsigned short tpdo_id;
    bool           read_something_else;
};
#endif /* PDO_H_ */
