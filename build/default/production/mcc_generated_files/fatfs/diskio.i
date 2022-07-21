# 1 "mcc_generated_files/fatfs/diskio.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/root/.mchp_packs/Microchip/PIC18F-Q_DFP/1.12.193/xc8/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "mcc_generated_files/fatfs/diskio.c" 2
# 10 "mcc_generated_files/fatfs/diskio.c"
# 1 "mcc_generated_files/fatfs/diskio.h" 1
# 12 "mcc_generated_files/fatfs/diskio.h"
# 1 "mcc_generated_files/fatfs/integer.h" 1
# 16 "mcc_generated_files/fatfs/integer.h"
typedef int INT;
typedef unsigned int UINT;


typedef unsigned char BYTE;


typedef short SHORT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;


typedef long LONG;
typedef unsigned long DWORD;


typedef unsigned long long QWORD;
# 12 "mcc_generated_files/fatfs/diskio.h" 2




typedef BYTE DSTATUS;


typedef enum {
 RES_OK = 0,
 RES_ERROR,
 RES_WRPRT,
 RES_NOTRDY,
 RES_PARERR
} DRESULT;






DSTATUS disk_initialize (BYTE pdrv);
DSTATUS disk_status (BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);
# 10 "mcc_generated_files/fatfs/diskio.c" 2

# 1 "mcc_generated_files/fatfs/sdcard.h" 1
# 26 "mcc_generated_files/fatfs/sdcard.h"
# 1 "/opt/microchip/xc8/v2.32/pic/include/c99/stdbool.h" 1 3
# 26 "mcc_generated_files/fatfs/sdcard.h" 2

# 1 "/opt/microchip/xc8/v2.32/pic/include/c99/stdint.h" 1 3



# 1 "/opt/microchip/xc8/v2.32/pic/include/c99/musl_xc8.h" 1 3
# 5 "/opt/microchip/xc8/v2.32/pic/include/c99/stdint.h" 2 3
# 22 "/opt/microchip/xc8/v2.32/pic/include/c99/stdint.h" 3
# 1 "/opt/microchip/xc8/v2.32/pic/include/c99/bits/alltypes.h" 1 3
# 127 "/opt/microchip/xc8/v2.32/pic/include/c99/bits/alltypes.h" 3
typedef unsigned long uintptr_t;
# 142 "/opt/microchip/xc8/v2.32/pic/include/c99/bits/alltypes.h" 3
typedef long intptr_t;
# 158 "/opt/microchip/xc8/v2.32/pic/include/c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 188 "/opt/microchip/xc8/v2.32/pic/include/c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 229 "/opt/microchip/xc8/v2.32/pic/include/c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 23 "/opt/microchip/xc8/v2.32/pic/include/c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 144 "/opt/microchip/xc8/v2.32/pic/include/c99/stdint.h" 3
# 1 "/opt/microchip/xc8/v2.32/pic/include/c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 145 "/opt/microchip/xc8/v2.32/pic/include/c99/stdint.h" 2 3
# 27 "mcc_generated_files/fatfs/sdcard.h" 2


_Bool sdCard_IsMediaPresent(void);
_Bool sdCard_MediaInitialize(void);
_Bool sdCard_IsMediaInitialized(void);

_Bool sdCard_IsWriteProtected(void);
uint16_t sdCard_GetSectorSize(void);
uint32_t sdCard_GetSectorCount(void);

_Bool sdCard_SectorRead(uint32_t sector_address, uint8_t* buffer, uint16_t sector_count);
_Bool sdCard_SectorWrite(uint32_t sector_address, const uint8_t* buffer, uint16_t sector_count);
# 11 "mcc_generated_files/fatfs/diskio.c" 2




enum DRIVER_LIST{
    sdCard = 0,
};





DSTATUS disk_status (
    BYTE pdrv
)
{
    DSTATUS stat = 0x01;

    switch (pdrv) {

        case sdCard:
            if ( sdCard_IsMediaPresent() == 0)
            {
               stat = 0x02;
            }
            else if ( sdCard_IsMediaInitialized() == 1)
            {
                stat &= ~0x01;
            }

            if ( sdCard_IsWriteProtected() == 1)
            {
                stat |= 0x04;
            }

            break;

        default:
            break;
    }
    return stat;
}







DSTATUS disk_initialize (
    BYTE pdrv
)
{
    DSTATUS stat = 0x01;

    switch (pdrv) {
        case sdCard :
            if(sdCard_MediaInitialize() == 1)
            {
                stat = RES_OK;
            }
            else
            {
                stat = RES_ERROR;
            }
            break;
        default:
            break;
    }

    return stat;
}







DRESULT disk_read (
    BYTE pdrv,
    BYTE *buff,
    DWORD sector,
    UINT count
)
{
    DRESULT res = RES_PARERR;

    switch (pdrv) {
        case sdCard :
            if(sdCard_SectorRead(sector, buff, count) == 1)
            {
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;

        default:
            break;
    }

    return res;
}







DRESULT disk_write (
    BYTE pdrv,
    const BYTE *buff,
    DWORD sector,
    UINT count
)
{
    DRESULT res = RES_PARERR;

    switch (pdrv) {
        case sdCard :
            if(sdCard_SectorWrite(sector, buff, count) == 1)
            {
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;

        default:
            break;
    }

    return res;
}







DRESULT disk_ioctl (
    BYTE pdrv,
    BYTE cmd,
    void *buff
)
{
    DRESULT res = RES_OK;

    switch (pdrv) {
        case sdCard :
            return res;

        default:
            break;
    }

    return RES_PARERR;
}
