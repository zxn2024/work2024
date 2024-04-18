/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        crc.c   
 * @author      WangJun
 * @version     None
 * @date        2023-09-11     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "crc.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/
#define CRC_16_POLY 0x1021      //CRC16多项式
#define CRC_32_POLY 0x04C11DB7	//CRC32多项式

/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @Name	  : crc16
* @Brief  : 
* @Param  : pchMsg: [输入/出] 
**			 wDataLen: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-07-24
* @Note   :
*****************************************************************************/
uint16_t crc16( uint8_t *pchMsg, uint16_t wDataLen )
{
    uint16_t CrcResult=0;
    uint8_t i;

    while( wDataLen-- )
    {
        for( i=0x80; i!=0; i>>=1 )
        {
            if( CrcResult & 0x8000 )
            {
                CrcResult <<= 1;
                CrcResult ^= CRC_16_POLY;
            }
            else
                CrcResult <<= 1;
			
            if( (*pchMsg & i) != 0 )
                CrcResult ^= CRC_16_POLY;
        }
        pchMsg++;
    }
    return CrcResult;
}



/*****************************************************************************
* @Name	  : crc32
* @Brief  : 
* @Param  : input: [输入/出] 
**			 len: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2023-07-24
* @Note   :
*****************************************************************************/
uint32_t crc32( void* input, uint32_t len )
{

    uint32_t crc = 0xFFFFFFFF;
	uint8_t *pt = (uint8_t *)input;
	
    for( uint32_t i = 0; i < len; i++ )
    {
        crc = crc ^ *( pt + i );
		
        for( uint8_t j = 0; j < 8; ++j )
        {
            crc = (crc >> 1) ^ ( 0xEDB88320 & ( -( crc & 1 ) ) );
        }
    }
    return crc ^ 0xFFFFFFFF;
}

/*****************************************************************************
* @Name	  : crc32_s
* @Brief  : 分段计算crc32
* @Param  : crc: 上一次计算的crc32结果
**			input: [输入/出] 
**			len: [输入/出] 
* @Retval : 
* @Author : 
* @Data   : 2024-02-27
* @Note   :
*****************************************************************************/
uint32_t crc32_s(uint32_t crc, void* input, uint32_t len )
{
	uint8_t *pt = (uint8_t *)input;
	
	crc = crc ^ 0xFFFFFFFF;
    for( uint32_t i = 0; i < len; i++ )
    {
        crc = crc ^ *( pt + i );
		
        for( uint8_t j = 0; j < 8; ++j )
        {
            crc = (crc >> 1) ^ ( 0xEDB88320 & ( -( crc & 1 ) ) );
        }
    }
    return crc ^ 0xFFFFFFFF;
}
