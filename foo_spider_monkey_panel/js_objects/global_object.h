#pragma once

#pragma warning( push )  
#pragma warning( disable : 4251 ) // dll interface warning
#pragma warning( disable : 4996 ) // C++17 deprecation warning
#include <jsapi.h>
#pragma warning( pop )  

#include <optional>
#include <set>

class js_panel_window;

namespace mozjs
{

class JsContainer;
class GlobalHeapManager;

class JsGlobalObject
{
public:
    static constexpr bool HasProto = false;
    static constexpr bool HasProxy = false;

    static const JSClass& JsClass;

public:
    ~JsGlobalObject();    

    static JSObject* CreateNative( JSContext* cx, JsContainer &parentContainer, js_panel_window& parentPanel );

public:
    void Fail( pfc::string8_fast errorText );

    GlobalHeapManager& GetHeapManager() const;

    void RemoveHeapTracer();

public: // methods
    std::optional<std::nullptr_t> IncludeScript( const pfc::string8_fast& path );

private:
    JsGlobalObject( JSContext* cx, JsContainer &parentContainer, js_panel_window& parentPanel );

private:
    JSContext * pJsCtx_ = nullptr;
    JsContainer &parentContainer_;
    js_panel_window& parentPanel_;

private: // proto
    size_t curProtoSlotIdx_ = 1;

    size_t activeX_protoSlot_ = 0;
    size_t fbMetadbHandle_protoSlot_ = 0;
    size_t fbTitleFormat_protoSlot_ = 0;
    size_t gdiFont_protoSlot_ = 0;

private: // heap
    std::unique_ptr<GlobalHeapManager> heapManager_;
};

}
