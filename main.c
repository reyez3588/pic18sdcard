/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F47Q43
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "xlcd.h"
#include "mcc_generated_files/examples/spi_master_example.h"

#include "spisdcard.h"
#include "mcc_generated_files/fatfs/fatfs_demo.h"

/*
                         Main application
 */



void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    
    OSCENbits.MFOEN = 1; //habilitar el MFOSCINT 500kHz
    while(!OSCSTATbits.MFOR)
    {
        //Esperar a que el oscilador interno este listo
    }
    
    OpenXLCD(EIGHT_BIT&LINES_5X7); //Inicializar el LCD
    putrsXLCD("micro SD"); //Mandar datos al LCD
    __delay_ms(1000);
 
    
    
    
    FatFsDemo_Tasks();
    
//    SDCSTATE flagInitSD;
//    if(MediaInitialize(&flagInitSD)==0){
//        LED_SetLow();
//    }
     
    while (1)
    {
        // Add your application code
    }
}

//void MSDReadCapacityHandler()
//{
//    uint32_t one = 0x1;
//    uint32_t C_size;
//    uint32_t C_mult;
//    uint32_t Mult;
//    uint32_t C_size_U;
//    uint32_t C_size_H;
//    uint32_t C_size_L;
//    uint32_t C_mult_H;
//    uint32_t C_mult_L;
//    uint32_t C_Read_Bl_Len;
//    
//    // The block length is in byte 5, bits 3..0 in the MultiMediaCard's CSD register.
//    // Block length = 2^(C_Read_Bl_Len)
//    // If block length = 512, C_Read_Bl_Len = 9 because 2^9 = 512.
//    C_Read_Bl_Len = gblCSDReg._byte[5] & 0x0f;
//    // Shift left C_Read_Bl_Len positions to get the block-length value.
//    gblBLKLen._dword = one << C_Read_Bl_Len;
//    
//    // The C_size value is 12 bits.
//    // The two MSbs are in byte 6, bits 1..0.
//    // The next 8 bits are in byte 7.
//    // The two LSbs are in byte 8, bits 7..6.
//    C_size_U = gblCSDReg._byte[6] & 0x03;
//    C_size_H = gblCSDReg._byte[7];
//    C_size_L = (gblCSDReg._byte[8]&0xC0) >> 6;
//    C_size = (C_size_U<<10) | (C_size_H<<2) | (C_size_L);
//    
//    // C_mult is a 3-bit value stored in two bytes.
//    // The two MSbs are in byte 9, bits 1..0.
//    // The LSb is in byte 10, bit 7.
//    C_mult_H = gblCSDReg._byte[9] & 0x03;
//    C_mult_L = (gblCSDReg._byte[10] & 0x80) >> 7;
//    C_mult = (C_mult_H << 1 ) | C_mult_L;
//
//    // See the MultiMediaCard spec, section 5.3, for the calculations below.
//    Mult = one << (C_mult + 2);
//    
//    // Return a value equal to the last LBA - 1.
//    gblNumBLKS._dword = Mult * (C_size + 1) - 1;
//    
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
//    
//    return;// Success.
//}




/**
 End of File
*/

