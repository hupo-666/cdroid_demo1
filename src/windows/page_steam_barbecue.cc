#include "page_steam_barbecue.h"

PageSteamBarbecue::PageSteamBarbecue(ViewGroup *root_layout, int mode):PageBase(root_layout,"@layout/steam_barbecue_layout",PAGE_STEAM_BARBECUE_)
{
    //home按键
    bt_home=(ImageView*)m_layout->findViewById(demo1::R::id::bt_home_in_sb_layout);
    bt_home->setOnClickListener([this](View& v){
        Main_Wind_Mgr->Show_Page(PAGE_HOME_);
    });

    //开始和预约按键
    bt_start=(TextView*)m_layout->findViewById(demo1::R::id::bt_start_in_sb_layout);
    bt_appointment=(TextView*)m_layout->findViewById(demo1::R::id::bt_appointment_in_sb_layout);

    //设置Numberpicker

    //Numberpicker烹饪模式
    std::vector<std::string> catalogList;
    catalogList.push_back("鲜蒸");
    catalogList.push_back("嫩烤");
    catalogList.push_back("香炸");
    catalogList.push_back("炖焗");
    numpicker_cook_mode_choice=(NumberPicker*)m_layout->findViewById(demo1::R::id::cook_mode_choice);
    
    numpicker_cook_mode_choice->setFormatter([this,catalogList](int index)->std::string{
        if(index>=0&&index<4)
        {
            return catalogList[index];
        }
        else
        {
            return "超出设置范围";
        }
       
    });

    numpicker_cook_mode_choice->setValue(mode);


    //Numberpicker温度
    numpicker_temperature=(NumberPicker*)m_layout->findViewById(demo1::R::id::numberpicker_in_display_temperature);
    numpicker_temperature->setFormatter([this](int value)->std::string{
        return std::to_string(value);
    });
    numpicker_temperature->setValue(120);

    //Numberpicker小时
    numpicker_time_hour=(NumberPicker*)m_layout->findViewById(demo1::R::id::numberpicker_in_display_time_hour);
    numpicker_time_hour->setFormatter([this](int value)->std::string{
        if(value<10)
        {
            return std::to_string(value).insert(0,1,'0');
        }
        else
        {
            return std::to_string(value);
        }
    });
    numpicker_temperature->setValue(0);

    //Numberpicker分钟
    numpicker_time_min=(NumberPicker*)m_layout->findViewById(demo1::R::id::numberpicker_in_display_time_min);
    numpicker_time_min->setFormatter([this](int value)->std::string{
        if(value<10)
        {
            return std::to_string(value).insert(0,1,'0');
        }
        else
        {
            return std::to_string(value);
        }
    });
    numpicker_time_min->setValue(1);

    //Numberpicker微波辅助
    numpicker_wei_bo_fu_zhu=(NumberPicker*)m_layout->findViewById(demo1::R::id::np_wei_bo_fu_zhu);
    std::vector<std::string> weiboLevel;
    weiboLevel.push_back("低");
    weiboLevel.push_back("中");
    weiboLevel.push_back("高");

    numpicker_wei_bo_fu_zhu->setFormatter([this,weiboLevel](int index)->std::string{
        if(index>=0&&index<3)
        {
            return weiboLevel[index];
        }
        else
        {
            return "超出设置范围";
        }
    });
    numpicker_wei_bo_fu_zhu->setValue(1);

    //Numberpicker蒸汽辅助
    numpicker_zheng_qi_fu_zhu=(NumberPicker*)m_layout->findViewById(demo1::R::id::np_zheng_qi_fu_zhu);
    std::vector<std::string> zhengqiLevel;
    zhengqiLevel.push_back("低温");
    zhengqiLevel.push_back("中温");
    zhengqiLevel.push_back("高温");
    numpicker_zheng_qi_fu_zhu->setFormatter([this,zhengqiLevel](int index)->std::string{
        if(index>=0&&index<3)
        {
            return zhengqiLevel[index];
        }
        else
        {
            return "超出设置范围";
        }
    });
    numpicker_zheng_qi_fu_zhu->setValue(1);

    //Numberpicker烘焙模式
    numpicker_baking_mode=(NumberPicker*)m_layout->findViewById(demo1::R::id::bt_baking_mode_in_sb_layout);
    std::vector<std::string> bakingMode;
    bakingMode.push_back("3D热风");
    bakingMode.push_back("上下烤");
    bakingMode.push_back("独立烘焙");

    numpicker_baking_mode->setFormatter([this,bakingMode](int index)->std::string{
        if(index>=0&&index<3)
        {
            return bakingMode[index];
        }
        else
        {
            return "超出设置范围";
        }
    });

    numpicker_baking_mode->setValue(1);

    ViewGroup* relativegroup=(ViewGroup*)m_layout->findViewById(demo1::R::id::main_dislpay_layout_in_sb_layout);
    ((BitmapDrawable*)((LayerDrawable*)relativegroup->getBackground())->getDrawable(5))->setFilterBitmap(true);
    ((BitmapDrawable*)((LayerDrawable*)relativegroup->getBackground())->getDrawable(8))->setFilterBitmap(true);


    

}

PageSteamBarbecue::~PageSteamBarbecue()
{
}

void PageSteamBarbecue::Start_Page_action(pagetype type, int position)
{
    numpicker_cook_mode_choice->setValue(position);
}
