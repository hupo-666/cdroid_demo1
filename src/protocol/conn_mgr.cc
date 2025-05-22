
#include "conn_mgr.h"

#include <conf_mgr.h>
// #include <data.h>
#include <wind_mgr.h>
#include "conf_mgr.h"
// #include <homewindow.h>
// #include <tuya_os_mgr.h>

#define TICK_TIME 50 // tick触发时间（毫秒）

CConnMgr *g_objConnMgr = CConnMgr::ins();
//////////////////////////////////////////////////////////////////

CConnMgr::CConnMgr() {
    mPacket            = new SDHWPacketBuffer(BT_MCU);
    mUartMCU           = 0;
    mNextEventTime     = 0;
    mLastSendTime      = 0;
    mLastStatusTime    = 0;
    mSetData           = 0;
    mMcuType           = 0;
    mLastWarnTime      = 0;
    mLastBeepTime      = 0;
    commErrorCount     = 0;
    mIsFactory         = false;

    g_objHandler->addHandler(BT_MCU, this);
}

CConnMgr::~CConnMgr() {
    __del(mUartMCU);
}

int CConnMgr::init() {
    UartOpenReq ss;

    snprintf(ss.serialPort, sizeof(ss.serialPort), "/dev/ttyS3");
    ss.speed     = 9600;
    ss.flow_ctrl = 0;
    ss.databits  = 8;
    ss.stopbits  = 1;
    ss.parity    = 'N';

    mLastWarnTime = SystemClock::uptimeMillis();
    mUartMCU = new UartClient(mPacket, BT_MCU, ss, "192.168.0.103", 9876, 0);
    mUartMCU->init();

    // 启动延迟一会后开始发包
    mNextEventTime = SystemClock::uptimeMillis() + TICK_TIME * 10;
    App::getInstance().addEventHandler(this);

    // 静音标识
    // closeVolumn(g_objConf->getMuteFlag());

    return 0;
}

int CConnMgr::checkEvents() {
    int64_t curr_tick = SystemClock::uptimeMillis();
    if (curr_tick >= mNextEventTime) {
        mNextEventTime = curr_tick + TICK_TIME;
        return 1;
    }
    return 0;
}

int CConnMgr::handleEvents() {
    int64_t now_tick = SystemClock::uptimeMillis();

    if (mUartMCU) mUartMCU->onTick();

    if (mSetData > 0 || now_tick - mLastSendTime >= 1000) { send2MCU(); }

    return 1;
}

void CConnMgr::setProductTest(pop_callback canlePop){
    BuffData *mCurrRecv;
    if(canlePop){
        mIsFactory = true;
        mUartMCU->setProductCallback([this,canlePop](IAck *ack){
            // if (len > 0) { 
            //     LOG(VERBOSE) << "len:" << len << " hex:" << hexstr(buf, len); 
            //     if(canlePop) canlePop();
            // }
            if((ack->getData(BUF_SND_D12) & 0x10) &&  (ack->getData(BUF_SND_D2) == 0xcc)){
                if(canlePop) canlePop();
            }
            return true;
        });
    }else{
        mIsFactory = false;
        mUartMCU->setProductCallback(nullptr);
    }
    
}

// 放水动作
void CConnMgr::setWaterAction(WaterAction t,bool isComplete) {
    mSndData[BUF_SND_D3] = t;
    mSetData++;
#ifdef TUYA_OS
    bool isOutWater = (t != WA_NULL);
    if((t == WA_HOT_WATER) && (g_appData.outWaterTem == 25)){
        g_appData.eqStatus = ES_PURIFIED;
    }else if(t != WA_NULL){
        g_appData.eqStatus = ES_HEATING;
    }else if(t == WA_NULL){
        if(isComplete)      g_appData.eqStatus = ES_JOB_DONE;
        else                g_appData.eqStatus = ES_ACTIVE_IDLE;
        
    }
    g_tuyaOsMgr->reportDpData(TYCMD_SWITCH_FLOW,PROP_BOOL, &isOutWater);                  // 取水开关(出水)
    g_tuyaOsMgr->reportDpData(TYCMD_EQUIPMENT,PROP_ENUM, &g_appData.eqStatus);            // 设备状态
#endif
    
}

// 水温设定
void CConnMgr::setWaterTem(int value) {
    // if(value > g_appData.lowBoilTem) value = g_appData.lowBoilTem;
    mSndData[BUF_SND_D4] = value;
    mSetData++;
#ifdef TUYA_OS
    g_tuyaOsMgr->reportDpData(TYCMD_RECOMM_TEMP_SET,TYTYPE_VALUE,&value);
#endif
    
}

