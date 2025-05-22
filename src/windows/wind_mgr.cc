#include "wind_mgr.h"
#include"page_home.h"
#include"page_steam_barbecue.h"
#include"page_water_mode.h"
#include"page_water_mode_modify.h"
#include"poppage.h"

void wind_mgr::Init_Window()
{
    main_window=new Window(0,0,-1,-1);
    root_layout=(ViewGroup*)LayoutInflater::from(main_window->getContext())->inflate("@layout/main_window",main_window);
    root_page=(ViewGroup*)root_layout->findViewById(demo1::R::id::page_box);
    root_pop_page=(ViewGroup*)root_layout->findViewById(demo1::R::id::pop_box);
    cur_showpage=nullptr;
    cur_show_poppage=nullptr;
    page_close_message.what=MSG_PAGE_DELAY_CLOSE;
    page_recycle_list.clear();
}


void wind_mgr::Show_Page(pagetype type, bool isNewPage , int mode , int position )
{
    //判断当前页面是否已经在显示
    if(cur_showpage&&cur_showpage->Get_pagetype()==type)
    {
        //true,直接返回
        return;
    }
    //false,去page_recycle_list中查找
    //定义是否启用循环回收模式
#if defined(USE_DELAY_RECYCLE_)&&(USE_DELAY_RECYCLE_)
    //取消掉消息处理looper中要移除页面的事件，防止影响去回收队列中查找复用页面
    Looper::getMainLooper()->removeMessages(this,MSG_PAGE_DELAY_CLOSE);
    if(cur_showpage)
    {
        //将其加入循环回收队列
        cur_showpage->Stop_Page_action();
        cur_showpage->RemovePage();
        page_recycle_list.push_back(cur_showpage);
    }

    for(auto it=page_recycle_list.begin();it!=page_recycle_list.end();it++)
    {
        //找到，直接添加页面，直接复用
        if((*it)->Get_pagetype()==type)
        {
            LOGE("----------position:%d",position);
            (*it)->AddPage();
            (*it)->Start_Page_action(type,position);
            cur_showpage=(*it);
            page_recycle_list.erase(it);
            Looper::getMainLooper()->sendMessageDelayed(200,this,page_close_message);
            return;

        }   
    }
    Looper::getMainLooper()->sendMessageDelayed(200,this,page_close_message);
  
#endif
    //没找到，或者不启用回收列表时构造展示新页面
    PageBase*tmp=nullptr;
    switch (type)
    {
    case PAGE_HOME_:
        tmp=new PageHome(root_page);
        break;
    case PAGE_STEAM_BARBECUE_:
        tmp=new PageSteamBarbecue(root_page,mode);
        break;
    case PAGE_WATER_MODE_LAYOUT_:
        tmp=new PageWaterMode(root_page);
        break;
    case PAGE_WATER_MODIFY_:
        tmp=new PageWaterModify(root_page,PAGE_WATER_MODIFY_,position);
        break;
    case PAGE_WATER_ADD_:
        tmp=new PageWaterModify(root_page,PAGE_WATER_ADD_,position);
    default:
        LOGE("无跳转页面");
        break;
    }
    cur_showpage=tmp;
    
    
}

void wind_mgr::Show_Pop_Page(Poptype type, pop_callback enterCb, pop_callback cancelCb, int position)
{
    if(cur_show_poppage)
    {
        LOGE("当前弹出窗口已存在");
        return;
    }
    cur_show_poppage=new PopPage(root_pop_page,type,enterCb,cancelCb,position);
    root_page->setVisibility(View::INVISIBLE);
    root_pop_page->setVisibility(View::VISIBLE);
}

void wind_mgr::Deal_Close_Pop_Page()
{
    root_pop_page->setVisibility(View::INVISIBLE);
    root_page->setVisibility(View::VISIBLE);
    delete cur_show_poppage;
    cur_show_poppage=nullptr;
}

void wind_mgr::handleMessage(Message &message)
{
    switch (message.what)
    {
    case MSG_PAGE_DELAY_CLOSE:
        for(auto it=page_recycle_list.begin();it!=page_recycle_list.end();it++)
        {
            (*it)->Stop_Page_action();
            (*it)->RemovePage();
        }
        break;
    
    default:
        break;
    }
}
