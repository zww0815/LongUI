﻿#include <Control/UISlider.h>
#include <Core/luiManager.h>
#include <algorithm>

/// <summary>
/// Render_chain_backgrounds this instance.
/// UISlider 背景渲染
/// </summary>
/// <returns></returns>
void LongUI::UISlider::render_chain_background() const noexcept {
    Super::render_chain_background();
    // 默认背景?
    if (!m_bDefaultBK) return;
    // 默认背景?
    {
        constexpr float SLIDER_TRACK_BORDER_WIDTH = 1.f;
        constexpr float SLIDER_TRACK_WIDTH = 3.f;
        constexpr UINT SLIDER_TRACK_BORDER_COLOR = 0xD6D6D6;
        constexpr UINT SLIDER_TRACK_COLOR = 0xE7EAEA;
        D2D1_RECT_F border_rect; this->GetViewRect(border_rect);
        // 垂直滑块
        if (this->IsVerticalSlider()) {
            auto half = this->thumb_size.height * 0.5f;
            border_rect.left = (border_rect.left + border_rect.right) * 0.5f -
                SLIDER_TRACK_BORDER_WIDTH - SLIDER_TRACK_WIDTH * 0.5f;
            border_rect.right = border_rect.left +
                SLIDER_TRACK_BORDER_WIDTH * 2.f + SLIDER_TRACK_WIDTH;
            border_rect.top += half;
            border_rect.bottom -= half;
        }
        // 水平滑块
        else {
            auto half = this->thumb_size.width * 0.5f;
            border_rect.left += half;
            border_rect.right -= half;
            border_rect.top = (border_rect.top + border_rect.bottom) * 0.5f -
                SLIDER_TRACK_BORDER_WIDTH - SLIDER_TRACK_WIDTH * 0.5f;
            border_rect.bottom = border_rect.top +
                SLIDER_TRACK_BORDER_WIDTH * 2.f + SLIDER_TRACK_WIDTH;
        }
        // 渲染滑槽边框
        m_pBrush_SetBeforeUse->SetColor(D2D1::ColorF(SLIDER_TRACK_BORDER_COLOR));
        UIManager_RenderTarget->FillRectangle(&border_rect, m_pBrush_SetBeforeUse);
        // 渲染滑槽
        m_pBrush_SetBeforeUse->SetColor(D2D1::ColorF(SLIDER_TRACK_COLOR));
        border_rect.left += SLIDER_TRACK_BORDER_WIDTH;
        border_rect.top += SLIDER_TRACK_BORDER_WIDTH;
        border_rect.right -= SLIDER_TRACK_BORDER_WIDTH;
        border_rect.bottom -= SLIDER_TRACK_BORDER_WIDTH;
        UIManager_RenderTarget->FillRectangle(&border_rect, m_pBrush_SetBeforeUse);
    }
}

/// <summary>
/// Render_chain_foregrounds this instance.
/// UISlider 前景
/// </summary>
/// <returns></returns>
void LongUI::UISlider::render_chain_foreground() const noexcept {
    // 边框
    m_uiElement.Render(m_rcThumb);
    {
        constexpr float THUMB_BORDER_WIDTH = 1.f;
        D2D1_RECT_F thumb_border = {
            m_rcThumb.left + THUMB_BORDER_WIDTH * 0.5f,
            m_rcThumb.top + THUMB_BORDER_WIDTH * 0.5f,
            m_rcThumb.right - THUMB_BORDER_WIDTH * 0.5f,
            m_rcThumb.bottom - THUMB_BORDER_WIDTH * 0.5f,
        };
        m_pBrush_SetBeforeUse->SetColor(&m_colorBorderNow);
        UIManager_RenderTarget->DrawRectangle(&thumb_border, m_pBrush_SetBeforeUse);
    }
    // 父类
    Super::render_chain_foreground();
}


/// <summary>
/// Renders this instance.
/// 渲染 
/// </summary>
/// <returns></returns>
void LongUI::UISlider::Render() const noexcept {
    // 背景渲染
    this->render_chain_background();
    // 主景渲染
    this->render_chain_main();
    // 前景渲染
    this->render_chain_foreground();
}


