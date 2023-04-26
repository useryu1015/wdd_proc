#include "data_convert.h"
#include "sys_config.h"
#include "stdio.h"

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


