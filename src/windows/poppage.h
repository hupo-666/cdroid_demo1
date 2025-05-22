#ifndef PAGE_POP_H_
#define PAGE_POP_H_
#include<iostream>
#include"pagebase.h"
#include"ModeData.h"


enum Poptype       //便于为多复杂弹出页面做拓展
{
    POP_MODE_DELETE
};

typedef std::function<void()> pop_callback;


class PopPage
{
    public:
        PopPage(ViewGroup *wind_pop_box, int type, pop_callback enterCb, pop_callback cancelCb, int position);
        ~PopPage();
        void on_Btn_ClickListener(View& v);
        
    protected:
        ViewGroup *m_WindPopBox;   //Poppage要显示的位置
        ViewGroup *m_PopLayout;    //当前的Poppage布局

        TextView *m_PopModeName;
        TextView *m_PopModeTemp;
        TextView *m_PopModeChuShui;

        TextView *m_PopSubmit;
        TextView *m_PopCancel;

        ModeData tmp_modedata;         //存放数据

        int m_Position;
        int m_Type;
        pop_callback m_EnterCallback;
        pop_callback m_CancelCallback;


};

#endif // !PAGE_POP_H_
