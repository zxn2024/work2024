#include "ecr8668_driver.h"
#include "hi_systick.h"
#include "hi_irq.h"
#include "MCU_config.h"
#include "main.h"
#include "bsp_wdg.h"
#include "bsp_flash_addr.h"
#include "bsp_config.h"
#include "bsp_rf_global.h"
#include "bsp_rf_att.h"

/*---------------------- 宏定义 ------------------------------------------*/
#define ADC_ANTIBLOCK_LOWPWR_TRIGGER_CNT (20) // 20次连续进入adc饱和中断,且功率不到起控点,判断需要进入抗阻塞
#define TIA_ANTIBLOCK_LOWPWR_TRIGGER_CNT (20) // 20次连续进入tia饱和中断,且功率不到起控点,判断需要进入抗阻塞
#define ADC_ANTIBLOCK_HIPWR_TRIGGER_CNT (100) // 100次连续进入adc饱和中断,且功率超过起控点,判断需要进入抗阻塞
#define TIA_ANTIBLOCK_HIPWR_TRIGGER_CNT (100) // 100次连续进入tia饱和中断,且功率超过起控点,判断需要进入抗阻塞

/*---------------------- 全局变量 ----------------------*/

//增益档位(衰减档位)和本振校准值查找表-B3
//不足的补0
extern gain_cali_lookup_tab_T normal_gain_cali_lookup_table_b3[MAX_ALC_GAIN_NUM];
extern gain_cali_lookup_tab_T blocking_gain_cali_lookup_table_b3[MAX_ALC_GAIN_NUM];

//增益档位(衰减档位)和本振校准值查找表-B8
extern gain_cali_lookup_tab_T normal_gain_cali_lookup_table_b8[MAX_ALC_GAIN_NUM];
extern gain_cali_lookup_tab_T blocking_gain_cali_lookup_table_b8[MAX_ALC_GAIN_NUM];

//增益档位(衰减档位)和本振校准值查找表-B39
extern gain_cali_lookup_tab_T normal_gain_cali_lookup_table_b39[MAX_ALC_GAIN_NUM];
extern gain_cali_lookup_tab_T blocking_gain_cali_lookup_table_b39[MAX_ALC_GAIN_NUM];

//增益档位(衰减档位)和本振校准值查找表-b40
extern gain_cali_lookup_tab_T normal_gain_cali_lookup_table_b40[MAX_ALC_GAIN_NUM];
extern gain_cali_lookup_tab_T blocking_gain_cali_lookup_table_b40[MAX_ALC_GAIN_NUM];

//增益档位(衰减档位)和本振校准值查找表-N41
extern gain_cali_lookup_tab_T normal_gain_cali_lookup_table_n41[MAX_ALC_GAIN_NUM];
extern gain_cali_lookup_tab_T blocking_gain_cali_lookup_table_n41[MAX_ALC_GAIN_NUM];

//增益档位(衰减档位)和本振校准值查找表-MIMO
extern gain_cali_lookup_tab_T normal_gain_cali_lookup_table_mimo[MAX_ALC_GAIN_NUM];
extern gain_cali_lookup_tab_T blocking_gain_cali_lookup_table_mimo[MAX_ALC_GAIN_NUM];

//额定增益索引值数组的索引值查找表
extern rated_gain_index_lookup_tab_t rated_gain_index_lookup_tab[17];

factroy_data_T factroy_data_buf = {0};// 公共的数据读取缓冲区


/*****************************************************************************
* @Name	  : get_8668_cali_data_save_addr
* @Brief  : 获取校准参数保存数据首地址
* @Param  : band: 频段号, 1
* @Retval : 若参数均正确, 则返回相应的存储地址, 若参数错误, 则返回SAVE_ADDR_ERROR ( 0xFFFFFFFF )
* @Author :
* @Data   : 2023-11-25
* @Note   :
*****************************************************************************/
#define B3_CALI_ADDR (0x00007000)
#define B8_CALI_ADDR (0x00008000)
#define B39_CALI_ADDR (0x00009000)
#define B40_CALI_ADDR (0x0000A000)
#define N41_CALI_ADDR (0x0000B000)
#define MIMO_CALI_ADDR (0x0000C000)


uint32_t get_8668_cali_data_save_addr(RF_BAND_E band)
{
    uint32_t flash_addr = 0xffffffff;
    switch(band)//选择对应band的查找表
    {
        case BAND_3:
            flash_addr = B3_CALI_ADDR;
            break;

        case BAND_8:
            flash_addr = B8_CALI_ADDR;

            break;
        case BAND_39:
            flash_addr = B39_CALI_ADDR;

            break;
        case BAND_40:
            flash_addr = B40_CALI_ADDR;

            break;
        case BAND_41:
            flash_addr = N41_CALI_ADDR;

            break;
        case BAND_41M:
            flash_addr = MIMO_CALI_ADDR;

            break;
        default:
            flash_addr = 0xffffffff;
            break;
    }
    return flash_addr;
}


/**********************************************************
 *函数名：   BSP_866x_anti_block_switch
 *函数功能： 根据adc饱和状态,切换到抗阻塞模式,当adc饱和消失后30s恢复正常模式,恢复次数上限为3次
 *返回值：   无
 */
void BSP_866x_anti_block_switch(RF_SYS_E sys, TRX8668_FPGA_t * h8668)
{
    //adc饱和,进入阻塞模式
    if((h8668->adc_overload_flag == B_TRUE || h8668->tia_overload_flag == B_TRUE) && h8668->anti_block_stat == B_FALSE)
    {
        h8668->anti_block_stat = B_TRUE;
        h8668->anti_block_tick = hi_get_tick();
        rtt_printf(RTT_8668, RTT_TEXT_YELLOW"trx866x %d enter anti block mode\r\n", h8668->config.dev_number);
    }

    //adc饱和状态恢复正常持续30秒,恢复正常模式,恢复超过3次则不再恢复,直到设备重启
    if((h8668->adc_overload_flag == B_FALSE && h8668->tia_overload_flag == B_FALSE) && h8668->anti_block_stat == B_TRUE )
    {
        if(hi_get_tick() - h8668->anti_block_tick >= EXIT_ANTI_BLOCK_TIME && h8668->anti_block_cnt < EXIT_ANTI_BLOCK_LIMIT)
        {
            h8668->anti_block_cnt++;
            h8668->anti_block_stat = B_FALSE;
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"trx866x %d exit anti block mode\r\n", h8668->config.dev_number);
        }
    }
}

/**********************************************************
 *函数名：   BSP_866x_gain_tab_switch
 *函数功能： 根据当前的射频参数,band,带宽,增益等,切换增益表
 *返回值：   无
 */

