﻿
#ifndef __packet_buffer_h__
#define __packet_buffer_h__

#include <list>
#include "ipacket_buffer.h"
#include "mcu_ui.h"

class SDHWPacketBuffer : public IPacketBuffer {
public:
    SDHWPacketBuffer(BufferType type);
    ~SDHWPacketBuffer();

protected:

protected:
    virtual BuffData   *obtain();              // 创建
    virtual void        recycle(BuffData *buf);                     // 回收
    virtual int         add(BuffData *buf, uchar *in_buf, int len); // 添加数据
    virtual bool        complete(BuffData *buf);                    // 数据完整
    virtual bool        compare(BuffData *src, BuffData *dst);      // 对比数据
    virtual bool        check(BuffData *buf);                       // 校验数据
    virtual std::string str(BuffData *buf);                         // 格式化字符串
    virtual void        check_code(BuffData *buf);                  // 生成校验码
    virtual IAck       *ack(BuffData *bf);                  // 转化成应答包
private:
    BufferType          mType;

    std::list<BuffData *> mBuffs;

    IAsk                *mSND;
    IAck                *mRCV;
};

#endif
