/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "mcc_generated_files/spi1.h"   //para utilizar SPI
#include <stdbool.h>

#define _XTAL_FREQ 64000000         //define for correct delays

#define SDC_CS          LATCbits.LATC3
#define SDC_CS_DIR      TRISCbits.TRISC3

//#define SOCKET          //Uncomment for socket with CD and WD pins
#ifdef SOCKET
    #define MEDIA_CD        LATCbits.LATC4
    #define MEDIA_CD_DIR    TRISCbits.TRISC4
    #define MEDIA_WD        LATCbits.LATC5
    #define MEDIA_WD_DIR    TRISCbits.TRISC5
#endif

#define INPUT           1
#define OUTPUT          0

// Description: This macro indicates that the SD card expects to transmit or receive more data
#define MORE_DATA       !0
// Description: This macro indicates that the SD card does not expect to transmit or receive more data
#define NO_DATA         0

//For CSD read fuction
#define CSD_SIZE            16
#define DATA_START_TOKEN    0xFE // The Start Block token

// This macro writes FFh twice to clock in two CRC bytes.
#define mReadCRC()          WriteSPI(0xFF); WriteSPI(0xFF);
#define SDC_FLOATING_BUS    0xFF
#define SDC_BAD_RESPONSE    SDC_FLOATING_BUS
#define SDC_SECTOR_SIZE     512


// This macro writes FFh twice to send two CRC bytes.
// The code assumes CRC values are ignored (the default in SPI mode).
#define mSendCRC()          WriteSPI(0xFF); WriteSPI(0xFF);
#define DATA_ACCEPTED       0b00000101

#define mSend8ClkCycles()   WriteSPI(0xFF); 




// The SDMMC Commands
//          COMMAND                     firts byte      command index
#define     cmdGO_IDLE_STATE            0x40            //0
#define     cmdSEND_OP_COND             0x41            //1
#define     cmdSEND_CSD                 0x49            //9
#define     cmdSEND_CID                 0x4A            //10
#define     cmdSTOP_TRANSMISSION        0x4C            //12
#define     cmdSEND_STATUS              0x4D            //13
#define     cmdSET_BLOCKLEN             0x50            //16
#define     cmdREAD_SINGLE_BLOCK        0x51            //17
#define     cmdREAD_MULTI_BLOCK         0x52            //18
#define     cmdWRITE_SINGLE_BLOCK       0x58            //24
#define     cmdWRITE_MULTI_BLOCK        0x59            //25
#define     cmdTAG_SECTOR_START         0x60            //32
#define     cmdTAG_SECTOR_END           0x61            //33
#define     cmdUNTAG_SECTOR             0x62            //34
#define     cmdTAG_ERASE_GRP_START      0x63            //35
#define     cmdTAG_ERASE_GRP_END        0x64            //36
#define     cmdUNTAG_ERASE_GRP          0x65            //37
#define     cmdERASE                    0x66            //38
#define     cmdSD_APP_OP_COND           0x69            //41
#define     cmdLOCK_UNLOCK              0x71            //49
#define     cmdAPP_CMD                  0x77            //55
#define     cmdREAD_OCR                 0x7A            //58
#define     cmdCRC_ON_OFF               0x7B            //59



// Summary: An SD command packet
// Description: This union represents different ways to access an SD card command packet
typedef union
{
    // This structure allows array-style access of command uint8_ts
    struct
    {
        uint8_t field[6];      // uint8_t array
    };
    // This structure allows uint8_t-wise access of packet command uint8_ts
    struct
    {
        uint8_t crc;               // The CRC uint8_t
        uint8_t addr0;             // Address uint8_t 0
        uint8_t addr1;             // Address uint8_t 1
        uint8_t addr2;             // Address uint8_t 2
        uint8_t addr3;             // Address uint8_t 3
        uint8_t cmd;               // Command code uint8_t
    };
    // This structure allows bitwise access to elements of the command uint8_ts
    struct
    {
        uint8_t  END_BIT:1;        // Packet end bit
        uint8_t  CRC7:7;           // CRC value
        uint32_t     address;      // Address
        uint8_t  command;        // command
    };
} CMD_PACKET;

////////////////////////////////


////////////////////////////////

typedef union{
    uint8_t _byte;
    struct{
        unsigned IN_IDLE_STATE:1;
        unsigned ERASE_RESET:1;
        unsigned ILLEGAL_CMD:1;
        unsigned CRC_ERR:1;
        unsigned ERASE_SEQ_ERR:1;
        unsigned ADDRESS_ERR:1;
        unsigned PARAM_ERR:1;
        unsigned B7:1;
    };
} RESPONSE_1;

////////////////////////////////