static void BSP_866x_gain_tab_switch(TRX8668_FPGA_t * h8668)
{
    GAIN_TABLE_T * ptr_flash_gain_tab = NULL, * ptr_default_tab = NULL;
    uint32_t * ptr_uint32 = NULL;
    uint8_t bw_index, temp_index = 0;
//    BOOL_E chk_flag = B_FALSE;
//    int i = 0;

    switch(h8668->band)//选择对应band的查找表
    {
        case BAND_3:
            if(h8668->anti_block_stat == B_TRUE)
            {
                ptr_flash_gain_tab = &h8668->factroy_data.blocking_gain_tab;
                ptr_default_tab = &gain_tab_b3_antiblocking;
            }
            else
            {
                ptr_flash_gain_tab = &h8668->factroy_data.normal_gain_tab;
                ptr_default_tab = &gain_tab_b3;
            }
            break;

        case BAND_8:
            if(h8668->anti_block_stat == B_TRUE)
            {
                ptr_flash_gain_tab = &h8668->factroy_data.blocking_gain_tab;
                ptr_default_tab = &gain_tab_b8_antiblocking;
            }
            else
            {
                ptr_flash_gain_tab = &h8668->factroy_data.normal_gain_tab;
                ptr_default_tab = &gain_tab_b8;
            }
            break;


        case BAND_39:
            if(h8668->anti_block_stat == B_TRUE)
            {
                ptr_flash_gain_tab = &h8668->factroy_data.blocking_gain_tab;
                ptr_default_tab = &gain_tab_b39_antiblocking;
            }
            else
            {
                ptr_flash_gain_tab = &h8668->factroy_data.normal_gain_tab;
                ptr_default_tab = &gain_tab_b39;
            }
            break;


        case BAND_40:
            if(h8668->anti_block_stat == B_TRUE)
            {
                ptr_flash_gain_tab = &h8668->factroy_data.blocking_gain_tab;
                ptr_default_tab = &gain_tab_b40_antiblocking;
            }
            else
            {
                ptr_flash_gain_tab = &h8668->factroy_data.normal_gain_tab;
                ptr_default_tab = &gain_tab_b40;
            }
            break;


        case BAND_41:
            if(h8668->anti_block_stat == B_TRUE)
            {
                ptr_flash_gain_tab = &h8668->factroy_data.blocking_gain_tab;
                ptr_default_tab = &gain_tab_n41_antiblocking;
            }
            else
            {
                ptr_flash_gain_tab = &h8668->factroy_data.normal_gain_tab;
                ptr_default_tab = &gain_tab_n41;
            }
            break;


        case BAND_41M:
            if(h8668->anti_block_stat == B_TRUE)
            {
                ptr_flash_gain_tab = &h8668->factroy_data.blocking_gain_tab;
                ptr_default_tab = &gain_tab_n41_antiblocking;
            }
            else
            {
                ptr_flash_gain_tab = &h8668->factroy_data.normal_gain_tab;
                ptr_default_tab = &gain_tab_n41;
            }
            break;

        default:
            rtt_printf(RTT_8668, RTT_TEXT_RED"Error BSP_866x_gain_tab_switch, band = %d\r\n", h8668->band);
            return;
    }

    ptr_uint32 = (uint32_t *)ptr_flash_gain_tab;

    if(ptr_flash_gain_tab->gain_tab_number <= MAX_ALC_GAIN_NUM && ptr_flash_gain_tab->gain_tab_number != 0 && *ptr_uint32 != 0xffffffff) //如果flash校准增益表有值,则使用
    {
        h8668->gain_tab = ptr_flash_gain_tab;
    }else
    {
        h8668->gain_tab = ptr_default_tab;
    }
    
    // 读取码表索引值
    for(int i = 0; i < sizeof(rated_gain_index_lookup_tab)/sizeof(rated_gain_index_lookup_tab_t); i++)
    {
        if(h8668->band == rated_gain_index_lookup_tab[i].band && h8668->ch_status.bw_choose == rated_gain_index_lookup_tab[i].bw)
        {
            bw_index = rated_gain_index_lookup_tab[i].index_of_rated_gain_index;
            if(h8668->anti_block_stat == B_TRUE)
            {
                temp_index = h8668->factroy_data.rated_gain_index_anti_block[bw_index];
            }else
            {
                temp_index = h8668->factroy_data.rated_gain_index_normal[bw_index];
            }
            break;
        }
    }
    //如果索引值合法则使用,否则使用默认值
    if(temp_index >= MAX_ALC_GAIN_NUM || temp_index >= h8668->gain_tab->gain_tab_number)
        h8668->gain_tab->rated_gain_index = ptr_default_tab->rated_gain_index;
    else        
        h8668->gain_tab->rated_gain_index = temp_index;
}

/**********************************************************
 *函数名：   BSP_866x_adc_warning_exti_handle
 *函数功能： Adc饱和告警中断处理函数
 *返回值：   无
 */
void BSP_866x_adc_warning_exti_handle(TRX8668_FPGA_t * h8668)
{
    h8668->adc_overload_trigger = B_TRUE;
    hi_irq_off(h8668->config.adc_irq);
}

/**********************************************************
 *函数名：   BSP_866x_tia_warning_exti_handle
 *函数功能： Adc饱和告警中断处理函数
 *返回值：   无
 */
void BSP_866x_tia_warning_exti_handle(TRX8668_FPGA_t * h8668)
{
    h8668->tia_overload_trigger = B_TRUE;
    hi_irq_off(h8668->config.tia_irq);
}

/**********************************************************
 *函数名：   BSP_866x_tia_adc_switch
 *函数功能： 饱和中断使能开关
 *返回值：   无
 */
void BSP_866x_tia_adc_switch(RF_SYS_E sys, uint8_t OnOff)
{
    TRX8668_FPGA_t *ptTRX8668[RF_SYS_NUM] = {&h8668_LTE, &h8668_NR, &h8668_NR_MIMO};
    if(OnOff == 0)
    {
        ptTRX8668[sys]->anti_block_stat = B_FALSE;
    #if(MCU_SERIES == MCU_GD32F4XX)
        nvic_irq_disable(ptTRX8668[sys]->config.tia_irq);
        nvic_irq_disable(ptTRX8668[sys]->config.adc_irq);
    #elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
        HAL_NVIC_DisableIRQ(ptTRX8668[sys]->config.tia_irq);
        HAL_NVIC_DisableIRQ(ptTRX8668[sys]->config.adc_irq);
    #endif
    }
    else{
    #if(MCU_SERIES == MCU_GD32F4XX)
        nvic_irq_enable(ptTRX8668[sys]->config.tia_irq, 15, 0);
        nvic_irq_enable(ptTRX8668[sys]->config.adc_irq, 15, 0);
    #elif(MCU_SERIES == MCU_STM32F4XX || MCU_SERIES == MCU_STM32H5XX)
        HAL_NVIC_EnableIRQ(ptTRX8668[sys]->config.tia_irq);
        HAL_NVIC_EnableIRQ(ptTRX8668[sys]->config.adc_irq);
    #endif
    }
}

/**********************************************************
 *函数名：   BSP_866x_Proc_warning_exti_periodic
 *函数功能： 周期性(50ms)打开adc饱和中断
 *返回值：   无
 */

