#ifndef PAGE_HOME_H_
#define PAGE_HOME_H_
#include"pagebase.h"
#include"wind_mgr.h"
#include"my_viewpager_adapter_up.h"
#include"my_viewpager_adapter_down.h"

class PageHome:public PageBase
{

    public:
        PageHome(ViewGroup*root_layout);
        ~PageHome();

    private:
        TextView* bt_fresh_steam;
        TextView* bt_juicy_barbecue;
        TextView* bt_fried;
        TextView* bt_boil_baked;
        TextView* bt_cloud_recipe;
        ViewPager* view_pager_up;
        ViewPager* view_pager_down;
        My_Viewpager_Adapter_Up* adapter_up;
        My_Viewpager_Adapter_Down* adapter_down;

};

#endif // !PAGE_HOM_E_H
