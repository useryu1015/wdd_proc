#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <sys/queue.h>
#include <sys/shm.h>


int shm_open(key_t key, uint32_t size, void **addr);
void shm_close(key_t shm_id);