void BSP_866x_Proc_warning_exti_periodic(RF_SYS_E sys, TRX8668_FPGA_t * h8668)
{
    #if( LOCAL_MODULE_TYPE == _MU )
    Pow_T *ptdPow = &t_rf_sys[h8668->config.dev_number].t_ch[RF_CH_DW].tPow;
    ATT_CH_T * pt_hal = pt_ch_att[sys][1]; 
    #else
    Pow_T *ptdPow = &t_rf_sys[h8668->config.dev_number].t_ch[RF_CH_UP].tPow;
    #endif

    if(h8668->anti_block_switch == CLOSE)
        return;
        
    if(hi_get_tick() - h8668->exti_trigger_tick >= 100)
    {
        //adc告警计数处理
        h8668->exti_trigger_tick = hi_get_tick();
        hi_irq_on(h8668->config.adc_irq, 15);

        if(h8668->adc_overload_trigger == B_TRUE)
        {
            h8668->adc_overload_trigger = B_FALSE;
            h8668->adc_overload_flag = B_TRUE; //adc饱和告警状态

            //抗阻塞模式计数
            if(ptdPow->scOutPow < ptdPow->scTheoOutPow ) //当功率连续小于起控点,且饱和,快速进行抗阻塞计数
            {
                if(h8668->adc_antiblock_lopwr_cnt < ADC_ANTIBLOCK_LOWPWR_TRIGGER_CNT)
                    h8668->adc_antiblock_lopwr_cnt++;
            }
            else//当功率大于起控点,且饱和,缓慢计数
            {
                if(h8668->adc_antiblock_cnt < ADC_ANTIBLOCK_HIPWR_TRIGGER_CNT)
                    h8668->adc_antiblock_cnt++;
            }
        }
        else
        {
            h8668->adc_overload_flag = B_FALSE; //adc饱和告警恢复正常状态

            if(h8668->adc_antiblock_lopwr_cnt > 0)
                h8668->adc_antiblock_lopwr_cnt--;

            if(h8668->adc_antiblock_cnt > 0)
                h8668->adc_antiblock_cnt--;
        }


        //tia告警处理
        if(h8668->tia_overload_trigger == B_TRUE)
        {
            h8668->tia_overload_trigger = B_FALSE;
            h8668->tia_overload_flag = B_TRUE; //adc饱和告警状态

            //抗阻塞模式计数
            if(ptdPow->scOutPow < ptdPow->scTheoOutPow ) //当功率连续小于起控点,且饱和,快速进行抗阻塞计数
            {
                if(h8668->tia_antiblock_lopwr_cnt < ADC_ANTIBLOCK_LOWPWR_TRIGGER_CNT)
                    h8668->tia_antiblock_lopwr_cnt++;
            }
            else//当功率大于起控点,且饱和,缓慢计数
            {
                if(h8668->tia_antiblock_cnt < ADC_ANTIBLOCK_HIPWR_TRIGGER_CNT)
                    h8668->tia_antiblock_cnt++;
            }
        }
        else
        {
            h8668->tia_overload_flag = B_FALSE; //adc饱和告警恢复正常状态

            if(h8668->tia_antiblock_lopwr_cnt > 0)
                h8668->tia_antiblock_lopwr_cnt--;

            if(h8668->tia_antiblock_cnt > 0)
                h8668->tia_antiblock_cnt--;
        }
            
        //正常/抗阻塞模式切换
        //当adc/tia满足抗阻塞触发时,进入抗阻塞模式
        if((h8668->adc_antiblock_lopwr_cnt >= ADC_ANTIBLOCK_LOWPWR_TRIGGER_CNT || h8668->tia_antiblock_lopwr_cnt >= ADC_ANTIBLOCK_LOWPWR_TRIGGER_CNT|| h8668->adc_antiblock_cnt >= ADC_ANTIBLOCK_HIPWR_TRIGGER_CNT || h8668->tia_antiblock_cnt >= ADC_ANTIBLOCK_HIPWR_TRIGGER_CNT)
            && h8668->anti_block_stat == B_FALSE) 
        {
            h8668->anti_block_stat = B_TRUE;
            pt_hal->digit_att1.refresh = B_TRUE;
            h8668->anti_block_tick = hi_get_tick();
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"trx866x %d enter anti block mode\r\n", h8668->config.dev_number);
        }
        //当adc/tia全部恢复时,退出抗阻塞模式
        else if((h8668->adc_antiblock_cnt == 0 && h8668->tia_antiblock_cnt == 0 && h8668->adc_antiblock_lopwr_cnt == 0 && h8668->tia_antiblock_lopwr_cnt == 0) 
                && h8668->anti_block_stat == B_TRUE
                && (hi_get_tick() - h8668->anti_block_tick >= EXIT_ANTI_BLOCK_TIME))
        {
            h8668->anti_block_stat = B_FALSE;
            pt_hal->digit_att1.refresh = B_TRUE;
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"trx866x %d quit anti block mode\r\n", h8668->config.dev_number);
        }
    }
}


/**********************************************************
 *函数名：   BSP_866x_error_handle
 *函数功能： 8668错误处理
 *返回值：   无
 */
static void BSP_866x_error_handle(TRX8668_FPGA_t * h8668)
{
    if(h8668->error_cnt <= 3)
        ;
    else
        ;//系统复位
}

/**********************************************************
 *函数名：   BSP_866x_load_factroy_data
 *函数功能： trx8668从片外flash读取校准参数,包括增益表,校准值等
 *返回值：   无
 */
static Status_t BSP_866x_load_factroy_data(factroy_data_T * factroy_data_buf, RF_BAND_E band)
{
    uint32_t exter_flash_addr = get_8668_cali_data_save_addr(band);	//  8668参数类型下不再细分组,直接一次读取整个band的参数
    uint16_t data_size = sizeof(factroy_data_T);

    if(exter_flash_addr && data_size)
    {
        taskENTER_CRITICAL();
        spi_flash.flash_read((uint8_t *)factroy_data_buf, exter_flash_addr, data_size);
        taskEXIT_CRITICAL();
        return STAT_OK;
    }
    else
        return STAT_ERROR;
}

/**********************************************************
 *函数名：   BSP_866x_set_cali_data
 *函数功能： 保存校准值,写入到flash
 *参数:      band 频段
 *参数:      group 0-5 校准参数按工况分几档,具体分档方式等射频调测结果
 *参数:      cali_data_type 校准值类型
 *参数:      factroy_data 校准值
 *返回值：   无
 */
Status_t BSP_866x_set_cali_data(RF_BAND_E band, CALI_8668_TYPE_E cali_data_type, uint8_t group, uint32_t cali_data)
{
    //先读出来,修改值,再写回去

    //读出来,前提是一个band的所有参数占一个section
    uint32_t exter_flash_addr;
    uint16_t data_size;
    uint16_t flash_sector;

    exter_flash_addr = get_8668_cali_data_save_addr(band);	//  8668参数类型下不再细分组,直接一次读取整个band的参数
    data_size = sizeof(factroy_data_T);
    flash_sector = spi_flash.flash_addr_to_sector(exter_flash_addr);

    BSP_866x_load_factroy_data(&factroy_data_buf, band);

    //写值
    switch(cali_data_type)
    {
        case CALI_8668_RX_LO_I:
            if(group >= sizeof(factroy_data_buf.cali_rx_lo_I) / sizeof(factroy_data_buf.cali_rx_lo_I[0]))
                return STAT_ERROR;
            factroy_data_buf.cali_rx_lo_I[group] = cali_data;
            break;

        case CALI_8668_RX_LO_Q:
            if(group >= sizeof(factroy_data_buf.cali_rx_lo_Q) / sizeof(factroy_data_buf.cali_rx_lo_Q[0]))
                return STAT_ERROR;
            factroy_data_buf.cali_rx_lo_Q[group] = cali_data;
            break;

        case CALI_8668_TX_LO_I:
            if(group >= sizeof(factroy_data_buf.cali_tx_lo_I) / sizeof(factroy_data_buf.cali_tx_lo_I[0]))
                return STAT_ERROR;
            factroy_data_buf.cali_tx_lo_I[group] = cali_data;
            break;

        case CALI_8668_TX_LO_Q:
            if(group >= sizeof(factroy_data_buf.cali_tx_lo_Q) / sizeof(factroy_data_buf.cali_tx_lo_Q[0]))
                return STAT_ERROR;
            factroy_data_buf.cali_tx_lo_Q[group] = cali_data;
            break;

        case CALI_8668_MIRROR:
            if(group >= sizeof(factroy_data_buf.cali_mirror) / sizeof(factroy_data_buf.cali_mirror[0]))
                return STAT_ERROR;
            factroy_data_buf.cali_mirror[group] = cali_data;
            break;

        default:
            break;
    }
    //更新当前配置
    if(band == h8668_LTE.band)
	{
//		if( (CALI_8668_RX_LO_Q != cali_data_type) && (group != 1))
//		{
			memcpy(&h8668_LTE.factroy_data, &factroy_data_buf, sizeof(factroy_data_buf));
//		}
	}
    else if(band == h8668_NR.band)
        memcpy(&h8668_NR.factroy_data, &factroy_data_buf, sizeof(factroy_data_buf));
    else if(band == h8668_NR_MIMO.band)
        memcpy(&h8668_NR_MIMO.factroy_data, &factroy_data_buf, sizeof(factroy_data_buf));

    //写回去
    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);
    spi_flash.flash_write((uint8_t *)&factroy_data_buf, exter_flash_addr, data_size);
    taskEXIT_CRITICAL();

    return STAT_OK;
}

