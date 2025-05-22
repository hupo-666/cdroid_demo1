#ifndef MODE_DATA_H_
#define MODE_DATA_H_
#include<iostream>

class ModeData
{
public:
    
    int dataPos;   //datapos从1开始计数
    int chuShuiLiang;
    int canDelete;
    std::string dataType; //用于标识是否是默认的选项，默认的选项不能删除
    std::string modeName;
    std::string temperature;
    std::string imageDir;
    
    ModeData(){};

    ModeData(int _chushuiliang,bool _delete,int _pos,std::string _datatype,std::string _name
    ,std::string _temp,std::string _imagedir)
    {
        dataPos=_pos;
        chuShuiLiang=_chushuiliang;
        canDelete=_delete;
        dataType=_datatype;
        modeName=_name;
        temperature=_temp;
        imageDir=_imagedir;
    }
    ~ModeData(){}

    std::string toString()
    {
        return "{\"dataPos\":\"" + std::to_string(dataPos) + 
        "\",\"chuShuiLiang\":\"" + std::to_string(chuShuiLiang) + 
        "\",\"canDelete\":\"" + std::to_string(canDelete)+ 
        "\",\"dataType\":\"" + dataType + 
        "\",\"imageDir\":\"" + imageDir + 
        "\",\"modeName\":\"" + modeName + 
        "\",\"temperature\":\"" + temperature + "\"}";
    }
};


#endif // !MODE_DATA_H_




