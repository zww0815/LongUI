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

#include "UIButton.h"

// LongUI namespace
namespace LongUI {
    // list
    class UIList;
    // default combo box control 默认组合框
    class UIComboBox : public UIButton {
        // super class
        using Super = UIButton;
        // clean this control 清除控件
        virtual void cleanup() noexcept override;
        // string list
        using StringList = EzContainer::PointerVector<const wchar_t>;
    public:
        // Render 渲染 
        virtual void Render() const noexcept override;
        // udate 刷新
        //virtual void Update() noexcept override;
        // do event
        //virtual bool DoEvent(const EventArgument& arg) noexcept override;
        // do mouse event
        //virtual bool DoMouseEvent(const MouseEventArgument& arg) noexcept override;
        // recreate 重建
        virtual auto Recreate() noexcept ->HRESULT override;
    protected:
        // something must do before deleted
        void before_deleted() noexcept { Super::before_deleted(); }
        // ui call
        virtual bool uniface_addevent(SubEvent sb, UICallBack&& call) noexcept override;
        // render chain -> background
        void render_chain_background() const noexcept { return Super::render_chain_background(); }
        // render chain -> mainground
        void render_chain_main() const noexcept { return Super::render_chain_main(); }
        // render chain -> foreground
        void render_chain_foreground() const noexcept;
    public:
        // create 创建
        static auto CreateControl(CreateEventType, pugi::xml_node) noexcept ->UIControl*;
        // constructor 构造函数
        UIComboBox(UIContainer* cp) noexcept : Super(cp) { }
        // get state
        auto GetControlState() const noexcept { return m_uiElement.GetNowBasicState(); }
        // set state
        void SetControlState(ControlState state) noexcept;
        // get vector
        const auto&GetVector() const noexcept { return m_vItems; }
        // get vector
        auto GetItemCount() const noexcept { return m_vItems.size(); }
        // get selected index
        auto GetSelectedIndex() const noexcept { return m_indexSelected; }
        // set selected index
        void SetSelectedIndex(uint32_t) noexcept;
        // remove item
        void RemoveItem(uint32_t index) noexcept;
        // push item to string list and item list
        void PushItem(const wchar_t* str) noexcept;
        // push item to string list and item list in utf-8
        void PushItem(const char* str) noexcept;
        // insert item to string list and item list
        void InsertItem(uint32_t index, const wchar_t* str) noexcept;
    protected:
        // sync list
        void sync_list() noexcept;
        // initialize, maybe you want call v-method
        void initialize(pugi::xml_node node) noexcept;
        // destructor 析构函数
        ~UIComboBox() noexcept;
        // deleted function
        UIComboBox(const UIComboBox&) = delete;
    protected:
        // ui callback
        UICallBack                  m_eventChanged;
        // string buffer
        CUIShortStringAllocator     m_strAllocator;
        // strings
        StringList                  m_vItems;
        // selected index
        uint32_t                    m_indexSelected = uint32_t(-1);
        // max display line
        uint16_t                    m_uMaxLine = 8;
        // draw down arrow
        bool                        m_bDrawDownArrow = false;
        // list changed
        bool                        m_bChanged = false;
        // host control to display items
        UIList*                     m_pItemList = nullptr;
#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
    };
#ifdef LongUIDebugEvent
    // 重载?特例化 GetIID
    template<> const IID& GetIID<LongUI::UIComboBox>() noexcept;
#endif
}