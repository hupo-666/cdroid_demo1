#include "pagebase.h"

PageBase::PageBase(ViewGroup *mWindPageBox, const std::string resource, pagetype type)
:m_mWindPageBox(mWindPageBox),m_pagetype(type)
{
    m_layout=nullptr;
    left_time=nullptr;
    wifi=nullptr;
    lock=nullptr;
    light_bulb=nullptr;
    set=nullptr;
    InitPageBase(resource);
}

PageBase::~PageBase()
{
    delete m_layout;
}

void PageBase::AddPage()
{
    LOGE("----m_layout---:%d",m_layout);
    m_mWindPageBox->addView(m_layout);
}

void PageBase::RemovePage()
{
    m_layout->removeCallbacks(m_Timer_cb);
    left_time->stop();
    LOGE("----m_layout---:%d",m_layout);
    m_mWindPageBox->removeView(m_layout);
}

void PageBase::Start_Page_action(pagetype type,int position)
{
    left_time->start();
    OnTimer();
}

void PageBase::Stop_Page_action()
{
    m_layout->removeCallbacks(m_Timer_cb);
}

void PageBase::InitPageBase(const std::string resource)
{
    m_layout=(ViewGroup*)LayoutInflater::from(m_mWindPageBox->getContext())->inflate(resource,nullptr);
    AddPage();
    if(m_layout!=nullptr)
    {
        left_time=(TimeTextView*)m_layout->findViewById(demo1::R::id::leftedge_time);
        wifi=(ImageView*)m_layout->findViewById(demo1::R::id::leftedge_wifi);
        lock=(ImageView*)m_layout->findViewById(demo1::R::id::leftedge_lock);
        light_bulb=(ImageView*)m_layout->findViewById(demo1::R::id::leftedge_light);
        set=(ImageView*)m_layout->findViewById(demo1::R::id::leftedge_set);
    }

    left_time->start();
    m_Timer_Time=1000;
    m_Timer_cb=std::bind(&PageBase::OnTimer,this);
    OnTimer();
}

void PageBase::OnTimer()
{
    left_time->setText(getTimeStr());
    m_layout->postDelayed(m_Timer_cb,m_Timer_Time);
}
