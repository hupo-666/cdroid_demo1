#include "page_water_mode_modify.h"

PageWaterModify::PageWaterModify(ViewGroup *root_layout, pagetype type,int position):PageBase(root_layout,"@layout/page_water_mode_modify",type)
{
//获取页面控键资源
   bt_back=(ImageView*)m_layout->findViewById(demo1::R::id::bt_back_in_wma_layout);
   bt_back_home=(ImageView*)m_layout->findViewById(demo1::R::id::bt_icon_home_wma_layout);
   bt_delete=(ImageView*)m_layout->findViewById(demo1::R::id::bt_icon_delete_wma_layout);
   bt_minus_temp=(ImageView*)m_layout->findViewById(demo1::R::id::bt_minus_temperatur_in_wma);
   bt_add_temp=(ImageView*)m_layout->findViewById(demo1::R::id::bt_add_water_temperature_in_wma);
   bt_minus_chushuiliang=(ImageView*)m_layout->findViewById(demo1::R::id::bt_minus_water_output_in_wma);
   bt_add_chushuiliang=(ImageView*)m_layout->findViewById(demo1::R::id::bt_add_water_output_in_wma);
   display_temp=(EditText*)m_layout->findViewById(demo1::R::id::bt_water_temperature_in_wma);
   display_chushuiliang=(EditText*)m_layout->findViewById(demo1::R::id::bt_water_output_in_wma);
   display_name=(EditText*)m_layout->findViewById(demo1::R::id::bt_water_name_in_wma);
   display_different_mode=(TextView*)m_layout->findViewById(demo1::R::id::display_different_mode);
   bt_confirm=(TextView*)m_layout->findViewById(demo1::R::id::bt_confirm);
   bt_change_water_pic=(ImageView*)m_layout->findViewById(demo1::R::id::bt_change_water_picture);
   display_water_picture=(ImageView*)m_layout->findViewById(demo1::R::id::display_change_picture);

    
   



    //把两个页面共同的功能实现
   bt_back->setOnClickListener([this](View& v){
        Main_Wind_Mgr->Show_Page(PAGE_WATER_MODE_LAYOUT_);
   });

    

//实现water_modify_layout和water_add_layout各自的功能
   Start_Page_action(type,position);

}   

PageWaterModify::~PageWaterModify()
{
}

void PageWaterModify::SetAddMinusButton(ImageView *bt, EditText*data,int mode, int offset,int item)
{
    
    bt->setOnClickListener([this,mode,offset,data,item](View& v){
        std::string tmp_data=data->getText();
        ExtractDigits(tmp_data);
        int result=std::stoi(tmp_data);
        if(mode==0&&item==0)
        {
            result-=offset;
            tmp_if_SET_modedata.temperature=std::to_string(result);
            tmp_data=std::to_string(result)+std::string("℃");
            data->setText(tmp_data);
        }
        else if(mode==1&&item==0)
        {
            result+=offset;
            tmp_if_SET_modedata.temperature=std::to_string(result);
            tmp_data=std::to_string(result)+std::string("℃");
            data->setText(tmp_data);
        }
        else if (mode==0&&item==1)
        {
            result-=offset;
            tmp_if_SET_modedata.chuShuiLiang=result;
            tmp_data=std::to_string(result)+std::string("mL");
            data->setText(tmp_data);
        }else if (mode==1&&item==1)
        {
            result+=offset;
            tmp_if_SET_modedata.chuShuiLiang=result;
            tmp_data=std::to_string(result)+std::string("mL");
            data->setText(tmp_data);
        }
        
    });
}



std::string PageWaterModify::ExtractDigits(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (std::isdigit(c)) {
            result += c;
        }
    }
    return result;
}

