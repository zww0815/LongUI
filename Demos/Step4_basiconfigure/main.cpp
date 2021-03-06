﻿#define _CRT_SECURE_NO_WARNINGS
#include "LongUI.h"
#include "../Step3_handleeventex/demo.h"
#include "LongUI/luiUiDConf.h"

// longui::demo namespace
LONGUI_NAMESPACE_BEGIN namespace Demo {
    // config
    class MyConfig final : public CUIDefaultConfigure {
        // super class
        using Super = CUIDefaultConfigure;
    public:
        // ctor
        MyConfig() : Super(UIManager) { }
        // add/release
        LONGUI_BASIC_INTERFACE_IMPL;
        // return true, if use cpu rendering
        virtual auto GetConfigureFlag() noexcept ->ConfigureFlag override { 
            auto base = IUIConfigure::Flag_OutputDebugString;
            //auto base = IUIConfigure::Flag_None;
            return base | (cpu_rendering ? IUIConfigure::Flag_RenderByCPU : IUIConfigure::Flag_None); ;
        }
        bool    cpu_rendering = true;
    };
}
LONGUI_NAMESPACE_END

// Entry for App
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow) {
    // every windows desktop app should do this
    ::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    // use OleInitialize to init ole and com
    if (SUCCEEDED(::OleInitialize(nullptr))) {
        // my config
        LongUI::Demo::MyConfig config;
        // -GPU
        if (!std::strcmp("-GPU", lpCmdLine)) config.cpu_rendering = false;
        // init longui manager
        if (SUCCEEDED(UIManager.Initialize(&config))) {
            // my style
            UIManager << DL_Hint << L"Battle Control Online!" << LongUI::endl;
            // create main window, return nullptr for some error
            UIManager.CreateUIWindow<LongUI::Demo::MainViewport>(DEMO_XML)->ShowWindow(nCmdShow);
            // run this app
            UIManager.Run();
            // my style
            UIManager << DL_Hint << L"Battle Control Terminated!" << LongUI::endl;
            // cleanup longui
            UIManager.Uninitialize();
        }
        // cleanup ole and com
        ::OleUninitialize();
    }
    // exit
    return EXIT_SUCCESS;
}