// 水量设定
void CConnMgr::setWaterYield(int value) {
    mSndData[BUF_SND_D5] = value >> 8;
    mSndData[BUF_SND_D6] = value & 0xff;
    mSetData++;
#ifdef TUYA_OS
    g_tuyaOsMgr->reportDpData(TYCMD_RECOMM_VOL_SET,TYTYPE_VALUE,&value);
#endif
}

// 冰水和冰块 动作（是否制冷或制冰）
void CConnMgr::setCooling(CoolStatus setValue) {
    mSndData[BUF_SND_D7]  = setValue;
    mSetData++;
#ifdef TUYA_OS 
    bool makeIce = (setValue & 0x02)? true:false;
    bool makeCool = (setValue & 0x01)? true:false;
    g_tuyaOsMgr->reportDpData(TYCMD_COLD_ICE_STATUS,TYTYPE_RAW,nullptr);
    g_tuyaOsMgr->reportDpData(TYCMD_ICE_POWER_SWITCH,TYTYPE_BOOL,&makeIce);
    g_tuyaOsMgr->reportDpData(TYCMD_COLDWATER_POWER,TYTYPE_BOOL,&makeCool);
    g_tuyaOsMgr->reportDpData(TYCMD_ICE_TUBE_TEMP,TYTYPE_VALUE,&g_appData.iceWaterTem);   // 冰水温度
    LOGE("g_appData.iceWaterTem = %d",g_appData.iceWaterTem);
#endif
}

// 放冰块动作
void CConnMgr::setIceAction(bool flag,bool isComplete) {
    mSndData[BUF_SND_D8] = flag? 1:0;
    mSetData++;
#ifdef TUYA_OS
    if(flag){
        g_appData.eqStatus = ES_ICE_DISCHARGE;
    }else{
        if(isComplete)  g_appData.eqStatus = ES_JOB_DONE;
        else            g_appData.eqStatus = ES_ACTIVE_IDLE;
        
    }
    g_tuyaOsMgr->reportDpData(TYCMD_EQUIPMENT,PROP_ENUM, &g_appData.eqStatus);            // 设备状态
#endif
}

// HPPC 滤芯复位
void CConnMgr::setHPCCReset(bool flag){
    mSndData[BUF_SND_D9] = flag? 1:0;
    mSetData++;
}
// RO 滤芯复位
void CConnMgr::setROReset(bool flag){
    mSndData[BUF_SND_D10] = flag? 1:0;
    mSetData++;
}

// 设置机器重置状态（停掉所有的负载，如在ota中、恢复出厂设置中）
void CConnMgr::setResetStatus(bool flag){
    if(flag){
        mSndData[BUF_SND_D3] = WA_NULL;
        mSndData[BUF_SND_D7] = CS_NULL;
        mSndData[BUF_SND_D8] = 0x00;
    }
    
    mSetData++;
}

void CConnMgr::setBeep(bool flag/* =true*/,const char *from/* = 0*/) {
    LOGE("beep from=%s", (from ? from : ""));
    mSndData[BUF_SND_D12] |= flag?0x01:0x00;
    mSetData++;
}

void CConnMgr::closeVolumn(bool flag) {
   
    mSetData++;
}

void CConnMgr::setStoveLight(bool flag) {
    
    mSetData++;
}

void CConnMgr::setSleep(bool flag) {
   
    mSetData++;
}

