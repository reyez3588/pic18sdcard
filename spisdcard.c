/*
 * File:   sdcard.c
 * Author: root
 *
 * Created on July 14, 2022, 8:38 PM
 */


#include <xc.h>
#include "spisdcard.h"

// Summary: Table of SD card commands and parameters
// Description: The sdmmc_cmdtable contains an array of SD card commands, the corresponding CRC code, the
//              response type that the card will return, and a parameter indicating whether to expect
//              additional data from the card.
const SDC_CMD sdmmc_cmdtable[] =
{
    // cmd                      crc     response    more data?
    {cmdGO_IDLE_STATE,          0x95,   R1,         NO_DATA},
    {cmdSEND_OP_COND,           0xF9,   R1,         NO_DATA},
    {cmdSEND_CSD,               0xAF,   R1,         MORE_DATA},
    {cmdSEND_CID,               0x1B,   R1,         MORE_DATA},
    {cmdSTOP_TRANSMISSION,      0xC3,   R1,         NO_DATA},
    {cmdSEND_STATUS,            0xAF,   R2,         NO_DATA},
    {cmdSET_BLOCKLEN,           0xFF,   R1,         NO_DATA},
    {cmdREAD_SINGLE_BLOCK,      0xFF,   R1,         MORE_DATA},
    {cmdREAD_MULTI_BLOCK,       0xFF,   R1,         MORE_DATA},
    {cmdWRITE_SINGLE_BLOCK,     0xFF,   R1,         MORE_DATA},
    {cmdWRITE_MULTI_BLOCK,      0xFF,   R1,         MORE_DATA},
    {cmdTAG_SECTOR_START,       0xFF,   R1,         NO_DATA},
    {cmdTAG_SECTOR_END,         0xFF,   R1,         NO_DATA},
    {cmdUNTAG_SECTOR,           0xFF,   R1,         NO_DATA},
    {cmdTAG_ERASE_GRP_START,    0xFF,   R1,         NO_DATA},
    {cmdTAG_ERASE_GRP_END,      0xFF,   R1,         NO_DATA},
    {cmdUNTAG_ERASE_GRP,        0xFF,   R1,         NO_DATA},
    {cmdERASE,                  0xDF,   R1b,        NO_DATA},
    {cmdLOCK_UNLOCK,            0x89,   R1b,        NO_DATA},
    {cmdSD_APP_OP_COND,         0xE5,   R1,         NO_DATA},
    {cmdAPP_CMD,                0x73,   R1,         NO_DATA},
    {cmdREAD_OCR,               0x25,   R3,         NO_DATA},
    {cmdCRC_ON_OFF,             0x25,   R1,         NO_DATA}
};

void SocketInitialize(void)
{
    #ifdef SOCKET
        MEDIA_CD_DIR = INPUT;
        MEDIA_WD_DIR = INPUT;
    #endif
    
    SDC_CS_DIR = OUTPUT;
    SDC_CS = 1;
    
    return;
}

int DetectSDCard(void)
{
    #ifdef SOCKET
        if (MEDIA_CD)
            return 0;   // Card not present.
        else
            return 1;   // Card is present.
    #endif
    
    return 1;
}

uint8_t IsWriteProtected(void){
    #ifdef SOCKET
        if (MEDIA_WD) return true;
        else return false;
    #endif
    
    return false;
}