/**********************************************************
 *函数名：   BSP_866x_set_cali_data
 *函数功能： 保存校准值,写入到flash
 *参数:      band 频段
 *参数:      group 0-10 校准参数按工况分几档,具体分档方式根据调测结果决定
 *参数:      cali_data_type 校准值类型
 *返回值：   cali_data 校准值
 */
uint32_t BSP_866x_get_cali_data(RF_BAND_E band, CALI_8668_TYPE_E cali_data_type, uint8_t group)
{
    //先读出来,修改值,再写回去

    //读出来,前提是一个band的所有参数占一个section
    uint32_t cali_data = 0xffffffff;
    memset(&factroy_data_buf, 0, sizeof(factroy_data_buf));

    BSP_866x_load_factroy_data(&factroy_data_buf, band);

    //写值
    switch(cali_data_type)
    {
        case CALI_8668_RX_LO_I:
            if(group >= sizeof(factroy_data_buf.cali_rx_lo_I) / sizeof(factroy_data_buf.cali_rx_lo_I[0]))
                return STAT_ERROR;
            cali_data = factroy_data_buf.cali_rx_lo_I[group];
            break;

        case CALI_8668_RX_LO_Q:
            if(group >= sizeof(factroy_data_buf.cali_rx_lo_Q) / sizeof(factroy_data_buf.cali_rx_lo_Q[0]))
                return STAT_ERROR;
            cali_data = factroy_data_buf.cali_rx_lo_Q[group] ;
            break;

        case CALI_8668_TX_LO_I:
            if(group >= sizeof(factroy_data_buf.cali_tx_lo_I) / sizeof(factroy_data_buf.cali_tx_lo_I[0]))
                return STAT_ERROR;
            cali_data = factroy_data_buf.cali_tx_lo_I[group] ;
            break;

        case CALI_8668_TX_LO_Q:
            if(group >= sizeof(factroy_data_buf.cali_tx_lo_Q) / sizeof(factroy_data_buf.cali_tx_lo_Q[0]))
                return STAT_ERROR;
            cali_data = factroy_data_buf.cali_tx_lo_Q[group] ;
            break;

        case CALI_8668_MIRROR:
            if(group >= sizeof(factroy_data_buf.cali_mirror) / sizeof(factroy_data_buf.cali_mirror[0]))
                return STAT_ERROR;
            cali_data = factroy_data_buf.cali_mirror[group] ;
            break;

        default:
            break;
    }

    return cali_data;
}
/**********************************************************
 *函数名：   BSP_866x_set_default_gain
 *函数功能： 保存默认增益,写入到flash (目前未用此参数)
 *参数:      band 频段
 *参数:      default_gain 默认增益
 *返回值：   无
 */
Status_t BSP_866x_set_default_gain(RF_BAND_E band, char default_gain)
{
    //先读出来,修改值,再写回去

    //读出来,前提是一个band的所有参数占一个section
    uint32_t exter_flash_addr;
    uint16_t data_size;
    uint16_t flash_sector;

    memset(&factroy_data_buf, 0, sizeof(factroy_data_buf));

    exter_flash_addr = get_8668_cali_data_save_addr(band);	
    data_size = sizeof(factroy_data_T);
    flash_sector = spi_flash.flash_addr_to_sector(exter_flash_addr);

    BSP_866x_load_factroy_data(&factroy_data_buf, band);

    //写值
    factroy_data_buf.default_gain = default_gain;

    //更新当前配置
    if(band == h8668_LTE.band)
        h8668_LTE.factroy_data.default_gain = default_gain;
    else if(band == h8668_NR.band)
        h8668_NR.factroy_data.default_gain = default_gain;
    else if(band == h8668_NR_MIMO.band)
        h8668_NR_MIMO.factroy_data.default_gain = default_gain;

    //写回去
    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);
    spi_flash.flash_write((uint8_t *)&factroy_data_buf, exter_flash_addr, data_size);
    taskEXIT_CRITICAL();

    return STAT_OK;
}

/**********************************************************
 *函数名：   BSP_866x_get_default_gain
 *函数功能： 保存默认增益,写入到flash
 *参数:      band 频段
 *参数:      default_gain 默认增益
 *返回值：   无
 */
char BSP_866x_get_default_gain(RF_BAND_E band)
{
    //先读出来,修改值,再写回去

    //读出来,前提是一个band的所有参数占一个section
    char default_gain = 0xff;

    memset(&factroy_data_buf, 0, sizeof(factroy_data_buf));
    BSP_866x_load_factroy_data(&factroy_data_buf, band);

    //写值
    default_gain = factroy_data_buf.default_gain;

    return default_gain;
}


/**********************************************************
 *函数名：   BSP_866x_set_rated_gain_index
 *函数功能： 设置额定增益在码表的索引值
 *参数:      band 频段
 *参数:      mode 0正常模式 1抗阻塞模式
 *参数:      index 索引值
 *返回值：   无
 */
Status_t BSP_866x_set_rated_gain_index(RF_BAND_E band, TRX_MODE_E mode, uint32_t bw, uint8_t gain_index)
{
    //先读出来,修改值,再写回去

    //读出来,前提是一个band的所有参数占一个section
    uint32_t exter_flash_addr;
    uint16_t data_size;
    uint16_t flash_sector;
    uint8_t gain_bw_index = 0;

    memset(&factroy_data_buf, 0, sizeof(factroy_data_buf));

    exter_flash_addr = get_8668_cali_data_save_addr(band);
    data_size = sizeof(factroy_data_T);
    flash_sector = spi_flash.flash_addr_to_sector(exter_flash_addr);

    BSP_866x_load_factroy_data(&factroy_data_buf, band);
    
    // 读取码表索引值
    for(int i = 0; i < sizeof(rated_gain_index_lookup_tab)/sizeof(rated_gain_index_lookup_tab_t); i++)
    {
        if(band == rated_gain_index_lookup_tab[i].band && bw == rated_gain_index_lookup_tab[i].bw)
        {
            gain_bw_index = rated_gain_index_lookup_tab[i].index_of_rated_gain_index;
            if(mode == TRX_MODE_ANTI_BLOCKING)
            {
                factroy_data_buf.rated_gain_index_anti_block[gain_bw_index] = gain_index;
            }else
            {
                factroy_data_buf.rated_gain_index_normal[gain_bw_index] = gain_index;
            }
            break;
        }
    }

    //写回去
    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);
    spi_flash.flash_write((uint8_t *)&factroy_data_buf, exter_flash_addr, data_size);
    taskEXIT_CRITICAL();

    return STAT_OK;
}

/**********************************************************
 *函数名：   BSP_866x_get_rated_gain_index
 *函数功能： 读取额定增益在码表的索引值
 *参数:      band 频段
 *返回值:      mode  0正常模式 1抗阻塞模式
 */
