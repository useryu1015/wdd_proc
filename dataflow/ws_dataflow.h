#pragma once
#include "sys_config.h"
#include "common.h"


typedef struct _hw_vtype_t {
    hw_dev_type_e id;
    char name[WS_VTYPE_STRLEN_MAX];
    char unit[WS_UNIT_STRLEN_MAX];
    char desc[WS_STRLEN_MAX];
    // void *hw_type_map();     // 数据类型 映射算法： 模数转换
} hw_vtype_t;

typedef struct _hw_vkind_t {
    hw_dev_kind_e id;
    char name[8];
    char desc[32];
} hw_vkind_t;

char *hw_var2string(float val);

hw_dev_type_e get_hw_type_by_name(const char *name);
const char *get_hw_name_by_type(hw_dev_type_e vtype);
const char *get_hw_unit_by_type(hw_dev_type_e vtype);

_Bool ws_format_hardware_data(hw_info_t* hwd, hw_cache_t **pData, uint16_t number, char **out);

