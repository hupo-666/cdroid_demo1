
#include <iostream>
#include <iomanip>
#include "packet_buffer.h"

SDHWPacketBuffer::SDHWPacketBuffer(BufferType type):mType(type) {
    switch(mType){
        case BT_MCU:{
            mSND = new UI2MCU;
            mRCV = new MCU2UI;
            break;
        }case BT_TUYA_MCU:{
            mSND = new UI2TUYA;
            mRCV = new TUYA2UI;
            break;
        }
    }
}

SDHWPacketBuffer::~SDHWPacketBuffer(){
    for (BuffData *bf : mBuffs){
        free(bf);
    }
    mBuffs.clear();
}

BuffData *SDHWPacketBuffer::obtain() {
    // 这里最容易出现访问越界
    // 根据不同的类型，申请buf的大小。收发都是共用一个，因此需要手动修改较大的长度
    short len = 8;
    switch (mType) {
        case BT_MCU: len = MCU2UI::BUF_LEN; break;
        case BT_TUYA_MCU: len = TUYA2UI::BUF_LEN; break;
        default: break;
    }

    for (auto it = mBuffs.begin(); it != mBuffs.end(); it++) {
        BuffData *bf = *it;
        if (bf->type == mType && bf->slen == len) {
            bf->len = 0;
            mBuffs.erase(it);
            return bf;
        }
    }

    BuffData *bf = (BuffData *)calloc(1, sizeof(BuffData) + len);
    bf->type = mType;
    bf->slen = len;
    bf->len  = 0;
    bzero(bf->buf, bf->slen);

    return bf;
}
// 回收，知道就行
void SDHWPacketBuffer::recycle(BuffData *buf) {
    bzero(buf->buf, buf->slen);
    buf->len = 0;
    mBuffs.push_back(buf);
}

// 添加
int SDHWPacketBuffer::add(BuffData *buf, uchar *in_buf, int len) {
    // LOGE("in_buf = %u", in_buf);
    mRCV->parse(buf);
    return mRCV->add(in_buf, len);
}
// 判断数据是否完整
bool SDHWPacketBuffer::complete(BuffData *buf) {
    mRCV->parse(buf);
    return mRCV->complete();
}
// 判断数据是否重复，一般是可以直接return false
bool SDHWPacketBuffer::compare(BuffData *src, BuffData *dst) {    
    // 有按键数据每一帧都需要处理
    if(mType == BT_MCU){
        LOGV("src->type != dst->type == %d", src->type != dst->type);
        LOGV("src->len != dst->len == %d", src->len != dst->len);
        if (src->type != dst->type)
            return false;
        if (src->len != dst->len)
            return false;
        for (short i = 0; i < src->len; i++) {
            LOGV("for    src->buf[i] != dst->buf[i] == %d", src->buf[i] != dst->buf[i]);
            if (src->buf[i] != dst->buf[i])
                return false;
        }

        if(src->buf[6] || src->buf[7] || src->buf[8] || src->buf[9])
            return false;
    }else if(mType == BT_TUYA_MCU){
        return false;
    }   
    return true;
}
// 检查校验码
bool SDHWPacketBuffer::check(BuffData *buf) {
    // LOGI("bool SDHWPacketBuffer::check(BuffData *buf) {");
    mRCV->parse(buf);    
    return mRCV->check();
}
// 母鸡干嘛的
std::string SDHWPacketBuffer::str(BuffData *buf) {
    std::ostringstream oss;
    for (int i = 0; i < buf->len; i++) {
        oss << std::hex << std::setfill('0') << std::setw(2) << (int)buf->buf[i] << " ";
    }
    return oss.str();
}
// 发送数据帧的 添加校验码，以上的（check）的接收帧的检查，不同的功能
void SDHWPacketBuffer::check_code(BuffData *buf) {
    mSND->parse(buf);
    mSND->checkcode();
}
//获取buf
IAck *SDHWPacketBuffer::ack(BuffData *bf) {
    mRCV->parse(bf);
    return mRCV;
}
