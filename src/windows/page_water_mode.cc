#include "page_water_mode.h"

PageWaterMode::PageWaterMode(ViewGroup *root_layout):PageBase(root_layout,"@layout/page_water_mode_layout",PAGE_WATER_MODE_LAYOUT_)
{
    //获取组件ID
    bt_back_home=(ImageView*)m_layout->findViewById(demo1::R::id::bt_back_in_wm_layout);
    bt_home=(ImageView*)m_layout->findViewById(demo1::R::id::bt_icon_home_water_mode);
    bt_mode_add=(ViewGroup*)m_layout->findViewById(demo1::R::id::bt_mode_add);
    rcyc_water_mode=(RecyclerView*)m_layout->findViewById(demo1::R::id::recyclerview_water_picture_layout);
    slide_bar=(ViewGroup*)m_layout->findViewById(demo1::R::id::main_line);

    //实现返回功能
    bt_back_home->setOnClickListener([this](View&){
        Main_Wind_Mgr->Show_Page(PAGE_HOME_);
    });

    
    bt_home->setOnClickListener([this](View&){
        Main_Wind_Mgr->Show_Page(PAGE_HOME_);
    });

    //实现新增功能
    bt_mode_add->setOnClickListener([this](View& v){
        Main_Wind_Mgr->Show_Page(PAGE_WATER_ADD_);

    });



    //实现RecyclerView

    //设置布局管理器用于处理滚动和回收机制，默认为垂直
    layout_manager=new LinearLayoutManager(m_layout->getContext(),LinearLayout::HORIZONTAL,false);
    rcyc_water_mode->setLayoutManager(layout_manager);

    //开启预加载当前显示页面相邻页面的功能，使得页面更顺畅
    rcyc_water_mode->getLayoutManager()->setItemPrefetchEnabled(true);
    m_rcy_adpter=new MyRecylerViewAdapter();
    rcyc_water_mode->setAdapter(m_rcy_adpter);
    
    //设置Viewholder缓存池的大小
    rcyc_water_mode->getRecycledViewPool().setMaxRecycledViews(0,10);

    rcyc_water_mode->setItemAnimator(nullptr);

    setRecyclerViewScrollLine(rcyc_water_mode,slide_bar,LinearLayout::HORIZONTAL);


}

PageWaterMode::~PageWaterMode()
{
    delete  layout_manager;
    delete  m_rcy_adpter;
}

void PageWaterMode::Start_Page_action(pagetype type, int position)
{
    left_time->start();
    OnTimer();
    m_rcy_adpter->setModeDataList(d_fileLoad->getModeDataList());
    m_rcy_adpter->notifyDataSetChanged();
}
