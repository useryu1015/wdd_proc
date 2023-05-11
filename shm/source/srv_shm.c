#include "common.h"
#include "srv_shm.h"


int shm_hardware_init(void **shmData)
{
    if (*shmData)
    {
        zlog_info("shm not NULL");
    }

    // if (!shm_open(SHM_HW_KEY, sizeof(SHM_DATA_DEF), (void **)shmData))
    if (shm_open(SHM_HW_KEY, 1024 * 4, (void **)shmData) < 0)
    {
        zlog_error("shm init failed");
        return -1;
    }

    return 0;
}


int shm_init(shm_handle_t *shm_handle)
{
    int shmid;

    if (!shm_handle)
    {
        zlog_error("shm init NULL");
        return -1;
    }

    shm_handle->shm_id = shm_open(shm_handle->key, shm_handle->size,
                                    (void **)&shm_handle->paddr);
    if (shm_handle->shm_id < 0)
    {
        zlog_error("shm init failed");
        return -1;
    }

    return 0;
}

