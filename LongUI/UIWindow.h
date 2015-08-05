﻿#pragma once
/**
* Copyright (c) 2014-2015 dustpg   mailto:dustpg@gmail.com
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


// LongUI namespace
namespace LongUI{
    // ui's window
    class UIWindow : public UIVerticalLayout, public Helper::ComStatic<
        Helper::QiListSelf<IUnknown, Helper::QiList<IDropTarget>>> {
        // 父类申明
        using Super = UIVerticalLayout ;
        // 友元申明
        friend class CUIManager;
        // message id for TaskbarBtnCreated
        static const UINT s_uTaskbarBtnCreatedMsg;
    public: // UIControl 接口实现
        // Render 渲染 
        virtual void Render(RenderType type) const noexcept override;
        // udate 刷新
        virtual void Update() noexcept override;
        // do event 事件处理
        virtual bool DoEvent(const LongUI::EventArgument&) noexcept override;
        // recreate 重建
        virtual auto Recreate(LongUIRenderTarget*) noexcept ->HRESULT override;
        // close this control 关闭控件
        virtual void Cleanup() noexcept override;
    protected:
        // constructor
        UIWindow(pugi::xml_node node, UIWindow* parent) noexcept;
        // destructor
        ~UIWindow() noexcept;
        // deleted 
        UIWindow(const UIWindow&) = delete; UIWindow() = delete;
    public: // some new
        // on close event
        virtual auto OnClose() noexcept -> bool { this->Cleanup(); return true; };
    public: // IDropTarget interface
        // impl for IDropTarget::DragEnter
        HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj,DWORD grfKeyState, POINTL pt,DWORD *pdwEffect) noexcept override;
        // impl for IDropTarget::DragOver
        HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) noexcept override;
        // impl for IDropTarget::DragLeave
        HRESULT STDMETHODCALLTYPE DragLeave(void) noexcept override;
        // impl for IDropTarget::Drop
        HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) noexcept override;
    public:
        // begin render
        void BeginDraw() const noexcept;
        // end ender
        void EndDraw() const noexcept;
        // wait for VS
        void WaitVS() const noexcept;
        // register for calling PreRender
        void RegisterOffScreenRender(UIControl* c, bool is3d) noexcept;
        // unregister for calling PreRender
        void UnRegisterOffScreenRender(UIControl*) noexcept;
        // set the caret
        void SetCaretPos(UIControl* c, float _x, float _y) noexcept;
        // create the caret
        void CreateCaret(float width, float height) noexcept;
        // show the caret
        void ShowCaret() noexcept;
        // hide the caret
        void HideCaret() noexcept;
        // get control by CUIString
        auto FindControl(const CUIString&) noexcept->UIControl*;
        // get control by wchar_t pointer
        auto FindControl(const wchar_t* name) noexcept { CUIString n(name); return this->FindControl(n); }
        // find control where mouse pointed
        auto FindControl(const D2D1_POINT_2F pt) noexcept { return static_cast<UIContainer*>(this)->FindControl(pt); }
        // add control with name
        void AddControl(const std::pair<CUIString, void*>& pair) noexcept;
    public: // 内联区
        // register for calling PreRender with 3d content
        LongUIInline auto RegisterOffScreenRender3D(UIControl* c) noexcept { return this->RegisterOffScreenRender(c, true); }
        // register for calling PreRender with 2d content
        LongUIInline auto RegisterOffScreenRender2D(UIControl* c) noexcept { return this->RegisterOffScreenRender(c, false); }
        // start render in sec.
        LongUIInline auto StartRender(float t, UIControl* c) noexcept { return m_uiRenderQueue.PlanToRender(0.f, t, c); }
        // plan to render in sec.
        LongUIInline auto PlanToRender(float w, float t, UIControl* c) noexcept { return m_uiRenderQueue.PlanToRender(w, t, c); }
        // update control later
        LongUIInline auto Invalidate(UIControl* c) noexcept { return m_uiRenderQueue.PlanToRender(0.f, 0.f, c); }
        // get delta time in second
        LongUIInline auto GetDeltaTime() const noexcept { return m_fDeltaTime > 0.04f ? 0.f : m_fDeltaTime; };
        // get window handle
        LongUIInline auto GetHwnd() const noexcept { return m_hwnd;};
        // set mouse capture
        LongUIInline auto SetCapture(UIControl* c) noexcept { ::SetCapture(m_hwnd); m_pCapturedControl = c; };
        // release mouse capture
        LongUIInline auto ReleaseCapture() noexcept { ::ReleaseCapture(); m_pCapturedControl = nullptr; };
        // is release mouse capture
        LongUIInline auto IsReleasedControl(UIControl* c) noexcept { return m_pCapturedControl == c; };
        // get back buffer
        LongUIInline auto GetBackBuffer() noexcept { return ::SafeAcquire(m_pTargetBimtap); }
        // is rendered
        LongUIInline auto IsRendered() const noexcept { return m_bRendered; }
        // render window
        LongUIInline auto RenderWindow() const noexcept { this->BeginDraw(); this->Render(RenderType::Type_Render); return this->EndDraw(); }
        // next frame
        LongUIInline auto NextFrame() noexcept { m_uiRenderQueue.GetCurrentUnit()->length = 0; ++m_uiRenderQueue; }
        // post update message
        LongUIInline auto PostUpdateMessage() noexcept { ::PostMessageW(m_hwnd, LongUIUpdateMessage, 0, 0); }
    private:
        // release data
        void release_data() noexcept;
        // draw the caret
        void draw_caret() noexcept;
        // refresh the caret
        void refresh_caret() noexcept;
        // set the present
        void set_present_parameters(DXGI_PRESENT_PARAMETERS&)const noexcept;
    public: // MSG MAPPING ZONE
        // on WM_CREATE
        bool OnCreated(HWND hwnd) noexcept;
        // on WM_MOUSEMOVE
        bool OnMouseMove(const LongUI::EventArgument&) noexcept;
        // on WM_MOUSEWHEEL
        bool OnMouseWheel(const LongUI::EventArgument&) noexcept;
        // resize window
        void OnResize(bool force = false) noexcept;
    protected:
        // will use BitArray instead of them
        // BitArray32           m_baBool;
        // caret in(true) or out?
        bool                    m_bCaretIn = false;
        // in draging?
        bool                    m_bInDraging = false;
        // window rendered in last time, or want to render in this time
        bool                    m_bRendered = false;
        // new size?
        bool                    m_bNewSize = false;
        // full rendering in this frame?
        bool                    m_bFullRendering = false;
        // unused
        bool                    m_unused[1];
        // text anti-mode
        uint16_t                m_textAntiMode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
        // mini size
        D2D1_SIZE_U             m_miniSize = D2D1::SizeU();
        // window handle
        HWND                    m_hwnd = nullptr;
        // current data object
        IDataObject*            m_pCurDataObject = nullptr;
        // task bar list
        ITaskbarList4*          m_pTaskBarList = nullptr;
        // helper for drop target
        IDropTargetHelper*      m_pDropTargetHelper = nullptr;
        // wait for vsync
        HANDLE                  m_hVSync = nullptr;
        // swap chain
        IDXGISwapChain2*        m_pSwapChain = nullptr;
        // target bitmap
        ID2D1Bitmap1*           m_pTargetBimtap = nullptr;
        // Direct Composition Device
        IDCompositionDevice*    m_pDcompDevice = nullptr;
        // Direct Composition Target
        IDCompositionTarget*    m_pDcompTarget = nullptr;
        // Direct Composition Visual
        IDCompositionVisual*    m_pDcompVisual = nullptr;
        // mouse point control (only one)
        UIControl*              m_pPointedControl = nullptr;
        // now focused control (only one)
        UIControl*              m_pFocusedControl = nullptr;
        // now dragdrop control (only one)
        UIControl*              m_pDragDropControl = nullptr;
        // now captured control (only one)
        UIControl*              m_pCapturedControl = nullptr;
        // blink timer id
        UINT_PTR                m_idBlinkTimer = 0;
        // normal  l-param
        LPARAM                  m_normalLParam = 0;
        // delta time [in sec.]
        float                   m_fDeltaTime = 0.f;
        // show the caret
        uint32_t                m_cShowCaret = 0;
        // caret rect in px
        RectLTWH_U              m_rcCaretPx;
        // timer
        UITimer                 m_timer;
    public:
        // the real pixel size  of window(HWND)
        D2D1_SIZE_U     const   window_size = D2D1::SizeU();
        // default(arrow)cursor
        HCURSOR         const   default_cursor = ::LoadCursor(nullptr, IDC_ARROW);
        // now sursor
        HCURSOR                 now_cursor = default_cursor;
        // debug info
#ifdef _DEBUG
        uint32_t                test_D2DERR_RECREATE_TARGET = false;
        uint32_t                test_unknown = 0;
        uint32_t                full_render_counter = 0;
        uint32_t                dirty_render_counter = 0;
#endif
        // clear color @xml "clearcolor"
        D2D1::ColorF            clear_color = D2D1::ColorF(D2D1::ColorF::White);
    protected: // huge obj
        // rendering queue
        CUIRenderQueue          m_uiRenderQueue;
        // now unit
        CUIRenderQueue::UNIT    m_aUnitNow;
        // dirty rects : +1 for caret
        RECT                    m_dirtyRects[LongUIDirtyControlSize + 1];
        // track mouse event: end with DWORD
        TRACKMOUSEEVENT         m_csTME;
        // current STGMEDIUM: begin with DWORD
        STGMEDIUM               m_curMedium;
        // registered control
        BasicContainer          m_vRegisteredControl;
        // control name ->map-> control pointer
        StringMap               m_mapString2Control;
#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
    };
#ifdef LongUIDebugEvent
    // 重载?特例化 GetIID
    template<> LongUIInline const IID& GetIID<LongUI::UIWindow>() {
        // {64F7B3E5-621E-4864-9535-7E6A29F670C1}
        static const GUID IID_LongUI_UIWindow = { 
            0x64f7b3e5, 0x621e, 0x4864,{ 0x95, 0x35, 0x7e, 0x6a, 0x29, 0xf6, 0x70, 0xc1 } 
        };
        return IID_LongUI_UIWindow;
    }
#endif
}