/// <summary>
/// Updates this instance.
/// UI滑动条: 刷新
/// </summary>
/// <returns></returns>
void LongUI::UISlider::Update() noexcept {
    // 更新计时器
    m_uiElement.Update();
    // 垂直滑块
    if (this->IsVerticalSlider()) {
        // 根据 value 计算滑块位置
        m_rcThumb.left = (this->view_size.width - this->thumb_size.width) *0.5f;
        m_rcThumb.right = m_rcThumb.left + this->thumb_size.width;
        {
            auto slider_height = this->view_size.height - this->thumb_size.height;
            m_rcThumb.top = slider_height * m_fValue;
        }
        m_rcThumb.bottom = m_rcThumb.top + this->thumb_size.height;
    }
    // 水平滑块
    else {
        // 根据 value 计算滑块位置
        m_rcThumb.top = (this->view_size.height - this->thumb_size.height) *0.5f;
        m_rcThumb.bottom = m_rcThumb.top + this->thumb_size.height;
        {
            auto slider_width = this->view_size.width - this->thumb_size.width;
            m_rcThumb.left = slider_width * m_fValue;
        }
        m_rcThumb.right = m_rcThumb.left + this->thumb_size.width;
    }
    // 父类刷新
    return Super::Update();
}


// UISlider 构造函数
void LongUI::UISlider::initialize() noexcept {
    Super::initialize();
    assert(!"notimpl");
}

/// <summary>
/// Sets the value01.
/// </summary>
/// <param name="value">The value.</param>
/// <returns></returns>
void LongUI::UISlider::SetValue01(float value) noexcept {
#ifdef _DEBUG
    if (!(value >= 0.f && value <= 1.f)) {
        UIManager << DL_Hint
            << L"Out of Range:"
            << value
            << LongUI::endl;
    }
#endif
    auto old = m_fValue;
    m_fValue = std::min(std::max(value, 0.f), 1.f);
    if (old != m_fValue) {
        this->CallUiEvent(m_event, SubEvent::Event_ValueChanged);
    }
    this->InvalidateThis(); 
}

// UISlider 初始化
void LongUI::UISlider::initialize(pugi::xml_node node) noexcept {
    assert(node && "call UISlider::initialize() if no xml");
    // 链式调用
    Super::initialize(node);
    m_uiElement.Init(this->check_state(), 0, node);
    // 可被设为焦点
    auto flag = this->flags | Flag_Focusable;
    // 设置
    {
        const char* str = nullptr;
        // 起始值
        if ((str = node.attribute("start").value())) {
            m_fStart = LongUI::AtoF(str);
        }
        // 终止值
        if ((str = node.attribute("end").value())) {
            m_fEnd = LongUI::AtoF(str);
        }
        // 当前值
        if ((str = node.attribute("value").value())) {
            this->SetValueSE(LongUI::AtoF(str));
        }
        // 步进值
        if ((str = node.attribute("step").value())) {
            m_fStep = LongUI::AtoF(str);
        }
        // 滑块大小
        Helper::MakeFloats(
            node.attribute("thumbsize").value(),
            force_cast(thumb_size)
        );
        // 默认背景
        m_bDefaultBK = node.attribute("defaultbk").as_bool(true);
    }
    // 修改flag
    force_cast(this->flags) = flag;
}


/// <summary>
/// Creates the UISlider control.
/// </summary>
/// <param name="type">The type.</param>
/// <param name="node">The node.</param>
/// <returns></returns>
auto LongUI::UISlider::CreateControl(CreateEventType type, pugi::xml_node node) noexcept ->UIControl* {
    UISlider* pControl = nullptr;
    switch (type)
    {
    case LongUI::Type_Initialize:
        break;
    case LongUI::Type_Recreate:
        break;
    case LongUI::Type_Uninitialize:
        break;
    case_LongUI__Type_CreateControl:
        LongUI__CreateWidthCET(UISlider, pControl, type, node);
    }
    return pControl;
}


// do event 事件处理
bool LongUI::UISlider::DoEvent(const LongUI::EventArgument& arg) noexcept {
    // longui 消息
    switch (arg.event)
    {
        float value_old;
    case LongUI::Event::Event_SetEnabled:
        // 修改状态
        m_uiElement.SetBasicState(arg.ste.enabled ? State_Normal : State_Disabled);
        return true;
    case LongUI::Event::Event_SetFloat:
        // 修改浮点数据
        value_old = m_fValue;
        this->SetValueSE(arg.stf.value);
        if (value_old != m_fValue) {
            this->CallUiEvent(m_event, SubEvent::Event_ValueChanged);
        }
        return true;
    case LongUI::Event::Event_GetFloat:
        // 获取浮点数据
        arg.fvalue = this->GetValueSE();
        return true;
    case LongUI::Event::Event_KeyDown:
        value_old = m_fValue;
        // 左上按键
        if (arg.key.ch == UIInput.KB_LEFT || arg.key.ch == UIInput.KB_UP) {
            this->SetValueSE(this->GetValueSE() - m_fStep);
        }
        // 右下按键
        else if (arg.key.ch == UIInput.KB_RIGHT || arg.key.ch == UIInput.KB_DOWN) {
            this->SetValueSE(this->GetValueSE() + m_fStep);
        }
        // 修改事件
        if (value_old != m_fValue) {
            this->CallUiEvent(m_event, SubEvent::Event_ValueChanged);
        }
        return true;
    }
    return Super::DoEvent(arg);
}


