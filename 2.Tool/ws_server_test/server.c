#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "libwebsockets.h"
#include "../../hpsdc_const.h"

static volatile int exit_sig = 0;
#define MAX_PAYLOAD_SIZE  10 * 1024

void sighdl( int sig ) {
    lwsl_notice( "%d traped", sig );
    exit_sig = 1;
}

/**
 * 会话上下文对象，结构根据需要自定义
 */
struct session_data {
    int msg_count;
    unsigned char buf[LWS_PRE + MAX_PAYLOAD_SIZE];
    int len;
    bool bin;
    bool fin;
};

static int protocol_my_callback( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len ) {
    struct session_data *data = (struct session_data *) user;
    switch ( reason ) {
        case LWS_CALLBACK_ESTABLISHED:       // 当服务器和客户端完成握手后
            printf("Client connect!\n");
            break;
        case LWS_CALLBACK_RECEIVE:           // 当接收到客户端发来的帧以后
            printf("\n\n");
            system("date");            
            system("free -h");
            printf("recvied message:%s\n",in);
            break;
        case LWS_CALLBACK_SERVER_WRITEABLE:   // 当此连接可写时
            break;
    }
    // 回调函数最终要返回0，否则无法创建服务器
    return 0;
}

/**
 * 支持的WebSocket子协议数组
 * 子协议即JavaScript客户端WebSocket(url, protocols)第2参数数组的元素
 * 你需要为每种协议提供回调函数
 */
struct lws_protocols protocols[] = {
    {
        //协议名称，协议回调，接收缓冲区大小
        TEST_WS_PROTO_NAME, protocol_my_callback, sizeof( struct session_data ), MAX_PAYLOAD_SIZE,
    },
    LWS_PROTOCOL_LIST_TERM
};
 
int main(int argc,char **argv)
{
    // 信号处理函数
    signal( SIGTERM, sighdl );
 
    struct lws_context_creation_info ctx_info = { 0 };
    ctx_info.port = TEST_WS_SEVER_PORT;
    ctx_info.iface = NULL; // 在所有网络接口上监听
    ctx_info.protocols = protocols;
    ctx_info.gid = -1;
    ctx_info.uid = -1;
    ctx_info.options = LWS_SERVER_OPTION_VALIDATE_UTF8;

    // fix:  2023年3月17日 ca报错
    // ctx_info.ssl_ca_filepath = "../ca/ca-cert.pem";
    // ctx_info.ssl_cert_filepath = "./server-cert.pem";
    // ctx_info.ssl_private_key_filepath = "./server-key.pem";
    // ctx_info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    //ctx_info.options |= LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT;
    
    struct lws_context *context = lws_create_context(&ctx_info);
    while ( !exit_sig ) {
        lws_service(context, 1000);
    }
    lws_context_destroy(context);

    return 0;
}
