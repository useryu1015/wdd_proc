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

const hw_vtype_t hw_vtype_values[] = {
    {HW_TYPE_CUR, "电流", "A"},
    {HW_TYPE_VOL, "电压", "V"},
    {HW_TYPE_TEMP, "温度", "℃"},
    {HW_TYPE_HUMI, "湿度", "%%RH"},
    {HW_TYPE_NIOSE, "噪声", "MPa"},
    {HW_TYPE_PM2_5, "PM2.5", "μg/m³"},
    {HW_TYPE_PM10, "PM10", "μg/m³"},
    {HW_TYPE_O2, "氧气", "%%"},
    {HW_TYPE_TVOC, "TVOC", "mg/m³"},
    // 水压:MPa  气压:Kpa
};


hw_device_type_e get_hw_type_by_name(const char *name)
{
    int i = 0, cnt = sizeof(hw_vtype_values) / sizeof(hw_vtype_values[0]);

    for (; i < cnt; ++i)
    {
        if (0 == strcasecmp(name, hw_vtype_values[i].name))
        {
            return (hw_device_type_e)hw_vtype_values[i].id;
        }
    }

    return HW_TYPE_TBD;
}

const char *get_hw_name_by_type(hw_device_type_e vtype)
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

const char *get_hw_unit_by_type(hw_device_type_e vtype)
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

    if (!jp || !param)
        return false;

    // sprintf(ref, )

    cJSON_AddStringToObject(jp, "kks", "kks编码");
    // cJSON_AddStringToObject(jp, "ref", ); 
    cJSON_AddStringToObject(jp, "vtype", get_hw_name_by_type(param->dev_type)); 
    cJSON_AddStringToObject(jp, "val", hw_var2string(param->val));
    // cJSON_AddStringToObject(jp, "t", ); 

    return true;
}

_Bool ws_format_hardware_data(hw_cache_t **pData, uint16_t number, char **out)
{
    int i;
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();

    if (!pData)
    {
        return -1;
    }

    // zlog_info("format hw data");
    cJSON_AddStringToObject(root, "name", "发变811层");         // fix: TBD
    cJSON_AddStringToObject(root, "id", "202303110004xl");
    cJSON_AddStringToObject(root, "terminal", "CUR");
    cJSON_AddStringToObject(root, "mtype", "MSGE");
    cJSON_AddStringToObject(root, "t", "2023-04-05 09:28:47.475");

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
    
        zlog_info("param:[ kks:%s ref:%sval:%f type:%d ]", pd->val, pd->dev_type);
        hw->hw_used_cache(pd);                      // set used
        _ws_format_hardware_param(jparam, pd);      // 构建json参数数据集
        
        cJSON_AddItemToArray(array, jparam);
    }
    cJSON_AddItemToObject(root, "param", array);

    *out = cJSON_Print(root);
    cJSON_Delete(root);

    return true;
}



