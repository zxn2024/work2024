/***************************** Copyright ****************************************
*
* (C) Copyright 2023, Shaanxi Tianji Communication
*  All Rights Reserved
*
* FileName   : bsp_testmode_data_rw.c
* Version    : none
* Author     :
* Date       : 2023-12-13
* Description: none
*******************************************************************************/
#include "main.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "string.h"
//#include "MCU_config.h"
#include "public_define.h"
#include "public_enum.h"
#include "public_struct.h"
#include "bsp_testmode_data_rw.h"
#include "bsp_testmode_define.h"



/*---------------------- 端口定义 --------------------------------------*/


/*---------------------- 宏定义  ---------------------------------------*/


/*---------------------- 结构体 ----------------------------------------*/


/*---------------------- 常量 ------------------------------------------*/


/*---------------------- 全局变量 --------------------------------------*/


/*---------------------- 局部变量 --------------------------------------*/


/*---------------------- 声明 ------------------------------------------*/



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DRV ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ APP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//宏定义代替代码段
#define TM_DATA_ERR_CHECK( cmd, value, min, max )	if( TM_CMD_SET == cmd )					\
													{										\
														if( value < min )					\
														{									\
															err = TM_ERR_VALUE_L;			\
														}									\
														else if( value > max )				\
														{									\
															err = TM_ERR_VALUE_H;			\
														}									\
														else								\
														{									\
															err = TM_ERR_OK;				\
														}									\
													}										\
													else									\
													{										\
														if((value < min) || (value > max))	\
														   err = TM_ERR_DATA_INVALID;		\
													}


/*****************************************************************************
* @Name	  : tm_data_valid_check
* @Brief  : 校验数据是否在有效的范围之内
* @Param  : cmdRw: [输入/出]
**			 src: [输入/出]
**			 tag: [输入/出]
**			 type: [输入/出]
**			 min: [输入/出]
**			 max: [输入/出]
* @Retval :
* @Author :
* @Data   : 2023-05-16
* @Note   :
*****************************************************************************/
uint8_t tm_data_valid_check(uint8_t cmd, tm_oid_data_t * pt_src, void * tag, uint8_t type, int32_t min, int32_t max)
{
    uint8_t   err = TM_ERR_OK;

    uint8_t  value_uint1, min_uint1, max_uint1;
    uint16_t value_uint2, min_uint2, max_uint2;
    uint32_t value_uint4, min_uint4, max_uint4;
    int8_t   value_int1,  min_int1,  max_int1;
    int16_t  value_int2,  min_int2,  max_int2;
    int32_t  value_int4,  min_int4,  max_int4;

    uint8_t * pst_dat = &pt_src->dat;

    switch(type)
    {
        case TM_BOOL:
            break;

        case TM_UINT1:
            if(TM_CMD_SET == cmd)
                value_uint1 = *(uint8_t *)pst_dat;
            else
                value_uint1 = *(uint8_t *)tag;
            
            min_uint1   = (uint8_t)min;
            max_uint1   = (uint8_t)max;
            if(min_uint1 < max_uint1)
            {
                TM_DATA_ERR_CHECK(cmd, value_uint1, min_uint1, max_uint1);
            }
            break;

        case TM_SINT1:
            if(TM_CMD_SET == cmd)
                value_int1 = *(int8_t *)pst_dat;
            else
                value_int1 = *(int8_t *)tag;
        
            min_int1   = (int8_t)min;
            max_int1   = (int8_t)max;
            if(min_int1 < max_int1)
            {
                TM_DATA_ERR_CHECK(cmd, value_int1, min_int1, max_int1);
            }
            break;

        case TM_UINT2:
            if(TM_CMD_SET == cmd)
                value_uint2 = *(uint16_t *)pst_dat;
            else
                value_uint2 = *(uint16_t *)tag;
            
            min_uint2   = (uint16_t)min;
            max_uint2   = (uint16_t)max;
            if(min_uint2 < max_uint2)
            {
                TM_DATA_ERR_CHECK(cmd, value_uint2, min_uint2, max_uint2);
            }
            break;

        case TM_SINT2:
            if(TM_CMD_SET == cmd)
                value_int2 = *(int16_t *)pst_dat;
            else
                value_int2 = *(int16_t *)tag;
            
            min_int2   = (int16_t)min;
            max_int2   = (int16_t)max;
            if(min_int2 < max_int2)
            {
                TM_DATA_ERR_CHECK(cmd, value_int2, min_int2, max_int2);
            }
            break;

        case TM_UINT4:
            if(TM_CMD_SET == cmd)
                value_uint4 = *(uint32_t *)pst_dat;
            else
                value_uint4 = *(uint32_t *)tag;
            
            min_uint4   = (uint32_t)min;
            max_uint4   = (uint32_t)max;
            if(min_uint4 < max_uint4)
            {
                TM_DATA_ERR_CHECK(cmd, value_uint4, min_uint4, max_uint4);
            }
            break;

        case TM_SINT4:
            if(TM_CMD_SET == cmd)
                value_int4 = *(int32_t *)pst_dat;
            else
                value_int4 = *(int32_t *)tag;
            
            min_int4   = (int32_t)min;
            max_int4   = (int32_t)max;
            if(min_int4 < max_int4)
            {
                TM_DATA_ERR_CHECK(cmd, value_int4, min_int4, max_int4);
            }
            break;

        case TM_STR:
            break;

        default:
            break;
    }

    return err;
}

