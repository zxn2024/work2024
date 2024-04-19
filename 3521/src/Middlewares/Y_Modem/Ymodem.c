/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "Ymodem.h"
#include "rtt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hi_systick.h"
/*********************************************************************
 * CONSTANTS
 */
#define YMODEM_DATA_SIZE_128 128
#define YMODEM_DATA_SIZE_1024 1024

#define YMODEM_RX_IDLE 0
#define YMODEM_RX_ACK 1
#define YMODEM_RX_EOT 2
#define YMODEM_RX_ERR 3
#define YMODEM_RX_EXIT 4

#define YMODEM_TX_IDLE 0
#define YMODEM_TX_IDLE_ACK 1
#define YMODEM_TX_DATA 2
#define YMODEM_TX_DATA_ACK 3
#define YMODEM_TX_EOT 4
#define YMODEM_TX_ERR 5
#define YMODEM_TX_EXIT 6
/*********************************************************************
 * GLOBAL VARIABLES
 */
static uint8_t ym_rx_status = YMODEM_RX_IDLE;
static uint8_t ym_tx_status = YMODEM_RX_IDLE;
static size_t pac_size;
static size_t ym_tx_fil_sz;
static char ym_tx_pbuf[1100];
static uint8_t ym_cyc; // 发送时的轮转变量
uint8_t usart_protocol_direction = USART_PROTOCOL_DEFAULT;

static size_t ym_rx_fil_sz;
static size_t seek;
static int32_t cur_pack_num = -1;
static char rx_fil_nm[128] = {0}; //传输的文件名
/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 *********************************************************************/
uint8_t ymodem_rx_exit(void);
/*********************************************************************
 * TYPE_DEFS
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
uint8_t ymodem_rx_file_init(char * fil_nm, size_t fil_sz);
uint8_t ymodem_rx_finish_handler(uint8_t status);
uint8_t ymodem_rx_pac_store_handler(char * buf, size_t seek, size_t size);
uint8_t ymodem_tx_file_init(char ** fil_nm, size_t * fil_sz);
uint8_t ymodem_tx_finish_handler(uint8_t status);
uint8_t ymodem_tx_pac_store_handler(char * buf, size_t seek, size_t size);
void __putchar(char ch);
void __putbuf(char * buf, size_t len);

// 核心函数
unsigned short crc16_cal(const unsigned char * buf, unsigned long count)
{
    unsigned short crc = 0;
    int i;

    while(count--)
    {
        crc = crc ^ *buf++ << 8;

        for(i = 0; i < 8; i++)
        {
            if(crc & 0x8000)
            {
                crc = crc << 1 ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

static const char * u32_to_str(unsigned int val)
{
    /* Maximum number of decimal digits in u32 is 10 */
    static char num_str[11];
    int pos = 10;
    num_str[10] = 0;

    if(val == 0)
    {
        /* If already zero then just return zero */
        return "0";
    }

    while((val != 0) && (pos > 0))
    {
        num_str[--pos] = (val % 10) + '0';
        val /= 10;
    }

    return &num_str[pos];
}

static unsigned long str_to_u32(char * str)
{
    const char * s = str;
    unsigned long acc;
    int c;

    /* strip leading spaces if any */
    do
    {
        c = *s++;
    } while(c == ' ');

    for(acc = 0; (c >= '0') && (c <= '9'); c = *s++)
    {
        c -= '0';
        acc *= 10;
        acc += c;
    }
    return acc;
}
// 返回包的类型
uint8_t ymodem_rx_pac_check(char * buf, size_t sz, uint8_t * pack_number)
{
    char ch;
    ch = buf[0];

    if(ch == EOT || ch == ACK || ch == NAK || ch == CAN || ch == CNC)
    {
        int i = 1;
        while(i < sz && buf[i++] == ch)
            ;        // 判断包中所有内容是否一样
        if(sz == i)  // 是全部一样的话，则认为此命令包有效
            return ch;
        else
            return 0xff;
    }
    else if(ch == SOH || ch == STX)
    {
        *pack_number = buf[1];
        uint16_t crc1 = crc16_cal((uint8_t *)(buf + PACKET_HEADER), sz - PACKET_OVERHEAD);
        uint16_t crc2 = ((uint16_t)(buf[sz - 2])) * 256 + buf[sz - 1];
        if(crc1 == crc2 && 0xff == (uint8_t)buf[1] + (uint8_t)buf[2])
            return ch;
        else
            return 0xff; // 数据包校验为错
    }
    else
        return 0xff; // 错误的指令码
}