// 鼠标事件
bool LongUI::UISlider::DoMouseEvent(const MouseEventArgument& arg) noexcept {
    // 禁用状态禁用鼠标消息
    if (!this->GetEnabled()) return true;
    // 坐标转换
    D2D1_POINT_2F pt4self = LongUI::TransformPointInverse(
        this->world, D2D1_POINT_2F{arg.ptx, arg.pty}
        );
    bool nocontinued = false;
    const auto value_old = m_fValue;
    // 修改值
    auto changed_value = [this, pt4self](float pos) noexcept {
        // 获取基本值
        if (this->IsVerticalSlider()) {
            auto slider_height = this->view_size.height - this->thumb_size.height;
            m_fValue = (pt4self.y - pos) / slider_height;
        }
        else {
            auto slider_width = this->view_size.width - this->thumb_size.width;
            m_fValue = (pt4self.x - pos) / slider_width;
        }
        // 阈值检查
        m_fValue = std::min(std::max(m_fValue, 0.f), 1.f);
    };
    // 一般移动
    auto normal_moving = [this, pt4self]() noexcept {
        // 移到Thumb上方
        if (IsPointInRect(m_rcThumb, pt4self)) {
            // 鼠标移进:
            if (!m_bMouseMoveIn) {
                // 设置UI元素状态
                this->SetControlState(LongUI::State_Hover);
                m_bMouseMoveIn = true;
            }
        }
        else {
            // 鼠标移出:
            if (m_bMouseMoveIn) {
                // 设置UI元素状态
                this->SetControlState(LongUI::State_Normal);
                m_bMouseMoveIn = false;
            }
        }
    };
    // 分类
    switch (arg.event)
    {
    case LongUI::MouseEvent::Event_MouseLeave:
        // 鼠标移出: 设置UI元素状态
        this->SetControlState(LongUI::State_Normal);
        m_bMouseClickIn = false;
        m_bMouseMoveIn = false;
        nocontinued = true;
        break;
    case  LongUI::MouseEvent::Event_MouseMove:
        // 点中并且移动
        if (UIInput.IsMbPressed(UIInput.MB_L)) {
            // 修改
            if (m_bMouseClickIn) changed_value(m_fClickPosition);
        }
        // 一般移动
        else normal_moving();
        nocontinued = true;
        break;
    case  LongUI::MouseEvent::Event_LButtonDown:
        // 左键按下
        m_pWindow->SetCapture(this);
        // 点中thumb
        if (IsPointInRect(m_rcThumb, pt4self)) {
            m_fClickPosition = this->IsVerticalSlider() ?
                (pt4self.y - m_rcThumb.top) : (pt4self.x - m_rcThumb.left);
        }
        // 直接移动
        else {
            float offset = this->IsVerticalSlider() ?
                (m_rcThumb.bottom - m_rcThumb.top) : (m_rcThumb.right - m_rcThumb.left);
            changed_value(offset * 0.5f);
        }
        m_bMouseClickIn = true;
        nocontinued = true;
        this->SetControlState(LongUI::State_Pushed);
        break;
    case LongUI::MouseEvent::Event_LButtonUp:
        // 右键按下
        m_bMouseClickIn = false;
        m_pWindow->ReleaseCapture();
        this->SetControlState(LongUI::State_Hover);
        nocontinued = true;
        break;
    case LongUI::MouseEvent::Event_MouseWheelV:
    case LongUI::MouseEvent::Event_MouseWheelH:
        m_fValue -= arg.wheel.delta * 0.1f;
        // 阈值检查
        m_fValue = std::min(std::max(m_fValue, 0.f), 1.f);
        nocontinued = true;
        break;
    }
    // 检查事件
    if (value_old != m_fValue) {
        // 调用
        this->CallUiEvent(m_event, SubEvent::Event_ValueChanged);
        // 刷新
        this->InvalidateThis();
    }
    return nocontinued;
}

// recreate 重建
auto LongUI::UISlider::Recreate() noexcept ->HRESULT {
    m_uiElement.Recreate();
    return Super::Recreate();
}

// 添加事件监听器(雾)
bool LongUI::UISlider::uniface_addevent(SubEvent sb, UICallBack&& call) noexcept {
    if (sb == SubEvent::Event_ValueChanged) {
        m_event += std::move(call);
        return true;
    }
    return Super::uniface_addevent(sb, std::move(call));
}

// close this control 关闭控件
void LongUI::UISlider::cleanup() noexcept {
    // 删前调用
    this->before_deleted();
    // 删除对象
    delete this;
}
