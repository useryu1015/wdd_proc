#include "ws_test_client.h"
#include "srv_hardware.h"
#include "ws_dataflow.h"
#include "srv_websocket.h"
#include "service.h"

int g_running = true;
int g_table = false;

void signal_handler(int signo)
{
    g_running = false;
}

void signal_table_handler(int signo)
{
    g_table = !g_table;
}

/**
 * 业务主线程:
 *  WDOG、重启、
 *  优化： 将每个任务改为独立线程
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

        /* 上位机数据交互*/
        ws_heartbeat();

        if (g_table && ratelimit_connects(&tsec, 0u)) {
            hw->show_hw_table();
        }

        usleep(10 *10 * 1000);          // 业务频率， 改为 ulimit-time
    }
    
    return -1;
}

/* WDOG： reboot*/
void drv_init()
{
    SHM_DATA_DEF *shmData;

    shm_hardware_init((void **)&shmData);

    hw = new hardware(shmData);
}


/**
 * 解析json配置， 初始化hw_cache_t **pCache
*/
void serve_init()
{
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

    /* 通道维护服务*/
    start_channel_services((void *)0);
    sleep(2);
    
    while (g_running)
    {
        loop();

        zlog_fatal("sys crash!");
    }

    delete hw;

    return 0;
}