SDC_RESPONSE SendSDCCmd(uint8_t cmd, uint32_t address){
    CMD_PACKET  CmdPacket;
    uint8_t    index;
    SDC_RESPONSE response;
    uint16_t    timeout = 9;
    
    // Bring the card?s chip-select line low.
    SDC_CS = 0;
    
    // Store a command byte, address, and CRC value in the CMD_PACKET structure.
    CmdPacket.cmd = sdmmc_cmdtable[cmd].CmdCode;
    CmdPacket.address = address;
    CmdPacket.crc = sdmmc_cmdtable[cmd].CRC;
    
    // Send the command byte, address bytes, and CRC byte.
    // The WriteSPI library function writes a byte on the SPI bus.
    WriteSPI(CmdPacket.cmd);
    WriteSPI(CmdPacket.addr3);
    WriteSPI(CmdPacket.addr2);
    WriteSPI(CmdPacket.addr1);
    WriteSPI(CmdPacket.addr0);
    WriteSPI(CmdPacket.crc);
    
    // Is the command's response type R1 or R1b?
    if (sdmmc_cmdtable[cmd].responsetype == R1 || sdmmc_cmdtable[cmd].responsetype == R1b){
        do{
            // Read a byte from the card until the byte doesn't equal FFh or a timeout occurs.
            response.r1._byte = ReadMedia();
            timeout--;
        } while ((response.r1._byte == 0xFF) && (timeout != 0));
    }
    
    // Is the command?s response type R2?
    else if (sdmmc_cmdtable[cmd].responsetype == R2){
        do{
            // read the first bye of the response.
            // _byte0 transmits first.
            response.r2._byte0 = ReadMedia();
            timeout--;
        } while ((response.r2._byte0 == 0xFF) && (timeout != 0));
        // If the first byte was read, read the second byte.
        if (response.r2._byte0 != 0xFF)
        response.r2._byte1 = ReadMedia();
    }
    
    // Is the response type R1b?
    if (sdmmc_cmdtable[cmd].responsetype == R1b){
        // The R1b response byte has been read.
        // Wait for not busy status by reading from the card until a byte doesn't equal 00h
        // or a timeout occurs..
        response.r1._byte = 0x00;
        for (index = 0; index < 0xFF && response.r1._byte == 0x00; index++){
            timeout = 0xFFFF;
            do{
                response.r1._byte = ReadMedia();
                timeout--;
            } while ((response.r1._byte == 0x00) && (timeout != 0));
        }
    }

    // Generate 8 clock cycles.
    mSend8ClkCycles();
    
    // If no more data is expected for this command, deselect the card.
    if (!(sdmmc_cmdtable[cmd].moredataexpected))
    SDC_CS = 1;
    return(response);
}

SDC_Error CSDRead(void){
    uint32_t       address = 0x00;
    uint8_t     cmd = SEND_CSD;
    CMD_PACKET  CmdPacket;
    uint8_t     data_token;
    uint16_t        index;
    SDC_RESPONSE response;
    SDC_Error   status = sdcValid;
    uint16_t        timeout = 0x2ff;
    
    // Select the card.
    SDC_CS = 0;
    // Store a command byte, address, and CRC value in the CMD_PACKET structure.
    CmdPacket.cmd = sdmmc_cmdtable[cmd].CmdCode;
    CmdPacket.address = address;
    CmdPacket.crc = sdmmc_cmdtable[cmd].CRC;
    // Send the command byte, address bytes, and CRC byte.
    // The WriteSPI library function writes a byte on the SPI bus.
    WriteSPI(CmdPacket.cmd);
    WriteSPI(CmdPacket.addr3);
    WriteSPI(CmdPacket.addr2);
    WriteSPI(CmdPacket.addr1);
    WriteSPI(CmdPacket.addr0);
    WriteSPI(CmdPacket.crc);
    
    // Read a byte from the card until the byte doesn't equal FFh or a timeout occurs.
    do{
        response.r1._byte = ReadMedia();
        timeout--;
    } while ((response.r1._byte == 0xFF) && (timeout != 0));
    
    // A response of 00h means the command was accepted.
    if (response.r1._byte != 0x00){
        status = sdcCardBadCmd;
    }
    
    else{
        index = 0x2FF;
    
        //Wait for the data_start token or a timeout.
        do{
            data_token = ReadMedia();
            index--;
        } while ((data_token == SDC_FLOATING_BUS) && (index != 0));
        
        if ((index == 0) || (data_token != DATA_START_TOKEN))
            status = sdcCardTimeout;
        else
        {
            // A data start token was received.
            // Read the CSD register's 16 bytes.
            for (index = 0; index < CSD_SIZE; index++){
                gblCSDReg._byte[index] = ReadMedia();
            }
        }
    // Generate 8 clock cycles to complete the command.
    mSend8ClkCycles();
    }
    // Deselect the card.
    SDC_CS = 1;
    return(status);
}