uint8_t BSP_866x_get_rated_gain_index(RF_BAND_E band, TRX_MODE_E mode, uint32_t bw)
{
    //先读出来,修改值,再写回去

    //读出来,前提是一个band的所有参数占一个section
    uint8_t gain_index = 0xff;
    uint8_t gain_bw_index = 0;

    memset(&factroy_data_buf, 0, sizeof(factroy_data_buf));
    BSP_866x_load_factroy_data(&factroy_data_buf, band);

    // 读取码表索引值
    for(int i = 0; i < sizeof(rated_gain_index_lookup_tab)/sizeof(rated_gain_index_lookup_tab_t); i++)
    {
        if(band == rated_gain_index_lookup_tab[i].band && bw == rated_gain_index_lookup_tab[i].bw)
        {
            gain_bw_index = rated_gain_index_lookup_tab[i].index_of_rated_gain_index;
            if(mode == TRX_MODE_ANTI_BLOCKING)
            {
                gain_index = factroy_data_buf.rated_gain_index_anti_block[gain_bw_index] ;
            }else
            {
                gain_index = factroy_data_buf.rated_gain_index_normal[gain_bw_index] ;
            }
            break;
        }
    }
    return gain_index;
}
/**********************************************************
 *函数名：   BSP_866x_set_gain_table
 *函数功能： 保存校准值,写入到flash
 *参数:      band 频段
 *参数:      gain_tab_type 增益表类型 普通模式增益或阻塞模式增益
 *参数:      gain_tab 增益表
 *参数:      len 增益表长度
 *返回值：   无
 */
Status_t BSP_866x_set_gain_table(RF_BAND_E band, TRX_MODE_E gain_tab_type, uint8_t * gain_tab, uint32_t len)
{
    //先读出来,修改值,再写回去

    //读出来,一个band的所有参数占一个section
    uint32_t exter_flash_addr;
    uint16_t data_size = 0, tab_length = 0, i = 0, j = 0;
    uint16_t flash_sector;

    memset(&factroy_data_buf, 0, sizeof(factroy_data_buf));
    if(len % 16 != 0 || len < 400 || gain_tab == NULL) // 增益表应该是四列4字节数,所以不被16整除的为非法长度
    {
        return STAT_ERROR;
    }

    exter_flash_addr = get_8668_cali_data_save_addr(band);	// 读取整个band的参数
    data_size = sizeof(factroy_data_T);
    flash_sector = spi_flash.flash_addr_to_sector(exter_flash_addr);

    BSP_866x_load_factroy_data(&factroy_data_buf, band);

    tab_length = len / 16;
    for(i = 0; i < len; i += 16) //全0判断,当连续四个uint32码字都是0时,判断为填充位,记录实际增益码表长度
    {
        for(j = 0; j < 16; j++)
        {
            if(gain_tab[i + j] != 0)
            {
                break;
            }
        }
        if(j == 16)
        {
            tab_length = i / 16;
            break;
        }
    }

    //写值
    switch(gain_tab_type)
    {
        case TRX_MODE_NORMAL:
            memcpy(&factroy_data_buf.normal_gain_tab.gain_tab, gain_tab, len);
            factroy_data_buf.normal_gain_tab.gain_tab_number = tab_length;
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"Set band %d 8668 normal Gaintab, len = %d\r\n", band, tab_length);

            //更新当前配置
            if(band == h8668_LTE.band)
            {
                memcpy(h8668_LTE.factroy_data.normal_gain_tab.gain_tab, gain_tab, len);
                h8668_LTE.factroy_data.normal_gain_tab.gain_tab_number = tab_length;
            }
            else if(band == h8668_NR.band)
            {
                memcpy(h8668_NR.factroy_data.normal_gain_tab.gain_tab, gain_tab, len);
                h8668_NR.factroy_data.normal_gain_tab.gain_tab_number = tab_length;
            }
            else if(band == h8668_NR_MIMO.band)
            {
                memcpy(h8668_NR_MIMO.factroy_data.normal_gain_tab.gain_tab, gain_tab, len);
                h8668_NR_MIMO.factroy_data.normal_gain_tab.gain_tab_number = tab_length;
            }
            break;

        case TRX_MODE_ANTI_BLOCKING:
            memcpy(&factroy_data_buf.blocking_gain_tab.gain_tab, gain_tab, len);
            factroy_data_buf.blocking_gain_tab.gain_tab_number = tab_length;
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"Set band %d 8668 blocking Gaintab, len = %d\r\n", band, tab_length);

            //更新当前配置
            if(band == h8668_LTE.band)
            {
                memcpy(h8668_LTE.factroy_data.blocking_gain_tab.gain_tab, gain_tab, len);
                h8668_LTE.factroy_data.blocking_gain_tab.gain_tab_number = tab_length;
            }
            else if(band == h8668_NR.band)
            {
                memcpy(h8668_NR.factroy_data.blocking_gain_tab.gain_tab, gain_tab, len);
                h8668_NR.factroy_data.blocking_gain_tab.gain_tab_number = tab_length;
            }
            else if(band == h8668_NR_MIMO.band)
            {
                memcpy(h8668_NR_MIMO.factroy_data.blocking_gain_tab.gain_tab, gain_tab, len);
                h8668_NR_MIMO.factroy_data.blocking_gain_tab.gain_tab_number = tab_length;
            }

            break;
        default:
            return STAT_ERROR;
    }

    //写回去
    taskENTER_CRITICAL();
    spi_flash.flash_erase_sector(flash_sector);
    spi_flash.flash_write((uint8_t *)&factroy_data_buf, exter_flash_addr, data_size);
    taskEXIT_CRITICAL();

    return STAT_OK;
}

/**********************************************************
 *函数名：   BSP_866x_get_gain_table
 *函数功能： 读出gain table
 *参数:      band 频段
 *参数:      gain_tab_type 增益表类型 普通模式增益或阻塞模式增益
 *参数:      gain_tab 增益表
 *参数:      len 增益表长度
 *返回值：   无
 */
Status_t BSP_866x_get_gain_table(RF_BAND_E band, TRX_MODE_E gain_tab_type, uint8_t * gain_tab, uint32_t len)
{
    //先读出来,修改值,再写回去
    //一个band的所有参数占一个section

    memset(&factroy_data_buf, 0, sizeof(factroy_data_buf));
    if(len % 16 != 0 || len < 400 || gain_tab == NULL) // 增益表应该是四列4字节数,所以不被16整除的为非法长度
    {
        return STAT_ERROR;
    }

    BSP_866x_load_factroy_data(&factroy_data_buf, band);

    //写值
    switch(gain_tab_type)
    {
        case TRX_MODE_NORMAL:
            if(len <= sizeof(factroy_data_buf.normal_gain_tab.gain_tab))
                memcpy(gain_tab, &factroy_data_buf.normal_gain_tab.gain_tab, len);
            break;

        case TRX_MODE_ANTI_BLOCKING:
            if(len <= sizeof(factroy_data_buf.blocking_gain_tab.gain_tab))
                memcpy(gain_tab, &factroy_data_buf.blocking_gain_tab.gain_tab, len);
            break;
        default:
            return STAT_ERROR;
    }
    return STAT_OK;
}


/**********************************************************
 *函数名：   BSP_866x_cali_data_switch
 *函数功能： 根据当前的射频参数,band,带宽,增益等,设置校准参数
 *返回值：   无
 */
