#include "conf_mgr.h"
#include "json_func.h"
#include "waterModeDataLocal.h"

CConfMgr *g_objConf = nullptr;

CConfMgr::CConfMgr()
{
    g_objConf = this;
    mUpdates = 0;
}

int CConfMgr::init()
{
    mDataFileName = "/home/huangpeng/cdroid/apps/demo1/src/data/waterModeData.json";
    bool flag = loadLocalJson(mDataFileName, data);
    LOGE("--------flag=%d", flag);

    if (!flag)
    {
        convertStringToJson(__modeDataLocalJson__, data);
        std::ofstream file(mDataFileName);
        if(file)
        {
            LOGE("文件创建成功!!!!!!!files yes yes yes");
            saveLocalJson(mDataFileName, data);
            file.close();
        } else {
            LOGE("文件创建失败~~~~~~~files no no no");
        }
    }

    // 数据处理
    if (data.isArray())
    {
        for (int i = 0; i < data.size(); i++)
        {
            ModeData modeData;
            Json::Value jsonModeData = data[i];
            modeData.dataPos = getJsonInt(jsonModeData, "dataPos");
            modeData.dataType = getJsonString(jsonModeData, "dataType");
            modeData.imageDir = getJsonString(jsonModeData, "imageDir");
            modeData.modeName = getJsonString(jsonModeData, "modeName");
            modeData.temperature = getJsonString(jsonModeData, "temperature");
            modeData.chuShuiLiang = getJsonInt(jsonModeData, "chuShuiLiang");
            modeData.canDelete = getJsonInt(jsonModeData, "canDelete");

            modeDataList.emplace_back(modeData);
        }
    }

    std::sort(modeDataList.begin(), modeDataList.end(), compareByPos);

    // if (!data["addNew"].empty() && data["addNew"].isArray())
    // {
    //     for (int i = 0; i < data["addNew"].size(); i++)
    //     {
    //         ModeData modeData;
    //         Json::Value jsonModeData = data["addNew"][i];
    //         modeData.imageDir = getJsonString(jsonModeData, "imageDir");
    //         modeData.modeName = getJsonString(jsonModeData, "modeName");
    //         modeData.temperature = getJsonString(jsonModeData, "temperature");
    //         modeData.chuShuiLiang = getJsonInt(jsonModeData, "chuShuiLiang");
    //         modeData.canDelete = getJsonInt(jsonModeData, "canDelete");

    //         modeDataAddNew.emplace_back(modeData);
    //     }
    // }

    // if (!modeDataAddNew.empty())
    // {
    //     modeDataList = modeDataDefault;
    //     modeDataList.insert(modeDataList.end(), modeDataAddNew.begin(), modeDataAddNew.end());
    // }
    // else
    // {
    //     modeDataList = modeDataDefault;
    // }

    return 0;
}

bool CConfMgr::compareByPos(const ModeData& a, const ModeData& b) {
    return a.dataPos < b.dataPos; // 按pos升序排序
}

void CConfMgr::updateModeDataByPos(ModeData modeData, int position)
{
    if (position == -1)
    {
        modeData.dataPos = modeDataList.size() + 1; //可能会改成modeData.dataPos = modeDataList.size()
        // LOGE("updateModeDataByPos   changeModeData = %s", modeData.toString().c_str());
        modeDataList.emplace_back(modeData);
        // modeDataAddNew.emplace_back(modeData);
    }
    else
    {
        modeDataList[position] = modeData;
        // if (position < 3 && position >= 0)
        // {
        //     modeDataDefault[position] = modeData;
        // }
        // else
        // {
        //     modeDataAddNew[position - 3] = modeData;
        // }
    }
    std::string modeDataJsonArray = "[";
    for (int i = 0; i < modeDataList.size(); i++)
    {
        modeDataJsonArray += modeDataList[i].toString();
        if (i != modeDataList.size() - 1)
        {
            modeDataJsonArray += ",";
        }
    }
    // modeDataJsonArray += "],\"addNew\":[";
    // for (int i = 0; i < modeDataAddNew.size(); i++)
    // {
    //     modeDataJsonArray += modeDataAddNew[i].toString();
    //     if (i != modeDataAddNew.size() - 1)
    //     {
    //         modeDataJsonArray += ",";
    //     }
    // }
    modeDataJsonArray += "]";
    // LOGE("-------------modeDataJsonArray------updateModeDataByPos---------------------%s", modeDataJsonArray.c_str());

    Json::Value root;
    convertStringToJson(modeDataJsonArray, root);
    saveLocalJson("/home/huangpeng/cdroid/apps/demo1/src/data/waterModeData.json", root);
}

void CConfMgr::deleteModeData(int position)
{
    modeDataList.erase(modeDataList.begin() + position);
    // modeDataAddNew.erase(modeDataAddNew.begin() + position - 3);
    std::string modeDataJsonArray = "[";
    for (int i = 0; i < modeDataList.size(); i++)
    {
        if (i >= position)
        {
            // LOGE("modeDataList pos pre = %d", modeDataList[i].dataPos);
            modeDataList[i].dataPos -= 1;
            // LOGE("modeDataList pos post = %d", modeDataList[i].dataPos);
        }
        
        modeDataJsonArray += modeDataList[i].toString();
        if (i != modeDataList.size() - 1)
        {
            modeDataJsonArray += ",";
        }
    }
    // modeDataJsonArray += "],\"addNew\":[";
    // for (int i = 0; i < modeDataAddNew.size(); i++)
    // {
    //     modeDataJsonArray += modeDataAddNew[i].toString();
    //     if (i != modeDataAddNew.size() - 1)
    //     {
    //         modeDataJsonArray += ",";
    //     }
    // }
    modeDataJsonArray += "]";
    // LOGE("-------------modeDataJsonArray----------deleteModeData-----------------%s", modeDataJsonArray.c_str());

    Json::Value root;
    convertStringToJson(modeDataJsonArray, root);
    saveLocalJson("/home/huangpeng/cdroid/apps/demo1/src/data/waterModeData.json", root);
}

void CConfMgr::movedMode(int fromPos, int toPos)
{
    std::swap(modeDataList[fromPos], modeDataList[toPos]);
    std::swap(modeDataList[fromPos].dataPos, modeDataList[toPos].dataPos);
    std::string modeDataJsonArray = "[";
    for (int i = 0; i < modeDataList.size(); i++)
    {
        modeDataJsonArray += modeDataList[i].toString();
        if (i != modeDataList.size() - 1)
        {
            modeDataJsonArray += ",";
        }
    }
   
    modeDataJsonArray += "]";
    // LOGE("-------------modeDataJsonArray---------movedMode------------------%s", modeDataJsonArray.c_str());

    Json::Value root;
    convertStringToJson(modeDataJsonArray, root);
    saveLocalJson("/home/huangpeng/cdroid/apps/demo1/src/data/waterModeData.json", root);
}

void CConfMgr::update() {}

// 首次初始化
// bool CConfMgr::isInitSet() {}
void CConfMgr::setInitSetFlag() {}

// 回复出厂设置
void CConfMgr::reset() {}