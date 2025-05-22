#include "poppage.h"
#include"conf_mgr.h"
#include"wind_mgr.h"

PopPage::PopPage(ViewGroup *wind_pop_box, int type, pop_callback enterCb, pop_callback cancelCb, int position):
m_WindPopBox(wind_pop_box),m_Type(type),m_EnterCallback(enterCb),m_CancelCallback(cancelCb),m_Position(position)
{
    m_PopLayout=(ViewGroup*)LayoutInflater::from(m_WindPopBox->getContext())->inflate("@layout/mode_delete_fix",nullptr);
    m_WindPopBox->addView(m_PopLayout);

    tmp_modedata=d_fileLoad->getModeDataList()[position];
    m_PopModeName=(TextView*)m_PopLayout->findViewById(demo1::R::id::mode_name_in_mdf);
    m_PopModeTemp = (TextView *)m_PopLayout->findViewById(demo1::R::id::temp_num_in_mdf);
    m_PopModeChuShui = (TextView *)m_PopLayout->findViewById(demo1::R::id::chu_shui_liang_num);
    m_PopSubmit = (TextView *)m_PopLayout->findViewById(demo1::R::id::mode_delete_fix_sure);
    m_PopCancel = (TextView *)m_PopLayout->findViewById(demo1::R::id::mode_delete_fix_quxiao);

    m_PopModeName->setText(tmp_modedata.modeName);
    m_PopModeTemp->setText(tmp_modedata.temperature);
    m_PopModeChuShui->setText(std::to_string(tmp_modedata.chuShuiLiang));

    m_PopLayout->setOnClickListener(std::bind(&PopPage::on_Btn_ClickListener,this,std::placeholders::_1)); //防止点击穿透
    m_PopSubmit->setOnClickListener(std::bind(&PopPage::on_Btn_ClickListener,this,std::placeholders::_1));
    m_PopCancel->setOnClickListener(std::bind(&PopPage::on_Btn_ClickListener,this,std::placeholders::_1));
}

PopPage::~PopPage()
{
    if (m_PopLayout != nullptr) {
        m_WindPopBox->removeView(m_PopLayout);
        delete m_PopLayout;
        m_PopLayout = nullptr; // 避免悬空指针
    }

}

void PopPage::on_Btn_ClickListener(View &v)
{
    switch (v.getId())
    {
    case demo1::R::id::mode_delete_fix_sure :
        if(m_EnterCallback)
        {
            m_EnterCallback();
        }
        Main_Wind_Mgr->Deal_Close_Pop_Page();
        break;

    case demo1::R::id::mode_delete_fix_quxiao:
        if(m_CancelCallback)
        {
            m_CancelCallback();
        }
        Main_Wind_Mgr->Deal_Close_Pop_Page();
        break;
    
    default:
        break;
    }
}