// 返回包的长度
size_t ymodem_rx_pac_split(char * buf, size_t sz)
{
    char ch;
    ch = buf[0];

    if(ch == EOT || ch == ACK || ch == NAK || ch == CAN || ch == CNC)
    {
        return 1;
    }
    else if(ch == SOH)
    {
        uint16_t crc1 = crc16_cal((uint8_t *)(buf + PACKET_HEADER), sz - PACKET_OVERHEAD);
        uint16_t crc2 = ((uint16_t)(buf[sz - 2])) * 256 + buf[sz - 1];
        if(crc1 == crc2 && 0xff == (uint8_t)buf[1] + (uint8_t)buf[2])
            return 133;
        else
            return 0; // 数据包校验为错
    }
    else if(ch == STX)
    {
        uint16_t crc1 = crc16_cal((uint8_t *)(buf + PACKET_HEADER), sz - PACKET_OVERHEAD);
        uint16_t crc2 = ((uint16_t)(buf[sz - 2])) * 256 + buf[sz - 1];
        if(crc1 == crc2 && 0xff == (uint8_t)buf[1] + (uint8_t)buf[2])
            return 1029;
        else
            return 0; // 数据包校验为错
    }
    else
        return 0; // 错误的指令码
}
//**********************************************************************接收部分
uint8_t ymodem_rx_pac_if_empty(char * buf, size_t sz)
{
    size_t offset = 0;
    while(buf[offset] == 0x00 && ++offset < sz)
        ;
    if(offset == sz)
        return B_TRUE;
    else
        return B_FALSE;
}

// 解析出头包中的文件名和大小
// 入参: buf 收到的第一个包数据
// 入参: sz 收到的第一个包大小
uint8_t ymodem_rx_prepare(char * buf, size_t sz)
{
    uint8_t ret = YMODEM_OK;
    uint8_t fil_nm_len;//文件长度

    fil_nm_len = strlen(buf);
    memcpy(rx_fil_nm, buf, fil_nm_len);
    ym_rx_fil_sz = (size_t)str_to_u32(buf + fil_nm_len + 1);
    ret = ymodem_rx_file_init(rx_fil_nm, ym_rx_fil_sz);

    if(ym_rx_fil_sz != 0 && fil_nm_len != 0 && ret == YMODEM_OK)
        return YMODEM_OK;
    else
        return YMODEM_ERR;
}
/*********************************************************************
 * @fn      ymodem_tx_pack : Ymodem接收时，逻辑轮转调用函数
 * @param   buf :收到的数据包,以一整包为单位输入,所以需要在通信协议里考虑组包 rx_sz : 数据大小
 */
