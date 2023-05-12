#include "dataDef.h"
#include "srv_hardware.h"
#include "ws_dataflow.h"
#include "zlog.h"

hardware *hw;

const hw_vkind_t hw_vkind_values[] = {
    {HW_KIND_COL, "COL", "标段二"},
    {HW_KIND_CUR, "CUR", "电流"},
    {HW_KIND_VOL, "VOL", "电压"},
    {HW_KIND_ENV, "ENV", "环境"},
    {HW_KIND_ASE, "ASE", "有源传感器"},
    {HW_KIND_PSE, "PSE", "无源传感器"},
    {HW_KIND_TBD, "TBD", "未定义"}
};

hw_dev_kind_e get_device_kind_by_name(char *name)
{
    int i = 0, cnt = sizeof(hw_vkind_values) / sizeof(hw_vkind_values[0]);

    for (; i < cnt; ++i)
    {
        if (0 == strcasecmp(name, hw_vkind_values[i].name))
        {
            return (hw_dev_kind_e)hw_vkind_values[i].id;
        }
    }

    return HW_KIND_TBD;
}

/* fix: 解析shm硬件 类型+型号: CLO:DA_IO_02 */
// inline void hw_kind_init(char *name, hw_dev_kind_e *kind, char *model)
void hardware::_hw_kind_init()
{
    // char kind[32];
    // fscanf(dev_info->ver.type, "%s:%s", kind, hwInfo.model);

    hwInfo._kind[3] = '\0';
    strncpy(hwInfo._kind, dev_info->ver.type, 3);
    strncpy(hwInfo.model, dev_info->ver.type+4, 32);
    hwInfo.kind = get_device_kind_by_name(hwInfo._kind);

    zlog_info("kind[%d]:%s model:%s", hwInfo.kind, hwInfo._kind, hwInfo.model);

}

/* 获取硬件通道数量*/
int hardware::hw_get_channel_sum()
{
    switch (hwInfo.kind)
    {
    case HW_KIND_CUR:   return dev_info->currentChNum;
    case HW_KIND_VOL:   return dev_info->voltageChNum;
    case HW_KIND_ENV:   return get_env_isEnable_number();       // fix
    case HW_KIND_ASE:
    case HW_KIND_PSE:   return dev_info->aiChNumI;              // 有源和无源 共用xx
    default:
        zlog_error("Channel number too less ");
        break;
    }

    return 1;
}

int hardware::get_param_ref(char *ref, hw_cache_t *param)
{
    int rtn;

    if (!ref || !param)
        return -1;

    /* A: */
    rtn = sprintf(ref, "%s", param->ref);
    return rtn;

    /* B: */
    if (hwInfo.kind == HW_KIND_ENV)
        rtn = sprintf(ref, "%s/%s.%s", hwInfo.name, param->zone, get_hw_name_by_type(param->val_type));
    else
        rtn = sprintf(ref, "%s/%s.%s%d", hwInfo.name, param->zone, get_hw_name_by_type(param->val_type), param->_GPIO.io);

    if (!rtn) {
        zlog_error("error");
        return -1;
    }

    return 0;
}

float hardware::get_val(hw_cache_t *param)
{
    int rtn = 0;
    float buf;
    // char vals[128];

    if (!param)
        return -1;

    if (hwInfo.kind == HW_KIND_ASE || hwInfo.kind == HW_KIND_PSE) {
        buf = data_AD_convert(param->val, 
                param->rangeI[0], param->rangeI[1], param->rangeD[0], param->rangeD[1]);
    } else
        buf = param->val;

    return buf;
}

int hardware::get_val_strUnit(char *buf, hw_cache_t *param, float val)
{
    int rtn;
    // char vals[128];

    if (!buf || !param)
        return -1;

    // "4.8-A"
    rtn = sprintf(buf, "%.1f-%s", val, get_hw_unit_by_type(param->val_type));

    return rtn;
}

int hardware::get_val_strUnit(char *buf, hw_cache_t *param)
{
    int rtn;
    // char vals[128];

    if (!buf || !param)
        return -1;

    // "4.8-A"
    rtn = sprintf(buf, "%.1f-%s", param->val, get_hw_unit_by_type(param->val_type));
    // zlog_info("unit: %s", vals);

    return rtn;
}


