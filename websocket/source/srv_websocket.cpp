#include "srv_websocket.h"
#include "ws_dataflow.h"
#include "srv_hardware.h"


#define CMD_MES_FORMAT spritf



/**
 * 根据上位机数据流格式， 构建上报报文
 *  示例：sprintf((char *)buf, "{\"cmdCode\":1,\"cmdMerge\":65538,\"data\":[123,34,4,48,46,53,-34,44,34,11,125,93,125,125]}");
*/
int ws_format_uplink_data(char *buf, char *jdata, int cmd_merge)
{
    int i, len;
    char *ps = buf;
    char *val;

    if (!buf || !jdata)
        return -1;

    /* 1. */
    len = sprintf(ps, "{\"cmdCode\":%d,\"cmdMerge\":%d,\"data\":[", 1, cmd_merge);           // 效率较低， （和逐帧拷贝相比）
    ps += len;

    /* 2.format value */
    for (i = 0; *(jdata + i) != '\0' && i < strlen(jdata); i++)
    {
        len = sprintf(ps, "%d,", *(jdata + i));
        ps = ps + len;
    }
    // ps -= 1;            // delete ,

    /* 3. */
    sprintf(ps - 1, "]}");

    /* normal*/
    // sprintf(buf, "{\"cmdCode\":%d,\"cmdMerge\":%d,\"data\":[%s]", 1, cmd_merge, val);           // 效率较低， （和逐帧拷贝相比）

    return 0;
}

int ws_format_add_json()
{


}

int ws_data_pack_and_send(hw_info_t* hwd, hw_cache_t **pData, uint16_t number, ws_sub_protocol_t *wsd)
{
    ws_sub_protocol_t *vhd = (ws_sub_protocol_t *)wsd;
    char *str = NULL;
    static char dbuf[1024 * 100];
    int len;

    if (!vhd || !vhd->p_user)
        return -1;

    zlog_info("format websocket data");

    /* test */
    // char strX[] = "{\"json\":{\"name\":\"终端1\",\"id\":\"202303110001xl\",\"terminal\":\"COL\",\"mtype\":\"MSGE\",\"t\":\"2023-04-05 09:28:47.475\",\"params\":[{\"kks\":\"kks编码\",\"ref\":\"发变811层/机组1.水压1\",\"val\":\"0.56-Mpa\",\"t\":\"2023-04-05 09:28:47.872\"}]}}";
    // zlog_debug("Data json[%d]: \n%s", strlen(strX), strX);

    /* 构建上报json数据*/
    ws_format_hardware_data(hwd, pData, number, &str);
    zlog_debug("Data json[%d]: \n%s", strlen(str), str);

    ws_format_uplink_data(dbuf, str, CMD_MES_PARAM);
    zlog_trace("Data uplink: \n%s", dbuf);

    /* 通过ws接口，发送数据*/
    // 正常发送 测试报文
    // len = sprintf((char *)vhd->p_user, "{\"cmdCode\":1,\"cmdMerge\":65537,\"data\":[123,34,106,115,111,110,34,58,123,34,110,97,109,101,34,58,34,-25,-69,-120,-25,-85,-81,49,34,44,34,105,100,34,58,34,50,48,50,51,48,51,49,49,48,48,48,49,120,108,34,44,34,116,101,114,109,105,110,97,108,34,58,34,67,79,76,34,44,34,109,116,121,112,101,34,58,34,77,83,71,69,34,44,34,116,34,58,34,50,48,50,51,45,48,52,45,48,53,32,48,57,58,50,56,58,52,55,46,52,55,53,34,44,34,112,97,114,97,109,115,34,58,91,123,34,107,107,115,34,58,34,107,107,115,-25,-68,-106,-25,-96,-127,34,44,34,114,101,102,34,58,34,-27,-113,-111,-27,-113,-104,56,49,49,-27,-79,-126,47,-26,-100,-70,-25,-69,-124,49,46,-26,-80,-76,-27,-114,-117,49,34,44,34,118,97,108,34,58,34,48,46,53,54,45,77,112,97,34,44,34,116,34,58,34,50,48,50,51,45,48,52,45,48,53,32,48,57,58,50,56,58,52,55,46,56,55,50,34,125,93,125,125]}");
    len = sprintf((char *)vhd->p_user, "%s", dbuf);             // fix: memove
    if (vhd->wsi_multi)
        lws_callback_on_writable(vhd->wsi_multi);                   // 触发lws SEND回调

    if (str)
        free(str);

    return 0;
}

/**
 * 监控hardware状态 有数据更新,则推送到ws   (重构)
 *  暂不考虑加锁,队列 推送最新的数据
*/
void ws_param_monitor()
{
    int i;


    if (hw->hw_look_cache_status(hw->pCache, hw->nChNum) == HW_DATA_UPTARE)     // 1. 监听硬件状态
    {
        // 2. 构建jison
        // 3. 封装上位机 数据流
        // 4. websock send
        ws_data_pack_and_send(&hw->hwInfo, hw->pCache, hw->nChNum, &ws_prot[0]);        // fix: 现在只用了一组websocket，整理全局变量xx
    }
    
}

void ws_heartbeat()
{
    ws_param_monitor();
    
}