void ymodem_rx_pack(char * buf, size_t rx_sz)
{
    uint8_t pack_num = 0;
    uint8_t err_flag = B_FALSE;
    uint8_t exit_flag = B_FALSE;
//    static uint32_t rx_over_time_cnt;
    static uint32_t last_CC_tick;
    static uint32_t last_packet_tick;

    if(rx_sz == 0) //当无数据时,定时输出CCC以通知主机,从机就绪
    {
        if(ym_rx_status == YMODEM_RX_IDLE)
        {
            if(hi_get_tick() - last_CC_tick >= CCC_MS)
            {
                last_CC_tick = hi_get_tick();
                __putchar('C');
            }
            return;
        }
        else if(hi_get_tick() - last_packet_tick >= PACKET_TIMEOUT_MS)
        {
            exit_flag = B_TRUE;
            rtt_printf(RTT_UPGRADE, "<y-modem>Error: RX Timeout!\r\n");
        }
        else
            return;
    }
    else //有数据时,记录收包时间
    {
        last_packet_tick = hi_get_tick();
    }

    rtt_printf(RTT_UPGRADE, "<y-modem>RX PACK Stat=0x%02x code=0x%02x siz=%d \r\n", ym_rx_status, buf[0], rx_sz);
    switch(ym_rx_status)
    {
        case YMODEM_RX_IDLE:
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                case SOH:
                    if(pack_num == 0 && YMODEM_OK == ymodem_rx_prepare(buf + PACKET_HEADER, pac_size))
                    {
                        cur_pack_num = pack_num;
                        __putchar(ACK);
                        seek = 0; // 初始化变量，用于接收新文件
                        __putchar('C');
                        ym_rx_status = YMODEM_RX_ACK;
                        rtt_printf(RTT_UPGRADE, "<y-modem>: first pack OK!\r\n");
                    }
                    else
                        err_flag = B_TRUE; // 在IDLE中接收到一个1024的数据包，则肯定是状态有问题
                    break;

                case STX:
                    err_flag = B_TRUE;
                    break;

                case EOT:
                    err_flag = B_TRUE; // 这是在本循环必须完成的操作，所以需要用到 goto 语句
                    break;

                default:
                    //     __putchar( NAK );      //不正常的状态，调试用
                    err_flag = B_TRUE; // 这儿暂时认为，包有误，就退出
                    break;
            }
            break;
        case YMODEM_RX_ACK:                          // 1级——文件接收状态中
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                case SOH:
                case STX:
                    //vTaskDelay(10);
                    __putchar(ACK);
                    if(seek < ym_rx_fil_sz)//判断累积收到的长度没有超过文件长度
                    {
                        if((cur_pack_num + 1) % 256 == pack_num) //判断序号是否衔接上一包
                        {
                            cur_pack_num = pack_num;
                            pac_size = (uint8_t)(buf[0]) == SOH ? PACKET_SIZE : PACKET_1K_SIZE;

                            if(ym_rx_fil_sz - seek < pac_size)
                                pac_size = ym_rx_fil_sz - seek;
                            ymodem_rx_pac_store_handler(buf + PACKET_HEADER, seek, pac_size); // 将接收的包保存
                            seek += pac_size;
                            //__putchar('C');
                            rtt_printf(RTT_UPGRADE, "<y-modem>RX :addr%d len%d\r\n", seek, pac_size);
                        }
                        else if(cur_pack_num == pack_num) //收到了重发包
                        {
                            //__putchar('C');
                            rtt_printf(RTT_UPGRADE, "<y-modem>RX :addr%d len%d\r\n", seek, pac_size);
                        }
                        else
                        {
                            rtt_printf(RTT_UPGRADE, "<y-modem>YMODEM_RX_ACK: ERROR Wrong pack number\r\n");
                            err_flag = B_TRUE; // 包序号有误，退出
                        }
                    }
                    else //接收数据超出文件长度
                    {
                        rtt_printf(RTT_UPGRADE, "<y-modem>YMODEM_RX_ACK: ERROR rx data %d over the filesize %d!\r\n", seek + pac_size, ym_rx_fil_sz);
                        err_flag = B_TRUE; // 包序号有误，退出
                    }
                    break;
                // 指令包
                case EOT:
                    __putchar(NAK);
                    ym_rx_status = YMODEM_RX_EOT;
                    rtt_printf(RTT_UPGRADE, "<y-modem>RX EOT 1\r\n");
                    break;
                case CAN:
                    rtt_printf(RTT_UPGRADE, "<y-modem>YMODEM_RX_ACK: CAN %d \r\n");
                    err_flag = B_TRUE;
                    break;
                default:
                    __putchar(NAK); // 不正常的状态，调试用
                    rtt_printf(RTT_UPGRADE, "<y-modem>YMODEM_RX_ACK: ERROR code=0x%02x siz=%d \r\n", buf[0], rx_sz);
                    //          goto err;           //这儿暂时认为，包有误，就重发
                    break;
            }
            break;
        case YMODEM_RX_EOT: //
        {
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                // 指令包
                case EOT:
                    __putchar(ACK);
                    __putchar('C');
                    rtt_printf(RTT_UPGRADE, "<y-modem>RX EOT 2\r\n");
                    break;
                case SOH:
                    if(pack_num == 0)//收到了结束包
                    {
                        __putchar(ACK);
                        cur_pack_num = -1;
                        ymodem_rx_finish_handler(YMODEM_OK); // 确认发送完毕，保存文件
                        ym_rx_status = YMODEM_RX_IDLE;
                        vTaskDelay(10);
                        rtt_printf(RTT_UPGRADE, "\r\n<y-modem>RX %s Success! size = %d\r\n", rx_fil_nm, seek);
                    }
                    else
                        err_flag = B_TRUE;
                    break;
                default:
                    err_flag = B_TRUE;
                    break;
            }
        }
        break;
        default:
            break;
    }

    if(err_flag == B_TRUE)//错误退出,清理状态
    {
        ym_rx_status = YMODEM_RX_IDLE;
        cur_pack_num = -1;
        ym_rx_fil_sz = 0;

        __putchar(CAN);
        ymodem_rx_finish_handler(YMODEM_ERR);
        rtt_printf(RTT_UPGRADE, "<y-modem>error: pack error!\r\n");
        return;
    }

    if(exit_flag == B_TRUE)//TODO :中断退出,保存现场,用于续传
    {
        ym_rx_status = YMODEM_RX_IDLE;
        cur_pack_num = -1;
        //*这里还需要进行某些操作，使在退出后，不会再重新进入ymodem_rx_put()函数
        ymodem_rx_finish_handler(YMODEM_ERR);
        rtt_printf(RTT_UPGRADE, "<y-modem>: rx exit!\r\n");
        return;
    }

}
//**********************************************************************发送部分
// pbuf 是指向缓冲区的最开始的地方， pac_sz 是数据区的大小
uint8_t ymodem_tx_make_pac_data(char * pbuf, size_t pac_sz)
{
    uint8_t ans = YMODEM_ERR;
    uint16_t crc;

    pbuf[0] = pac_sz == 128 ? SOH : STX;
    pbuf[1] = ym_cyc;
    pbuf[2] = ~ym_cyc;
    crc = crc16_cal((unsigned char const *)pbuf, pac_sz);
    pbuf[PACKET_HEADER + pac_sz] = (uint8_t)(crc / 256);
    pbuf[PACKET_HEADER + pac_sz + 1] = (uint8_t)(crc & 0x00ff);
    ym_cyc++;
    return ans;
}
uint8_t ymodem_tx_make_pac_header(char * pbuf, char * file_name, size_t fil_sz)
{
    uint8_t ans = YMODEM_ERR;
    uint8_t nm_len;
    memset(pbuf + PACKET_HEADER, 0, 128);
    if(file_name)
    {
        nm_len = strlen(file_name);
        strcpy(pbuf + PACKET_HEADER, file_name);
        strcpy(pbuf + PACKET_HEADER + nm_len + 1, u32_to_str(fil_sz));
    }
    ym_cyc = 0x00;
    ymodem_tx_make_pac_data(pbuf, 128);
    return ans;
}
/*********************************************************************
 * @fn      ymodem_tx_pack : Ymodem发送时，逻辑轮转调用函数
 * @param   buf : 数据缓冲区 buf : 数据大小
 * 说明：
 * 1.发送 [包  头] 状态：如果没有文件名，则发送空包，否则发送封装的头包
 * 2.发送 [数据包] 状态：发送数据包，出现问题或结束，则进入结束状态
 * 3.发送 [结  束] 状态：处理发送完成的相关事情
 */
