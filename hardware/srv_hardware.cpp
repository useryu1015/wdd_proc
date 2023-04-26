#include "dataDef.h"
#include "srv_hardware.h"
#include "common.h"


hardware *hw;

int hw_shm_init()
{

}

int hw_param_monitor(RTU_INFO *p_info, SHM_DATA_RO *p_data){
    
}

/**
 * planA: shm中updateTime更新， 便更新数据
*/
int _hw_param_monitor(hw_device_info_t *p_dev, RTU_INFO *p_info, SHM_DATA_RO *p_data)
{
    int i;

    if (!p_info || !p_data || !p_dev)
        return -1;


    for (i = 0; i < p_info->aiChNumI; i++)
    {
        // if(p_data->updateTime == 0)
            // continue;

        p_data->aiValI[i];
    }


    // for (i = 0; i < p_dev->nDevice; i++)
    // {
        
    // }

}


/**
 * planB: 监听每个参数是否有修改。。
*/
int __hw_param_monitor(SHM_DATA_RO *p_data_last, SHM_DATA_RO *p_data, RTU_INFO *p_info, hw_data_l *)
{
    int i;
    SHM_DATA_RO *last = p_data_last;

    if (!p_info || !p_data || !last)
        return -1;

    for (i = 0; i < p_info->aiChNumI; i++)
    {
        if (last->aiValI[i] == p_data->aiValI[i])
            continue;

    }


}

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






void hardware::show_hw_table()
{
    int i;
    hw_cache_t *pcache;
    int x, y = 1, row, line = hw->nAiChI + hw->nAiChV + hw->nAiChEnv;
    char *hw_table = s_table_create_0("hardware data", 6, 2+line-10, 13);           // 列，行，宽（长，宽，间距）

    s_table_valuesetxs(hw_table, 0, 1, "kks ref vtype val t", ' ');
    s_table_valuesetys(hw_table, 0, 1, "1 2 3 4 5 6", ' ');

    for (i = 0, x = 0; i < hw->nAiChI; i++) {
        char str[1024] = {0};
        pcache = hw->pAiChI[i];

        sprintf(str,"%d %s %s %d %s %s", y, "TBD", "TBD", pcache->dev_type, 
                                        hw_var2string(pcache->val), "2023-04-25");
        s_table_valuesetxs(hw_table, y++, x, str, ' ');
    }

    

    /*打印表格*/
    const char *tb_str = s_table_string_get(hw_table);
    zlog_info("\n%s", tb_str);

    free(hw_table);
}

void table_init(){
    // new table
}

void table_delete(){
    // free(hw_table);    
}

void _show_hw_table()
{

}


/*********    drv 初始化组件      ******/
void *objects_new_2arr(unsigned int nitems, unsigned int obj_size)
{
    unsigned int i;
    void **pptr;
    char *ptr;

    pptr = (void**)calloc(nitems + 1, sizeof(long));
    ptr = (char *)calloc(nitems, obj_size);

    for (i = 0; i < nitems; ++i)
    {
        pptr[i] = &ptr[i * obj_size];
    }

    return pptr;
}

int get_device_kind_by_type(hw_device_type_e vtype)
{
    switch (vtype)
    {
    case HW_TYPE_CUR: return HW_KIND_CUR;
    case HW_TYPE_VOL:   return HW_KIND_VOL;
    case HW_TYPE_TEMP:
    case HW_TYPE_HUMI:
    case HW_TYPE_NIOSE:
    case HW_TYPE_PM2_5:
    case HW_TYPE_PM10: 
    case HW_TYPE_O2:
    case HW_TYPE_TVOC:  return HW_KIND_ENV;
    default:
        zlog_error("No such datatype is defined!"); 
        break;
    }

    return -1;
}

float hardware::get_env_value_by_vtype(hw_device_type_e vtype)
{
    switch (vtype)
    {
    case HW_TYPE_TEMP:  return real_data->envMonitor.temperature;
    case HW_TYPE_HUMI:  return real_data->envMonitor.humidity;
    case HW_TYPE_NIOSE: return real_data->envMonitor.noise;
    case HW_TYPE_PM2_5: return real_data->envMonitor.pm2_5;
    case HW_TYPE_PM10:  return real_data->envMonitor.pm10;
    case HW_TYPE_O2:    return real_data->envMonitor.O2;
    case HW_TYPE_TVOC:  return real_data->envMonitor.tvoc;
    default:
        zlog_error("No such datatype is defined!"); 
        break;
    }

    return -1;
}

