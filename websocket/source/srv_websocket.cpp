#include "srv_websocket.h"
#include "ws_dataflow.h"
#include "srv_hardware.h"


int ws_data_pack_and_send(hw_info_t* hwd, hw_cache_t **pData, uint16_t number, ws_sub_protocol_t *wsd)
{
    ws_sub_protocol_t *vhd = (ws_sub_protocol_t *)wsd;
    char *str = NULL;

    if (!vhd || !vhd->p_user)
        return -1;

    zlog_info("format websocket data");
    ws_format_hardware_data(hwd, pData, number, &str);
    zlog_debug("Data json: \n%s", str);


zlog_info("size pusr: data:%d", strlen(str));
    /* 通过ws接口，发送数据*/
    int len = sprintf((char *)vhd->p_user, "%s", str);          // fix: memove
    // zlog_info("Send[%d]: %s", len, vhd->p_user);
    lws_callback_on_writable(vhd->wsi_multi);
    // vhd->user_state = USER_TX;

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

