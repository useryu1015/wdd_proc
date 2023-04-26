#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

/* 限通用驱动程序，避免交叉引用业务程序*/
#include "zlog.h"
#include "sys_config.h"
#include "sys_shm.h"
#include "stringtable.h"
#include "cJSON.h"
#include "data_convert.h"

#include "srv_shm.h"
#include "libwebsockets.h"


using namespace std;
/* TBD name*/

#define EMPTY_STRING ("")

int ratelimit_connects(unsigned int *last, unsigned int secs);




