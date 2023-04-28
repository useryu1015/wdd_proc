#pragma once
#include "dataDef.h"
#include "common.h"

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
} hw_dev_type_e;

/* 设备种类： “terminal”字段*/
typedef enum _hw_device_kind_e
{
    HW_KIND_COL = 0,
    HW_KIND_CUR,
    HW_KIND_VOL,
    HW_KIND_ENV,
    HW_KIND_ASE,    // 有源
    HW_KIND_PSE,    // 无源
    HW_KIND_TBD     // 
} hw_dev_kind_e;

/* 数据状态定义： */
typedef enum _hw_data_status_e
{
    HW_DATA_UPTARE = 0,
    HW_DATA_KEEP,
    HW_DATA_USED,
    HW_DATA_DOWN,
    HW_DATA_INIT
} hw_data_status_e;

typedef struct _hw_dev_port
{
    uint16_t io;        // io口
    uint8_t name[16];   // io类型

}__attribute__((packed)) hw_dev_port;

/* 参数点信息： 参数属性、参数值 */
typedef struct _hw_cache_t
{
    hw_dev_port _GPIO;         // *接入物理通道端口（0-32）   *数据来源

    char kks[64];
    hw_dev_type_e val_type;     // 数据类型
    // hw_dev_kind_e dev_kind;  // 设备种类
    float val;
    struct timeval t;
    char room[64];          // 安装位置

    int rangeI[2];           // 量程：模拟量  [下限, 上限]
    int rangeD[2];           // 量程：数字量

    pthread_mutex_t lock_ring;
    hw_data_status_e status;             // 保持、更新、掉线、
}__attribute__((packed)) hw_cache_t;

typedef struct _hw_info_t
{
    /* shm更新*/
    hw_dev_kind_e kind;  // 设备种类
    char _kind[32];
    char model[32];      // 板卡型号
    // pVERSION *ver;

    /* 用户配置 main.json*/
    bool disable;
    char name[64];          // 安装区域
    char id[64];            // 硬件序列号
    char terminal[32];      // 终端类型，      == _kind
    char time[64];          // 更新时间

    hw_data_status_e status;             // 保持、更新、掉线、
}__attribute__((packed)) hw_info_t;

class hardware
{
public:
    hardware();
    hardware(SHM_DATA_DEF *shmD);
    ~hardware();                // delete call

    /* A: 数据结构寄存所有硬件参数*/
    // uint16_t nAiChI;            // intensity ch number
    // hw_cache_t **pAiChI;
    // uint16_t nAiChV;
    // hw_cache_t **pAiChV;
    // uint16_t nAiChEnv;
    // hw_cache_t **pChEnv;

    /* A: */
    hw_info_t hwInfo;
    uint16_t nChNum;            // 参数数量
    hw_cache_t **pCache;        // 参数信息

    /* B: 使用链表寄存有更新的数据（监控链表以推送数据到上位机）*/
    // hw_data_l update_list;

    void hw_heartbeat();
    void hw_param_monitor();
    int hw_used_cache(hw_cache_t *data);
    int hw_look_cache_status(hw_cache_t **pData, uint16_t number);
    int hw_look_cache_status(hw_dev_kind_e vkind);

    uint16_t get_env_isEnable_number();
    uint16_t get_env_isEnable_number(RTU_INFO *info);
    float get_env_value_by_vtype(hw_dev_type_e vtype);
    uint16_t get_env_isIns_by_vtype(hw_dev_type_e vtype, RTU_INFO *info);
    int get_device_kind_by_type(hw_dev_type_e vtype);
    
    int get_param_ref(char *ref, hw_cache_t *param);
    int get_val_strUnit(char *buf, hw_cache_t *param);
    
    void show_hw_table();
private:
    int hw_update_cache(hw_cache_t *data, uint16_t ch);
    int hw_info_init(SHM_DATA_DEF *shmD);
    void _hw_kind_init();
    int hw_get_channel_sum();
    int get_gpio_val(float *val, hw_dev_kind_e kind, hw_dev_type_e type, int gpio);

    SHM_DATA_DEF *shm_data;
    RTU_INFO *dev_info;
    SHM_DATA_RO *real_data;     // fix: maybe cant use point
    // SHM_DATA_RO *last_data;

};

extern hardware *hw;
