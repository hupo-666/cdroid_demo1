#ifndef PAGE_WATER_MODE_MODIFY_H
#define PAGE_WATER_MODE_MODIFY_H
#include"pagebase.h"
#include"wind_mgr.h"
#include"conf_mgr.h"
#include"ModeData.h"



const std::vector<std::string> imageDirs= {
    "@mipmap/pic_chushui_changwenshui",
    "@mipmap/pic_chushui_feishui",
    "@mipmap/pic_chushui_fengmishui",
    "@mipmap/pic_chushui_gouji",
    "@mipmap/pic_chushui_huacha",
    "@mipmap/pic_chushui_kafei",
    "@mipmap/pic_chushui_lengshui",
    "@mipmap/pic_chushui_lvcha",
    "@mipmap/pic_chushui_ningmengshui",
    "@mipmap/pic_chushui_paonai",
    "@mipmap/pic_chushui_wulong"
};

class PageWaterModify:public PageBase
{
    private:
        ImageView* bt_back;
        ImageView* bt_back_home;
        ImageView* bt_minus_temp;
        ImageView* bt_add_temp;
        ImageView* bt_minus_chushuiliang;
        ImageView* bt_add_chushuiliang;
        ImageView* bt_delete;
        ImageView* bt_change_water_pic;
        EditText* display_name;
        EditText* display_temp;
        EditText* display_chushuiliang;
        TextView* display_different_mode;  //在增加和设置页面中需要显示不同内容
        TextView* bt_confirm; 
        ImageView* display_water_picture; 
        ModeData tmp_modedata;        //用于最终显示各参数的值
        ModeData tmp_if_SET_modedata; //用于临时存放改变参数的值
        

    public:
        PageWaterModify(ViewGroup*root_layout,pagetype type,int position);
        ~PageWaterModify();
        void SetAddMinusButton(ImageView*bt,EditText*data,int mode,int offset,int item);//mode=0为减，1为加,item为0表示温度，为1表示出水量
        std::string ExtractDigits(const std::string& input);
        void Start_Page_action(pagetype type,int position=-1)override;
    };





#endif // !PAGE_WATER_MODE_MODIFY_