void BSP_866x_cali_data_switch(TRX8668_FPGA_t * h8668)
{
    uint32_t data_tx_lo_I, data_tx_lo_Q, data_rx_lo_I, data_rx_lo_Q, data_mirror;
    uint8_t index_rx = 0;
    uint8_t index_tx = 0;
    //uint8_t index_tx = 0;

    //根据增益档位选择建准数据(rx本振校准)
    //增益档位和rx本振校准的查找表,查找rx本振校准值
    switch(h8668->band)//选择对应band的查找表
    {
        case BAND_3:
            if(h8668->anti_block_stat != B_TRUE)//正常模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = normal_gain_cali_lookup_table_b3;
            else//阻塞模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = blocking_gain_cali_lookup_table_b3;
            break;

        case BAND_8:
            if(h8668->anti_block_stat != B_TRUE)//正常模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = normal_gain_cali_lookup_table_b8;
            else//阻塞模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = blocking_gain_cali_lookup_table_b8;
            break;

        case BAND_39:
            if(h8668->anti_block_stat != B_TRUE)//正常模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = normal_gain_cali_lookup_table_b39;
            else//阻塞模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = blocking_gain_cali_lookup_table_b39;
            break;

        case BAND_40:
            if(h8668->anti_block_stat != B_TRUE)//正常模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = normal_gain_cali_lookup_table_b40;
            else//阻塞模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = blocking_gain_cali_lookup_table_b40;
            break;

        case BAND_41:
            if(h8668->anti_block_stat != B_TRUE)//正常模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = normal_gain_cali_lookup_table_n41;
            else//阻塞模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = blocking_gain_cali_lookup_table_n41;
            break;

        case BAND_41M:
            if(h8668->anti_block_stat != B_TRUE)//正常模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = normal_gain_cali_lookup_table_mimo;
            else//阻塞模式, 增益档位对应的rx本振校准值查找表
                h8668->gain_cali_lookup_table = blocking_gain_cali_lookup_table_mimo;
            break;

        default:
            h8668->gain_cali_lookup_table = NULL;
            return;
    }
    //选择rx本振校准数据(rx本振校准)
    index_rx = h8668->gain_cali_lookup_table[h8668->ch_status.gain_tab_index].rx_cali_index;
    data_rx_lo_I = h8668->factroy_data.cali_rx_lo_I[index_rx];
    data_rx_lo_Q = h8668->factroy_data.cali_rx_lo_Q[index_rx];

    //选择tx本振校准数据(tx本振校准)
    index_tx = h8668->gain_cali_lookup_table[h8668->ch_status.gain_tab_index].tx_cali_index;
    data_tx_lo_I = h8668->factroy_data.cali_tx_lo_I[index_tx];
    data_tx_lo_Q = h8668->factroy_data.cali_tx_lo_Q[index_tx];
    
    //根据band选择校准数据(镜像校准)
    //默认:镜像校准只和band相关,每个band一组
    data_mirror = h8668->factroy_data.cali_mirror[0];

    // index_tx = h8668->gain_cali_lookup_table[h8668->ch_status.gain_tab_index].tx_cali_index;
    // data_tx_lo_I = h8668->factroy_data.cali_rx_lo_I[index_tx];
    // data_tx_lo_Q = h8668->factroy_data.cali_rx_lo_Q[index_tx];

    //和当前校准值比对,当有校准值更新时,写入新的校准数据
    if(data_tx_lo_I != 0xffffffff)// && h8668->ch_status.cur_cali_tx_lo_I != data_tx_lo_I)
    {
        ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CH0_TX_CALI_LO_I_ADDR, data_tx_lo_I);
        h8668->ch_status.cur_cali_tx_lo_I = data_tx_lo_I;
    }
    if(data_tx_lo_Q != 0xffffffff)// && h8668->ch_status.cur_cali_tx_lo_Q != data_tx_lo_Q)
    {
        ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CH0_TX_CALI_LO_Q_ADDR, data_tx_lo_Q);
        h8668->ch_status.cur_cali_tx_lo_Q = data_tx_lo_Q;
    }
    if(data_rx_lo_I != 0xffffffff)// && h8668->ch_status.cur_cali_rx_lo_I != data_rx_lo_I)
    {
        ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CH0_RX_CALI_LO_I_ADDR, data_rx_lo_I);
        hi_delay_ms(1);
        ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR_ADC_RESET_RELSASE_ADDR, ECR_ADC_RESET_RELSASE_VAL);
        h8668->ch_status.cur_cali_rx_lo_I = data_rx_lo_I;
    }
    if(data_rx_lo_Q != 0xffffffff)// && h8668->ch_status.cur_cali_rx_lo_Q != data_rx_lo_Q)
    {
        ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CH0_RX_CALI_LO_Q_ADDR, data_rx_lo_Q);
        hi_delay_ms(1);
        ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR_ADC_RESET_RELSASE_ADDR, ECR_ADC_RESET_RELSASE_VAL);
        h8668->ch_status.cur_cali_rx_lo_Q = data_rx_lo_Q;
    }
    if(data_mirror != 0xffffffff)// && h8668->ch_status.cur_cali_mirror != data_mirror)
    {
        ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CH0_CALI_MIRROR_ADDR, data_mirror);
        h8668->ch_status.cur_cali_mirror = data_mirror;
    }

    h8668->cali_refersh_tick = hi_get_tick(); //更新校准值时间戳
}


/**********************************************************
 *函数名：   BSP_get_bin_addr
 *函数功能： 根据当前band获取bin文件地址
 *返回值：   无
 */

static uint32_t BSP_get_bin_addr(RF_BAND_E band)
{
    return 0;
}

static uint32_t BSP_get_bin_size(RF_BAND_E band)
{
    return 0;
}

/**********************************************************
 *函数名：   BSP_866x_set_band
 *函数功能： trx8668根据选定的band,带宽,频点进行初始化,重加载bin文件,并加载对应band的增益表和校准参数
 *函数参数: band 频段
 *函数参数: band_width 带宽
        #define PLC_LTE_10MHZ (10)         // LTE 10 MHz
        #define PLC_LTE_15MHZ (15)         // LTE 15 MHz
        #define PLC_LTE_20MHZ (20)         // LTE 20 MHz
        #define PLC_LTE_25MHZ (25)         // LTE 15 MHz
        #define PLC_LTE_30MHZ (30)         // LTE 20 MHz
        #define PLC_LTE_50MHZ (50)         // LTE 20 MHz
        #define PLC_NR_40MHZ (40)          // NR 40 MHz
        #define PLC_NR_60MHZ (60)          // NR 60 MHz
        #define PLC_NR_80MHZ (80)          // NR 80 MHz
        #define PLC_NR_100MHZ (100)        // NR 100 MHz
 *函数参数: freq 频点,单位100kHz
 *返回值：   无
 */

Status_t BSP_866x_set_band(TRX8668_FPGA_t * h8668, RF_BAND_E band, uint32_t band_width, uint32_t freq)
{
    // 参数合法性判断
    if(h8668 == NULL)
        return STAT_ERROR;
    if(BSP_866x_wait_lock(h8668,5000) != STAT_OK)
        return STAT_ERROR;
    // 重置通道状态
    h8668->band = band;
    memset(&h8668->ch_status, 0, sizeof(h8668->ch_status));
    h8668->adc_overload_flag = B_FALSE;
    h8668->anti_block_stat = B_FALSE;
    h8668->silent_mode = B_FALSE;

    switch(h8668->band)
    {
        case BAND_3:
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"\r\n8668 %d. Init with band3\r\n", h8668->config.dev_number);
            break;
        case BAND_8:
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"\r\n8668 %d. Init with band8\r\n", h8668->config.dev_number);
            break;
        case BAND_39:
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"\r\n8668 %d. Init with band39\r\n", h8668->config.dev_number);
            break;
        case BAND_40:
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"\r\n8668 %d. Init with band40\r\n", h8668->config.dev_number);
            break;
        case BAND_41:
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"\r\n8668 %d. Init with nr\r\n", h8668->config.dev_number);
            break;
        case BAND_41M:
            rtt_printf(RTT_8668, RTT_TEXT_YELLOW"\r\n8668 %d. Init with mimo\r\n", h8668->config.dev_number);
            break;
        default:
            break;
    }
    // 加载固件
    h8668->bin_addr = BSP_get_bin_addr(band);
    h8668->bin_siz = BSP_get_bin_size(band);
    if(BSP_Ecr8668Init(h8668) == STAT_ERROR) //执行初始化,加载bin文件
    {
        BSP_866x_unlock(h8668);
        return STAT_ERROR;
    }

    // 加载校准值,增益表
    if(BSP_866x_load_factroy_data(&h8668->factroy_data, band) == STAT_ERROR)
    {
        BSP_866x_unlock(h8668);
        return STAT_ERROR;
    }

    // 通道参数配置

    //wdg_in_task(); // 喂狗,防止时间过长重启

    Ecr866xChModeCfg(h8668, CH0_1T1R_OWN_TRX_LO); // 通道模式配置

    Ecr866xBwChoose(h8668, h8668->band, band_width); // 带宽配置

    Ecr866xRxFreqCfg(h8668, h8668->ch, freq); // 接收频率配置

    //Ecr866xTxFreqCfg(h8668, h8668->ch, freq); // 发送频率配置

    Ecr866xRxGainCfg(h8668, h8668->ch, -20); // RX增益偏移配置

    Ecr866xTxGainCfg(h8668, h8668->ch, 0); // TX增益偏移配置


    vTaskDelay(1000); //

    Ecr866xPortCfg(h8668, h8668->config.port); // 端口配置

    Ecr866xLoopCfg(h8668, LOOP_CH0_OPEN); // 回环配置

    ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CLOSE_RBDP_ADDR, (ECR866x_CLOSE_RBDP_DATA)); // 关闭RBDP

    //选择校准参数
    BSP_866x_gain_tab_switch(h8668);//根据当前band选择增益表
    BSP_866x_cali_data_switch(h8668);//根据当前band选择校准值

    //RSSIEN
    ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CH0_RSSI_EN_ADDR0, (ECR866x_CH0_RSSI_EN_DATA0));
    ECR8660_write(h8668->config.spi_port, SPI_RW_EXTERNAL, ECR866x_CH0_RSSI_EN_ADDR1, (ECR866x_CH0_RSSI_EN_DATA1));


    vTaskDelay(500); //
    Ecr866xPortCfg(h8668, h8668->config.port); // 端口配置
    vTaskDelay(500); //
	
    BSP_866x_unlock(h8668);
    return STAT_OK;
}

