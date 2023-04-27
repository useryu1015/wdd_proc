#include <stdbool.h>
#include "service.h"
#include "cJSON.h"
#include "dataDef.h"
#include "sys_config.h"
#include "srv_hardware.h"

#include "sys_file.h"










/**
 * 生成参数点配置文件
*/
#define GENERATE_NAME_USER "_TBD_"       // 用户自定义 字段
#define GENERATE_NAME_HARD "_HDW_"       // 根据硬件配置字段

/**
 * 脚本开发： 将用户配置的 usr_main.jcon 拷贝为 dev_main.json   并备份历史配置文件xxxx
 *  历史配置： 数据库格式，配置文件的： key、name、function、device、 （usage：根据用户选择配置文件，启用终端不同功能）
*/ 
#define TREMINAL_CONF_FILE "1.RunEnv/config/dev/usr_main.json"

_Bool _usr_genrate_param(cJSON *jp, hw_cache_t *param)
{
    char buf[WS_STRLEN_MAX];
    static int i = 1;

    if (!jp || !param)
        return false;

    sprintf(buf, "%skks编码-%d", GENERATE_NAME_USER, i++);
    cJSON_AddStringToObject(jp, "kks", buf);
    sprintf(buf, "%sref-%d", GENERATE_NAME_USER, i);
    cJSON_AddStringToObject(jp, "ref", buf);
    cJSON_AddStringToObject(jp, "vtype", "_HDW_温度");
    cJSON_AddStringToObject(jp, "port", "_HDW_接入端口");
    // cJSON_AddStringToObject(jp, "conf_sta", "INIT");    // 参数点配置状态： 初始、更新、追加、删除、 在线、掉线

    return true;
}

_Bool usr_genrate_proc_json(char **out)
{
    int i;
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    
    // zlog_info("生成程序基础配置文件");

    cJSON_AddStringToObject(root, "name", "_TBD_安装区域");
    cJSON_AddStringToObject(root, "id", "202303110004xl");      // 只读 且唯一
    cJSON_AddStringToObject(root, "terminal", "_HDW_CUR");      // 终端类型：hw type
    cJSON_AddFalseToObject (root, "disable");
    cJSON_AddStringToObject(root, "time", "2023-04-05 09:28:47.475");

    for (i = 0; i < hw->nAiChI; i++)
    {
        hw_cache_t *pd = hw->pAiChI[i];
        cJSON *jparam;

        if (!(jparam = cJSON_CreateObject()))
            return -1;
    
        _usr_genrate_param(jparam, pd);      // 构建json参数数据集
        
        cJSON_AddItemToArray(array, jparam);
    }
    cJSON_AddItemToObject(root, "param", array);

    *out = cJSON_Print(root);
    cJSON_Delete(root);

    return true;
}

/* 生成主程序配置文件，并写入到xx.json文件*/
void generate_json_file()
{
    int rtn;
    char *buf;

    zlog_info("add terminal conf file");

    usr_genrate_proc_json(&buf);
    zlog_trace("\n%s", buf);
    
    write_file(TREMINAL_CONF_FILE, (uint8_t *)buf, strlen(buf));

}






int option_main(int argc, char **argv)
{
    int result;
 
    opterr = 0;  //使getopt不行stderr输出错误信息
 
    while( (result = getopt(argc, argv, ":g::a::b:c:D:")) != -1 )
    {
           switch(result)
          {
            case 'g':
                generate_json_file();
                exit(0);
                break;
            case 'a':
                zlog_info("option=a, optopt=%c, optarg=%s", optopt, optarg);
                break;
            case 'b':
                zlog_info("option=b, optopt=%c, optarg=%s", optopt, optarg);
                break;
            case 'c':
                zlog_info("option=c, optopt=%c, optarg=%s", optopt, optarg);
                break;
            case '?':
                zlog_info("result=?, optopt=%c, optarg=%s", optopt, optarg);
                break;
            default:
                zlog_info("default, result=%c",result);
                break;
           }
        zlog_info("argv[%d]=%s", optind, argv[optind]);
    }
 
    for(result = 1; result < argc; result++)
          zlog_info("at the end-----argv[%d]=%s", result, argv[result]);

    return 0;
}