void ymodem_tx_pack(char * buf, size_t rx_sz)
{
    uint8_t pack_num = 0;
    char * tx_fil_nm = NULL;
    size_t fil_sz = NULL;
    switch(ym_tx_status)
    {
        case YMODEM_TX_IDLE:
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                case CNC:
                    {
                        if(YMODEM_OK == ymodem_tx_file_init(&tx_fil_nm, &fil_sz))  // 得到 文件名和大小
                        {
                            // 封装一个包头，然后发送出去
                            ym_tx_fil_sz = fil_sz;
                            ymodem_tx_make_pac_header(ym_tx_pbuf, tx_fil_nm, fil_sz);
                            __putbuf(ym_tx_pbuf, PACKET_OVERHEAD + PACKET_SIZE);
                            ym_tx_status = YMODEM_TX_IDLE_ACK;
                        }
                        else // 封装一个空包，然后发出去
                        {
                            ymodem_tx_make_pac_header(ym_tx_pbuf, NULL, NULL);
                            __putbuf(ym_tx_pbuf, PACKET_OVERHEAD + PACKET_SIZE);
                        }
                    }
                    break;
                case CAN:
                    ym_rx_status = YMODEM_TX_ERR;
                    goto err_tx;
                    //break;
                default:
                    goto err_tx; // 这儿暂时认为，包有误，就退出
                    //break;
            }
            break;
        case YMODEM_TX_IDLE_ACK:
        {
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                case ACK: // 准备发数据包
                    ym_tx_status = YMODEM_TX_DATA;
                    break;
                case NAK: // 准备重发包头
                    ym_tx_status = YMODEM_TX_IDLE;
                    break;
                default: // 啥也不做
                    break;
            }
        }
        break;