uint16_t hardware::get_env_isIns_by_vtype(hw_device_type_e vtype, RTU_INFO *info)
{
    if (!info)
        return -1;

    switch (vtype)
    {
    case HW_TYPE_TEMP:  return info->envMonitorSet.isInsTemperature;
    case HW_TYPE_HUMI:  return info->envMonitorSet.isInsHumidity;
    case HW_TYPE_NIOSE: return info->envMonitorSet.isInsNoise;
    case HW_TYPE_PM2_5: return info->envMonitorSet.isInsPm2_5;
    case HW_TYPE_PM10:  return info->envMonitorSet.isInsPm10;
    case HW_TYPE_O2:    return info->envMonitorSet.isInsO2;
    case HW_TYPE_TVOC:  return info->envMonitorSet.isInsTvoc;
    default:
        zlog_error("No such datatype is defined!");             // fix: 是否便于后续 环境参数拓展?
        break;
    }

    return -1;
}


/**
 * 获取环境传感器 安装数量
 *  TBD：刘淼提供 或遍历状态
*/
uint16_t hardware::get_env_isEnable_number()
{
    return JSON_HW_ENV_INS_NUM;
}

uint16_t hardware::get_env_isEnable_number(RTU_INFO *info)
{
    uint16_t num = 0;

    if (!info)
        return -1;

    return num;
}


inline int hardware::hw_update_cache(hw_cache_t *data, uint16_t ch)
{
    if (!data)
        return -1;

    data->val = real_data->aiValI[ch];
    data->t = real_data->updateTime;

    data->status = HW_DATA_UPTARE;

    return 0;
}

int hardware::hw_used_cache(hw_cache_t *data)
{
    data->status = HW_DATA_USED;

    return 0;
}

/* 监测各组数据 有无更新*/
int hardware::hw_look_cache_status(hw_cache_t **pData, uint16_t number)
{
    int i;

    if (!pData)
        return -1;
    
    for (i = 0; i < number; i++)
    {
        hw_cache_t *pd;

        if (!(pd = pData[i]))
        {
            zlog_error("void address");
            return -1;      // continue;
        }

        if (pd->status == HW_DATA_UPTARE)
            return HW_DATA_UPTARE;
    }

    return HW_DATA_KEEP;
}

int hardware::hw_look_cache_status(hw_device_kind_e vkind)
{
    
    switch (vkind)
    {
    case HW_KIND_CUR:   return hw_look_cache_status(pAiChI, nAiChI);
    case HW_KIND_VOL:   return hw_look_cache_status(pAiChV, nAiChV);
    case HW_KIND_ENV:   return hw_look_cache_status(pChEnv, nAiChEnv);
    default:
        zlog_error("No such datatype is defined!"); 
        break;
    }

    return -1;
}