SDC_Error SectorRead(uint32_t sector_addr, uint8_t* buffer){
    uint8_t    data_token;
    uint16_t       index;
    SDC_RESPONSE response;
    SDC_Error   status = sdcValid;

    // Issue a READ_SINGLE_BLOCK command.
    // Specify the address of the first byte to read in the media.
    // To obtain the address of a sector?s first byte,
    // shift the sector address left 9 times to multiply by 512 (sector size).
    response = SendSDCCmd(READ_SINGLE_BLOCK, (sector_addr << 9));
    // A response of 00h indicates success.
    if (response.r1._byte != 0x00){
        status = sdcCardBadCmd;
    }
    else{
        // The command was accepted.
        index = 0x2FF;

        do{
            // Read from the card until receiving a response or a timeout.
            data_token = ReadMedia();
            index--;
        } while ((data_token == SDC_FLOATING_BUS) && (index != 0));

        if ((index == 0) || (data_token != DATA_START_TOKEN))
            // The card didn?t send a data start token.
            status = sdcCardTimeout;
        else{
            // The card sent a data start token.
            // Read a sector?s worth of data from the card.
            for (index = 0; index < SDC_SECTOR_SIZE; index++){
                buffer[index] = ReadMedia();
            }
            // Read the CRC bytes.
            mReadCRC();
        }
        // Generate 8 clock cycles to complete the command.
        mSend8ClkCycles();
    }
    // Deselect the card.
    SDC_CS = 1;
    return(status);
}

////////////////////////////

SDC_Error SectorWrite(uint32_t sector_addr, uint8_t* buffer){
    uint8_t    data_response;
    uint16_t index;
    SDC_RESPONSE response;
    SDC_Error   status = sdcValid;
    
    // Issue a WRITE_SINGLE_BLOCK command.
    // Pass the address of the first byte to write in the media.
    // To obtain the address of a sector?s first byte,
    // shift the sector address left 9 times to multiply by 512 (sector size).
    response = SendSDCCmd(WRITE_SINGLE_BLOCK, (sector_addr << 9));
    // A response of 00h indicates success.
    if (response.r1._byte != 0x00)
    status = sdcCardBadCmd;

    else{
        // The command was accepted.
        // Send a data start token.
        WriteSPI(DATA_START_TOKEN);
        // Send a sector?s worth of data.
        for(index = 0; index < 512; index++)
            WriteSPI(buffer[index]);
        // Send the CRC bytes.
        mSendCRC();
        // Read the card?s response.
        data_response = ReadMedia();
        
        if ((data_response & 0x0F) != DATA_ACCEPTED){
            status = sdcCardDataRejected;
        }
        else{
            // The card is writing the data into the storage media.
            // Wait for the card to return non-zero (not busy) or a timeout.
            index = 0;
            do{
                data_response = ReadMedia();
                index++;
            } while ((data_response == 0x00) && (index != 0));
            
            if (index == 0){
                // The write timed out.
                status = sdcCardTimeout;
            }
                
        }
        // The write was successful.
        // Generate 8 clock cycles to complete the command.
        mSend8ClkCycles();
    }
    // Deselect the card.
    SDC_CS = 1;
    return(status);
}




