/**
* @file t_algo.c
* @brief algo
* @author muse
* @date 2022-03-31 muse: create 
*/
#include "system_config.h"

#ifndef T_ALGO_H_
#define T_ALGO_H_

/**
* @brief CRC16 calculator
* @param[in] buf: input buffer
* @param[in] length: input buffer length
* @return CRC16 value
*/
uint16_t t_algo_calculator_crc16(uint8_t* buf, uint16_t length);

/**
* @brief 将十六进制数据转换为ascii字符串形式存储的十六进制数据
* @param[in] hex_data: 输入十六进制数据
* @param[out] ascii_str: 输出ascii字符串形式存储的十六进制数据
* @param[in] len: 输入十六进制数据长度
* @return 输出ascii字符串形式存储的十六进制数据长度
*/

int t_algo_hex_2_ascii(uint8_t* hex_data, char* ascii_str, int len);

/**
* @brief 将ascii字符串形式存储的十六进制数据转换为十六进制数据
* @param[in] ascii_str:   输入十六进制字符串
* @param[out] hex_data: 输出十六进制数据
* @param[in] len:       输入字符串长度
* @return               输出十六进制数据长度
*/
int t_algo_ascii_2_hex(uint8_t *ascii_str, uint8_t *hex_data, int len);

/**
* @brief 判断字符串是否为数字字符串
* @param[in] str: 输入字符串
* @param[in] len: 输入字符串长度
* @return B_TRUE: 是数字字符串 B_FALSE: 不是数字字符串
*/
BOOL_E t_is_num_string(uint8_t* str, uint8_t len);

/**
* @brief 数字字符串转换为整型数
* @param[in] buf: 输入字符串
* @param[in] len: 输入字符串长度
* @return 整数值
*/
int t_atoi(char* buf, uint8_t len);

#endif

