#include "ws_test_client.h"


/**
 * 多线程xx
 *  1. 封装系统线程函数
 *  2. 开发 线程调度机制
 *  3. 
*/
int start_channel_services(void *arg)
{
    pthread_t thread_id;


    /* websocket 上位机 数据交互线程*/
    pthread_create(&thread_id, NULL, run_ws_client, NULL);
    // pthread_detach(thread_id);





}