#ifndef PAGE_BASE_H
#define PAGE_BASE_H
#include<iostream> 
#include<widget/imageview.h>
#include<widget/textview.h>
#include<cdroid.h>
#include"time_textview.h"
#include"R.h"
#include"comm_func.h"

enum pagetype{
    PAGE_None_=-1 ,
    PAGE_HOME_=0,
    PAGE_STEAM_BARBECUE_,
    PAGE_WATER_MODE_LAYOUT_,
    PAGE_WATER_MODIFY_,
    PAGE_WATER_ADD_,
};


class PageBase
{
public:
    PageBase();
    PageBase(ViewGroup*mWindPageBox,const std::string resource,pagetype pagetype);
    virtual ~PageBase();
    void AddPage();
    void RemovePage();
    ViewGroup* Get_m_pagelayout()
    {
        return m_layout;
    };
    pagetype Get_pagetype()
    {
        return m_pagetype;
    }

    virtual void Start_Page_action(pagetype type,int position=-1);  //需要在子页面类中重写，不能忘记重新给页面绑定m_Timer_cb，以确保timetextview的正常使用
    virtual void Stop_Page_action();

protected:

    ViewGroup* m_mWindPageBox; //总的展示页面的布局，可以认为是页面的根布局
    ViewGroup* m_layout;       //当前页面的布局
    pagetype m_pagetype;

    //页面左侧的布局中的控件，由于左侧布局在所有页面中都会出现，所以将其放在基类中实现
    TimeTextView* left_time;
    ImageView* wifi;
    ImageView* lock;
    ImageView* light_bulb;
    ImageView* set;
    void OnTimer();
    Runnable m_Timer_cb;
    int m_Timer_Time;
private:
    
    void InitPageBase(const std::string resource);
    

};



#endif //  PAGE_BASE_H


