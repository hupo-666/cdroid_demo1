#include "RecyclerViewAdapter.h"

MyRecylerViewAdapter::MyRecylerViewAdapter()
{
    mode_data_list=d_fileLoad->getModeDataList();
    for (int i = 0; i < mode_data_list.size(); i++)
    {
        LOGE("modeDataList = %s", mode_data_list[i].toString().c_str());
    }
}


RecyclerView::ViewHolder *MyRecylerViewAdapter::onCreateViewHolder(ViewGroup *parent, int viewType)
{
    ViewGroup* itemView=(ViewGroup*)LayoutInflater::from(parent->getContext())->inflate("@layout/page_water_mode",nullptr);
    if (itemView == nullptr)
    {
        LOGE("itemView = nullptr");
    }
    return new MyViewHolder(itemView);
}

void MyRecylerViewAdapter::onBindViewHolder(RecyclerView::ViewHolder &holder, int position)
{
    ModeData tmp_data=mode_data_list[position];
    MyViewHolder* tmp_holder=(MyViewHolder*) (&holder);
    //设置控键要显示的值
    if(tmp_data.dataPos>=0&&tmp_data.dataPos<10)
    {
        LOGE("-----position:%d,datapos:%d",position,tmp_data.dataPos);
        tmp_holder->mode_no->setText(std::to_string(tmp_data.dataPos).insert(0,1,'0'));
    }else
    {
        tmp_holder->mode_no->setText(std::to_string(tmp_data.dataPos));
    }
    tmp_holder->mode_name->setText(tmp_data.modeName);
    tmp_holder->mode_temperature_num->setText(tmp_data.temperature+std::string("℃"));
    tmp_holder->mode_chushuiliang_num->setText(std::to_string(tmp_data.chuShuiLiang)+ std::string("mL"));
    tmp_holder->mode_image->setImageResource(tmp_data.imageDir);


    //左右改变位置按键
    if(position==0)
    {
        tmp_holder->pic_down_left->setImageResource("@mipmap/icon_noleft");
        ((MarginLayoutParams*)tmp_holder->item_whole_layout->getLayoutParams())->setMarginStart(48);
        ((MarginLayoutParams*)tmp_holder->item_whole_layout->getLayoutParams())->setMarginEnd(0);
    }else if(position==mode_data_list.size()-1)
    {
        tmp_holder->pic_down_right->setImageResource("@mipmap/icon_noright");
        ((MarginLayoutParams*)tmp_holder->item_whole_layout->getLayoutParams())->setMarginStart(20);
        ((MarginLayoutParams*)tmp_holder->item_whole_layout->getLayoutParams())->setMarginEnd(52);
    }else
    {
        ((MarginLayoutParams*)tmp_holder->item_whole_layout->getLayoutParams())->setMarginStart(20);
        ((MarginLayoutParams*)tmp_holder->item_whole_layout->getLayoutParams())->setMarginEnd(0);
        tmp_holder->pic_down_left->setImageResource("@mipmap/icon_left");
        tmp_holder->pic_down_right->setImageResource("@mipmap/icon_right");
    }

    tmp_holder->down_left->setOnClickListener([this,position,tmp_holder](View& v){
        if (position>0)
        {
            tmp_holder->pic_down_left->setImageResource("@mipmap/icon_left");
            tmp_holder->pic_down_right->setImageResource("@mipmap/icon_right");
            d_fileLoad->movedMode(position,position-1);
            mode_data_list=d_fileLoad->getModeDataList();
            notifyItemMoved(position,position-1);
            notifyDataSetChanged();
        }
        else
        {
            tmp_holder->pic_down_left->setImageResource("@mipmap/icon_noleft");
            Toast::makeText(tmp_holder->item_whole_layout->getContext(),"无法再向左移动");
        }
    });

    tmp_holder->down_right->setOnClickListener([this,position,tmp_holder](View& v){

        if(position<mode_data_list.size()-1)
        {
            tmp_holder->pic_down_left->setImageResource("@mipmap/icon_left");
            tmp_holder->pic_down_right->setImageResource("@mipmap/icon_right");
            d_fileLoad->movedMode(position,position+1);
            mode_data_list=d_fileLoad->getModeDataList();
            notifyItemMoved(position,position+1);
            notifyDataSetChanged();
        }
        else
        {
            tmp_holder->pic_down_right->setImageResource("@mipmap/icon_noright");
            Toast::makeText(tmp_holder->item_whole_layout->getContext(),"无法再向右移动");
        }

    });


    //删除按键

    if(tmp_data.canDelete==0)
    {
        tmp_holder->down_delete->setVisibility(View::GONE);
    }else
    {
        tmp_holder->down_delete->setVisibility(View::VISIBLE);
        tmp_holder->down_delete->setOnClickListener([this,position](View& v){
            
            Main_Wind_Mgr->Show_Pop_Page(POP_MODE_DELETE,[this,position]{
                d_fileLoad->deleteModeData(position);
                mode_data_list= d_fileLoad->getModeDataList();
                notifyDataSetChanged();
            },nullptr,position);
            
        });
    }

    //设置点击Item,进入修改模式
    tmp_holder->up_layout->setOnClickListener([this,position](View& v){

        //Main_Wind_Mgr->Show_Page("@layout/page_water_mode_modify")
        LOGE("----------position:%d",position);
        Main_Wind_Mgr->Show_Page(PAGE_WATER_MODIFY_,true,1,position);
        

    });
  

  


    

    






}