/*****************************************************************************
* @Name	  : tm_pud_copy_data
* @Brief  : 拷贝数据
* @Param  :  cmd:    读写命令
**			 pt_src: 源数据
**			 tag:    设备数据
**			 type:   数据类型
**			 len:    数据长度(按字节算)
* @Retval :
* @Author :
* @Data   : 2023-05-16
* @Note   :
*****************************************************************************/
void tm_pud_copy_data(uint8_t cmd, tm_oid_data_t * pt_src, void * tag, uint8_t type, uint8_t len)
{
    uint32_t i = 0;
    uint8_t * pst_dat = &pt_src->dat;

    switch(type)
    {
        case TM_BOOL:
            if(TM_CMD_SET == cmd)
            {
                *(uint8_t *)tag = (*pst_dat == 0) ? 0 : 1;
            }
            else
            {
                *pst_dat = *(uint8_t *)tag == 0 ? 0 : 1;
            }
            break;

        case TM_UINT1:
        case TM_UINT2:
        case TM_UINT4:
            if(TM_CMD_SET == cmd)
            {
                for(i = 0; i < len; i++)
                    *((uint8_t *)tag + i) = *(pst_dat + i);
            }
            else
            {
                for(i = 0; i < len; i++)
                    *(pst_dat + i) = *((uint8_t *)tag + i);
            }
            break;

        case TM_SINT1:
        case TM_SINT2:
        case TM_SINT4:
            if(TM_CMD_SET == cmd)
            {
                for(i = 0; i < len; i++)
                    *((int8_t *)tag + i) = *((int8_t *)pst_dat + i);
            }
            else
            {
                for(i = 0; i < len; i++)
                    *((int8_t *)pst_dat + i) = *((int8_t *)tag + i);
            }
            break;

        case TM_STR:
            if(TM_CMD_SET == cmd)
            {
                for(i = 0; i < (pt_src->len - 6); i++)
                    *((int8_t *)tag + i) = *((int8_t *)pst_dat + i);
            }
            else
            {
                for(i = 0; i < (pt_src->len - 6); i++)
                {
                    *((int8_t *)pst_dat + i) = *((int8_t *)tag + i);
                    /*
                    //位置信息中有效数据存在0x00的情况
                    if(i < strlen(tag))// 获取实际长度
                    {-
                        *((int8_t *)pst_dat + i) = *((int8_t *)tag + i);
                    }
                    else
                    {
                        *((int8_t *)pst_dat + i) = 0x00;    //超过实际长度用0x00填充
                    }
                    */
                }
            }
            break;

        default:
            break;
    }
}


/*****************************************************************************
* @Name	  : tm_pdu_data_rw
* @Brief  :
* @Param  :  rws: 读写属性
**			 cmd: 读写选择
**			 pt_src: 源数据
**			 tag: 指向设备参数
**			 type: 数据类型
**			 len: 数据长度
**			 min: 最小值
**			 max: 最大值
**			 save: 是否保持
* @Retval :  TM_CMD_ACT_OK( 正常执行 ) / TM_CMD_ACT_CONDITION ( 有条件执行 )
* @Author :
* @Data   : 2023-12-13
* @Note   :
*****************************************************************************/
uint8_t tm_pdu_data_rw(uint8_t rws, uint8_t cmd, tm_oid_data_t * pt_src, void * tag, uint8_t type, uint16_t len, int32_t min, int32_t max, uint8_t * save)
{
    uint8_t res = TM_CMD_ACT_OK;
    uint8_t err = TM_ERR_OK;

//	cmdRw += 2; //将READ/WRITE 与PDU_READ/PDU_WRITE相匹配
    if(((TM_CMD_SET == cmd) && (TM_R == rws))
        || ((TM_CMD_QUERY == cmd) && (TM_W == rws))
        || ((TM_CMD_SET != cmd) && (TM_CMD_QUERY != cmd)))
    {
        err = TM_ERR_WIDTH_ERR;   //命令错
    }

    if(type >= TM_DATA_TYPE_END)
        return TM_ERR_OTHER;

    if((pt_src->len < 7) || (pt_src->len > (len + 6)))
    {
        pt_src->sys |= TM_ERR_LEN_ERR; //长度错误
        return TM_CMD_ACT_CONDITION;
    }

    if((min == 0) && (max == 0))
        err = 0;
    else
        err = tm_data_valid_check(cmd, pt_src, tag, type, min, max);	//判断数据是否在有效范围内

    if(err)
    {
        pt_src->sys |= err;
        res = TM_CMD_ACT_CONDITION;    //命令被有条件执行
    }
    else
    {
        tm_pud_copy_data(cmd, pt_src, tag, type, pt_src->len - 6);
        if((rws == TM_RW) && (TM_CMD_SET == cmd))
            *save |= 1;
        res = TM_CMD_ACT_OK;   //可读可写的参数需要保存, 读的时候不需要, 只写的参数也不需要保存
    }

    return res;
}


