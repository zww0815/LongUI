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

#include "UIContainer.h"
#include "../Platless/luiPlEzC.h"
#include "../Graphics/luiGrAnim.h"

// LongUI namespace
namespace LongUI {
    // page-like container, child cannot be fixed width/height
    // State_Self1 : Animation Direction
    class UIPage : public UIContainer {
        // super class
        using Super = UIContainer;
        // clean this control 清除控件
        virtual void cleanup() noexcept override;
    protected:
        // something must do before deleted
        void before_deleted() noexcept;
        // init
        void initialize(pugi::xml_node node) noexcept;
        // ctor
        UIPage(UIContainer* cp) noexcept;
        // dtor
        ~UIPage() noexcept;
    public:
        // render this
        virtual void Render() const noexcept override;
        // update this
        virtual void Update() noexcept override;
        // do event
        virtual bool DoEvent(const LongUI::EventArgument& arg) noexcept override;
        // do mouse event
        //virtual bool DoMouseEvent(const LongUI::MouseEventArgument& arg) noexcept override;
        // recreate this
        virtual auto Recreate() noexcept ->HRESULT override;
        // find child control by mouse point
        virtual auto FindChild(const D2D1_POINT_2F& pt) noexcept ->UIControl* override final;
    public:
        // refresh this layout
        virtual void RefreshLayout() noexcept override final;
        // push back
        virtual void Push(UIControl* child) noexcept override final;
        // remove child
        virtual void Remove(UIControl* child) noexcept override final;
    public:
        void Insert(uint32_t index, UIControl* child) noexcept;
        // display next page
        void DisplayNextPage(uint32_t page) noexcept;
        // display next page
        void DisplayNextPage(UIControl* page) noexcept;
        // get index of display page
        auto GetDisplayPage() const noexcept { m_pNowDisplay; }
        // create 创建
        static auto CreateControl(CreateEventType type, pugi::xml_node) noexcept ->UIControl*;
    private:
        // slide to ->
        auto set_slider_to_left() noexcept { m_state.SetFalse<State_Self1>(); }
        // slide to <-
        auto set_slider_to_right() noexcept { m_state.SetTrue<State_Self1>(); }
        // -->?
        auto is_slide_to_right() const noexcept { return m_state.Test<State_Self1>(); }
    protected:
        // render chain -> background
        void render_chain_background() const noexcept { return Super::render_chain_background(); }
        // render chain -> mainground
        void render_chain_main() const noexcept;
        // render chain -> foreground
        void render_chain_foreground() const noexcept { return Super::render_chain_foreground(); }
    public: // for C++ 11;
        // begin itr
        auto begin() noexcept { return m_vChildren.begin(); }
        // end itr
        auto end() noexcept { return m_vChildren.end(); }
        // const begin itr
        auto begin() const noexcept { return m_vChildren.begin(); }
        // const end itr
        auto end() const noexcept { return m_vChildren.end(); }
#ifdef _DEBUG
    private:
        // debug for_each
        virtual void debug_for_each(const CUIFunction<void(UIControl*)>& call) noexcept override {
            for (auto ctrl : m_vChildren) call(ctrl);
        }
#endif
    protected:
        // now display
        UIControl*                  m_pNowDisplay = nullptr;
        // next display
        UIControl*                  m_pNextDisplay = nullptr;
        // vector
        ControlVector               m_vChildren;
        // animation
        CUIAnimation<float>         m_animation;
#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
    };
    // 重载?特例化 GetIID
    template<> const IID& GetIID<LongUI::UIPage>() noexcept;
#else
};
#endif
}