void CConnMgr::send2MCU() {
    BuffData *bd = mPacket->obtain();
    UI2MCU   snd(bd);
    for (int i = BUF_SND_D3; i < BUF_SND_D15; i++) { snd.setData(i, mSndData[i]); }

    //如果在UV杀菌的预约时间内
    // if(g_appData.UVshajunMode == PRO_STATE_UV_RESERVATION){
    //     if(isTimeRange(g_appData.star_time_uv, g_appData.end_time_uv) != g_appData.isUVshajun){
    //         g_appData.isUVshajun = !g_appData.isUVshajun;
    //         Activity_header::ins()->changeState(PRO_STATE_UV_RESERVATION);
    //     }        
    // }else if(g_appData.UVshajunMode == PRO_STATE_UV_KEEP_ENABLED){
    //     // 暂无操作 
    // }
     

    
    // if( g_appData.outWaterStatus != WA_NULL){
    //     // 放水动作（工作状态）
    //     snd.setData(BUF_SND_D3, g_appData.outWaterStatus);
    //     // 水温设定
    //     snd.setData(BUF_SND_D4,g_appData.outWaterTem);
    // }else{
    //     // 放水动作（工作状态）
    //     snd.setData(BUF_SND_D3, WA_NULL);
    //     // 水温设定
    //     snd.setData(BUF_SND_D4,0x00);
    //     // snd.setData(BUF_SND_D5,0x00);
    //     // snd.setData(BUF_SND_D6,0x00);
    // }

    // 出冰块动作
    // snd.setData(BUF_SND_D8, g_appData.iceStatus ? 1:0);

    // HPPC 滤芯复位
    // snd.setData(BUF_SND_D9, g_appData.isResetHPCC ? 1:0); 

    // RO 滤芯复位
    // snd.setData(BUF_SND_D10, g_appData.isResetRO ? 1:0);

    
    // if(g_appData.LampMode == TY_LAMP_NOR_CLOSED || g_appData.isUserContLamp){
    //     snd.setData(BUF_SND_D11, g_appData.zhaoming == PRO_STATE_ZHAOMING?0x01:0x00);
    // }else if(g_appData.LampMode == TY_LAMP_INDUCTIVE){
    //     snd.setData(BUF_SND_D11, 0x02);
    // }else if(g_appData.LampMode == TY_LAMP_RESERVATION){
    //     if(isTimeRange(g_appData.star_time_lamp,g_appData.end_time_lamp)){
    //         snd.setData(BUF_SND_D11, 0x02);
    //     }else{
    //         snd.setData(BUF_SND_D11, 0x00);
    //     }
    // }

    // HPPC 滤芯寿命  目前一直发100
    snd.setData(BUF_SND_D13, 100);
    // RO   滤芯寿命  目前一直发100
    snd.setData(BUF_SND_D14, 100);

    uchar statusByte = mSndData[BUF_SND_D12];
    // 限制蜂鸣器响声间隔
    // if (!g_appData.isMute && (statusByte & 0x01)) {
    //     int64_t now_tick = SystemClock::uptimeMillis();
    //     if (now_tick - mLastBeepTime < 150) {
    //         LOG(WARN) << "limit beep time. t=" << (int)statusByte << " diff=" << (now_tick - mLastBeepTime);
    //         statusByte &= ~0x01;
    //     } else {
    //         mLastBeepTime = now_tick;
    //     }
    // }
    // if(g_appData.isWashing) statusByte |= 0x02;
    // if(g_appData.isUVshajun) statusByte |= 0x04;
    // if(g_appData.isDrain_Water) statusByte |= 0x08;
    // if(mIsFactory){
    //     snd.setData(BUF_SND_D0, 0x02);
    //     snd.setData(BUF_SND_D1, 0x10);
    //     snd.setData(BUF_SND_D2, 0xcc);
    //     statusByte |= 0x10;
    // }
    // if(g_appData.isSuspend)  statusByte |= 0x80;
    snd.setData(BUF_SND_D12, statusByte);


    snd.checkcode();    // 修改检验位
    // LOGE("send Beep = %d",statusByte & 0x01);
    LOG(VERBOSE) << "send to mcu. bytes=" << hexstr(bd->buf, bd->len);
    mUartMCU->send(bd);
    
    mSndData[BUF_SND_D12] = 0;
    mSetData      = 0;
    mLastSendTime = SystemClock::uptimeMillis();
}

bool CConnMgr::getVersion(int &a, int &b, int &c) {
    if (mMcuType == 0) return false;
    a = mMcuType;
    return true;
}

