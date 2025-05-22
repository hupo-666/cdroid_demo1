#ifndef __conf_mgr_h__
#define __conf_mgr_h__

#include <cdroid.h>
#include "ModeData.h"
#include <json/json.h>

#define d_fileLoad CConfMgr::ins()

class CConfMgr
{
private:
    int mUpdates;
    Preferences mDataPrefer;
    std::string mDataFileName;
    // std::string mDataFileNameDefault;
    // std::string mDataFileNameAdd;
    std::vector<ModeData> modeDataDefault;
    std::vector<ModeData> modeDataAddNew;
    std::vector<ModeData> modeDataList;
    Json::Value data;

public:
    CConfMgr();
    static CConfMgr *ins()
    {
        static CConfMgr stIns;
        return &stIns;
    }

    int init();
    void update();

    // 首次初始化
    bool isInitSet();
    void setInitSetFlag();

    // 回复出厂设置
    void reset();

    std::vector<ModeData> getModeDataDefault() { return modeDataDefault; };
    std::vector<ModeData> getModeDataAddNew() { return modeDataAddNew; };
    std::vector<ModeData> getModeDataList() { return modeDataList; };
    void updateModeDataByPos(ModeData modeData, int position);
    void deleteModeData(int position);
    void movedMode(int fromPos, int toPos);
    static bool compareByPos(const ModeData& a, const ModeData& b);
};

#endif