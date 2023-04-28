/***************************************************************************
电流参数
{
    "name": "发变811层",
    "id": "202303110004xl",
    "terminal": "CUR",
    "mtype": "MSGE",
    "t": "2023-04-05 09:28:47.475",
    "params": [
        {
            "kks": "kks编码",
            "ref": "发变811层/CUR.AC",
            "vtype": "电流",
            "val": "4.8-A",
            "t": "2023-04-05 09:28:47.475"
        }
    ]
}
*/
#include "ws_dataflow.h"
#include "srv_hardware.h"
#include "cJSON.h"
#include "common.h"

const hw_vtype_t hw_vtype_values[] = {
    {HW_TYPE_CUR,   "电流", "A", "安培"},
    {HW_TYPE_VOL,   "电压", "V"},
    {HW_TYPE_TEMP,  "温度", "℃"},
    {HW_TYPE_HUMI,  "湿度", "%%RH"},
    {HW_TYPE_NIOSE, "噪声", "MPa", "分贝，不需要换算"},
    {HW_TYPE_PM2_5, "PM2.5", "μg/m³", "微克/立方米，一般可省略单位"},
    {HW_TYPE_PM10,  "PM10", "μg/m³", "微克/立方米，一般可省略单位"},
    {HW_TYPE_O2,    "氧气", "%%", "体积比，不需要换算"},
    {HW_TYPE_TVOC,  "TVOC", "mg/m³"},
    {HW_TYPE_FLOW,  "流量", "m³/m"},
    {HW_TYPE_SFLOW, "累积流量", "m³"},
    {HW_TYPE_RSPD,  "转速", "r/min"},
    {HW_TYPE_FREQ,  "频率", "Hz"},    
    {HW_TYPE_POWER, "功率", "W"},
    {HW_TYPE_ELEC,  "电能", "KW.h"},
    // 水压:MPa  气压:Kpa
};

hw_dev_type_e get_hw_type_by_name(const char *name)
{
    int i = 0, cnt = sizeof(hw_vtype_values) / sizeof(hw_vtype_values[0]);

    for (; i < cnt; ++i)
    {
        // zlog_info("%s -- %s", name, hw_vtype_values[i].name);
        if (0 == strcasecmp(name, hw_vtype_values[i].name))
        {
            return (hw_dev_type_e)hw_vtype_values[i].id;
        }
    }

    return HW_TYPE_TBD;
}

const char *get_hw_name_by_type(hw_dev_type_e vtype)
{
    int i = 0, cnt = sizeof(hw_vtype_values) / sizeof(hw_vtype_values[0]);

    for (; i < cnt; ++i)
    {
        if (vtype == hw_vtype_values[i].id)
        {
            return hw_vtype_values[i].name;
        }
    }

    return EMPTY_STRING;
}

const char *get_hw_unit_by_type(hw_dev_type_e vtype)
{
    int i = 0, cnt = sizeof(hw_vtype_values) / sizeof(hw_vtype_values[0]);

    for (; i < cnt; ++i)
    {
        if (vtype == hw_vtype_values[i].id)
        {
            return hw_vtype_values[i].unit;
        }
    }

    return EMPTY_STRING;
}


_Bool _ws_format_hardware_param(cJSON *jp, hw_cache_t *param)
{
    char ref[WS_STRLEN_MAX];
    char buf[128] = {0};

    if (!jp || !param)
        return false;

    hw->get_param_ref(ref, param);

    // sprintf(buf, "%skks编码-%d", GENERATE_NAME_USER, ++i);
    cJSON_AddStringToObject(jp, "kks", param->kks);
    cJSON_AddStringToObject(jp, "ref", ref); 
    cJSON_AddStringToObject(jp, "vtype", get_hw_name_by_type(param->val_type));
    hw->get_val_strUnit(buf, param);
    cJSON_AddStringToObject(jp, "val", buf);
    cJSON_AddStringToObject(jp, "t", hw->hwInfo.time);

    return true;
}

_Bool ws_format_hardware_data(hw_info_t* hwd, hw_cache_t **pData, uint16_t number, char **out)
{
    int i;
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();

    if (!pData)
    {
        return -1;
    }

    // zlog_info("format hw data");
    cJSON_AddStringToObject(root, "name", hwd->name);         // fix: TBD
    cJSON_AddStringToObject(root, "id", hwd->id);
    cJSON_AddStringToObject(root, "terminal", hwd->_kind);
    cJSON_AddStringToObject(root, "mtype", "MSGE");
    cJSON_AddStringToObject(root, "t", hwd->time);

    for (i = 0; i < number; i++)
    {
        hw_cache_t *pd;
        cJSON *jparam;

        if (!(pd = pData[i]))
        {
            zlog_error("void address");
            return -1;      // continue;
        }

        if (pd->status != HW_DATA_UPTARE)
            continue;

        if (!(jparam = cJSON_CreateObject()))
            return -1;
    
        // zlog_info("param:[ kks:%s ref:%sval:%f type:%d ]"," ", "", pd->val, pd->val_type);
        hw->hw_used_cache(pd);                      // set used
        _ws_format_hardware_param(jparam, pd);      // 构建json上报参数集
        
        cJSON_AddItemToArray(array, jparam);
    }
    cJSON_AddItemToObject(root, "param", array);

    *out = cJSON_Print(root);
    cJSON_Delete(root);

    return true;
}



