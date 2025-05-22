#include "my_viewpager_adapter_up.h"

My_Viewpager_Adapter_Up::My_Viewpager_Adapter_Up()
{
    DataList.emplace_back(0,nullptr);
    DataList.emplace_back(1,nullptr);
}

int My_Viewpager_Adapter_Up::getCount()
{
    return DataList.size();
}

void *My_Viewpager_Adapter_Up::instantiateItem(ViewGroup *container, int position)
{
    ViewGroup* tmp=nullptr;
    switch (position)
    {
    case 0:
        tmp=(ViewGroup*)LayoutInflater::from(container->getContext())->inflate("@layout/view_pager_up_1",nullptr);
        break;
    case 1:
        tmp=(ViewGroup*)LayoutInflater::from(container->getContext())->inflate("@layout/view_pager_up_2",nullptr);
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

void My_Viewpager_Adapter_Up::destroyItem(ViewGroup *container, int position, void *object)
{
    container->removeView((View*)object);
    delete (ViewGroup*)object;
}

bool My_Viewpager_Adapter_Up::isViewFromObject(View *view, void *object)
{
    return view==object;
}

std::string My_Viewpager_Adapter_Up::getPageTitle(int position)
{
    return "";
}

float My_Viewpager_Adapter_Up::getPageWidth(int position)
{
    return 1.f;
}
