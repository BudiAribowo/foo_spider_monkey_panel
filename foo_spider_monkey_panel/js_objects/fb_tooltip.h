#pragma once

#include <js_objects/object_base.h>

#include <panel_tooltip_param.h>

#include <optional>
#include <memory>
#include <string>

class JSObject;
struct JSContext;
struct JSClass;

namespace mozjs
{

class JsFbTooltip
    : public JsObjectBase<JsFbTooltip>
{
public:
    static constexpr bool HasProto = true;
    static constexpr bool HasGlobalProto = false;
    static constexpr bool HasProxy = false;
    static constexpr bool HasPostCreate = false;

    static const JSClass JsClass;
    static const JSFunctionSpec* JsFunctions;
    static const JSPropertySpec* JsProperties;
    static const JsPrototypeId PrototypeId;

public:
    ~JsFbTooltip();

    static std::unique_ptr<JsFbTooltip> CreateNative( JSContext* cx, HWND hParentWnd, smp::PanelTooltipParam& p_param_ptr );
    static size_t GetInternalSize( HWND hParentWnd, const smp::PanelTooltipParam& p_param_ptr );

public:
    std::optional<std::nullptr_t> Activate();
    std::optional<std::nullptr_t> Deactivate();
    std::optional<uint32_t> GetDelayTime( uint32_t type );
    std::optional<std::nullptr_t> SetDelayTime( uint32_t type, int32_t time );
    std::optional<std::nullptr_t> SetMaxWidth( uint32_t width );
    std::optional<std::nullptr_t> TrackPosition( int x, int y );

public:
    std::optional<std::wstring> get_Text();
    std::optional<std::nullptr_t> put_Text( const std::wstring& text );
    std::optional<std::nullptr_t> put_TrackActivate( bool activate );

private:
    JsFbTooltip( JSContext* cx, HWND hParentWnd, HWND hTooltipWnd, std::unique_ptr<TOOLINFO> toolInfo, smp::PanelTooltipParam& p_param_ptr );

private:
    JSContext * pJsCtx_ = nullptr;

    HWND hTooltipWnd_;
    HWND hParentWnd_;
    std::wstring tipBuffer_;
    std::unique_ptr<TOOLINFO> toolInfo_;
    smp::PanelTooltipParam& panelTooltipParam_;
};

}
