#ifndef __proto_h__
#define __proto_h__

// 交互协议相关定义
////////////////////////////////////////////////////////////////////////////////////////////////
// UI 发送给 MCU  （ 显示板 发送给 主板 ）

/* 放水动作
    0x00 表示不放水
    0x01 表示放热水
    0x02 表示放冰水
*/
typedef enum {
    WA_NULL         = 0,
    WA_HOT_WATER    = 1,
    WA_COLL_WATER   = 2,
} WaterAction;

/* 制冷、制冰状态
    0x00 面板上制冷按钮和制冰按钮都未按下 (此种状态压缩机不启动)
    0x01 面板上制冷按钮按下， 制冰按钮未按下
    0x02 面板上制冰按钮按下， 制冷按钮未按下
    0x03 面板上制冷和制冰块按钮同时按下
*/
typedef enum {
    CS_NULL     = 0, // 制冷制冰 均未启动
    CS_COOL     = 1, // 制冷
    CS_ICE      = 2, // 制冰(未用上，制冰需开启制冷，因此为 CS_COOL_ICE)
    CS_COOL_ICE = 3, // 制冷和制冰  
} CoolStatus;

// 辅助功能
typedef enum {
    HF_VOLUMN      = 1,      // 声音开关
    HF_STOVE_LIGHT = 1 << 1, // 炉灯
} HelperFunc;

// 蜂鸣器声音
typedef enum {
    BEEP_TYPE_POWERON         = 1, // 开机
    BEEP_TYPE_PRESSED         = 2, // 有效按键
    BEEP_TYPE_UNPRESS         = 3, // 无效按键
    BEEP_TYPE_PREHOT_COMPLETE = 4, // 预热完成
    BEEP_TYPE_AUTO_PAUSE      = 5, // 烹饪自动暂停
    BEEP_TYPE_RECIPES_OVER    = 6, // 烹饪结束
} BeepType;

////////////////////////////////////////////////////////////////////////////////////////////////
// MCU 发送给 UI  （ 主板 发送给 显示板 ）

typedef enum {
    SC_NULL = 0,    // 无状态更新

    SC_TDS_STATE,               // 自定义信号传输 TDS（净水TDS）
    SC_PRUEWATER_TDS,           // 纯水TDS
    SC_INWATER_TEM,             // 进水温度、常温水温度变化
    SC_COOL_WATER_TEM,          // 制冷温度，冰水温度变化
    SC_HOT_WATER_TEM,           // 热水温度变化

    SC_WARN_CUSTOMER = 0x100,   // 自定义错误码
    SC_WARN_INTAKE_WATER_PROBE, // 进水温探头报警
    SC_WARN_ICE_WATER_PROBE,    // 冰水温探头报警
    SC_WARN_HOT_WATER_PROBE,    // 热水温探头报警
    SC_WARN_OVER_DRY_HEATING,   // 超温/干烧报警
    SC_WARN_MAKE_COOL,          // 制冷异常
    SC_WARN_MAKE_HOT,           // 制热异常
    SC_WARN_MAKE_ICE,           // 结冰报警
    SC_WARN_STORTAGE_WATER,     // 缺水报警

    SC_WARN_LEAKAGE_WATER,      // 漏水报警
    SC_WARN_LVXIN,              // 滤芯到期报警
    SC_WARN_FLOW,               // 流量传感器报警
    SC_WARN_COMM_FAIL,          // 通信故障(表示长时间未收到面板发往主板的信号)
    SC_UV_COMPLETE,             // UV功能完成
    SC_ICE_COMPLETE,            // 制冰完成(冰满)
    SC_WASH_COMPLETE,           // 冲洗完成
    SC_PIPE_BLOCKAGE,           // 管道阻塞报警
    SC_DRAIN_WATER_COMPLETE,    // 排水完成
    SC_OUTWATER_COUNT,          // 出水计数
    SC_LAMP_CHANGE,             // 出水灯状态变化
    SC_WARN,                    // 报警

    SC_OUT_WATER_COMPLETE = 0x200,  // 放水完成
    SC_OUT_ICE_COMPLETE,        // 出冰块完成
    SC_RO_RESET_COMPLETE,       // PP滤芯复位完成
    SC_HPCC_RESET_COMPLETE,       // RO滤芯复位完成
    
    SC_RO_PERCEN,                   // RO滤芯百分比
    SC_HPCC_PERCEN,                 // HPCC滤芯百分比

    SC_WIFI_ADAPTER_NOTIFI,     // wifi 列表更新
    SC_WIFI_CONNECT,            // wifi 连接成功
}appStatusChange;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TUYA 通讯命令（互发）
typedef enum{
    TUYA_WIFI_CMD_HEARTBEAT = 0,
    TUYA_WIFI_CMD_PRODUCT_INFO,                  // 1
    TUYA_WIFI_CMD_WORK_MODE,                     // 2
    TUYA_WIFI_CMD_NETWORK_STATUS,                // 3
    TUYA_WIFI_CMD_WIFI_RESET,                    // 4
    TUYA_WIFI_CMD_WIFI_RESET_CONFIG_MODE,        // 5 重置 Wi-Fi 选择配置模式
    TUYA_WIFI_CMD_CLOUD_CMD,                     // 6 命令下发
    TUYA_WIFI_CMD_MCU_STATUS_REPORT,             // 7 状态上报(异步)
    TUYA_WIFI_CMD_QUERY_STATUS = 8,              // 主要用于模组查询 MCU 所有的 obj 类型的 datapoint 状态
    TUYA_WIFI_CMD_OTA = 0x0A,                    // OTA上报（同步）
    TUYA_WIFI_CMD_OTA_REPLY = 0x0B,              // OTA上报（同步）
    TUYA_WIFI_CMD_REQUEST_TIME = 0x1C,           // 获取本地时间
    TUYA_WIFI_CMD_OPEN_WEATHER = 0x20,           // 打开天气数据功能
    TUYA_WIFI_CMD_SEND_WEATHER_DATA = 0x21,      // 模组下发天气数据
    TUYA_WIFI_CMD_MCU_STATUS_REPORT_SYNC = 0x22, // 状态上报（同步
    TUYA_WIFI_CMD_REQUEST_WEATHER = 0x34, // 主动请求天气服务数据

    TUYA_WIFI_CMD_MCU_OTA = 0x37, // OTA上报（同步）

} TUYA_WIFI_CMD_e;

