#include "ws_test_client.h"
#include "srv_hardware.h"
#include "ws_dataflow.h"
#include "srv_websocket.h"

int g_running = true;
int g_table = false;
/*段错信号，打印堆栈信息*/
// static void dump_segmentation(int signo)
// {
//     int i;
//     int nptrs;
//     void *p_buf[256];
//     char **p_strs;
//     nptrs = backtrace(p_buf, 256);
//     printf("backtrace size:%d\n", nptrs);
//     p_strs = backtrace_symbols(p_buf, nptrs);
//     if (p_strs == NULL)
//     {
//         perror("symbols err");
//     }
//     else
//     {
//         for (i = 0; i < nptrs; i++)
//         {
//             printf(" [%02d] %s\n", i, p_strs[i]);
//         }
//         free(p_strs);
//     }
//     exit(-1);
// }

void signal_handler(int signo)
{
    g_running = false;
}

void signal_table_handler(int signo)
{
    g_table = !g_table;
}

/**
 * 业务主线程
 *  WDOG、重启、
*/
int loop()
{
    static unsigned int tsec;

    while (g_running)
    {
        /* WDOG*/

        /* 更新共享内存数据*/
        // run_shm_heartbeat();

        /* 外设参数采集*/
        hw->hw_heartbeat();
        // run_hardware_monitor();

        /* 上位机数据交互*/
        ws_heartbeat();


        if (g_table && ratelimit_connects(&tsec, 1u)) {
            hw->show_hw_table();
        }

        // ws_format_hardware_data();

        usleep(10 * 1000);          // 业务频率
    }
    
    return -1;
}

/* WDOG： reboot*/
void drv_init()
{
    SHM_DATA_DEF *shmData;

    shm_hardware_init((void **)&shmData);
    // int shm_init(shm_handle_t *shm_handle)

    sleep(1);
    hw = new hardware(shmData);

}

int main()
{
    int rtn;

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signal_handler);     /* ctrl^c */
    signal(SIGQUIT, signal_handler);    /* ctrl^\ */
    signal(SIGTERM, signal_handler);    /* kill/killall */

    signal(SIGTSTP, signal_table_handler);    /* ctrl Z （stop show）/*/


    /* 驱动程序初始化*/
    drv_init();

    /* 上位机 数据交互 线程*/
    // run_ws_client();

    while (g_running)
    {
        loop();

        // zlog_fatal("sys crash!");
        // sleep(1);
    }

    delete hw;

    return 0;
}