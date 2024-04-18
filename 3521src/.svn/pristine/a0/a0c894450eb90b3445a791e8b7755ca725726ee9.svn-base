/**
 * **********************************************************************************
 * @copyright Copyright (c) 2023 Shaanxi Tianji Communication Technologies Co., Ltd.
 *                         
 * @file        msg_handle.c   
 * @author      WangJun
 * @version     None
 * @date        2023-09-14     
 * @brief       None   
 * 
 * **********************************************************************************
 */

#include "msg_handle.h"


/*---------------------- 端口定义 ----------------------*/


/*---------------------- 宏 定 义 ----------------------*/


/*---------------------- 结 构 体 ----------------------*/


/*---------------------- 常    量 ----------------------*/


/*---------------------- 全局变量 ----------------------*/


/*---------------------- 局部变量 ----------------------*/


/*---------------------- 函    数 ----------------------*/
/*****************************************************************************
* @Name	  : 合并
* @Brief  : 字符转义，0x5E,0x5D->0x5E, 0x5E,0x7D->0x7E
* @Param  : src: 指向源数据
*			dest: 指向转义后数据
*			len: 源数据长度
* @Retval :
* @Data   : 2022-08-22
* @Note   :
*****************************************************************************/
uint16_t msg_split_escape(uint8_t * src, uint8_t * dest, uint16_t len)
{
    uint16_t n, i;

    for(n = 0, i = 0; n < len; n++, i++)
    {
        if((*(src + n) == 0x5E) && (*(src + n + 1) == 0x5D))
        {
            dest[i] = 0x5E;
            n++;
        }
        else if((*(src + n) == 0x5E) && (*(src + n + 1) == 0x7D))
        {
            dest[i] = 0x7E;
            n++;
        }
        else
            dest[i] = *(src + n);
    }

    return i;
}


/*****************************************************************************
* @Name	  : 拆分
* @Brief  : 字符转义，0x5E->0x5E,0x5D, 0x7E->0x5E,0x7D
* @Param  : src: 指向待转换的数据
*			len: 待转换数据长度
* @Retval : 转换后数据长度
* @Data   : 2022-08-22
* @Note   :
*****************************************************************************/
uint16_t msg_escape(uint8_t * src, uint16_t len)
{
#define MAX_LENGTH_ESCAPE   (1500)

    uint16_t n = 0, i = 0;
    uint8_t buf_temp[MAX_LENGTH_ESCAPE] = {0};
    uint16_t len_result = 0;

    if(len <= 10 || len >= MAX_LENGTH_ESCAPE)
        return 0;

    if((*src == 0x7E) && (*(src + len - 1) == 0x7E))
    {
        buf_temp[0] = 0x7E;
        
        for(n = 1, i = 1; n <= len - 2; n++, i++)
        {
            if(*(src + n) == 0x5E)
            {
                buf_temp[i++] = 0x5E;
                buf_temp[i] = 0x5D;
            }
            else if(*(src + n) == 0x7E)
            {
                buf_temp[i++] = 0x5E;
                buf_temp[i] = 0x7D;
            }
            else
                buf_temp[i] = *(src + n);
        }
        buf_temp[i] = 0x7E;
        len_result = i + 1;
        memcpy(src, &buf_temp[0], len_result);
        
        return len_result;
    }
    else
        return 0;
}


/*****************************************************************************
* @Name	  : msg_split_print
* @Brief  : 将不可见字符串拆分成可见字符串，大于0x39的减去0x37，其余的减去0x30
* @Param  : src: 指向源数据
*			dst: 指向拆分后数据
*			len: 源数据长度
* @Retval : 
* @Data   : 2022-08-22
* @Note   : 
*****************************************************************************/
uint16_t msg_split_print(uint8_t * src, uint8_t * dst, uint16_t len)
{
    uint16_t n = 0;
    uint16_t i = 0;
    uint8_t dh, dl;

    if((src == NULL) || (dst == NULL) || (len == 0))
    {
        return 0;
    }

    for(i = 0, n = 0; i < len; i++)
    {
        dh = (*(src + i) >> 4) & 0x0F;
        dl = *(src + i) & 0x0F;

        if(dh > 0x09)
            *(dst + n) = dh + 0x37;
        else
            *(dst + n) = dh + 0x30;
        n++;
        if(dl > 0x09)
            *(dst + n) = dl + 0x37;
        else
            *(dst + n) = dl + 0x30;
        n++;
    }

    return n;
}