// DP 的ID枚举
typedef enum{
    TYCMD_PRUE_TDS                  = 1, // 纯水TDS               只上报    数值型
    TYCMD_CLEAN_TDS                 = 2, // 净水TDS               只上报    数值型
    TYCMD_OUTWATER_TEM              = 4, // 实际出水温度           只上报    数值型
    TYCMD_EQUIPMENT                 = 5, // 设备状态               只上报    枚举型
    TYCMD_FAULT                     = 6, // 故障警告               只上报    故障型
    TYCMD_SWITCH_FLOW               = 8, // 取水开关                        布尔型
    TYCMD_CHILD_LOCK                = 9, // 童锁开关                        布尔型          (取消该功能) 
    TYCMD_LIGHT                     = 10,// 照明灯                          布尔型
    TYCMD_AUTO_CLEAN                = 11,// 冲洗开关                        布尔型
    TYCMD_TEMP_SET                  = 12,// 温度设置                        数值型
    TYCMD_WORK_TYPE                 = 13,// 工作类型                        枚举型           (取消该功能) 
    TYCMD_EMPTY_DEVICE              = 15,// 清空设备                        布尔型
    TYCMD_FACTORY_RESET             = 16,// 恢复出厂设置                    布尔型
    TYCMD_FILTER_WATER_ONCE         = 26,// 过滤一次水量           只上报    数值型
    TYCMD_VOL_SET                   = 101,// 出水量设置                     数值型
    TYCMD_FILTER_HPCC_LIFE          = 102,// HPCC复合滤芯寿命       只上报   数值型
    TYCMD_COLDWATER_POWER           = 103,// 制冷开关                       布尔型
    TYCMD_FILTER_RO_LIFT            = 104,// RO滤芯寿命             只上报   数值型
    
    TYCMD_FILTER_HPCC_RESET         = 106,// HPCC复合滤芯复位        只下发    布尔型  *** (取消该功能)
    TYCMD_KEEP_COLD                 = 107,// 保冷状态               只上报   布尔型          (取消该功能) 
    TYCMD_FILTER_RO_RESET           = 108,// RO滤芯复位             只下发   布尔型   ***   (取消该功能) 
 
    TYCMD_LED_ON_START_TIME         = 110,// 出水开灯开始时间                数值型           (取消该功能) 
    TYCMD_LED_ON_END_TIME           = 111,// 出水开灯结束时间                数值型             (取消该功能) 
    TYCMD_CLEAN_NOTIC               = 112,// 清洗提醒               只上报   数值型
    TYCMD_RECOMM_TEMP_SET           = 113,// 推荐出水温度设置                数值型
    TYCMD_RECOMM_VOL_SET            = 114,// 推荐出水量设置                  数值型
    TYCMD_ICE_POWER_SWITCH          = 115,// 制冰开关                       布尔型
    TYCMD_UV_STERILIZE_MODE         = 116,// UV杀菌模式                     枚举型
    TYCMD_UV_STERILIZE_RANGE        = 117,// UV杀菌时间段范围                 raw（透传型）
    TYCMD_WATER_LIGHT_MODE          = 118,// 出水照明灯模式                  枚举型
    TYCMD_WATER_LIGHT_RANGE         = 119,// 出水照明灯时间段设置             raw（透传型）
    TYCMD_COLD_ICE_STATUS           = 120,// 制冷制冰状态           只上报    raw（透传型）
    TYCMD_ICE_TUBE_TEMP             = 121,// 冰胆温度               只上报   数值型
    TYCMD_TOTAL_FILTRATION          = 122,// 总过滤水量             只上报   数值型
}Tuya_Cloud_Cmd;

