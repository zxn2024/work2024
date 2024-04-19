#ifndef _M_YMODEM_H
#define _M_YMODEM_H
/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/

#include "MCU_config.h"
/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern uint8_t usart_protocol_direction;

/*********************************************************************
 * MARCOS
 */
#define PACKET_SEQNO_INDEX              (1)
#define PACKET_SEQNO_COMP_INDEX         (2)

#define PACKET_HEADER                   (3)  /* start, block, block-complement */
#define PACKET_TRAILER                  (2) /* CRC bytes */
#define PACKET_OVERHEAD                 (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE                     (128)
#define PACKET_1K_SIZE                  (1024)
#define CCC_MS                          (1000) //tx 'c' in idle state every 1000ms
#define PACKET_TIMEOUT_MS               (10*1000) //timeout 20s quit rx tranfer

#define FILE_NAME_LENGTH                (64)
#define FILE_SIZE_LENGTH                (16)

#define YMODEM_OK                       0
#define YMODEM_ERR                      1       // 校验包是否有问题，只能是“YMODEM_ERR==”而不应该“YMODEM_OK!=”
#define YMODEM_PAC_EMPTY                2 // 包校验正确，但是里面是空值，在（IDLE状态，判断是否需要结束，退出）

#define USART_PROTOCOL_YMODEM_UPLOAD    1
#define USART_PROTOCOL_YMODEM_DNLOAD    2

#define USART_PROTOCOL_DEFAULT          USART_PROTOCOL_YMODEM_DNLOAD

/*********************************************************************
 * TYPE_DEFS
 */
typedef enum
{
    FLASH_ON_CHIP = 0x00U,  // 片内flash
    FLASH_ON_BOARD = 0x01U, // 板上flash器件
    RAM_ON_CHIP = 0x02U, // 板上flash器件
} flash_type_t;

/* ASCII control codes: */
#define SOH         (0x01) /* start of 128-byte data packet */
#define STX         (0x02) /* start of 1024-byte data packet */
#define EOT         (0x04) /* end of transmission */
#define ACK         (0x06) /* receive OK */
#define NAK         (0x15) /* receiver error; retry */
#define CAN         (0x18) /* two of these in succession aborts transfer */
#define CNC         (0x43) /* character 'C' */

/* Number of consecutive receive errors before giving up: */
#define MAX_ERRORS  (5)

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
//*注：接收——只有ymodem_rx_start()是接收到消息的时候调用，其它都是用户实现，ymodem自动调用
//*注：发送——用户调用ymodem_tx_header()只有ymodem_tx_start()是接收到消息的时候调用，其它都是用户实现，ymodem自动调用
void ymodem_rx_pack(char * buf, size_t rx_sz);
void ymodem_tx_pack(char * buf, size_t rx_sz);
size_t ymodem_rx_pac_split(char * buf, size_t sz);
void ymodem_CCC(void);

// 功能函数
uint8_t ymodem_rx_file_init(char * fil_nm, size_t fil_sz);                   // 当检测到包头的时候，自动调用此函数（打开文件）
uint8_t ymodem_rx_finish_handler(uint8_t status);                            // 返回结束原因，成功还是出错（关闭文件）
uint8_t ymodem_rx_pac_store_handler(char * buf, size_t offset, size_t size); //(写文件)
// 发送函数
uint8_t ymodem_tx_file_init(char ** fil_nm, size_t * fil_sz);                // 系统调用，用来获取文件名称和大小（用来封头包）
uint8_t ymodem_tx_finish_handler(uint8_t status);                            // 返回结束原因，成功还是出错(关闭文件)
uint8_t ymodem_tx_pac_store_handler(char * buf, size_t offset, size_t sz_st); // 得到待传输的数据(读文件)
// 底层函数
void __putchar(char ch);
void __putbuf(char * buf, size_t len);

// 用户接口
void ymodem_set_protocol_dir(uint8_t direction);                      // 设置传输方向 收或发 USART_PROTOCOL_YMODEM_UPLOAD 1  USART_PROTOCOL_YMODEM_DNLOAD 2
void ymodem_set_rx_flash_addr(flash_type_t type, uint32_t dest_addr); // 设置flash接收的设备(片内/片外)和地址
void ymodem_run(char * data, size_t len);                             // ymodem运行函数,当工作时需要以小于50ms的周期循环调用,有数据时输入数据,无数据时参数为(NULL,0)
#endif
