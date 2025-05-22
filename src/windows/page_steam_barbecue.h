#ifndef PAGE_STEAM_BARBECUE_H_
#define PAGE_STEAM_BARBECUE_H_
#include"pagebase.h"
#include"wind_mgr.h"

class PageSteamBarbecue:public PageBase
{
private:
    NumberPicker* numpicker_cook_mode_choice;
    NumberPicker* numpicker_temperature;
    NumberPicker* numpicker_time_hour;
    NumberPicker* numpicker_time_min;
    NumberPicker* numpicker_wei_bo_fu_zhu;
    NumberPicker* numpicker_zheng_qi_fu_zhu;
    NumberPicker* numpicker_baking_mode;
    ImageView* bt_home;
    TextView* bt_start;
    TextView* bt_appointment;

public:
    PageSteamBarbecue(ViewGroup*root_layout,int mode);
    ~PageSteamBarbecue();
    void Start_Page_action(pagetype type,int position=-1)override;
};


#endif // !PAGE_STEAM_BARBECUE_H_
