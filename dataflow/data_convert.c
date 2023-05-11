#include "data_convert.h"
#include "sys_config.h"
#include "stdio.h"
#include "zlog.h"

/**
 * val转为字符串类型（拓展
 *  当前业务数据类型只有float
*/
char *hw_var2string(float val)
{
    static char buf[WS_VAL_STRLEN_MAX];

    sprintf(buf, "%f", val);

    return buf;
}






/**
 * 模数转换
 *  将模拟量4~20mA 根据物理量阈值 xx
 * */
float data_AD_convert(float val, float inMin, float inMax, float outMin, float outMax)
{
    float buf;

    buf = (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    zlog_info("in:%.2f out:%.2f  rangA:[%.2f, %.2f] rangD:[%.2f, %.2f]", 
                    val, buf, inMin, inMax, outMin, outMax);

    return buf;
}


int data_DA_convert()
{




    
}

