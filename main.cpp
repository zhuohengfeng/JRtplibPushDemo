//
// Created by hengfeng zhuo on 2019-08-01.
//
#include "main.h"

using namespace std;


void initFFmpeg() {
    // 设置LOG等级
    av_log_set_level(AV_LOG_INFO);
    av_log(NULL, AV_LOG_INFO, "Now let's start the game....\n\n");
    // 注册所有的编解码器
    avcodec_register_all();
    // 注册所有封装器
    av_register_all();
    // 注册所有网络协议
    avformat_network_init();
}

const char* RTMP_URL = "";


/**
 * 从RTMP上拉流，解封装出H264和AAC，然后打包成RTP包后推流
 */
int main(int argc, char* argv[]) {

    initFFmpeg();






    return 0;
}