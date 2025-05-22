#include "my_viewpager_adapter_down.h"

My_Viewpager_Adapter_Down::My_Viewpager_Adapter_Down()
{
    DataList.emplace_back(0,nullptr);
    DataList.emplace_back(1,nullptr);
}

int My_Viewpager_Adapter_Down::getCount()
{
    return DataList.size();
}

void *My_Viewpager_Adapter_Down::instantiateItem(ViewGroup *container, int position)
{
    ViewGroup* tmp=nullptr;
    switch (position)
    {
    case 0:
        tmp=(ViewGroup*)LayoutInflater::from(container->getContext())->inflate("@layout/view_pager_down_1",nullptr);
        break;
    case 1:
        tmp=(ViewGroup*)LayoutInflater::from(container->getContext())->inflate("@layout/view_pager_down_2",nullptr);
        break;
    default:
        break;
    }
    if(DataList[position].viewpager_data_layout==nullptr)
    {
        DataList[position].viewpager_data_layout=tmp;
    }
    container->addView(tmp);
    return tmp;
}

void My_Viewpager_Adapter_Down::destroyItem(ViewGroup *container, int position, void *object)
{
    container->removeView((View*)object);
    delete (ViewGroup*)object;
}

bool My_Viewpager_Adapter_Down::isViewFromObject(View *view, void *object)
{
    return view==object;
}

std::string My_Viewpager_Adapter_Down::getPageTitle(int position)
{
    return "";
}

float My_Viewpager_Adapter_Down::getPageWidth(int position)
{
    return 1.0f;
}