/**********************************************************
 *函数名：   BSP_866x_run
 *函数功能： trx8668的告警处理(adc饱和等),错误处理,器件状态监控以及其他一些8668器件本身的业务
 *返回值：   无
 */
void BSP_866x_run(TRX8668_FPGA_t * h8668)
{
    static uint32_t last_tick = 0;
    
    if(hi_get_tick() - last_tick > 50)
    {
        if(BSP_866x_try_lock(h8668) != STAT_OK)
            return ;
        BSP_866x_error_handle(h8668); //错误处理
        
        if(hi_get_tick() - h8668->cali_refersh_tick >= 500)
            BSP_866x_cali_data_switch(h8668);   //校准值周期更新

        BSP_866x_unlock(h8668);
        last_tick = hi_get_tick();
    }
}

/**********************************************************
 *函数名：   isr_866x_quit_silent_mode
 *函数功能： (从机)退出上行静默,在GPIO中断中调用
 *返回值：   无
*/
void isr_866x_quit_silent_mode(TRX8668_FPGA_t * h8668)
{
    rtt_printf(RTT_8668, RTT_TEXT_YELLOW"trx866x %d quit silent mode\r\n", h8668->config.dev_number);
    h8668->silent_mode = B_FALSE;
}

/**********************************************************
 *函数名：   BSP_866x_enter_silent_mode
 *函数功能： (从机)8668进入上行静默
 *返回值：   无
 */
void BSP_866x_enter_silent_mode(TRX8668_FPGA_t * h8668)
{
    // 进入静默模式
    rtt_printf(RTT_8668, RTT_TEXT_YELLOW"trx866x %d enter silent mode\r\n", h8668->config.dev_number);
    h8668->silent_mode = B_TRUE;
}

/**
 * @brief 设置8668衰减
 * @param ch
 * @param value
 * @return int
 */
Status_t BSP_8668AttSet(TRX8668_FPGA_t * h8668, int8_t value)
{    
    Status_t ret = STAT_OK;
    
    if(BSP_866x_try_lock(h8668) != STAT_OK)
        return STAT_ERROR;

    BSP_866x_gain_tab_switch(h8668);     //增益表切换
    ret = Ecr8668SetAtt(h8668, value);   //设置衰减值
    BSP_866x_cali_data_switch(h8668);    //校准值切换
    
    BSP_866x_unlock(h8668);
    return ret;
}

/**********************************************************
 *函数名：   BSP_866x_cali_test
 *函数功能： 8668校准存取测试
 *返回值：   无
 *说  明：   校准值存取,默认增益存取,码表存取
 */
