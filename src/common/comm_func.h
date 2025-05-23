﻿
#ifndef __comm_func_h__
#define __comm_func_h__

#include "common.h"
#include <view/view.h>
#ifndef __rgba
#define __rgba(r, g, b, a)                                                                                             \
    (((unsigned char)(a) << 24) | ((unsigned char)(r) << 16) | ((unsigned char)(g) << 8) | (unsigned char)(b))
#endif
#ifndef __rgb
#define __rgb(r, g, b) __rgba(r, g, b, 0xFF)
#endif

#define __dc(T, P) dynamic_cast<T *>(P)
#define __del(P)                                                                                                       \
    do {                                                                                                               \
        if (P) {                                                                                                       \
            delete P;                                                                                                  \
            P = 0;                                                                                                     \
        }                                                                                                              \
    } while (false)
#define __count_array(A) sizeof(A) / sizeof(A[0])
#define __rand_value(A, B) ((A) + rand() % ((B) - (A) + 1))

#define DAY_SECONDS 86400
#define HOUR_SECONDS 3600

#define HV_POPEN(__cmd...)                                                                                             \
    do {                                                                                                               \
        char __result[256] = {0};                                                                                      \
        snprintf(__result, sizeof(__result), ##__cmd);                                                                 \
        system(__result);                                                                                              \
        printf("# %s\r\n", __result);                                                                                  \
    } while (0)

#define SENDKEY(k,down) {InjectKey(0x01,k,down);}
////////////////////////////////////////////////////////////////////////////////////////////////////

// 是否全数字
bool isNumric(const char *str_nums);

// 时间字符串
std::string getTimeStr();

// 获取日期（星期）
std::string getWeekDay();
// 获取日期（月/日）
std::string getMonthDayDateTime();

//获得当前的秒数（现在1小时10分钟20秒，便返回20）
int64_t getTodayTimeSec();

// 获取秒级时间戳
int64_t getTimeSec();

// 获取毫秒级时间戳
int64_t getTimeMSec();

// 字符串时间
std::string getDateTime();
// 获取0点时间戳
int64_t getZeroTime();
// 是否今天
bool isToday(int64_t sec);

// 长度不够，添加前缀
std::string fillLength(int num, int len, std::string pre = "0");
std::string fillLength(std::string &str, int len, char end = ' ');

// 打印二进制数据
void hexdump(const char *label, unsigned char *buf, int len, int width = 16);
std::string hexstr(unsigned char *buf, int len);

// 大小写转换
std::string &toUpper(std::string &letter);
std::string &toLower(std::string &letter);

// 分割字符串
int stringSplit(const std::string &str, std::vector<std::string> &out, char ch = ',');

// 设置时间
void timeSet(int year, int month, int day, int hour, int min, int sec);
void timeSet(const int64_t &time_sec);

// 获取字的长度
int         wordLen(const char *buffer);
int         getChineseWordLen(const char *buffer);
std::string getWordChineseDouble(const char *buffer, int count);
std::string getWord(const char *buffer, int count);
std::string decLastWord(const char *buffer);

// system结果
std::string sysCommand(const std::string &cmd);

// host -> ip
std::string getHostIp(const std::string &host);

// finds the first occurrence of string2 in string1
char *stristr(const char *str1, const char *str2);

/* 编写函数：
 * string& replace_all (string& src, const string& old_value, const string& new_value); 
 * 参数：源字符串src    被替换的子串old_value    替换的子串new_value
 *
 * 功能：将 源串src 中 子串old_value 全部被替换为 new_value
 */ 
std::string& replace_all(std::string& src, const std::string& old_value, const std::string& new_value);

// 计算 wifi 信号的大小
int calculation_signal(float Quality,int SignalLevel);

// 播放点击的声音
void playSound(int type);

// 将一个layout的内容 一定倍数 绘制到 另外一个layout上
// scale 将内容缩放一定倍率，然后再放大，实现模糊的效果
void scaleDrawBackground(View *fromView,View *toView,int scale);

// 将mcu的版本号转换成字符串(V1.1.1.1)的形式
std::string getMCUVersion(int version);

void InjectKey(int type, int code, int value);

// 拆分版本号字符串为数字部分
std::vector<int> splitVersion(const std::string& version);
// 检查网络版本号 是否大于 当前的版本号
int checkVersion(const std::string &version,const std::string &localVersion);
// 是否固定的模式
bool isFixedMode(std::string name);
// 是否与现有模式重名
bool isDuplicateModeName(std::string name);
// 获取文件大小的函数
long getFileSize(const char* filename);
// log写入到文本中 
void logToFile(const char* message);
// 获取对应type的模糊背景图
std::string getBlurredBackground(int type);
// 判断字符串是否全为 空格
bool isWhitespace(const std::string& str);
#endif
