#include "Ymodem.h"
#include "hi_flash.h"
/**************************************************************************
 * 当通过[超时]+判断出串口接收到1串完整的包，相结合的方式调用
 * 第一次编译的时候，肯定会有很多错误，因为这里还包含了许多其它的函数包括，但不限于
 * pvPortMalloc()   FreeRTOS的动态内存申请函数√
 * vPortFree()      FreeRTOS的动态内存释放函数√
 * f_open()         FatFs的文件打开函数
 * f_close()        FatFs的文件关闭函数
 * f_read()         FatFs的文件读函数
 * f_write()        FatFs的文件写函数
 * cstr_create()    网上开源的动态字符串cstr_t的创建函数
 * cstr_free()      网上开源的动态字符串cstr_t的释放函数
 * usart_write()    自己实现的一个串口数据发送程序
 ***************************************************************************/

void ymodem_init(void)
{
    ymodem_set_protocol_dir(USART_PROTOCOL_YMODEM_DNLOAD);
    ymodem_set_rx_flash_addr(FLASH_ON_CHIP,0x00000000);//TODO Flash 分区
}

// ymodem运行函数,当工作时需要以小于50ms的周期循环调用,有数据时输入数据,无数据时参数为(NULL,0)
void ymodem_run(char *data, size_t len)
{
    //static int tm_out;
    char *data_ptr = data;
    size_t data_len = 0;

    if(usart_protocol_direction == USART_PROTOCOL_YMODEM_DNLOAD)//下载处理
    {
        while(1)
        {
            data_len = ymodem_rx_pac_split(data_ptr, len);//拆包处理
            ymodem_rx_pack((char *)data_ptr, data_len);//收包处理,包括空包和超时
            
            if(data_len == 0)//非法包,空包或拆完包后跳出
            {
                break;
            }
            data_ptr = data_ptr + data_len;        
        };
    }else//传输处理
    {
        ;
    }

}
