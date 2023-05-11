#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "zlog.h"
#include "ws_test_client.h"
#include "srv_hardware.h"
#include "ws_dataflow.h"
#include "srv_websocket.h"

int loop();
int option_main(int argc, char **argv);
int start_channel_services(void *arg);

