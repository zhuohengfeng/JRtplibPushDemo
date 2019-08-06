//
// Created by hengfeng zhuo on 2019-08-01.
//

/**
*Function：把h264和AAC封包为RTP包
* https://blog.csdn.net/shixin_0125/article/details/78798238
**/
#ifndef JRTPLIBPUSHDEMO_RTPPACK_H
#define JRTPLIBPUSHDEMO_RTPPACK_H

//NALU_INFO
typedef struct {
    int startcodeprefix_len;
    unsigned len;
    unsigned max_size;
    int forbidden_bit;
    int nal_reference_idc;
    int nal_unit_type; // I帧， SPS， PPS
    char *buf;
    unsigned short lost_packets;
} NALU_t;


//RTP Header
typedef struct tagRTPHEAHER
{
    unsigned char V:2;//版本
    unsigned char P:1;//填充
    unsigned char X:1;//扩展
    unsigned char CC:4;//CSRC计数
    unsigned char M:1;//
    unsigned char PT:7; //负载类型
    unsigned short nSeqNum; //序列号 -- 2个字节
    unsigned int  unTimestamp;
    unsigned int  unSSRC;//同步源

} RTP_HEAHER;

// 荷载格式定义三个不同的基本荷载结构，接收者可以通过RTP荷载的第一个字节后5位（如图2）识别荷载结构
//1) 单个NAL单元包：荷载中只包含一个NAL单元。NAL头类型域等于原始 NAL单元类型,即在范围1到23之间
//2) 聚合包：本类型用于聚合多个NAL单元到单个RTP荷载中。本包有四种版本,单时间聚合包类型A (STAP-A)，单时间聚合包类型B (STAP-B)，多时间聚合包类型(MTAP)16位位移(MTAP16), 多时间聚合包类型(MTAP)24位位移(MTAP24)。赋予STAP-A, STAP-B, MTAP16, MTAP24的NAL单元类型号分别是 24,25, 26, 27
//3) 分片单元：用于分片单个NAL单元到多个RTP包。现存两个版本FU-A，FU-B,用NAL单元类型 28，29标识
//常用的打包时的分包规则是：如果小于MTU采用单个NAL单元包，如果大于MTU就采用FUs分片方式。
//因为常用的打包方式就是单个NAL包和FU-A方式，所以我们只解析这两种。

// 如果是单个NAL单元包，那么NAL单元的第一字节和RTP荷载头第一个字节重合

// 单包发送，Nalu Header
typedef struct tagNALUHEAHER
{
    unsigned char F:1;
    unsigned char NRI:2;
    unsigned char Type:5;

} NALU_HEAHER;


// 分包发送
//一个分片由整数个连续NAL单元字节组成。每个NAL单元字节必须正好是该NAL单元一个分片的一部分。相同NAL单元的分片必须使用递增的RTP序号连续顺序发送(第一和最后分片之间没有其他的RTP包）。相似，NAL单元必须按照RTP顺序号的顺序装配。
//打包时，原始的NAL头的前三位为FU indicator的前三位，原始的NAL头的后五位为FU header的后五位。

//FU INDICATOR
typedef struct tagFUINDICATOR
{
    unsigned char F:1;
    unsigned char NRI:2;
    unsigned char Type:5;

}FU_INDICATOR;

//S: 1 bit 当设置成1,开始位指示分片NAL单元的开始。当跟随的FU荷载不是分片NAL单元荷载的开始，开始位设为0。
//E: 1 bit 当设置成1, 结束位指示分片NAL单元的结束，即, 荷载的最后字节也是分片NAL单元的最后一个字节。当跟随的 FU荷载不是分片NAL单元的最后分片,结束位设置为0。
//R: 1 bit 保留位必须设置为0，接收者必须忽略该位
//Fu Header
typedef struct tagFUHEADER
{
    unsigned char S:1;
    unsigned char E:1;
    unsigned char R:1;
    unsigned char Type:5;

}FU_HEADER;


class RtpPack {

public:
    RtpPack(void);
    ~RtpPack(void);

public:
    bool SetH264RtpPack ( unsigned char *NAL_Buf, unsigned long NAL_Size); // 设置H264包
    bool SetAACRtpPack  ( unsigned char *AAC_Buf, unsigned long AAC_Size); // 设置AAC包
    bool SendH264RtpPack(char *nal_buf, unsigned long nal_size); // 发送H264
    bool SendAACRtpPack (char *aac_buf, unsigned long aac_size); // 发送AAC

private:
    bool FindStartCode2(unsigned char *pdata ); // 00 00 01
    bool FindStartCode3(unsigned char *pdata ); // 00 00 00 01
    void PrintNaluTypeAndLen(NALU_t *n); // 打印NALU的类型和长度
    NALU_t *AllocNALU(int buffersize); // 分配NALU
    void FreeNALU(NALU_t *n);
    int GetOneNALU (NALU_t *nalu,unsigned char *pdata ,unsigned long nal_size);

private:
    // RTP 头，分成视频RTP头和音频RTP头
    RTP_HEAHER        *rtp_hdr;
    RTP_HEAHER        *rtp_AAChdr;
    // 单包发送
    NALU_HEAHER       *nalu_hdr;
    // 分包发送
    FU_INDICATOR      *fu_ind;
    FU_HEADER         *fu_hdr;

};


#endif //JRTPLIBPUSHDEMO_RTPPACK_H
