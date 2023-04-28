#include "ws_test_client.h"
#include "srv_hardware.h"
#include "ws_dataflow.h"
#include "srv_websocket.h"
#include "service.h"

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


        if (g_table && ratelimit_connects(&tsec, 0u)) {
            hw->show_hw_table();
        }

        usleep(10 *10 * 1000);          // 业务频率
    }
    
    return -1;
}

/* WDOG： reboot*/
void drv_init()
{
    SHM_DATA_DEF *shmData;

    shm_hardware_init((void **)&shmData);
    // int shm_init(shm_handle_t *shm_handle)

    hw = new hardware(shmData);
}


/**
 * 解析json配置， 初始化hw_cache_t **pCache
*/
void serve_init()
{
    // hw_cache_t *pc;

    // pc = hw->pCache[0];

    // pc->val_type;
    // pc->kks;
    // pc->_GPIO;

    conf_main_init();
    zlog_info("conf main init done");

}


int main(int argc, char* argv[])
{
    int rtn;

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signal_handler);     /* ctrl^c */
    signal(SIGQUIT, signal_handler);    /* ctrl^\ */
    signal(SIGTERM, signal_handler);    /* kill/killall */
    signal(SIGTSTP, signal_table_handler);    /* ctrl Z （stop show）/*/

    /* 驱动程序初始化*/
    drv_init();

    /* 业务程序初始化： json配置 lws */
    serve_init();
    
    /* Use Tool*/
    option_main(argc, argv);

    /* 上位机 数据交互 线程*/
    // run_ws_client();

    while (g_running)
    {
        loop();

        // sleep(1);
        zlog_fatal("sys crash!");
    }

    delete hw;

    return 0;
}