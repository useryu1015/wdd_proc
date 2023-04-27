#pragma once

#include "dataDef.h"
#include "common.h"

typedef struct _item_time_t
{
    time_t last_update; //数据更新时间
    time_t last_record; //数据记录时间
} item_time_t;


// lws 自定义子协议栈
typedef struct _hw_status_t
{

	struct lws_client_connect_info *conn_info;	// 客户端连接信息
	struct lws *wsi_multi;		// 协议接口                         // if (wsi == wsi_multi[n])
    unsigned int rl_multi;		// 连接时间 realtime
    const char *prot_name;		// 协议名称
    
// 1) 通过user寄存收发数据	
	void *p_user;				// user ponit
	int user_len;

// 2) 通过lws数据结构, 处理收发数据
	pthread_t pthread_spam;		// 业务线程
	pthread_mutex_t lock_ring;	/* serialize access to the ring buffer */
	struct lws_ring *ring;		/* ringbuffer 保存未发送的数据 */

	int counter;
	volatile char pthread_state;	// 业务线程状态:
	volatile char established;		// ws连接状态
	volatile char user_state;		// p_user收发状态

    /* io buf*/
    // ws_io_cache_t cache;
} hw_status_t;



/* 环境参数上报结构 */
typedef struct _hw_parame_env_t
{
    /* normal*/
    int terminal;
    int id;

    /* private*/
} hw_parame_env_t;




/* 数据类型： vtype*/
typedef enum _hw_device_type_e
{
    HW_TYPE_CUR = 0x00,
    HW_TYPE_VOL,
    // ENV or ASE or PSE
    HW_TYPE_TEMP,
    HW_TYPE_HUMI,
    HW_TYPE_NIOSE,
    HW_TYPE_PM2_5,
    HW_TYPE_PM10,
    HW_TYPE_O2,
    HW_TYPE_TVOC,
    // ASE or PSE
    HW_TYPE_FLOW,   //流量
    HW_TYPE_SFLOW,  //累积流量
    HW_TYPE_RSPD,   //转速
    HW_TYPE_FREQ,   //频率
    HW_TYPE_POWER,  //功率    
    HW_TYPE_ELEC,   //电能
    HW_TYPE_1,   //
    HW_TYPE_2,   //
    // 
    HW_TYPE_TBD,
    HW_TYPE_END         // fix: 遍历枚举 switch
} hw_device_type_e;

/* 设备种类： “terminal”字段*/
typedef enum _hw_device_kind_e
{
    HW_KIND_CUR,
    HW_KIND_VOL,
    HW_KIND_ENV,
    HW_KIND_ASE,    // 有源
    HW_KIND_PSE     // 无源
} hw_device_kind_e;

/* 数据状态定义： */
typedef enum _hw_data_status_e
{
    HW_DATA_UPTARE = 0,
    HW_DATA_KEEP,
    HW_DATA_USED,
    HW_DATA_DOWN,
    HW_DATA_INIT
} hw_data_status_e;

/* lose: 硬件初始化: 外设数据、外设状态、外设信息 */
typedef struct _hw_device_info_t
{
    item_time_t *pDevTime;      // 设备更新时间 （按照模块定义

    hw_device_type_e val_type;    // 设备类型
    uint32_t freq;              // 更新频率 ms
    
    // uint32_t nDevice;        // 在线设备总数
    // RTU_INFO *pDevInfo;
    // SHM_DATA_RO *pDeDate;       // = SHM_DATA_RO

    // uint32_t nChI; 
    // // p_valI

    // uint32_t nChV;
    // // p_valI

    // uint32_t nEnv;
    // // p_valEnv
} hw_device_info_t;

/* 使用链表，寄存需要上报的json数据集 （重构 */
typedef struct _hw_data_l
{
    hw_device_type_e val_type;    // 设备类型
    char *val;
    char *t;

    struct _hw_data_l *next;
} hw_data_l;

typedef struct _hw_cache_t
{
    hw_device_type_e val_type;      // 数据类型
    // hw_device_kind_e dev_kind;      // 设备种类
    union 
    {
        uint16_t aiChI;		/*!< AI:电流通道 */
        uint16_t aiChV;		/*!< AI:电压通道 */
        uint16_t diCh;		/*!< ＤI:通道 */
    };

    float val;
    struct timeval t;

    pthread_mutex_t lock_ring;
    hw_data_status_e status;             // 保持、更新、掉线、
}__attribute__((packed)) hw_cache_t;


class hardware
{
public:
    hardware();
    hardware(SHM_DATA_DEF *shmD);
    ~hardware();                // delete call

    /* A: 数据结构寄存所有硬件参数*/
    uint16_t nAiChI;            // intensity ch number
    hw_cache_t **pAiChI;
    uint16_t nAiChV;
    hw_cache_t **pAiChV;
    uint16_t nAiChEnv;
    hw_cache_t **pChEnv;

    /* A: */
    hw_device_kind_e devKind;   // 设备种类     // by HW info.type
    uint16_t nChNum;          // 参数数量
    hw_cache_t **pVal;        // 参数信息

    /* B: 使用链表寄存有更新的数据（监控链表以推送数据到上位机）*/
    // hw_data_l update_list;

    void hw_heartbeat();
    void hw_param_monitor();
    int hw_used_cache(hw_cache_t *data);
    int hw_look_cache_status(hw_cache_t **pData, uint16_t number);
    int hw_look_cache_status(hw_device_kind_e vkind);

    uint16_t get_env_isEnable_number();
    uint16_t get_env_isEnable_number(RTU_INFO *info);
    float get_env_value_by_vtype(hw_device_type_e vtype);
    uint16_t get_env_isIns_by_vtype(hw_device_type_e vtype, RTU_INFO *info);
    int get_device_kind_by_type(hw_device_type_e vtype);

    void show_hw_table();
private:
    int hw_update_cache(hw_cache_t *data, uint16_t ch);

    SHM_DATA_DEF *shm_data;
    RTU_INFO *dev_info;
    SHM_DATA_RO *real_data;     // fix: maybe cant use point
    // SHM_DATA_RO *last_data;

};

extern hardware *hw;
