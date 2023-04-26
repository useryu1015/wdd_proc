#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/shm.h>

#include "zlog.h"
// #include "dataDef.h"

/*关闭共享内存*/
void shm_close(key_t shm_id)
{
    shmctl(shm_id, IPC_RMID, NULL);
}

/*打开共享内存*/
int shm_open(key_t key, uint32_t size, void **addr)
{
    int shmid;

    if (*addr)
    {
        zlog_debug("addr not NULL");
        free(addr);
    }
    zlog_info("key:%d size:%d", key, size);

    shmid = shmget(key, size, 0666 || IPC_CREAT);
    if (shmid < 0)
    {
        zlog_error("shmget failed!");
        return -1;
    }

    *addr = shmat(shmid, (void *)0, 0);

    // shmData = (SHM_DATA_DEF *)pa;
    // zlog_info("shm HW: nChI:%d nChV:%d nChEnv:%d ", shmData->info.aiChNumI, shmData->info.aiChNumV, shmData->info.diChNum);

    if (!*addr)
    {
        zlog_error("shmat failed!");
        return -1;
    }

    return shmid;
}
