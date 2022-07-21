/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <stdbool.h>
#include <stdint.h>
#include "../../spisdcard.h"

bool sdCard_IsMediaPresent(void)
{
    return DetectSDCard();
    #warning "You will need to implement this function for this driver to work."
    return false;
}

bool sdCard_MediaInitialize(void)
{
   SDCSTATE flagInitSD;
    if(MediaInitialize(&flagInitSD)==0){
       return true;
    }
    
    #warning "You will need to implement this function for this driver to work."
    return false;
}

bool sdCard_IsMediaInitialized(void)
{
    if(SPI1CON0bits.EN == 1){
        return true;
    }
    #warning "You will need to implement this function for this driver to work."
    return false; 
}

bool sdCard_IsWriteProtected(void)
{
    return IsWriteProtected();
    #warning "You will need to implement this function for this driver to work."
    return false;
}

uint16_t sdCard_GetSectorSize(void)
{
    #warning "You will need to implement this function for this driver to work."
    return 512;
}

uint32_t sdCard_GetSectorCount(void)
{
    return MSDReadCapacityHandler();
    #warning "You will need to implement this function for this driver to work."
    return 0;
}

bool sdCard_SectorRead(uint32_t sector_address, uint8_t* buffer, uint16_t sector_count)
{
    if(SectorRead(sector_address, buffer)==0){
        return true;
    }
    #warning "You will need to implement this function for this driver to work."
    return false;
}

bool sdCard_SectorWrite(uint32_t sector_address, const uint8_t* buffer, uint16_t sector_count)
{
    if(SectorWrite(sector_address, buffer)==0){
        return true;
    }
    #warning "You will need to implement this function for this driver to work."
    return false;
}
