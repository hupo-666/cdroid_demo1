#ifndef WIND_MGR_H
#define WIND_MGR_H 

#include<cdroid.h>
#include<cdlog.h>
#include"pagebase.h"
#include"poppage.h"
#include<widget/cdwindow.h>


#define Main_Wind_Mgr wind_mgr::getInstance() 

class wind_mgr:public MessageHandler
{
    public:
        #define USE_DELAY_RECYCLE_ 1
        static wind_mgr* getInstance()
        {
            static wind_mgr instance;
            return &instance;

        } 
        void Init_Window();
        void Show_Page(pagetype type, bool isNewPage = true, int mode = 1, int position = -1);
        void Show_Pop_Page( Poptype type, pop_callback enterCb, pop_callback cancelCb, int position);
        void Deal_Close_Pop_Page();
        void handleMessage(Message& message) override;
        ViewGroup* Get_Root_Poppage() {return root_pop_page;}
        
    protected:
        enum
        {
            MSG_PAGE_DELAY_CLOSE, // 延迟关闭页面
        };

    private:
        std::vector<PageBase*> page_recycle_list;
        Window* main_window;
        ViewGroup* root_layout;         //链接main_window，page和pop_page都是建立在这个layout下
        ViewGroup* root_page;           //page要显示的地方
        ViewGroup* root_pop_page;       //pop_page要显示的地方
        PageBase* cur_showpage;        //指向当前显示的page
        PopPage* cur_show_poppage;     //指向当前显示的pop page
        Message page_close_message;      //关闭页面信息
        

        wind_mgr(){};
        wind_mgr(const wind_mgr&) = delete;
        wind_mgr& operator=(const wind_mgr&) = delete;
        

};

#endif // !WIND_MGR_H

