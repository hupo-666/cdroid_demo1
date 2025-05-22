
/*
 * 等宽数字跳动时间[00:00]
 */
#ifndef __time_textview_h__
#define __time_textview_h__

#include "libs.h"
#include <cdroid.h>

class TimeTextView : public TextView {
public:
    TimeTextView(int w, int h);
    TimeTextView(const std::string &txt, int w, int h);
    TimeTextView(Context *ctx, const AttributeSet &attrs);

    void start();
    void stop();
    void setIsUpdate(bool flag);
protected:
    void initViewData();
    void calcCharWidthHeight();
    void getConnMax(int &w, int &h);
    void getCharWH(uchar ch, Point &pt);
    void onTicker();

    virtual void onMeasure(int widthMeasureSpec, int heightMeasureSpec);
    virtual void onDraw(Canvas &cans);

protected:
    bool                   mAutoStart;
    bool                   mIsUpdate;
    uchar                  mIsDot : 1;
    uchar                  mIsStart : 1;
    uchar                  mNumWidth;
    uchar                  mNumHeight;
    uchar                  mCharWidth;
    uchar                  mCharHeight;
    std::map<uchar, Point> mCharWidthHeight;// 字符宽高
    Runnable               mTicker;
    int                    mTickerTime;
};

#endif
