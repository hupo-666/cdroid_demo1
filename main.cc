#include <cdroid.h>
#include <cdlog.h>
#include "R.h"
#include"pagebase.h"
#include"wind_mgr.h"
#include"conf_mgr.h"

int main(int argc, const char* argv[]) {
    App app(argc, argv);
    cdroid::Context* ctx = &app;

    Main_Wind_Mgr->Init_Window();
    Main_Wind_Mgr->Show_Page(PAGE_HOME_);
    
    d_fileLoad->init();

    return app.exec();
}