SDC_Error MediaInitialize(SDCSTATE *Flag){
    SDC_Error   CSDstatus = sdcValid;
    SDC_RESPONSE response;
    SDC_Error   status = sdcValid;
    uint16_t    timeout;
    Flag -> _byte = 0x0;
    
    //Initialize the socket
    SocketInitialize();
    
    // Deselect the card.
    SDC_CS = 1;
    // Open the SPI port.
    // Clock speed must be <= 400 kHz until the card is initialized
    // and the CSD register has been read.
    // MultiMediaCards require CKE = 0, CKP = 1,
    // and sampling DataOut in the middle of a clock cyle.
    OpenSPI();
    // Allow the card time to initialize.
    __delay_ms(100);
    
    // Generate clock cycles for 1 millisecond as required by the MultiMediaCard spec.
    for (timeout = 0; timeout < 50; timeout++)
        mSend8ClkCycles();
    
    // Send CMD0 (with CS = 0) to reset the media and put SD cards into SPI mode.
    timeout = 100;
    do
    {
        //Toggle chip select, to make media abandon whatever it may have been doing
        //before.  This ensures the CMD0 is sent freshly after CS is asserted low,
        //minimizing risk of SPI clock pulse master/slave synchronization problems, 
        //due to possible application noise on the SCK line.
        SDC_CS = 1;
        mSend8ClkCycles();                  //Send some "extraneous" clock pulses.  If a previous
                                            //command was terminated before it completed normally,
                                            //the card might not have received the required clocking
                                            //following the transfer.
        SDC_CS = 0;
        timeout--;

        //Send CMD0 to software reset the device
        response = SendSDCCmd(GO_IDLE_STATE, 0x0);
    }while((response.r1._byte != 0x01) && (timeout != 0));
    // A response of 01h means the card is in the idle state and is initializing.
    if (timeout == 0){
        status = sdcCardNotInitFailure;
        goto InitError;
    }
    
    // Issue the SEND_OP_COND command until the card responds or a timeout.
    timeout = 0xFFF;
    do{
        response = SendSDCCmd(SEND_OP_COND, 0x0);
        timeout--;
    } while (response.r1._byte != 0x00 && timeout != 0);
    
    if (timeout == 0){
        status = sdcCardInitTimeout;
        goto InitError;
    }
    else {
        // The command succeeded.
        // Read the CSD register.
        CSDstatus = CSDRead();
        if (!CSDstatus){
            // The response was zero. The CSD was read successfully.
            // OK to increase the clock speed.
            OpenSPI();
        }
        else{
            // Unable to read the CSD.
            status = sdcCardTypeInvalid;
        }
        
    }
    
    // Issue the SET_BLOCKLEN command to set the block length to 512.
    // (Optional, since this is the default.)
    SendSDCCmd(SET_BLOCKLEN, 512);
    // Set a bit in the SDCSTATE structure if the card is write-protected.
    if (IsWriteProtected()){
        Flag -> isWP = true;
    }
    
    // Read sector zero from the card into msd_buffer until success or a timeout.
    // Some cards require multiple attempts.
    for (timeout = 0xFF; timeout > 0 && SectorRead(0x0, (uint8_t*)msd_buffer) != sdcValid; timeout--)
        
    // The attempt to read timed out.
    if (timeout == 0){
        status = sdcCardNotInitFailure;
        goto InitError;
    }
    return(status);
    
    InitError:
    // On error or success, deselect the device.
    SDC_CS = 1;
    return(status);
}

///////////////////////////////////////////////////////////////////////

//For pic18f47q43
uint8_t WriteSPI( uint8_t data_out ){
//    // Write the passed byte to the SPI buffer.
//    SPI1CON2bits.TXR = 1; //Transmit Data-Required Bit;
//    SPI1CON2bits.RXR = 0; // Receive FIFO Space-Required Bit;    
//    SPI1TCNTL = 1;
//    SPI1_WriteByte(data_out);   
//    while(SPI1CON2bits.BUSY==0);
//    while(SPI1CON2bits.BUSY==1);
//
//    return ( 0 );
    
    SPI1TCNTL = 1;
    SPI1TXB = data_out;
    //while(!PIR3bits.SPI1RXIF);
    while(SPI1CON2bits.BUSY==0);
    while(SPI1CON2bits.BUSY==1);
    return SPI1RXB;
    
}

////////////////////////////////////////////////////////////////////

//ReadSPI holds DataIn low while reading a
//byte, and MultiMediaCards require DataIn to be high. The ReadMedia
//function reads a byte while holding DataIn high:


//For pic18f47q43

