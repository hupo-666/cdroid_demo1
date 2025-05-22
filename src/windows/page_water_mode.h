#ifndef PAGE_WATER_MODE_H_
#define PAGE_WATER_MODE_H_
#include"pagebase.h"
#include"wind_mgr.h"
#include"RecyclerViewAdapter.h"
#include"zzCommon.h"



class PageWaterMode:public PageBase
{
private:
    ImageView* bt_back_home;
    ImageView* bt_home;
    ViewGroup* bt_mode_add;
    RecyclerView* rcyc_water_mode;
    ViewGroup* slide_bar;
    LinearLayoutManager* layout_manager;
    MyRecylerViewAdapter* m_rcy_adpter;
public:
    PageWaterMode(ViewGroup*root_layout);
    ~PageWaterMode();
    void Start_Page_action(pagetype type,int position=-1)override;
};







#endif // !PAGE_WATER_MODE_H_#define PAGE_WATER_MODE_H_

