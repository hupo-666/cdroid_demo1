#ifndef Viewpager_adapter_DOWN_H_
#define Viewpager_adapter_DOWN_H_
#include<cdroid.h>
#include<iostream>

class viewpager_down_data
{
    public:
        int pos;
        ViewGroup* viewpager_data_layout;
        viewpager_down_data(int positon,ViewGroup*layout):pos(positon),viewpager_data_layout(layout){};
};



class My_Viewpager_Adapter_Down:public PagerAdapter
{
    
private:
    std::vector<viewpager_down_data> DataList;
public:
    My_Viewpager_Adapter_Down();
    int getCount() override;
    void*instantiateItem(ViewGroup* container, int position) override;
    void destroyItem(ViewGroup* container, int position, void* object)override;
    bool isViewFromObject(View* view, void* object)override;
    std::string getPageTitle(int position) override;
    float getPageWidth(int position) override;

    std::vector<viewpager_down_data> getDataList()
    {
        return DataList;
    }
    
};



#endif // !Viewpager_adapter_UP_H_