void PageWaterModify::Start_Page_action(pagetype type, int position)
{
    if(type==PAGE_WATER_MODIFY_)
   {
        tmp_modedata=d_fileLoad->getModeDataList()[position];
        tmp_if_SET_modedata=tmp_modedata;

        //将modedata数据展示在页面上
        display_temp->setText(tmp_modedata.temperature+std::string("℃"));
        display_chushuiliang->setText(std::to_string(tmp_modedata.chuShuiLiang)+std::string("mL"));
        display_name->setText(tmp_modedata.modeName);
        display_different_mode->setText("模式设置");
        display_water_picture->setImageResource(tmp_modedata.imageDir);
       

        //将加减按钮功能实现
        SetAddMinusButton(bt_minus_temp,display_temp,0,1,0);
        SetAddMinusButton(bt_add_temp,display_temp,1,1,0);
        SetAddMinusButton(bt_minus_chushuiliang,display_chushuiliang,0,50,1);
        SetAddMinusButton(bt_add_chushuiliang,display_chushuiliang,1,50,1);

        //将切换图片按钮功能实现
        bt_change_water_pic->setOnClickListener([this](View& v){
            auto it=std::find(imageDirs.begin(),imageDirs.end(),tmp_if_SET_modedata.imageDir);
            if(it!=imageDirs.end()){
                if(it==imageDirs.end()-1)
                {
                    it=imageDirs.begin();
                    tmp_if_SET_modedata.imageDir=(*it);
                }else
                {
                    it++;
                    tmp_if_SET_modedata.imageDir=(*it);
                }
                display_water_picture->setImageResource(tmp_if_SET_modedata.imageDir);
            }
        });


        bt_back_home->setOnClickListener([this](View& v){
            Main_Wind_Mgr->Show_Page(PAGE_HOME_);
        });
        bt_delete->setOnClickListener([this,position](View&v){
            //跳转到POP_PAGE
            Main_Wind_Mgr->Show_Pop_Page(POP_MODE_DELETE,[this,position]{
                d_fileLoad->deleteModeData(position);
                Main_Wind_Mgr->Show_Page(PAGE_WATER_MODE_LAYOUT_);
            },nullptr,position);
        });

        bt_confirm->setOnClickListener([this,position](View& v){
            if(tmp_modedata.temperature!=tmp_if_SET_modedata.temperature)
            {
                tmp_modedata.temperature=tmp_if_SET_modedata.temperature;
            }
            if(tmp_modedata.chuShuiLiang!=tmp_if_SET_modedata.chuShuiLiang)
            {
                tmp_modedata.chuShuiLiang=tmp_if_SET_modedata.chuShuiLiang;
            }
            if(tmp_modedata.imageDir!=tmp_if_SET_modedata.imageDir)
            {
                tmp_modedata.imageDir=tmp_if_SET_modedata.imageDir;
            }
            d_fileLoad->updateModeDataByPos(tmp_modedata,position);
            Main_Wind_Mgr->Show_Page(PAGE_WATER_MODE_LAYOUT_);
        });

   }else if(type==PAGE_WATER_ADD_)
   {    
        //隐藏delete键
        bt_delete->setVisibility(View::GONE);
        //设置home按键，并移动位置
        ViewGroup::MarginLayoutParams* layout_Params=(ViewGroup::MarginLayoutParams*)bt_back_home->getLayoutParams();
        layout_Params->setMarginStart(1370);

        //给tmp_data设置默认值
        tmp_modedata.canDelete=1;
        tmp_modedata.chuShuiLiang=500;
        tmp_modedata.dataPos=d_fileLoad->getModeDataList().size();
        tmp_modedata.dataType="addnew";
        tmp_modedata.imageDir="@mipmap/pic_chushui_changwenshui";
        tmp_modedata.modeName="温开水";
        tmp_modedata.temperature="55";
        tmp_if_SET_modedata=tmp_modedata;

        //将modedata数据展示在页面上
        display_temp->setText(tmp_modedata.temperature+std::string("℃"));
        display_chushuiliang->setText(std::to_string(tmp_modedata.chuShuiLiang)+std::string("mL"));
        display_name->setText(tmp_modedata.modeName);
        display_different_mode->setText("新增出水模式");
        display_water_picture->setImageResource(tmp_modedata.imageDir);
        

        //将加减按钮功能实现
        SetAddMinusButton(bt_minus_temp,display_temp,0,1,0);
        SetAddMinusButton(bt_add_temp,display_temp,1,1,0);
        SetAddMinusButton(bt_minus_chushuiliang,display_chushuiliang,0,50,1);
        SetAddMinusButton(bt_add_chushuiliang,display_chushuiliang,1,50,1);

        //将切换图片按钮功能实现
        bt_change_water_pic->setOnClickListener([this](View& v){
            auto it=std::find(imageDirs.begin(),imageDirs.end(),tmp_if_SET_modedata.imageDir);
            if(it!=imageDirs.end()){
                if(it==imageDirs.end()-1)
                {
                    it=imageDirs.begin();
                    tmp_if_SET_modedata.imageDir=(*it);
                }else
                {
                    it++;
                    tmp_if_SET_modedata.imageDir=(*it);
                }
                display_water_picture->setImageResource(tmp_if_SET_modedata.imageDir);
            }
        });
    
        
        bt_back_home->setOnClickListener([this](View& v){
            Main_Wind_Mgr->Show_Page(PAGE_HOME_);
        });
        
        bt_confirm->setOnClickListener([this,position](View& v){
            if(tmp_modedata.temperature!=tmp_if_SET_modedata.temperature)
            {
                tmp_modedata.temperature=tmp_if_SET_modedata.temperature;
            }
            if(tmp_modedata.chuShuiLiang!=tmp_if_SET_modedata.chuShuiLiang)
            {
                tmp_modedata.chuShuiLiang=tmp_if_SET_modedata.chuShuiLiang;
            }
            if(tmp_modedata.imageDir!=tmp_if_SET_modedata.imageDir)
            {
                tmp_modedata.imageDir=tmp_if_SET_modedata.imageDir;
            }
            
            d_fileLoad->updateModeDataByPos(tmp_modedata,position);
            Main_Wind_Mgr->Show_Page(PAGE_WATER_MODE_LAYOUT_);
        });

   }
}
