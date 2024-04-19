#ifndef _APP_MONITOR_REPORT_H_
#define _APP_MONITOR_REPORT_H_

#include "main.h"

#define SOCKET_REPORT_QUEUE_LEN     10
#define SOCKET_REPORT_QUEUE_SIZE    100

#define LOGIN_FAILD_TIME	        ( 3* 60 * 1000 )

uint16_t mu_monitor_report(uint8_t id, uint8_t type, uint8_t *data_in, uint16_t data_len,uint8_t *sendBuf);

void monitor_report_handle(void);


typedef struct
{
    uint8_t ucSocketSendLen;
    uint8_t ucSocketBuf[SOCKET_REPORT_QUEUE_SIZE];
}SocketQueue_T;

void socket_queue_create(void);
void monitor_queue_recv(void);


void report_process(uint8_t type);
void report_reply_process(uint8_t dev_id,uint8_t type);

void monitor_heart_beat_last_time_set(uint8_t dev_id);

#endif