typedef union{
    uint16_t _word;
    struct{
        uint8_t _byte0;
        uint8_t _byte1;
    };
    struct{
        unsigned IN_IDLE_STATE:1;
        unsigned ERASE_RESET:1;
        unsigned ILLEGAL_CMD:1;
        unsigned CRC_ERR:1;
        unsigned ERASE_SEQ_ERR:1;
        unsigned ADDRESS_ERR:1;
        unsigned PARAM_ERR:1;
        unsigned B7:1;
        unsigned CARD_IS_LOCKED:1;
        unsigned WP_ERASE_SKIP_LK_FAIL:1;
        unsigned ERROR:1;
        unsigned CC_ERROR:1;
        unsigned CARD_ECC_FAIL:1;
        unsigned WP_VIOLATION:1;
        unsigned ERASE_PARAM:1;
        unsigned OUTRANGE_CSD_OVERWRITE:1;
    };
} RESPONSE_2;

////////////////////////////////



// Summary: A union of responses from an SD card
// Description: The FILEIO_SD_RESPONSE union represents any of the possible responses that an SD card can return after
//              being issued a command.
typedef union
{
    RESPONSE_1  r1;  
    RESPONSE_2  r2;
}SDC_RESPONSE;

////////////////////////////////////////////////////

// Description: Enumeration of different SD response types
typedef enum
{
    R1,     // R1 type response
    R1b,    // R1b type response
    R2,     // R2 type response
    R3,     // R3 type response
    R7      // R7 type response
}RESP;


////////////////////////////

// Summary: SD card command data structure
// Description: The FILEIO_SD_COMMAND structure is used to create a command table of information needed for each relevant SD command
typedef struct
{
    uint8_t      CmdCode;          // The command code
    uint8_t      CRC;              // The CRC value for that command
    RESP    responsetype;       // The response type
    uint8_t    moredataexpected;   // Set to MOREDATA or NODATA, depending on whether more data is expected or not
} SDC_CMD;

/////////////////////////////////////////////////////////////////////////////////



typedef enum
{
    sdcValid = 0,               // No error
    sdcCardInitCommFailure,     // Communication hasn?t been established with the card.
    sdcCardNotInitFailure,      // Card did not initialize.
    sdcCardInitTimeout,         // Card initialization timed out.
    sdcCardTypeInvalid,         // Card type was not able to be defined.
    sdcCardBadCmd,              // Card did not recognize the command.
    sdcCardTimeout,             // Card timed out during a read, write or erase sequence.
    sdcCardCRCError,            // A CRC error occurred during a read.
    sdcCardDataRejected,        // CRC did not match.
    sdcEraseTimedOut            // Erase timed out.
}SDC_Error;

//////////////////////

//Reading the CSD Register
typedef union{
    struct{
        uint32_t _u320;
        uint32_t _u321;
        uint32_t _u322;
        uint32_t _u323;
    };
    struct{
        uint8_t _byte[16];
    };
}readCSD;

/////////////////////

//Card Information
//The SDCSTATE union below is a byte that identifies the device as an SD
//Card/MultiMediaCard and indicates if the card is write-protected:
typedef union _SDCstate{
    struct{
        uint8_t isSDMMC : 1;    // set for an SD Card or MultiMediaCard
        uint8_t isWP : 1;       // set if write protected
    };
    uint8_t _byte;
} SDCSTATE;

/////////////////////////////


// Summary: An enumeration of SD commands
// Description: This enumeration corresponds to the position of each command in the sdmmc_cmdtable array
//              These macros indicate to the FILEIO_SD_SendCmd function which element of the sdmmc_cmdtable array
//              to retrieve command code information from.
typedef enum{
    GO_IDLE_STATE,
    SEND_OP_COND,
    SEND_CSD,
    SEND_CID,
    STOP_TRANSMISSION,
    SEND_STATUS,
    SET_BLOCKLEN,
    READ_SINGLE_BLOCK,
    READ_MULTI_BLOCK,
    WRITE_SINGLE_BLOCK,
    WRITE_MULTI_BLOCK,
    TAG_SECTOR_START,
    TAG_SECTOR_END,
    UNTAG_SECTOR,
    TAG_ERASE_GRP_START,
    TAG_ERASE_GRP_END,
    UNTAG_ERASE_GRP,
    ERASE,
    LOCK_UNLOCK,
    SD_APP_OP_COND,
    APP_CMD,
    READ_OCR,
    CRC_ON_OFF
}sdmmc_cmd;



//variables
readCSD gblCSDReg={};
volatile uint8_t msd_buffer[512];
//Calculate Size of SDcard
uint32_t gblNumBLKS=0x00;
uint32_t gblBLKLen=0x00;

//Function Prototypes
void SocketInitialize(void);
int DetectSDCard(void);
uint8_t IsWriteProtected(void);
SDC_RESPONSE SendSDCCmd(uint8_t cmd, uint32_t address);
SDC_Error CSDRead(void);
SDC_Error SectorRead(uint32_t sector_addr, uint8_t* buffer);
SDC_Error SectorWrite(uint32_t sector_addr, uint8_t* buffer);
SDC_Error MediaInitialize(SDCSTATE *Flag);
uint32_t MSDReadCapacityHandler(void);

uint8_t ReadMedia(void);
uint8_t WriteSPI( uint8_t data_out );
void OpenSPI(void);



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

