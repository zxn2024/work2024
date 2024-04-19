/**
* @file t_algo.c
* @brief algo
* @author muse
* @date 2022-03-31 muse: create 
*/
#include "ascii_hex.h"

/**
* @brief CRC16 calculator
* @param[in] buf: input buffer
* @param[in] length: input buffer length
* @return CRC16 value
*/
uint16_t t_algo_calculator_crc16(uint8_t* buf, uint16_t length)
{
	uint16_t value, crc_reg = 0x0000;

	for (uint16_t i = 0; i < length; i ++) 
	{
		value = buf[i] << 8;
		for (uint8_t j = 0; j < 8; j ++) 
		{
			if ((int16_t)(crc_reg ^ value) < 0)
			{
				crc_reg = (crc_reg << 1) ^ 0x1021;
			}
			else
			{
				crc_reg <<= 1; 
			}
			value <<= 1; 
		}
	}
	
	return crc_reg;
}

    
/**
* @brief 将十六进制数据转换为ascii字符串形式存储的十六进制数据
* @param[in] hex_data: 输入十六进制数据
* @param[out] ascii_str: 输出ascii字符串形式存储的十六进制数据
* @param[in] len: 输入十六进制数据长度
* @return 输出ascii字符串形式存储的十六进制数据长度
*/

int t_algo_hex_2_ascii(uint8_t* hex_data, char* ascii_str, int len) 
{ 
	const char  ascTable[17] = "0123456789ABCDEF"; 
	char* tmp_p = ascii_str; 
	int i, pos; 

	pos = 0; 
	for(i = 0; i < len; i++) 
	{ 
		tmp_p[pos++] = ascTable[hex_data[i] >> 4]; 
		tmp_p[pos++] = ascTable[hex_data[i] & 0x0f]; 
	} 

	tmp_p[pos] = '\0'; 
	return pos;               
}

/**
* @brief 将ascii字符串形式存储的十六进制数据转换为十六进制数据
* @param[in] ascii_str:   输入十六进制字符串
* @param[out] hex_data: 输出十六进制数据
* @param[in] len:       输入字符串长度
* @return               输出十六进制数据长度
*/
int t_algo_ascii_2_hex(uint8_t *ascii_str, uint8_t *hex_data, int len) 
{ 
	int i,j,tmp_len;
	uint8_t tmpData; 
	uint8_t *in_buf = ascii_str;
	uint8_t *out_buf = hex_data;

	for(i = 0; i < len; i++)
	{
		if ((in_buf[i] >= '0') && (in_buf[i] <= '9')) 
		{ 
			tmpData = in_buf[i] - '0'; 
		} 
		else  if ((in_buf[i] >= 'A') && (in_buf[i] <= 'F'))  //A....F 
		{ 
			tmpData = in_buf[i] - 0x37; 
		} 
		else if((in_buf[i] >= 'a') && (in_buf[i] <= 'f'))  //a....f 
		{ 
			tmpData = in_buf[i] - 0x57; 
		} 
		else 
		{ 
			return -1; 
		} 
		
		in_buf[i] =  tmpData; 
	} 
	for(tmp_len = 0,j = 0; j < i; j+=2) 
	{ 
		out_buf[tmp_len++] = (in_buf[j]<<4) | in_buf[j+1]; 
	} 
	
	return tmp_len;  
}

/**
* @brief 判断字符串是否为数字字符串
* @param[in] str: 输入字符串
* @param[in] len: 输入字符串长度
* @return B_TRUE: 是数字字符串 B_FALSE: 不是数字字符串
*/
BOOL_E t_is_num_string(uint8_t* str, uint8_t len)
{
    uint8_t i;

    for(i = 0; i < len; i++)
    {
        if(str[i] < 0x30 || str[i] > 0x39)
        {
            return B_FALSE;
        }
    }

    return B_TRUE;
}

int t_atoi(char* buf, uint8_t len)
{
    uint8_t pos;
    int result = 0;
    int factor = 1;

    if(len == 0)
    {
        return result;
    }

    if(buf[0] == '-')
    {
        for(pos = 0; pos < len - 1; pos++)
        {
            result += (buf[len - 1 - pos] - '0') * factor;
            factor *= 10;
        }
        result = -result;
    }
    else
    {
        for(pos = 0; pos < len; pos++)
        {
            result += (buf[len - 1 - pos] - '0') * factor;
            factor *= 10;
        }
    }

    return result;
}

