#include "srv_websocket.h"
#include "ws_dataflow.h"
#include "srv_hardware.h"


int ws_data_pack_and_send(hw_info_t* hwd, hw_cache_t **pData, uint16_t number)
{
    char *str = NULL;

    ws_format_hardware_data(hwd, pData, number, &str);
    zlog_debug("Data json: \n%s", str);




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
        ws_data_pack_and_send(&hw->hwInfo, hw->pCache, hw->nChNum);
    }
    
    // if (hw->hw_look_cache_status(hw->pAiChV, hw->nAiChV) == HW_DATA_UPTARE)
    // {
    //     // ws_format_hardware_data(hw->pAiChV, hw->nAiChV);
    // }


}

void ws_heartbeat()
{
    ws_param_monitor();
    
}

