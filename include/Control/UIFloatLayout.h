﻿#pragma once
/**
* Copyright (c) 2014-2016 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#include "UIContainerBuiltIn.h"

// LongUI namespace
namespace LongUI {
    // Floating Layout -- 浮动布局
    class UIFloatLayout : public UIContainerBuiltIn {
        // super class
        using Super = UIContainerBuiltIn;
        // clean this control
        virtual void cleanup() noexcept override;
    public: // UIControl
        // render this
        //virtual auto Render() noexcept ->HRESULT override;
        // update this
        //virtual void AfterUpdate() noexcept override;
        // do event 
        //virtual bool DoEvent(LongUI::EventArgument& arg) noexcept override;
        // recreate
        //virtual auto Recreate() noexcept ->HRESULT override;
    public:
        // refresh layout
        virtual void RefreshLayout() noexcept override final;
    public:
        // create this
        static UIControl* CreateControl(CreateEventType, pugi::xml_node) noexcept;
        // ctor
        UIFloatLayout(UIContainer* cp) noexcept : Super (cp) { }
        // dtor
        ~UIFloatLayout() noexcept = default;
        // no copy ctor
        UIFloatLayout(const UIFloatLayout&) = delete;
    protected:
        // something must do before deleted
        void before_deleted() noexcept { Super::before_deleted(); }
        // init
        void initialize(pugi::xml_node node) noexcept { return Super::initialize(node); }
#ifdef LongUIDebugEvent
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
    };
#ifdef LongUIDebugEvent
    // 重载?特例化 GetIID
    template<> const IID& GetIID<LongUI::UIFloatLayout>() noexcept;
#endif
}
