
#ifndef __conn_mgr_h__
#define __conn_mgr_h__

#include "packet_buffer.h"
#include "uart_client.h"
#include "cmd_handler.h"

typedef std::function<void()> pop_callback;

class CConnMgr : public EventHandler, public IHandler {
protected:
    CConnMgr();
    ~CConnMgr();

public:
    static CConnMgr *ins() {
        static CConnMgr stIns;
        return &stIns;
    }

    // 产测
    void setProductTest(pop_callback canlePop);

    // 放水动作
    void setWaterAction(WaterAction t,bool isComplete = true);
    // 水温设定
    void setWaterTem(int value);
    // 水量设定
    void setWaterYield(int value);
    // 冰水和冰块 动作（是否制冷或制冰）
    void setCooling(CoolStatus setValue);
    // 放冰块动作
    void setIceAction(bool weight,bool isComplete = true);
    // PP 滤芯复位
    void setHPCCReset(bool flag);
    // RO 滤芯复位
    void setROReset(bool flag);
    // 设置机器重置状态（停掉所有的负载，如在ota中、恢复出厂设置中）
    void setResetStatus(bool flag);

    int init();
    // 发送给按键板
    void send2MCU();
    // 声音关闭
    void closeVolumn(bool flag);
    // 炉灯开关
    void setStoveLight(bool flag);
    // 待机状态[时钟]
    void setSleep(bool flag);
    // 蜂鸣器声音
    void setBeep(bool flag = true,const char *from = 0);
    
    // 版本号
    bool getVersion(int &a, int &b, int &c);

protected:
    virtual int checkEvents();
    virtual int handleEvents();
    virtual void onCommDeal(IAck *ack);
private:
    bool             mIsFactory;
    int64_t          mLastStatusTime;
    IPacketBuffer   *mPacket;
    int64_t          mNextEventTime;
    int64_t          mLastSendTime;
    int64_t          mLastWarnTime;
    int64_t          mLastBeepTime;
    UartClient      *mUartMCU; // mcu
    int              mSetData; // 设置数据次数
    int              commErrorCount;
    uchar            mSndData[UI2MCU::BUF_LEN]; // 发送缓冲区    

    uchar            mMcuType;           // 机型
};

extern CConnMgr *g_objConnMgr;

#endif