/**
 * xxx
 *  如果shm数据更新，则寄存到class中。 （直接监测数据，与频率无关）
*/
void hardware::hw_param_monitor()
{
    int i;

    // zlog_info("aiChNumI: %d", dev_info->aiChNumI);
    for (i = 0; i < dev_info->aiChNumI; i++)
    {
        hw_cache_t *pd = pAiChI[i];
        // zlog_info("Value [cache:%f realData:%f]", pd->val, real_data->aiValI[i]);

        /* if 未更新 && 超过定时更新周期*/
        // pd->status = HW_DATA_KEEP;

        // if (last_data->aiValI[i] == real_data->aiValI[i])
        if (pd->val == real_data->aiValI[i] && 1)
            continue;

        /* 更新数据 */
        pthread_mutex_lock(&pd->lock_ring); /* --------- ring lock { */
        pd->val = real_data->aiValI[i];     // fix: 定位方式
        pd->t = real_data->updateTime;      // fix: 时钟源

        pd->status = HW_DATA_UPTARE;
        pthread_mutex_unlock(&pd->lock_ring); /* } ring lock */
        // zlog_info("Value: %f", pd->val);
    }

    // zlog_info("aiChNumV: %d", dev_info->aiChNumV);
    for (i = 0; i < dev_info->aiChNumV; i++)
    {
        hw_cache_t *pd = pAiChV[i];
        
        if (pd->val == real_data->aiValV[i])
            continue;

        pthread_mutex_lock(&pd->lock_ring);
        hw_update_cache(pd, i);

        pthread_mutex_unlock(&pd->lock_ring);
        // zlog_info("Value: %f", pd->val);
    }

    /* 更新环境参数*/
    for (i = 0; i < nAiChEnv && 0; i++)
    {
        hw_cache_t *pd = pChEnv[i];
        float realdata = get_env_value_by_vtype(pd->dev_type);

        if (pd->val == realdata)
            continue;

        pthread_mutex_lock(&pd->lock_ring);
        pd->val = realdata;
        pd->t = real_data->updateTime;      // fix: 时钟源

        pd->status = HW_DATA_UPTARE;
        pthread_mutex_unlock(&pd->lock_ring);
        // zlog_info("Value: %f", pd->val);
    }

    // _show_hw_table();

}


void hardware::hw_heartbeat()
{
    hw_param_monitor();

}


hardware::hardware()
{
    
}


hardware::hardware(SHM_DATA_DEF *shmD)
{
    int i, n;

    if (!shmD || !&shmD->info || !&shmD->data) {
        zlog_error("hardware init error");      // fix
        delete hw;
        return;
    }
    
    /* mount shm To hardware class*/
    shm_data = shmD;
    dev_info = &shmD->info;
    real_data = &shmD->data;

    zlog_info("HW: nChI:%d nChV:%d nChEnv:%d ", shmD->info.aiChNumI, shmD->info.aiChNumV, shmD->info.diChNum);

    /**
     * 初始化硬件参数：内存分配、默认属性 
     */
    /*电流*/
    nAiChI = (dev_info->aiChNumI < AI_NUM_MAX) ? dev_info->aiChNumI : AI_NUM_MAX;
    if (nAiChI) {
        pAiChI = (hw_cache_t **)objects_new_2arr(nAiChI, sizeof(hw_cache_t));
    } else {
        zlog_debug("Channel number too less %d", nAiChI);
        // fix: avoid segment fault
        // pAiChI = (hw_cache_t **)objects_new_2arr(1, sizeof(hw_cache_t));
        // nAiChI = 1;
    }

    for (i = 0; i < nAiChI; i++) {
        pAiChI[i]->dev_type = HW_TYPE_CUR;

    }

    /*电压*/
    nAiChV = dev_info->aiChNumV;
    if (nAiChV) {
        pAiChV = (hw_cache_t **)objects_new_2arr(nAiChV, sizeof(hw_cache_t));
    } else {
        zlog_debug("Channel number too less %d", nAiChV);
    }

    for (i = 0; i < nAiChV; i++) {
        pAiChV[i]->dev_type = HW_TYPE_VOL;

    }

    /*环境*/
    nAiChEnv = get_env_isEnable_number();
    if (nAiChEnv) {
        pChEnv = (hw_cache_t **)objects_new_2arr(nAiChEnv, sizeof(hw_cache_t));
    } else {
        zlog_debug("Channel number too less %d", dev_info->aiChNumI);
    }

    for (i = 0, n = 0; i < JSON_HW_ENV_SNUM; i++) {
        hw_device_type_e vtype = (hw_device_type_e)(HW_TYPE_TEMP + i);     // fix
        
        if (!get_env_isIns_by_vtype(vtype, dev_info))
            continue;

        pChEnv[n]->dev_type = vtype;
        pChEnv[n]->status = HW_DATA_INIT;
        
        n++;
    }

    // zlog_info("HW: nChI:%d nChV:%d nChEnv:%d ", nAiChI, nAiChV, nAiChEnv);
    zlog_info("hardware init done");

}

hardware::~hardware()
{
    free(pAiChI);
    free(pAiChV);
    free(pChEnv);
    
}