void hardware::show_hw_table()
{
    int i;
    hw_cache_t *pcache;
    int x, y = 1, row, line = hw->nChNum;
    const int maxWidth = 25;
    char *hw_table = s_table_create_0("hardware data", 6, 2+line, maxWidth);    // 列，行，宽（长，宽，间距）

    s_table_valuesetxs(hw_table, 0, 1, "kks ref vtype val t", ' ');
    s_table_valuesetys(hw_table, 0, 1, "1 2 3 4 5 6", ' ');

    for (i = 0, x = 0; i < hw->nChNum; i++) {
        char str[maxWidth] = {0};
        char ref[maxWidth] = {0};
        char vtype[maxWidth] = {0};
        pcache = hw->pCache[i];

        sprintf(vtype, "%s", get_hw_name_by_type(hw->pCache[i]->val_type));
        sprintf(ref, "xx/%s.%s%d", vtype,hw->hwInfo._kind, hw->pCache[i]->_GPIO.io);
        // get_param_ref(ref, pcache);
        sprintf(str, "%d,%s,%s,%s,%s,%s", y, pcache->kks, ref, vtype, 
                                        hw_var2string(pcache->val), pcache->param_time);
        s_table_valuesetxs(hw_table, y++, x, str, ',');
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

    if (!nitems) {
        zlog_error("nitems: %d", nitems);
        nitems = 0;
    }

    pptr = (void**)calloc(nitems + 1, sizeof(long));
    ptr = (char *)calloc(nitems, obj_size);

    for (i = 0; i < nitems; ++i)
    {
        pptr[i] = &ptr[i * obj_size];
    }

    return pptr;
}

int get_device_kind_by_type(hw_dev_type_e vtype)
{
    // switch (vtype)
    // {
    // case HW_TYPE_CUR:   return HW_KIND_CUR;
    // case HW_TYPE_VOL:   return HW_KIND_VOL;
    // case HW_TYPE_TEMP:
    // case HW_TYPE_HUMI:
    // case HW_TYPE_NIOSE:
    // case HW_TYPE_PM2_5:
    // case HW_TYPE_PM10: 
    // case HW_TYPE_O2:
    // case HW_TYPE_TVOC:  return HW_KIND_ENV;
    // default:
    //     zlog_error("No such datatype is defined!"); 
    //     break;
    // }

    return -1;
}

float hardware::get_env_value_by_vtype(hw_dev_type_e vtype)
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

uint16_t hardware::get_env_isIns_by_vtype(hw_dev_type_e vtype, RTU_INFO *info)
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
    // data->t = real_data->updateTime;

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


/**
 *  gpio: 参数点对应的物理端口
*/
int hardware::get_gpio_val(float *val, hw_dev_kind_e kind, hw_dev_type_e type, int gpio)
{
    float rdata;
    int hwGpio = gpio - 1;      // rang:  0 ~ nChNum

    switch (kind)
    {
    case HW_KIND_CUR:
        rdata = real_data->currentA[hwGpio];      // fix: gpio定位方式
        break;
    case HW_KIND_VOL:
        rdata = real_data->voltageV[hwGpio];      // fix: gpio定位方式
        break;
    case HW_KIND_ASE:
    case HW_KIND_PSE:    
        rdata = real_data->aiValI[hwGpio];      // fix: gpio定位方式
        break;
    case HW_KIND_ENV:
        rdata = get_env_value_by_vtype(type);
        break;
    default:
        zlog_error("param error");
        return -1;
    }

    *val = rdata;
    return 0;
}


/**
 * xxx
 *  如果shm数据更新，则寄存到class中。 （直接监测数据，与频率无关）
*/
void hardware::hw_param_monitor()
{
    int i;
    float rdata;

    get_sys_time(hwInfo.time);
    
    for (i = 0; i < nChNum; i++)
    {
        hw_cache_t *pd = pCache[i];

        /* if 未更新 && 超过定时更新周期*/
        // pd->status = HW_DATA_KEEP;
        
        // zlog_info("pd->_GPIO.io:%d  ", pd->_GPIO.io);
        get_gpio_val(&rdata, hwInfo.kind, pd->val_type, pd->_GPIO.io);
        // zlog_info("type %d o%f r%f cache%f", hwInfo.kind, real_data->aiValI[i], rdata, pd->val);

        if (pd->val == rdata)
            continue;

        /* 更新数据 */
        // pthread_mutex_lock(&pd->lock_ring); /* --------- ring lock { */          // fix： bus error
        pd->val = rdata;
        get_sys_time(pd->param_time);

        pd->status = HW_DATA_UPTARE;
        // pthread_mutex_unlock(&pd->lock_ring); /* } ring lock */
        // zlog_info("Value: %f", pd->val);
    }
}

int hardware::hw_info_init(SHM_DATA_DEF *shmD)
{
    int i, n;

    if (!shmD || !&shmD->info || !&shmD->data) {
        zlog_error("hardware init error");
        // delete hw;
        return -1;
    }

    /* mount shm To hardware class*/
    shm_data = shmD;
    dev_info = &shmD->info;
    real_data = &shmD->data;

    /* 初始化硬件参数：内存分配、默认属性 */
    _hw_kind_init();

    nChNum = hw_get_channel_sum();
    pCache = (hw_cache_t **)objects_new_2arr(nChNum, sizeof(hw_cache_t));
    // nChNum = (dev_info->aiChNumI < AI_NUM_MAX) ? dev_info->aiChNumI : AI_NUM_MAX;
    zlog_info("GPIO sum: %d", nChNum);

    /* 根据硬件属性， 初始化各参数类型 */
    for (i = 0; i < nChNum; i++) {
        hw_cache_t *pd = pCache[i];

        if (hwInfo.kind == HW_KIND_CUR) {
            pd->val_type = HW_TYPE_CUR;
        } else if (hwInfo.kind == HW_KIND_VOL) {
            pd->val_type = HW_TYPE_VOL;
        } else if (hwInfo.kind == HW_KIND_ENV) {
            pd->val_type = (hw_dev_type_e)((int)HW_TYPE_TEMP + i);      // fix: 定位方式
        // } else if (hwInfo.kind == HW_KIND_ASE) {
        //     pd->val_type = HW_KIND_ASE;
        // } else if (hwInfo.kind == HW_KIND_PSE) {
        //     pd->val_type = HW_KIND_PSE;
        } else {
            // zlog_info("please config json field: vtype");
            // return -1;
        }


        /* other */
        // pthread_mutex_init(&pd->lock_ring, NULL);
    }

    zlog_info("hardware init done");
    return 0;
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

    if (hw_info_init(shmD) != 0)
    {
        zlog_info("hardware init failed");
        exit(1);
    }

}

hardware::~hardware()
{
    free(pCache);
    
}


