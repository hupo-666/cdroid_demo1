#ifndef RECYCLERVIEW_ADAPTER_H_
#define RECYCLERVIEW_ADAPTER_H_
#include<cdlog.h>
#include<iostream>
#include"R.h"
#include"conf_mgr.h"
#include"wind_mgr.h"




class MyRecylerViewAdapter:public RecyclerView::Adapter,public RecyclerView::AdapterDataObserver
{
    private:
        std::vector<ModeData> mode_data_list;
        std::vector<ViewGroup*> mode_data_layout;
        RecyclerView* mode_data_recyclerview;

    public:
        MyRecylerViewAdapter();
        ~MyRecylerViewAdapter(){mode_data_list.clear();}
        void setModeDataList(std::vector<ModeData> data) { mode_data_list = data; };

        class MyViewHolder:public RecyclerView::ViewHolder
        {
            public:
                ViewGroup* item_whole_layout;
                ViewGroup* up_layout;  //整个上部分的layout，为了便于点击直接进入模式修改界面
                TextView* mode_no;
                TextView* mode_name;
                TextView* mode_temperature_num;
                TextView* mode_chushuiliang_num;
                ImageView* mode_image;
                ImageView* pic_down_left;
                ImageView* pic_down_right;
                ViewGroup* down_left;
                ViewGroup* down_delete;
                ViewGroup* down_right;
            
            MyViewHolder(View* itemView):RecyclerView::ViewHolder(itemView)
            {
                item_whole_layout=(ViewGroup*)itemView;
                up_layout=(ViewGroup*)itemView->findViewById(demo1::R::id::up);
                mode_no=(TextView*)itemView->findViewById(demo1::R::id::mode_no);
                mode_name=(TextView*)itemView->findViewById(demo1::R::id::mode_name);
                mode_temperature_num=(TextView*)itemView->findViewById(demo1::R::id::mode_temp_num);
                mode_chushuiliang_num=(TextView*)itemView->findViewById(demo1::R::id::mode_chu_shui_liang_num);
                mode_image=(ImageView*)itemView->findViewById(demo1::R::id::mode_pic);
                pic_down_left=(ImageView*)itemView->findViewById(demo1::R::id::left);
                pic_down_right=(ImageView*)itemView->findViewById(demo1::R::id::right);
                down_left=(ViewGroup*)itemView->findViewById(demo1::R::id::down_left);
                down_delete=(ViewGroup*)itemView->findViewById(demo1::R::id::down_delete);
                down_right=(ViewGroup*)itemView->findViewById(demo1::R::id::down_right);

            }

        };

        RecyclerView::ViewHolder*onCreateViewHolder(ViewGroup* parent, int viewType)override;
        void onBindViewHolder(RecyclerView::ViewHolder& holder, int position)override;

        int getItemCount()override{return mode_data_list.size();}
        long getItemId(int position)override{
            return position;
        }

};




#endif // !RECYCLERVIEW_ADAPTER_H_