uint8_t ReadMedia(void){
//    // Write FFh to the SPI buffer to hold the MultiMediaCard?s DataIn line high
//    // while reading a byte.
    
//    SPI1STATUS = 0x00;
//    SPI1CON2bits.TXR = 1; //Transmit Data-Required Bit;
//    SPI1CON2bits.RXR = 1; // Receive FIFO Space-Required Bit;
//    SPI1TCNT = 1; // Load SPI Transfer Counter;
//    SPI1_WriteByte(0xFF);
//    while(SPI1CON2bits.BUSY==0);
//    while(SPI1CON2bits.BUSY==1);
//    //SPI1TXB = data; // Load data into SPI transmit buffer;
//    //while (PIR3bits.SPI1RXIF == 0); // Check for any SPI Receive Interrupts;
//    return (SPI1RXB); // Return data from SPI Receive Buffer Register;
    
   
    SPI1TCNTL = 1;
    SPI1TXB = 0xFF;
    //while(!PIR3bits.SPI1RXIF);
    while(SPI1CON2bits.BUSY==0);
    while(SPI1CON2bits.BUSY==1);
    return SPI1RXB;
}



////////////////////////////////////////////////////////////////////


//For pic18f47q43 and code configurator

void OpenSPI(void){
    SPI1CON1 = 0x20; // CKE = 0; CKP = 1; SDI/SDO Polarity;
    SPI1CON2 = 0x03; // TXR; RXR;
    SPI1BAUD = 0x01; // SPI Baud Pre-Scaler;
    SPI1CLK = 0x02;  // SPI Clock Select;
    SPI1CON0 = 0x82; // SPI Enable; BMODE; Master/Slave; MSb/LSb;
    
    //SPI1_Open(MASTER0_CONFIG);    //funcion de usuario
    
    return;
}

uint32_t MSDReadCapacityHandler(void){
    uint32_t one = 0x1;
    uint32_t C_size;
    uint32_t C_mult;
    uint32_t Mult;
    uint32_t C_size_U;
    uint32_t C_size_H;
    uint32_t C_size_L;
    uint32_t C_mult_H;
    uint32_t C_mult_L;
    uint32_t C_Read_Bl_Len;
    
    // The block length is in byte 5, bits 3..0 in the MultiMediaCard's CSD register.
    // Block length = 2^(C_Read_Bl_Len)
    // If block length = 512, C_Read_Bl_Len = 9 because 2^9 = 512.
    C_Read_Bl_Len = gblCSDReg._byte[5] & 0x0f;
    // Shift left C_Read_Bl_Len positions to get the block-length value.
    gblBLKLen = one << C_Read_Bl_Len;
    
    // The C_size value is 12 bits.
    // The two MSbs are in byte 6, bits 1..0.
    // The next 8 bits are in byte 7.
    // The two LSbs are in byte 8, bits 7..6.
    C_size_U = gblCSDReg._byte[6] & 0x03;
    C_size_H = gblCSDReg._byte[7];
    C_size_L = (gblCSDReg._byte[8]&0xC0) >> 6;
    C_size = (C_size_U<<10) | (C_size_H<<2) | (C_size_L);
    
    // C_mult is a 3-bit value stored in two bytes.
    // The two MSbs are in byte 9, bits 1..0.
    // The LSb is in byte 10, bit 7.
    C_mult_H = gblCSDReg._byte[9] & 0x03;
    C_mult_L = (gblCSDReg._byte[10] & 0x80) >> 7;
    C_mult = (C_mult_H << 1 ) | C_mult_L;

    // See the MultiMediaCard spec, section 5.3, for the calculations below.
    Mult = one << (C_mult + 2);
    
    // Return a value equal to the last LBA - 1.
    gblNumBLKS = Mult * (C_size + 1) - 1;
    
//    // Place gblNumBLKS and gblBLKLen in msd_buffer for sending to the host.
//    msd_buffer[0] = gblNumBLKS.v[3];
//    msd_buffer[1] = gblNumBLKS.v[2];
//    msd_buffer[2] = gblNumBLKS.v[1];
//    msd_buffer[3] = gblNumBLKS.v[0];
//    msd_buffer[4] = gblBLKLen.v[3];
//    msd_buffer[5] = gblBLKLen.v[2];
//    msd_buffer[6] = gblBLKLen.v[1];
//    msd_buffer[7] = gblBLKLen.v[0];
//    
//    // Set fields in the CSW.
//    msd_csw.dCSWDataResidue = 0x08; // Number of bytes in the response.
//    msd_csw.bCSWStatus = 0x00;
    
    return gblNumBLKS;// Success.
}