void CConnMgr::onCommDeal(IAck *ack) {
    // LOG(INFO) << "hex str=" << hexstr(ack->mBuf, ack->mDlen);
    if(mIsFactory) return;
    int64_t now_tick = SystemClock::uptimeMillis();

    uchar mcuVersion = ack->getData(BUF_RCV_D2);
    // if(g_appData.McuVersion != mcuVersion){
    //     g_appData.McuVersion = mcuVersion;
    //     LOGE("g_appData.McuVersion = %d",g_appData.McuVersion);
    // }
    

    // 进水温度 （常温）
    uchar realTem = ack->getData(BUF_RCV_D3);
    static int64_t lastChangeRealTemTime = 0;
    // if(g_appData.realWaterTem != realTem && (now_tick - lastChangeRealTemTime >= 5 *1000)){
    //     lastChangeRealTemTime = now_tick;
    //     g_objWindMgr->appStatusNotify(SC_INWATER_TEM,realTem);
    // }
    
    // 冰水温度
    uchar iceTem = ack->getData(BUF_RCV_D4);
    // if(g_appData.iceWaterTem != iceTem){
    //     g_objWindMgr->appStatusNotify(SC_COOL_WATER_TEM, iceTem);
    // }
    // 热水温度
    uchar hotTem = ack->getData(BUF_RCV_D5);
    // if((g_appData.hotWaterTem != hotTem) && (g_appData.outWaterStatus!= WA_NULL)){
    //     g_objWindMgr->appStatusNotify(SC_HOT_WATER_TEM, hotTem);
    // }
    // 净水
    int cleanTDS = ack->getData(BUF_RCV_D6)*256 + ack->getData(BUF_RCV_D7);
    // if(g_appData.cleanWaterTDS != cleanTDS){
    //     g_objWindMgr->appStatusNotify(SC_TDS_STATE, cleanTDS);
    // }
    // 纯水
    int prueTDS = ack->getData(BUF_RCV_D8)*256 + ack->getData(BUF_RCV_D9);
    // if(g_appData.prueWaterTDS != prueTDS){
    //     g_objWindMgr->appStatusNotify(SC_PRUEWATER_TDS, prueTDS);
    // }
      

    // 前置复合滤芯百分比（HPCC滤芯）
    uchar percenHPCC = ack->getData(BUF_RCV_D10);
    // if (percenHPCC != g_appData.HPCCLvxinPercen && !g_appData.isTestLvxin) { g_objWindMgr->appStatusNotify(SC_HPCC_PERCEN,percenHPCC); }

    // RO滤芯百分比
    uchar percenRo = ack->getData(BUF_RCV_D11);
    // if (percenRo != g_appData.ROLvxinPercen && !g_appData.isTestLvxin) { g_objWindMgr->appStatusNotify(SC_RO_PERCEN,percenRo); }

    uchar warnCode = ack->getData(BUF_RCV_D12);
    uchar warnCode2       = ack->getData(BUF_RCV_D13);
    int warnState = 0;
    if(warnCode != 0 || warnCode2 != 0){
        LOGV("mcu warn  status1 ....  code=0x%02x warnCode2 = 0x%02x", warnCode,warnCode2);
        if ((now_tick - mLastWarnTime >= 30 *1000)) {        /* 30s提醒一次 */
            mLastWarnTime = now_tick;
#if 0
            if(warnCode & 0x01) g_objWindMgr->appStatusNotify(SC_WARN_INTAKE_WATER_PROBE);
            if(warnCode & 0x02) g_objWindMgr->appStatusNotify(SC_WARN_ICE_WATER_PROBE);
            if(warnCode & 0x04) g_objWindMgr->appStatusNotify(SC_WARN_HOT_WATER_PROBE);
            if(warnCode & 0x08) g_objWindMgr->appStatusNotify(SC_WARN_OVER_DRY_HEATING);
            if(warnCode & 0x10) g_objWindMgr->appStatusNotify(SC_WARN_MAKE_COOL);
            if(warnCode & 0x20) g_objWindMgr->appStatusNotify(SC_WARN_MAKE_HOT);
            if(warnCode & 0x40) g_objWindMgr->appStatusNotify(SC_WARN_MAKE_ICE);
            if(warnCode & 0x80) g_objWindMgr->appStatusNotify(SC_WARN_STORTAGE_WATER);

            if(warnCode2 & 0x01) g_objWindMgr->appStatusNotify(SC_WARN_LEAKAGE_WATER);
            // if(status & 0x02) g_objWindMgr->appStatusNotify(SC_WARN_LVXIN);     // 目前已经取消
            if(warnCode2 & 0x04) g_objWindMgr->appStatusNotify(SC_WARN_FLOW);      
            if(warnCode2 & 0x08) g_objWindMgr->appStatusNotify(SC_WARN_COMM_FAIL);
            // if(status & 0x10) g_objWindMgr->appStatusNotify(SC_UV_COMPLETE);    // 目前已经取消
            // if(status & 0x40) g_objWindMgr->appStatusNotify(SC_WASH_COMPLETE);  // 目前已经取消
            if(warnCode2 & 0x80) g_objWindMgr->appStatusNotify(SC_PIPE_BLOCKAGE);
#endif 
            if(warnCode & 0x01) warnState |= E01;
            if(warnCode & 0x02) warnState |= E02;
            if(warnCode & 0x04) warnState |= E03;
            if(warnCode & 0x08) warnState |= E04;
            if(warnCode & 0x10) warnState |= E05;
            if(warnCode & 0x20) warnState |= E06;
            if(warnCode & 0x40){
                commErrorCount++;
                if(commErrorCount >= 30){
                    warnState |= E07;
                    commErrorCount = 0;
                }
            }else{ commErrorCount = 0; }
            if(warnCode & 0x80) warnState |= E08;

            if(warnCode2 & 0x01)warnState |= E09;
            // if(status & 0x02) g_objWindMgr->appStatusNotify(SC_WARN_LVXIN);     // 目前已经取消
            if(warnCode2 & 0x04) warnState |= E11;
            if(warnCode2 & 0x08) warnState |= E12;
            // if(status & 0x10) g_objWindMgr->appStatusNotify(SC_UV_COMPLETE);    // 目前已经取消
            // if(status & 0x40) g_objWindMgr->appStatusNotify(SC_WASH_COMPLETE);  // 目前已经取消
            if(warnCode2 & 0x80) warnState |= E13;
            // if(warnState && (g_appData.eqStatus != ES_RESET)) g_objWindMgr->appStatusNotify(SC_WARN,warnState);
        }
    // }else if(g_appData.warnState){
    //     g_appData.warnState = WARN_NONE;
    //     g_objWindMgr->AppCallback(CS_WARN_CLOSE);
    // }

    // if((g_appData.zhibing == PRO_STATE_ICE_DONE && !(warnCode2 & 0x20)) || 
    //     (g_appData.zhibing == PRO_STATE_ICE_ING && warnCode2 & 0x20)){
    //         g_objWindMgr->appStatusNotify(SC_ICE_COMPLETE,warnCode2 & 0x20);
    // }
    
    // （0:正常待机  1:以下功能）
    // (0bit):0 表示待机或放水中，  1 表示放水完成       
    // (1bit):0 待机或放冰块中，    1 表示出冰块完成   // 目前已经取消
    // (2bit):0 待机，              1 表示 HPPC 滤芯复位完成  
    // (3bit):0 待机，              1 表示 RO   滤芯复位完成
    // (4bit):0 待机，              1 表示 排水完成

    // (6bit):0 待机，              1 表示 照明灯的状态
    // (7bit):0 待机，              1 表示 出水口每出水100ml上报一次
    uchar waterLevel       = ack->getData(BUF_RCV_D14);
    // if(waterLevel != 0){
    //     LOGD("mcu reset status3 .... code=0x%02x", waterLevel);
    //     if(waterLevel & 0x01) g_objWindMgr->appStatusNotify(SC_OUT_WATER_COMPLETE);
    //     // if(waterLevel & 0x02) g_objWindMgr->appStatusNotify(SC_OUT_ICE_COMPLETE);
    //     if(waterLevel & 0x04) g_objWindMgr->appStatusNotify(SC_HPCC_RESET_COMPLETE);
    //     if(waterLevel & 0x08) g_objWindMgr->appStatusNotify(SC_RO_RESET_COMPLETE);
    //     if(waterLevel & 0x10) g_objWindMgr->appStatusNotify(SC_DRAIN_WATER_COMPLETE);
    //     if(waterLevel & 0x20) ;
    //     if(waterLevel & 0x40) ;
    //     if(waterLevel & 0x80) g_objWindMgr->appStatusNotify(SC_OUTWATER_COUNT);
    // }
    int lampStatus = waterLevel & 0x40?1:0;
    // if((g_appData.zhaoming != lampStatus + PRO_STATE_ZHAOMING_CLOSE) && (g_appData.LampMode != TY_LAMP_NOR_CLOSED) && !g_appData.isUserContLamp){
    //     g_objWindMgr->appStatusNotify(SC_LAMP_CHANGE,lampStatus);
    }   
#ifdef TUYA_OS
    static bool isOpen = false;
    if(isOpen != (g_appData.zhaoming == PRO_STATE_ZHAOMING)){
        isOpen = (g_appData.zhaoming == PRO_STATE_ZHAOMING);
        g_tuyaOsMgr->reportDpData(TYCMD_LIGHT,TYTYPE_BOOL,&isOpen);    // 一个过滤水量（每次增加100ml）
    }
#endif
}
