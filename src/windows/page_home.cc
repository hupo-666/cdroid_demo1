#include "page_home.h"

PageHome::PageHome(ViewGroup*root_layout):PageBase(root_layout,"@layout/page_home",PAGE_HOME_)
{
    //获取id
    bt_fresh_steam=(TextView*)m_layout->findViewById(demo1::R::id::bt_fresh_steam);
    bt_juicy_barbecue=(TextView*)m_layout->findViewById(demo1::R::id::bt_juicy_barbecue);
    bt_fried=(TextView*)m_layout->findViewById(demo1::R::id::bt_fried);
    bt_boil_baked=(TextView*)m_layout->findViewById(demo1::R::id::bt_boil_baked);
    bt_cloud_recipe=(TextView*)m_layout->findViewById(demo1::R::id::bt_cloud_recipe);

    //设置点击事件
    bt_fresh_steam->setOnClickListener([this](View&){
        Main_Wind_Mgr->Show_Page(PAGE_STEAM_BARBECUE_,true,0,0);
    });

    bt_juicy_barbecue->setOnClickListener([this](View&){
        Main_Wind_Mgr->Show_Page(PAGE_STEAM_BARBECUE_,true,1,1);
    });

    bt_fried->setOnClickListener([this](View&){
        Main_Wind_Mgr->Show_Page(PAGE_STEAM_BARBECUE_,true,2,2);
    });

    bt_boil_baked->setOnClickListener([this](View&){
        Main_Wind_Mgr->Show_Page(PAGE_STEAM_BARBECUE_,true,3,3);
    });

    bt_cloud_recipe->setOnClickListener([this](View&){
        Main_Wind_Mgr->Show_Page(PAGE_WATER_MODE_LAYOUT_);
    });

    //设置Viewpager

    //1.找组件id
    view_pager_up=(ViewPager*)m_layout->findViewById(demo1::R::id::view_pager_up);
    view_pager_down=(ViewPager*)m_layout->findViewById(demo1::R::id::view_pager_down);
    adapter_up=new My_Viewpager_Adapter_Up();
    adapter_down=new My_Viewpager_Adapter_Down();
    TabLayout* tablayout_up=(TabLayout*)m_layout->findViewById(demo1::R::id::tab_layout_up);
    TabLayout* tablayout_down=(TabLayout*)m_layout->findViewById(demo1::R::id::tab_layout_down);

    //2.设置Viewpager属性
    view_pager_up->setOffscreenPageLimit(adapter_up->getCount()); //设置 ViewPager 左右两侧预加载的页面数量
    view_pager_up->setOverScrollMode(View::OVER_SCROLL_ALWAYS); ////用于设置 ViewPager 的过度滚动模式，也就是当用户滑动到 ViewPager 的边界时的处理方式。
    view_pager_up->setAdapter(adapter_up);
    tablayout_up->setupWithViewPager(view_pager_up);

    view_pager_down->setOffscreenPageLimit(adapter_down->getCount());
    view_pager_down->setOverScrollMode(View::OVER_SCROLL_ALWAYS);
    view_pager_down->setAdapter(adapter_down);
    tablayout_down->setupWithViewPager(view_pager_down);
    
}

PageHome::~PageHome()
{
    delete adapter_up;
    delete adapter_down;
}
