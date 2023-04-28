#pragma once


#define CONF_PATH "./1.RunEnv/config/dev/"
#define CONF_NAME "dev_main"
#define CONF_EXT  "json"

#define CONF_PARAM_FIELD "param"
#define CONF_WEBSK_FIELD "websocket"




/* 此项改为json配置，或其他接口*/
#define JSON_HW_ENV_INS_NUM 7
#define JSON_HW_ENV_SNUM 7


#define SHM_HW_KEY (key_t)(201)

#define WS_STRLEN_MAX (256)
#define WS_VAL_STRLEN_MAX (WS_STRLEN_MAX)
#define WS_VTYPE_STRLEN_MAX (32)
#define WS_UNIT_STRLEN_MAX (16)








int conf_main_init();