dt_tx:
        case YMODEM_TX_DATA:                         // 1级——文件发送状态中
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                case CNC:
                    if(YMODEM_OK == ymodem_tx_pac_store_handler(ym_tx_pbuf + PACKET_HEADER, seek, PACKET_1K_SIZE))  // 读取下一组数据
                    {
                        if(YMODEM_OK == ymodem_tx_make_pac_data(ym_tx_pbuf, PACKET_1K_SIZE))
                        {
                            __putbuf(ym_tx_pbuf, PACKET_OVERHEAD + PACKET_1K_SIZE);
                            ym_tx_status = YMODEM_TX_DATA_ACK;
                        }
                        else // 读取数据出错，结束传输
                        {
                            ym_tx_status = YMODEM_TX_ERR;
                            goto err_tx;
                        }
                    }
                    break;
                case CAN:
                    ym_rx_status = YMODEM_TX_ERR;
                    goto err_tx;
                    //break;
                default: // 暂时啥也不做
                    break;
            }
            break;
        case YMODEM_TX_DATA_ACK:
        {
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                case ACK:
                    seek += PACKET_1K_SIZE;
                    if(seek < ym_tx_fil_sz)  // 数据未发送完（不能加‘=’！）
                        ym_tx_status = YMODEM_TX_DATA_ACK;
                    else // 数据发送完
                    {
                        ym_tx_status = YMODEM_TX_EOT;
                        __putchar(EOT);
                    }
                    break;
                case CNC: // 如果接收方不先应答[ACK]而是直接发'C'在这里处理
                    seek += PACKET_1K_SIZE;
                    if(seek < ym_tx_fil_sz)  // 数据未发送完（不能加‘=’！）
                    {
                        ym_tx_status = YMODEM_TX_DATA_ACK;
                        // 下面的状态，因为我需要马上回复，所以用goto
                        goto dt_tx; // 发送下一个数据包
                    }
                    else // 数据发送完
                    {
                        ym_tx_status = YMODEM_TX_EOT;
                        __putchar(EOT);
                    }
                    break;
                default:
                    break;
            }
        }
        break;
        case YMODEM_TX_EOT:
        {
            switch(ymodem_rx_pac_check(buf, rx_sz, &pack_num))  // 检查当前包是否合法,并返回包的类型
            {
                // 指令包
                case NAK:
                    __putchar(EOT);
                    break;
                case ACK:
                    __putchar(ACK);
                    ymodem_tx_finish_handler(YMODEM_OK);
                    ym_rx_status = YMODEM_TX_IDLE;
                    break;
                default:
                    break;
            }
        }
        break;
err_tx:
        case YMODEM_TX_ERR: // 在这里放弃保存文件,终止传输
            __putchar(CAN);
            ymodem_rx_finish_handler(YMODEM_ERR);
            // break;                    //没有break，和下面公用代码
        case YMODEM_TX_EXIT: // 到这里，就收拾好，然后退出
            ym_rx_status = YMODEM_RX_IDLE;
            //*这里还需要进行某些操作，使在退出后，不会再重新进入ymodem_rx_put()函数
            return;
        default:
            break;
    }
}


void ymodem_CCC(void)
{
    static int tm_out;
    if(ym_rx_status == YMODEM_RX_IDLE)
    {
        if(tm_out)
            tm_out--;
        else
        {
            __putchar('C');
            tm_out = 0x2A;
        }
    }
}

void ymodem_set_protocol_dir(uint8_t direction)
{
    if(direction == USART_PROTOCOL_YMODEM_UPLOAD || direction == USART_PROTOCOL_YMODEM_DNLOAD)
        usart_protocol_direction = direction;
}
