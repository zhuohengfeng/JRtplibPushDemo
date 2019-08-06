//
// Created by hengfeng zhuo on 2019-08-01.
//

#include "RtpPack.h"
#include <iostream>

#define MAX_RTP_PACK_LENGTH 1400
#define H264_PAYLOADTYPE 96 // h264类型类型
#define AAC_PAYLOADTYPE 97 // AAC封装类型
#define AU_HEADER_SIZE 4 //


using namespace std;

RtpPack::RtpPack(void) {

}

RtpPack::~RtpPack(void) {

}

bool RtpPack::SetH264RtpPack(unsigned char *NAL_Buf, unsigned long NAL_Size) {

    NALU_t* n = AllocNALU(MAX_RTP_PACK_LENGTH);
    // 指向payload
    char* nalu_payload;
    char sendbuf[1500];






    return false;
}

bool RtpPack::SetAACRtpPack(unsigned char *AAC_Buf, unsigned long AAC_Size) {

    return false;
}




bool RtpPack::SendH264RtpPack(char *nal_buf, unsigned long nal_size) {
    nal_buf;
    nal_size;
    //progress
    return true;
}

bool RtpPack::SendAACRtpPack(char *aac_buf, unsigned long aac_size) {
    //progress
    return true;
}

/**
 * 搜索 00 00 01
 */
bool RtpPack::FindStartCode2(unsigned char *pdata) {
    if(pdata[0]!=0 || pdata[1]!=0 || pdata[2] !=1)
        return false;
    else
        return true;
}

/**
 * 搜索 00 00 00 01
 */
bool RtpPack::FindStartCode3(unsigned char *pdata) {
    if(pdata[0]!=0 || pdata[1]!=0 || pdata[2] !=0 || pdata[3] !=1)
        return false;
    else
        return true;
}

void RtpPack::PrintNaluTypeAndLen(NALU_t *nalu) {
    if (!nalu)return;
    cout << "nalu len = " << nalu->len << endl;
    cout << "nal_unit_type = " << nalu->nal_unit_type << endl;
}

NALU_t *RtpPack::AllocNALU(int buffersize) {
    NALU_t* nalu = nullptr;
    nalu = (NALU_t *)(calloc(1, sizeof(NALU_t)));
    if (!nalu) {
        printf("AllocNALU: callo failed");
        return nullptr;
    }

    nalu->max_size = buffersize;
    nalu->buf = (char *)calloc(buffersize, sizeof(char));
    if (!nalu->buf) {
        free(nalu);
        printf("AllocNALU: callo buf failed");
        return nullptr;
    }

    return nalu;
}

void RtpPack::FreeNALU(NALU_t *n) {
    if (n)
    {
        if (n->buf)
        {
            free(n->buf);
            n->buf=NULL;
        }
        free (n);
    }
}

/**
 * 从pdata数据中解析出nalu
 * @param nalu
 * @param pdata
 * @param nal_size
 * @return
 */
int RtpPack::GetOneNALU(NALU_t *nalu, unsigned char *pdata, unsigned long nal_size) {
    if(FindStartCode2 (pdata))
    {
        nalu->startcodeprefix_len = 3; // 如果是3个起始位
    }
    if(FindStartCode3 (pdata))
    {
        nalu->startcodeprefix_len = 4; // 如果是4个起始位
    }
    nalu->len =nal_size - nalu->startcodeprefix_len;
    memcpy (nalu->buf, &pdata[nalu->startcodeprefix_len], nalu->len);
    nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
    nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
    nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;// 5 bit

    return nal_size;//有前缀的NALU的长度
}