# if (0)
factroy_data_T factroy_data_rd, factroy_data_rd1;
#define TEST_BAND BAND_41M
void BSP_866x_cali_test(void)
{

    BSP_866x_set_band(&h8668_LTE, TEST_BAND, PLC_LTE_20MHZ, 18000);

    BSP_866x_load_factroy_data(&factroy_data_rd, TEST_BAND);

    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 0, 0x2101);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 1, 0x2102);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 2, 0x2103);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 3, 0x2104);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 4, 0x2105);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 5, 0x2106);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 6, 0x2107);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 7, 0x2108);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 8, 0x2109);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 9, 0x210A);

    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 0, 0x2201);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 1, 0x2202);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 2, 0x2203);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 3, 0x2204);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 4, 0x2205);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 5, 0x2206);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 6, 0x2207);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 7, 0x2208);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 8, 0x2209);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 9, 0x220A);

    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 0, 0x2301);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 1, 0x2302);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 2, 0x2303);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 3, 0x2304);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 4, 0x2305);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 5, 0x2306);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 6, 0x2307);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 7, 0x2308);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 8, 0x2309);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 9, 0x230A);

    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 0, 0x2401);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 1, 0x2402);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 2, 0x2403);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 3, 0x2404);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 4, 0x2405);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 5, 0x2406);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 6, 0x2407);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 7, 0x2408);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 8, 0x2409);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 9, 0x240A);

    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_MIRROR, 0, 0x2501);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_MIRROR, 1, 0x2502);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_MIRROR, 2, 0x2503);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_MIRROR, 3, 0x2504);
    BSP_866x_set_cali_data(TEST_BAND, CALI_8668_MIRROR, 4, 0x2505);

    BSP_866x_set_default_gain(TEST_BAND, 0xab);

    BSP_866x_set_gain_table(TEST_BAND, TRX_MODE_ANTI_BLOCKING, (uint8_t *)default_gain_tab_antiblocking.gain_tab, MAX_ALC_GAIN_NUM * 4 * 4);
    BSP_866x_set_gain_table(TEST_BAND, TRX_MODE_NORMAL, (uint8_t *)default_gain_tab.gain_tab, MAX_ALC_GAIN_NUM * 4 * 4);

    BSP_866x_load_factroy_data(&factroy_data_rd, TEST_BAND);


    BSP_866x_set_default_gain(BAND_3, 0x03);
    BSP_866x_set_default_gain(BAND_8, 0x08);
    BSP_866x_set_default_gain(BAND_39, 0x09);
    BSP_866x_set_default_gain(BAND_40, 0x04);
    BSP_866x_set_default_gain(BAND_41, 0x05);
    BSP_866x_set_default_gain(BAND_41M, 0x06);

    char b3_g = BSP_866x_get_default_gain(BAND_3);
    char b8_g = BSP_866x_get_default_gain(BAND_8);
    char b39_g = BSP_866x_get_default_gain(BAND_39);
    char b40_g = BSP_866x_get_default_gain(BAND_40);
    char b41_g = BSP_866x_get_default_gain(BAND_41);
    char b41m_g = BSP_866x_get_default_gain(BAND_41M);

    rtt_printf(RTT_8668, "8668 default gain: %x %x %x %x %x %x\r\n", b3_g, b8_g, b39_g, b40_g, b41_g, b41m_g);

    factroy_data_rd1.cali_rx_lo_I[0] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 0);
    factroy_data_rd1.cali_rx_lo_I[1] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 1);
    factroy_data_rd1.cali_rx_lo_I[2] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 2);
    factroy_data_rd1.cali_rx_lo_I[3] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 3);
    factroy_data_rd1.cali_rx_lo_I[4] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 4);
    factroy_data_rd1.cali_rx_lo_I[5] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 5);
    factroy_data_rd1.cali_rx_lo_I[6] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 6);
    factroy_data_rd1.cali_rx_lo_I[7] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 7);
    factroy_data_rd1.cali_rx_lo_I[8] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 8);
    factroy_data_rd1.cali_rx_lo_I[9] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_I, 9);

    factroy_data_rd1.cali_rx_lo_Q[0] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 0);
    factroy_data_rd1.cali_rx_lo_Q[1] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 1);
    factroy_data_rd1.cali_rx_lo_Q[2] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 2);
    factroy_data_rd1.cali_rx_lo_Q[3] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 3);
    factroy_data_rd1.cali_rx_lo_Q[4] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 4);
    factroy_data_rd1.cali_rx_lo_Q[5] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 5);
    factroy_data_rd1.cali_rx_lo_Q[6] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 6);
    factroy_data_rd1.cali_rx_lo_Q[7] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 7);
    factroy_data_rd1.cali_rx_lo_Q[8] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 8);
    factroy_data_rd1.cali_rx_lo_Q[9] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_RX_LO_Q, 9);

    factroy_data_rd1.cali_tx_lo_I[0] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 0);
    factroy_data_rd1.cali_tx_lo_I[1] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 1);
    factroy_data_rd1.cali_tx_lo_I[2] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 2);
    factroy_data_rd1.cali_tx_lo_I[3] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 3);
    factroy_data_rd1.cali_tx_lo_I[4] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 4);
    factroy_data_rd1.cali_tx_lo_I[5] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 5);
    factroy_data_rd1.cali_tx_lo_I[6] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 6);
    factroy_data_rd1.cali_tx_lo_I[7] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 7);
    factroy_data_rd1.cali_tx_lo_I[8] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 8);
    factroy_data_rd1.cali_tx_lo_I[9] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_I, 9);

    factroy_data_rd1.cali_tx_lo_Q[0] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 0);
    factroy_data_rd1.cali_tx_lo_Q[1] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 1);
    factroy_data_rd1.cali_tx_lo_Q[2] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 2);
    factroy_data_rd1.cali_tx_lo_Q[3] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 3);
    factroy_data_rd1.cali_tx_lo_Q[4] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 4);
    factroy_data_rd1.cali_tx_lo_Q[5] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 5);
    factroy_data_rd1.cali_tx_lo_Q[6] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 6);
    factroy_data_rd1.cali_tx_lo_Q[7] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 7);
    factroy_data_rd1.cali_tx_lo_Q[8] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 8);
    factroy_data_rd1.cali_tx_lo_Q[9] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_TX_LO_Q, 9);

    factroy_data_rd1.cali_mirror[0] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_MIRROR, 0);
    factroy_data_rd1.cali_mirror[1] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_MIRROR, 1);
    factroy_data_rd1.cali_mirror[2] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_MIRROR, 2);
    factroy_data_rd1.cali_mirror[3] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_MIRROR, 3);
    factroy_data_rd1.cali_mirror[4] = BSP_866x_get_cali_data(TEST_BAND, CALI_8668_MIRROR, 4);

    BSP_866x_set_rated_gain_index(BAND_3, TRX_MODE_NORMAL, 4);
    BSP_866x_set_rated_gain_index(BAND_8, TRX_MODE_NORMAL, 4);
    BSP_866x_set_rated_gain_index(BAND_39, TRX_MODE_NORMAL, 4);
    BSP_866x_set_rated_gain_index(BAND_40, TRX_MODE_NORMAL, 4);
    BSP_866x_set_rated_gain_index(BAND_41, TRX_MODE_NORMAL, 4);
    BSP_866x_set_rated_gain_index(BAND_41M, TRX_MODE_NORMAL, 4);

    BSP_866x_set_rated_gain_index(BAND_3, TRX_MODE_ANTI_BLOCKING, 5);
    BSP_866x_set_rated_gain_index(BAND_8, TRX_MODE_ANTI_BLOCKING, 5);
    BSP_866x_set_rated_gain_index(BAND_39, TRX_MODE_ANTI_BLOCKING, 5);
    BSP_866x_set_rated_gain_index(BAND_40, TRX_MODE_ANTI_BLOCKING, 5);
    BSP_866x_set_rated_gain_index(BAND_41, TRX_MODE_ANTI_BLOCKING, 5);
    BSP_866x_set_rated_gain_index(BAND_41M, TRX_MODE_ANTI_BLOCKING, 5);

    char b3_i1 = BSP_866x_get_rated_gain_index(BAND_3, TRX_MODE_NORMAL);
    char b8_i1 = BSP_866x_get_rated_gain_index(BAND_8, TRX_MODE_NORMAL);
    char b39_i1 = BSP_866x_get_rated_gain_index(BAND_39, TRX_MODE_NORMAL);
    char b40_i1 = BSP_866x_get_rated_gain_index(BAND_40, TRX_MODE_NORMAL);
    char b41_i1 = BSP_866x_get_rated_gain_index(BAND_41, TRX_MODE_NORMAL);
    char b41m_i1 = BSP_866x_get_rated_gain_index(BAND_41M, TRX_MODE_NORMAL);

    rtt_printf(RTT_8668, "8668 rated_gain_index normal: %d %d %d %d %d %d\r\n", b3_i1, b8_i1, b39_i1, b40_i1, b41_i1, b41m_i1);

    char b3_i2 = BSP_866x_get_rated_gain_index(BAND_3, TRX_MODE_ANTI_BLOCKING);
    char b8_i2 = BSP_866x_get_rated_gain_index(BAND_8, TRX_MODE_ANTI_BLOCKING);
    char b39_i2 = BSP_866x_get_rated_gain_index(BAND_39, TRX_MODE_ANTI_BLOCKING);
    char b40_i2 = BSP_866x_get_rated_gain_index(BAND_40, TRX_MODE_ANTI_BLOCKING);
    char b41_i2 = BSP_866x_get_rated_gain_index(BAND_41, TRX_MODE_ANTI_BLOCKING);
    char b41m_i2 = BSP_866x_get_rated_gain_index(BAND_41M, TRX_MODE_ANTI_BLOCKING);

    rtt_printf(RTT_8668, "8668 rated_gain_index blocking: %d %d %d %d %d %d\r\n", b3_i2, b8_i2, b39_i2, b40_i2, b41_i2, b41m_i2);
}


/**********************************************************
 *函数名：   BSP_866xAttSetTest
 *函数功能： 用于测试8668衰减设置
 *返回值：   无
 */
void BSP_866xAttSetTest(void)
{
    static uint32_t i,cnt;
    uint8_t att_num = BSP_866x_get_att_num(&h8668_LTE);
    static uint32_t last_tick = 0;
    
    UNUSED_PARA(cnt);

    //cnt++;
    //if(cnt % 2000 < 1000)//测试adc饱和中断判断和增益表切换
        //h8668_LTE.adc_overload_trigger = B_TRUE;

    if (hi_get_tick() - last_tick < 1000)
    {
        return;
    }
    last_tick = hi_get_tick();

    BSP_8668AttSet(&h8668_LTE, i);
	att_analog_4g_dw.set( &att_analog_4g_dw, 13-i );
    i++;
    //if (i > att_num-13) //注入一个超出范围的att
	if (i > 13) //注入一个超出范围的att
    {
        i = 0;
    }

}

#endif
