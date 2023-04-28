#include "dataDef.h"
#include "srv_hardware.h"
#include "ws_dataflow.h"

// int hw_shm_init()
// {

// }

// int hw_param_monitor(RTU_INFO *p_info, SHM_DATA_RO *p_data){
    
// }

// /**
//  * planA: shm中updateTime更新， 便更新数据
// */
// int _hw_param_monitor(hw_device_info_t *p_dev, RTU_INFO *p_info, SHM_DATA_RO *p_data)
// {
//     int i;

//     if (!p_info || !p_data || !p_dev)
//         return -1;


//     for (i = 0; i < p_info->aiChNumI; i++)
//     {
//         // if(p_data->updateTime == 0)
//             // continue;

//         p_data->aiValI[i];
//     }


//     // for (i = 0; i < p_dev->nDevice; i++)
//     // {
        
//     // }

// }


// /**
//  * planB: 监听每个参数是否有修改。。
// */
// int __hw_param_monitor(SHM_DATA_RO *p_data_last, SHM_DATA_RO *p_data, RTU_INFO *p_info, hw_data_l *)
// {
//     int i;
//     SHM_DATA_RO *last = p_data_last;

//     if (!p_info || !p_data || !last)
//         return -1;

//     for (i = 0; i < p_info->aiChNumI; i++)
//     {
//         if (last->aiValI[i] == p_data->aiValI[i])
//             continue;

//     }


// }

/**
 * 采集硬件参数值
 *  1. 监听updateTime 更新数据到上位机
 *  2. 根据RTU_INFO 判断数据是否有效
 *  3. 数据类型转换？
*/
int run_hardware_monitor()
{

    /* */
    // hw_shm_init();


    /* 监控硬件参数， 如果有数据更新xx */
    // hw_param_monitor();


    // 上位机推送数据
    // lws_callback_on_writable(vhd->wsi_multi);       // 触发LWS_CALLBACK_CLIENT_WRITEABLE回调

    return 0;
}




// static int hardware_info_init_Bak(SHM_DATA_DEF *shmD)
// {
//     if (!shmD || !&shmD->info || !&shmD->data) {
//         zlog_error("hardware init error");      // fix
//         delete hw;
//         return;
//     }
    
//     /* mount shm To hardware class*/
//     shm_data = shmD;
//     dev_info = &shmD->info;
//     real_data = &shmD->data;

//     zlog_info("HW: nChI:%d nChV:%d nChEnv:%d ", shmD->info.aiChNumI, shmD->info.aiChNumV, shmD->info.diChNum);

//     /**
//      * 初始化硬件参数：内存分配、默认属性 
//      */
//     /*电流*/
//     nAiChI = (dev_info->aiChNumI < AI_NUM_MAX) ? dev_info->aiChNumI : AI_NUM_MAX;
//     if (nAiChI) {
//         pAiChI = (hw_cache_t **)objects_new_2arr(nAiChI, sizeof(hw_cache_t));
//     } else {
//         zlog_debug("Channel number too less %d", nAiChI);
//         // fix: avoid segment fault
//         // pAiChI = (hw_cache_t **)objects_new_2arr(1, sizeof(hw_cache_t));
//         // nAiChI = 1;
//     }

//     for (i = 0; i < nAiChI; i++) {
//         pAiChI[i]->val_type = HW_TYPE_CUR;

//     }

//     /*电压*/
//     nAiChV = dev_info->aiChNumV;
//     if (nAiChV) {
//         pAiChV = (hw_cache_t **)objects_new_2arr(nAiChV, sizeof(hw_cache_t));
//     } else {
//         zlog_debug("Channel number too less %d", nAiChV);
//     }

//     for (i = 0; i < nAiChV; i++) {
//         pAiChV[i]->val_type = HW_TYPE_VOL;

//     }

//     /*环境*/
//     nAiChEnv = get_env_isEnable_number();
//     if (nAiChEnv) {
//         pChEnv = (hw_cache_t **)objects_new_2arr(nAiChEnv, sizeof(hw_cache_t));
//     } else {
//         zlog_debug("Channel number too less %d", dev_info->aiChNumI);
//     }

//     for (i = 0, n = 0; i < JSON_HW_ENV_SNUM; i++) {
//         hw_dev_type_e vtype = (hw_dev_type_e)(HW_TYPE_TEMP + i);     // fix
        
//         if (!get_env_isIns_by_vtype(vtype, dev_info))
//             continue;

//         pChEnv[n]->val_type = vtype;
//         pChEnv[n]->status = HW_DATA_INIT;
        