typedef enum{
    TY_UV_KEEP_ENABLED              = 0, // 保持开启
    TY_UV_RESERVATION               = 1, // 预约开启（设定时间段）
}Tuya_Uv_Mode;

typedef enum{
    TY_LAMP_NOR_CLOSED              = 0, // 出水不亮灯
    TY_LAMP_INDUCTIVE               = 1, // 感应亮灯
    TY_LAMP_RESERVATION             = 2, // 预约时间段感应亮灯（设定时间段）
}Tuya_Lamp_Mode;

// 设备状态
typedef enum{
    ES_PURIFIED     = 0x00,     // 水已过滤（正在出常温水）
    ES_HEATING      = 0x01,     // 出水中
    ES_STANDBY      = 0x02,     // 息屏模式
    ES_CLEANING     = 0x03,     // 滤芯清洗中
    ES_RESET        = 0x04,     // 机器重置中(在线升级或重置出厂设置时)
    ES_EMPTY_DEVICE = 0x05,     // 排水进行中
    ES_SCREEN_LOCK  = 0x06,     // 童锁锁屏中
    ES_FAULT        = 0x07,     // 报错中
    ES_CLEANED      = 0x08,     // 滤芯清洗完成
    ES_EMPTIED      = 0x09,     // 排水完成
    ES_JOB_DONE     = 0x0A,     // 出水结束
    ES_ACTIVE_IDLE  = 0x0B,     // 就绪状态

    ES_REFRIGERATING= 0x0C,     // 制冷中                          （暂未使用）
    ES_COLDWATER_PROHIBITED= 0x0D,  // 制冷功能被主机禁止，不能使用  （暂未使用）
    ES_ICE_MAKING    = 0x0E,     // 制冰中                         （暂未使用）

    ES_ICE_DISCHARGE = 0x0F,     // 出冰中
}EquipmentStatus;

// 故障警告
typedef enum{
    WARN_NONE = 0x0000, 
    E01 = 0x0001,// 加热体 NTC 超温（温度值待定）
    E02 = 0x0002,    // 实际出水温度 高于 设置温度25度（制冷异常）
    E03 = 0x0004,    // 进水温度小于 1℃（水道结冰）
    E04 = 0x0008,    // 实际出水温度低于 设置温度45度（加热功能异常）
    E05 = 0x0010,    // 进水传感器故障（开路、短路）
    E06 = 0x0020,    // 出水传感器故障（开路、短路）
    E07 = 0x0040,    // 通讯故障
    E08 = 0x0080,    // 电源板过零检测故障
    E09 = 0x0100,    // 缺水故障（进水口压力小于 0.01Mpa）
    E10 = 0x0200,    // 滤芯寿命到期（暂时不做）
    E11 = 0x0400,    // 漏水故障
    E12 = 0x0800,    // 流量计感应故障
    E13 = 0x1000,    // 管路堵塞故障（滤芯寿命未到，流量不能满足调温需求）
    E14 = 0x2000,    // 加热传感器故障（开路、短路）
}FaultWarn;

// 工作类型
typedef enum{
    WORK_TYPE_PURIFIED  = 0x00, // 常温水       
    WORK_TYPE_MILK      = 0x01, // 泡奶
    WORK_TYPE_HONEY     = 0x02, // 蜂蜜水
    WORK_TYPE_LEMON     = 0x03, // 柠檬水
    WORK_TYPE_GREEN_TEA = 0x04, // 绿茶
    WORK_TYPE_FLOWER_TEA= 0x05, // 花茶
    WORK_TYPE_COFFEE    = 0x06, // 咖啡
    WORK_TYPE_PU_RE     = 0x07, // 普洱茶
    WORK_TYPE_COLD_WATER= 0x08, // 冰水
}WorkingType;




#endif
