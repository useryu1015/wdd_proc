#ifndef _SRV_SHM_H_
#define _SRV_SHM_H_

#include <stdbool.h>
#include <stdint.h>
#include <sys/queue.h>
#include <sys/shm.h>

#include "srv_hardware.h"

typedef struct _shm_handle_t {
	key_t key;
	int shm_id;			//共享内存的ID
	void *paddr;		//共享内存的首地址

	uint32_t size;		//共享内存的大小

	// pthread_rwlock_t *prwlock;	//共享内存的读写锁
} shm_handle_t;

int shm_hardware_init(void **shmData);
int shm_init(shm_handle_t *shm_handle);




/**
 * SINK
*/
#define SHM_FILE_PATH "/xlian/secret/ipc/"
#define SHM_SNAPSHOT "snapshot"
#define SHM_KEY_TAIL "_key"
#define SHM_LOCK_TAIL "_lock"
#define SHM_SNAPSHOT_ID (1)

// typedef struct _shm_handle_t {
// 	char f_key[256];
// 	char f_lock[256];
// 	int shm_id;			//共享内存的ID
// 	void *paddr;		//共享内存的首地址

// 	uint32_t size;		//共享内存的大小

// 	// pthread_rwlock_t *prwlock;	//共享内存的读写锁
// } shm_handle_t;

typedef struct _shm_srv
{
    shm_handle_t shm_real;
} shm_srv_t;

extern shm_srv_t *g_sink_shm;

int shm_creat_init(bool create);
int shm_update_sink();

/**
 * SINK
*/




#endif