//         n++;
//     }

//     // zlog_info("HW: nChI:%d nChV:%d nChEnv:%d ", nAiChI, nAiChV, nAiChEnv);
//     zlog_info("hardware init done");

// }

// void hardware::hw_param_monitor()
// {
//     int i;

//     // zlog_info("aiChNumI: %d", dev_info->aiChNumI);
//     for (i = 0; i < dev_info->aiChNumI; i++)
//     {
//         hw_cache_t *pd = pCache[i];
//         // zlog_info("Value [cache:%f realData:%f]", pd->val, real_data->aiValI[i]);

//         /* if 未更新 && 超过定时更新周期*/
//         // pd->status = HW_DATA_KEEP;

//         // if (last_data->aiValI[i] == real_data->aiValI[i])
//         if (pd->val == real_data->aiValI[i] && 1)
//             continue;

//         /* 更新数据 */
//         pthread_mutex_lock(&pd->lock_ring); /* --------- ring lock { */
//         pd->val = real_data->aiValI[i];     // fix: 定位方式
//         pd->t = real_data->updateTime;      // fix: 时钟源

//         pd->status = HW_DATA_UPTARE;
//         pthread_mutex_unlock(&pd->lock_ring); /* } ring lock */
//         // zlog_info("Value: %f", pd->val);
//     }

//     // zlog_info("aiChNumV: %d", dev_info->aiChNumV);
//     // for (i = 0; i < dev_info->aiChNumV; i++)


    /* 更新环境参数*/
    // for (i = 0; i < nAiChEnv && 0; i++)
    // {
    //     hw_cache_t *pd = pChEnv[i];
    //     float realdata = get_env_value_by_vtype(pd->val_type);

    //     if (pd->val == realdata)
    //         continue;

    //     pthread_mutex_lock(&pd->lock_ring);
    //     pd->val = realdata;
    //     pd->t = real_data->updateTime;      // fix: 时钟源

    //     pd->status = HW_DATA_UPTARE;
    //     pthread_mutex_unlock(&pd->lock_ring);
    //     // zlog_info("Value: %f", pd->val);
    // }

// }













// /* lose: 硬件初始化: 外设数据、外设状态、外设信息 */
// typedef struct _hw_device_info_t
// {
//     item_time_t *pDevTime;      // 设备更新时间 （按照模块定义

//     hw_dev_type_e val_type;    // 设备类型
//     uint32_t freq;              // 更新频率 ms
    
//     // uint32_t nDevice;        // 在线设备总数
//     // RTU_INFO *pDevInfo;
//     // SHM_DATA_RO *pDeDate;       // = SHM_DATA_RO

//     // uint32_t nChI; 
//     // // p_valI

//     // uint32_t nChV;
//     // // p_valI

//     // uint32_t nEnv;
//     // // p_valEnv
// } hw_device_info_t;

// /* 使用链表，寄存需要上报的json数据集 （重构 */
// typedef struct _hw_data_l
// {
//     hw_dev_type_e val_type;    // 设备类型
//     char *val;
//     char *t;

//     struct _hw_data_l *next;
// } hw_data_l;


// typedef struct _item_time_t
// {
//     time_t last_update; //数据更新时间
//     time_t last_record; //数据记录时间
// } item_time_t;


// // lws 自定义子协议栈
// typedef struct _hw_status_t
// {

// 	struct lws_client_connect_info *conn_info;	// 客户端连接信息
// 	struct lws *wsi_multi;		// 协议接口                         // if (wsi == wsi_multi[n])
//     unsigned int rl_multi;		// 连接时间 realtime
//     const char *prot_name;		// 协议名称
    
// // 1) 通过user寄存收发数据	
// 	void *p_user;				// user ponit
// 	int user_len;

// // 2) 通过lws数据结构, 处理收发数据
// 	pthread_t pthread_spam;		// 业务线程
// 	pthread_mutex_t lock_ring;	/* serialize access to the ring buffer */
// 	struct lws_ring *ring;		/* ringbuffer 保存未发送的数据 */

// 	int counter;
// 	volatile char pthread_state;	// 业务线程状态:
// 	volatile char established;		// ws连接状态
// 	volatile char user_state;		// p_user收发状态

//     /* io buf*/
//     // ws_io_cache_t cache;
// } hw_status_t;



// /* 环境参数上报结构 */
// typedef struct _hw_parame_env_t
// {
//     /* normal*/
//     int terminal;
//     int id;

//     /* private*/
// } hw_parame_env_t;