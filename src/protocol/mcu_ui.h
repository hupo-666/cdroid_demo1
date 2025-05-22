#ifndef __mcu_ui_ack_h__
#define __mcu_ui_ack_h__

#include "ipacket_buffer.h"
#include "proto.h"
#include "comm_func.h"

#include <crc.h>

typedef enum
{
    BUF_SND_D0 = 0, // 帧头
    BUF_SND_D1,     // 功能码
    BUF_SND_D2,
    BUF_SND_D3,
    BUF_SND_D4,
    BUF_SND_D5,
    BUF_SND_D6,
    BUF_SND_D7,
    BUF_SND_D8,
    BUF_SND_D9,
    BUF_SND_D10,
    BUF_SND_D11,
    BUF_SND_D12,
    BUF_SND_D13,
    BUF_SND_D14,
    BUF_SND_D15, // (CRC16)
} EMUI2MCU;

typedef enum
{
    BUF_RCV_D0 = 0, // 帧头
    BUF_RCV_D1,     // 功能码
    BUF_RCV_D2,     // 数据长度
    BUF_RCV_D3,     // 数据 0
    BUF_RCV_D4,
    BUF_RCV_D5,
    BUF_RCV_D6,
    BUF_RCV_D7,
    BUF_RCV_D8,
    BUF_RCV_D9,
    BUF_RCV_D10,
    BUF_RCV_D11,
    BUF_RCV_D12,
    BUF_RCV_D13,
    BUF_RCV_D14,
    BUF_RCV_D15,

} EMMCU2UI;

