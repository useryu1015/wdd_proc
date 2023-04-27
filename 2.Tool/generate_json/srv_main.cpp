// #include "../../hardware/dataDef.h"
// #include "../../config/sys_config.h"
// #include "../../common/cJSON.h"
// #include "../../common/common.h"


// _Bool _ws_format_hardware_param(cJSON *jp, hw_cache_t *param)
// {
//     char ref[WS_STRLEN_MAX];

//     if (!jp || !param)
//         return false;

//     // sprintf(ref, )

//     cJSON_AddStringToObject(jp, "kks", "kks编码");
//     cJSON_AddStringToObject(jp, "ref", "TBD"); 
//     cJSON_AddStringToObject(jp, "vtype", "温度"); 
//     cJSON_AddStringToObject(jp, "port", "安装位置");
//     // cJSON_AddStringToObject(jp, "t", ); 

//     return true;
// }

// _Bool ws_format_hardware_data(char **out)
// {
//     int i;
//     cJSON *root = cJSON_CreateObject();
//     cJSON *array = cJSON_CreateArray();


//     // zlog_info("format hw data");
//     cJSON_AddStringToObject(root, "name", "发变811层");         // fix: TBD
//     cJSON_AddStringToObject(root, "id", "202303110004xl");
//     cJSON_AddStringToObject(root, "terminal", "CUR");
//     cJSON_AddStringToObject(root, "mtype", "MSGE");
//     cJSON_AddStringToObject(root, "t", "2023-04-05 09:28:47.475");

//     for (i = 0; i < shmData->info.aiChNumI; i++)
//     {
//         hw_cache_t *pd;
//         cJSON *jparam;

//         if (!(jparam = cJSON_CreateObject()))
//             return -1;
    
//         _ws_format_hardware_param(jparam, pd);      // 构建json参数数据集
        
//         cJSON_AddItemToArray(array, jparam);
//     }
//     cJSON_AddItemToObject(root, "param", array);

//     *out = cJSON_Print(root);
//     cJSON_Delete(root);

//     return true;
// }

// SHM_DATA_DEF *shmData;

// void mian()
// {

//     char *buf;

//     shm_hardware_init((void **)&shmData);
    
//     ws_format_hardware_data(&buf);


//     sleep(1);

// }