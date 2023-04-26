#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include "shmdata.h"

#include "../../hardware/dataDef.h"
#include "../../config/sys_config.h"

typedef struct _shm_handle_t
{
    char f_key[256];
    char f_lock[256];
    int shm_id;  // 共享内存的ID
    void *paddr; // 共享内存的首地址

    uint32_t size; // 共享内存的大小

    // pthread_rwlock_t *prwlock;	//共享内存的读写锁
} shm_handle_t;

/*打开共享内存*/
int shmem_open(shm_handle_t *phandle, uint32_t size, int id, _Bool create)
{
    key_t key = ftok(phandle->f_key, id);

    if (key < 0)
    {
        fprintf(stderr, "ftok key failed!\n");
        return -1;
    }

    phandle->shm_id = shmget(key, size, IPC_CREAT | 0666);
    if (phandle->shm_id < 0)
    {
        fprintf(stderr, "shmget failed!\n");
        return -1;
    }

    phandle->size = size;
    phandle->paddr = shmat(phandle->shm_id, NULL, 0);

    /*设置进程共享的读写锁*/
    // int fd = open(phandle->f_lock, O_CREAT | O_RDWR, 0777);
    // ftruncate(fd, sizeof(pthread_rwlock_t));

    // phandle->prwlock = mmap(NULL, sizeof(pthread_rwlock_t),
    //                         PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    // zlog_info("open f_lock:%s fd:%d", phandle->f_lock, fd);
    // if (create)
    // {
    //     pthread_rwlockattr_t rwlock_attr;
    //     pthread_rwlockattr_init(&rwlock_attr);

    //     pthread_rwlockattr_setpshared(&rwlock_attr, PTHREAD_PROCESS_SHARED);
    //     pthread_rwlock_init(phandle->prwlock, &rwlock_attr);
    // }

    return 0;
}

int main(int argc, char **argv)
{
    int i;
    float rands = 2;
    void *shm = NULL;
    SHM_DATA_DEF *pData;
    int shmid;

    // 创建共享内存
    printf("key:%d size:%d\n", SHM_HW_KEY, sizeof(SHM_DATA_DEF));

    shmid = shmget((key_t)SHM_HW_KEY, sizeof(SHM_DATA_DEF), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // 将共享内存连接到当前的进程地址空间
    shm = shmat(shmid, (void *)0, 0);
    if (shm == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    pData = (SHM_DATA_DEF *)shm;
    printf("Memory attched at:%p %d\n", shm, (long)shm);

    /*硬件状态*/
    pData->info.aiChNumI = 4;
    pData->info.aiChNumV = 8;
    pData->info.diChNum = 2;

    strcpy(pData->info.hardwareVer, "V1.0 test");

    pData->info.envMonitorSet.isInsHumidity = 1;
    pData->info.envMonitorSet.isInsTemperature = 1;
    pData->info.envMonitorSet.isInsNoise = 1;
    pData->info.envMonitorSet.isInsPm10 = 1;
    // exit(0);

    rands++;

    for (i = 0; i < pData->info.aiChNumI; i++)
    {
        pData->data.aiValI[i] = i * rands;
    }

    for (i = 0; i < pData->info.aiChNumV; i++)
    {
        pData->data.aiValV[i] = i * rands;
    }

    pData->data.envMonitor.temperature = rands;

    while (1)
    {
        usleep(500 * 1000);
        sleep(1);
        // printf("%f \n", pData->data.aiValV[2]);
        // printf("num: %x %x \n", *(char *)shm, *(((char *)(shm))+1));
        // printf("num: %x %x \n", *(char *)pData, *(((char *)(pData)) +1));
        // (*(char *)shm)++;
        // pData->data.aiValV[2] = pData->data.aiValV[2] + 2;

        rands++;

        for (i = 0; i < pData->info.aiChNumI; i++)
        {
            // if (i % 3)
            //     continue;

            pData->data.aiValI[i] = pData->data.aiValI[i] + 1;
            printf("AI: %f \n", pData->data.aiValI[i]);
        }

        for (i = 0; i < pData->info.aiChNumV; i++)
        {
            pData->data.aiValV[i] = i * rands;
            printf("VI: %f \n", pData->data.aiValV[i]);
        }

        pData->data.envMonitor.temperature = rands;

        printf("\n\n");
        printf("ChI:%d ChV:%d ChEnv:%d\n", pData->info.aiChNumI, pData->info.aiChNumV, 10);
        printf("version: %s \n", pData->info.hardwareVer);
    }

    // 设置共享内存
    struct shared_use_st *shared = NULL;
    shared = (struct shared_use_st *)shm;
    char buffer[BUFSIZ + 1]; // 用于保存输入的文本
    while (1)                // 向共享内存中写数据
    {
        // 数据还没有被读取，则等待数据被读取，不能向共享内存中写入文本
        while (shared->written == 1)
        {
            sleep(1);
            printf("Waiting...\n");
        }

        // 向共享内存中写入数据
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->text, buffer, TEXT_SZ);

        // 写完数据，设置written使共享内存段可读
        shared->written = 1;

        // 输入了end，退出循环（程序）
        if (strncmp(buffer, "end", 3) == 0)
        {
            break;
        }
    }

    // 把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    sleep(1);
    exit(EXIT_SUCCESS);
}