class DataCheck
{
public:
    DataCheck() {}
    unsigned char Sum(unsigned char *ptr, unsigned char len)
    {
        unsigned char sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += ptr[i];
        }
        return sum;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////
// 串口通讯（与电控板）

// UI -> MCU
class UI2MCU : public IAsk, public DataCheck
{
public:
    constexpr static uchar BUF_LEN = 32;
    constexpr static uchar HEAD_BYTE = 0x02; // 帧头
public:
    UI2MCU() {}
    UI2MCU(BuffData *buf) { parse(buf); }
    // 初始化
    void parse(BuffData *buf)
    { // override
        mBf = buf;
        if (mBf->len == 0)
        {
            mBf->len = BUF_LEN;
            mBf->buf[BUF_SND_D0] = HEAD_BYTE;
        }
    }

protected:
    virtual int getCMD() { return 0; } // override

public:
    void setCMD(uchar cmd) { mBf->buf[BUF_SND_D1] = cmd; }
    void setData(uchar pos, uchar data) { mBf->buf[pos] = data; }
    void setLen(uchar len) { mBf->len = len; }
    void checkcode()
    { // override
        ushort cyc16 = CRC16_Modbus(mBf->buf, mBf->len - 2);
        mBf->buf[mBf->len - 2] = cyc16 & 0xFF;
        mBf->buf[mBf->len - 1] = cyc16 >> 8;
    }
};

// MCU -> UI
class MCU2UI : public IAck, public DataCheck
{
public:
    constexpr static uchar BUF_LEN = 32;
    constexpr static uchar HEAD_BYTE[2] = {0x02, 0x10}; // 帧头

public:
    MCU2UI() {}
    MCU2UI(BuffData *bf) { parse(bf); }
    // 初始化
    void parse(BuffData *bf)
    {
        mDlen = bf->len;
        mPlen = &bf->len;
        mBuf = bf->buf;
        // LOGE("mDlen = %d, mBuf = %s", mDlen, hexstr(mBuf, mDlen).c_str());
    }
    // 添加数据 并找到数据头
    int add(uchar *bf, int len)
    {
        int i, j, rlen = 0;
        bool find_head = false;
        while (mDlen < BUF_LEN && rlen < len)
        {
            // add data
            if (BUF_LEN - mDlen >= len - rlen)
            {
                memcpy(mBuf + mDlen, bf + rlen, len - rlen);
                mDlen += len - rlen;
                rlen = len;
            }
            else
            {
                memcpy(mBuf + mDlen, bf + rlen, BUF_LEN - mDlen);
                rlen += BUF_LEN - mDlen;
                mDlen = BUF_LEN;
            }
            // find head
            if (!find_head && mDlen > sizeof(HEAD_BYTE))
            {
                LOG(VERBOSE) << "mDlen == " << mDlen;
                for (i = 0, j = -1; i < mDlen; i++)
                {
                    if (mBuf[i] == HEAD_BYTE[0])
                    {
                        if (i < mDlen - 1 && mBuf[i + 1] == HEAD_BYTE[1])
                        {
                            j = i;
                            find_head = true;
                            break;
                        }
                        else
                        {
                            if (i == mDlen - 1)
                            {
                                j = i;
                            }
                        }
                        // if (i < mDlen - 1 && mBuf[i + 1] == HEAD_BYTE[1]) {
                        // if (i < mDlen - 2 && mBuf[i + 2] == HEAD_BYTE[2]) {
                        // j = i;
                        // find_head = true;
                        // break;
                        // } else {
                        //     if (i == mDlen - 1) { j = i; }
                        // }
                        // } else {
                        //     if (i == mDlen - 1) { j = i; }
                        // }
                    }
                }
                if (j == -1)
                {
                    mDlen = 0;
                }
                else if (j > 0)
                {
                    memmove(mBuf, mBuf + j, mDlen - j);
                    mDlen -= j;
                }
            }
            *mPlen = mDlen;
        }
        LOG(VERBOSE) << "return rlen == " << rlen;
        return rlen;
    }
    // 判断数据帧是否完整（基本是用来根据数据长度来判断是否完整）
    bool complete()
    {
        if (mBuf[0] == HEAD_BYTE[0] && mBuf[1] == HEAD_BYTE[1] && mDlen >= mBuf[1])
        {
            return true;
        }
        return false;
    }
    // 判断校验码
    bool check()
    {
        LOG(VERBOSE) << "mBuf == " << mBuf[mBuf[1] - 1];
        uchar sum = 0;
        for (int i = 0; i < mBuf[1] - 1; i++)
        {
            // LOGI("mbuf[%d], %s", i, mBuf[i]);
            sum += mBuf[i];
        }
        LOG(VERBOSE) << "mBuf == " << std::to_string(mBuf[mBuf[1] - 1]) << ",   sum == " << std::to_string(sum);
        return mBuf[mBuf[1] - 1] == sum;
    }

protected:
    // 都是用于处理完数据以后的回调使用
    virtual int getType() { return BT_MCU; }
    virtual int getCMD() { return mBuf[1]; }

public:
    int getData(int pos)
    {
        if (pos >= 0 && pos < BUF_LEN - 1)
        {
            return mBuf[pos];
        }
        return 0;
    }
};

//////////////////////////////////////////////////////////////////////////////////////
// tuya 的串口通讯

// UI -> Tuya
class UI2TUYA : public IAsk, public DataCheck
{
public:
    constexpr static uchar BUF_LEN = 128;
    constexpr static uchar HEAD_BYTE[3] = {0x55, 0xaa, 0x03};

public:
    UI2TUYA() {}
    UI2TUYA(BuffData *buf) { parse(buf); }
    void parse(BuffData *buf)
    {
        mBf = buf;
        if (mBf->len == 0)
        {
            mBf->len = BUF_LEN;
            mBf->buf[BUF_SND_D0] = HEAD_BYTE[0];
            mBf->buf[BUF_SND_D1] = HEAD_BYTE[1];
            mBf->buf[BUF_SND_D2] = HEAD_BYTE[2];
        }
    }

protected:
    virtual int getCMD() { return 0; }

public:
    void setCMD(uchar cmd) { mBf->buf[BUF_SND_D3] = cmd; }
    void setData(uchar pos, uchar data) { mBf->buf[pos] = data; }
    void checkcode()
    {
        int len = (mBf->buf[4] * 256 + mBf->buf[5]) + 7;
        mBf->buf[len - 1] = Sum(mBf->buf, len - 1);
        mBf->len = len;
    }
};

// Tuya -> UI
class TUYA2UI : public IAck, public DataCheck
{
public:
    constexpr static uchar BUF_LEN = 128;
    constexpr static uchar HEAD_BYTE[3] = {0x55, 0xaa, 0x00};

public:
    TUYA2UI() {}
    TUYA2UI(BuffData *bf) { parse(bf); }
    void parse(BuffData *bf)
    {
        mDlen = bf->len;
        mPlen = &bf->len;
        mBuf = bf->buf;
    }
    int add(uchar *bf, int len)
    {
        int i, j, rlen = 0;
        bool find_head = false;
        while (mDlen < BUF_LEN && rlen < len)
        {
            // add data
            if (BUF_LEN - mDlen >= len - rlen)
            {
                memcpy(mBuf + mDlen, bf + rlen, len - rlen);
                mDlen += len - rlen;
                rlen = len;
            }
            else
            {
                memcpy(mBuf + mDlen, bf + rlen, BUF_LEN - mDlen);
                rlen += BUF_LEN - mDlen;
                mDlen = BUF_LEN;
            }
            // find head
            if (!find_head && mDlen > sizeof(HEAD_BYTE))
            {
                for (i = 0, j = -1; i < mDlen; i++)
                {
                    if (mBuf[i] == HEAD_BYTE[0])
                    {
                        if (i < mDlen - 1 && mBuf[i + 1] == HEAD_BYTE[1])
                        {
                            if (i < mDlen - 2 && mBuf[i + 2] == HEAD_BYTE[2])
                            {
                                j = i;
                                find_head = true;
                                break;
                            }
                            else
                            {
                                if (i == mDlen - 2)
                                {
                                    j = i;
                                }
                            }
                        }
                        else
                        {
                            if (i == mDlen - 1)
                            {
                                j = i;
                            }
                        }
                    }
                }
                if (j == -1)
                {
                    mDlen = 0;
                }
                else if (j > 0)
                {
                    memmove(mBuf, mBuf + j, mDlen - j);
                    mDlen -= j;
                }
            }
            *mPlen = mDlen;
        }
        return rlen;
    }
    bool complete()
    {
        if (mDlen >= (mBuf[4] * 256 + mBuf[5] + 7) && (mBuf[0] == HEAD_BYTE[0]) && (mBuf[1] == HEAD_BYTE[1]) && (mBuf[2] == HEAD_BYTE[2]))
        {
            return true;
        }
        return false;
    }
    bool check()
    {
        int len = (mBuf[4] * 256 + mBuf[5]) + 7;
        return (mBuf[len - 1] == Sum(mBuf, len - 1));
    }

protected:
    virtual int getType() { return BT_TUYA_MCU; }
    virtual int getCMD() { return mBuf[BUF_SND_D3]; }

public:
    int getData(int pos)
    {
        if (pos >= 0 && pos < BUF_LEN - 1)
        {
            return mBuf[pos];
        }
        return 0;
    }
};

#endif
