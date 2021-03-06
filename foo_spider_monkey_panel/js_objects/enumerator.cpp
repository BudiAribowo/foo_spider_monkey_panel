#include <stdafx.h>

#include "enumerator.h"

#include <com_objects/dispatch_ptr.h>
#include <convert/com.h>
#include <js_engine/js_to_native_invoker.h>
#include <js_objects/active_x_object.h>
#include <js_utils/js_error_helper.h>
#include <js_utils/js_object_helper.h>
#include <utils/array_x.h>

#include <qwr/winapi_error_helpers.h>

// TODO: add font caching

namespace
{

using namespace mozjs;

JSClassOps jsOps = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    JsEnumerator::FinalizeJsObject,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

JSClass jsClass = {
    "Enumerator",
    JSCLASS_HAS_PRIVATE | JSCLASS_FOREGROUND_FINALIZE, // COM objects must be finalized in foreground
    &jsOps
};

MJS_DEFINE_JS_FN_FROM_NATIVE( atEnd, JsEnumerator::AtEnd )
MJS_DEFINE_JS_FN_FROM_NATIVE( item, JsEnumerator::Item )
MJS_DEFINE_JS_FN_FROM_NATIVE( moveFirst, JsEnumerator::MoveFirst )
MJS_DEFINE_JS_FN_FROM_NATIVE( moveNext, JsEnumerator::MoveNext )

constexpr auto jsFunctions = smp::to_array<JSFunctionSpec>(
    {
        JS_FN( "atEnd", atEnd, 0, kDefaultPropsFlags ),
        JS_FN( "item", item, 0, kDefaultPropsFlags ),
        JS_FN( "moveFirst", moveFirst, 0, kDefaultPropsFlags ),
        JS_FN( "moveNext", moveNext, 0, kDefaultPropsFlags ),
        JS_FS_END,
    } );

constexpr auto jsProperties = smp::to_array<JSPropertySpec>(
    {
        JS_PS_END,
    } );

MJS_DEFINE_JS_FN_FROM_NATIVE( Enumerator_Constructor, JsEnumerator::Constructor )

} // namespace

namespace mozjs
{

const JSClass JsEnumerator::JsClass = jsClass;
const JSFunctionSpec* JsEnumerator::JsFunctions = jsFunctions.data();
const JSPropertySpec* JsEnumerator::JsProperties = jsProperties.data();
const JsPrototypeId JsEnumerator::PrototypeId = JsPrototypeId::Enumerator;
const JSNative JsEnumerator::JsConstructor = ::Enumerator_Constructor;

JsEnumerator::JsEnumerator( JSContext* cx, EnumVARIANTComPtr pEnum )
    : pJsCtx_( cx )
    , pEnum_( pEnum )
{
}

std::unique_ptr<JsEnumerator>
JsEnumerator::CreateNative( JSContext* cx, IUnknown* pUnknown )
{
    assert( pUnknown );

    CDispatchPtr pCollection( pUnknown );
    EnumVARIANTComPtr pEnum( pCollection.Get( static_cast<DISPID>( DISPID_NEWENUM ) ) );

    auto pNative = std::unique_ptr<JsEnumerator>( new JsEnumerator( cx, pEnum ) );
    pNative->LoadCurrentElement();

    return pNative;
}

size_t JsEnumerator::GetInternalSize( IUnknown* /*pUnknown*/ )
{
    return 0;
}

JSObject* JsEnumerator::Constructor( JSContext* cx, ActiveXObject* pActiveXObject )
{
    return JsEnumerator::CreateJs( cx, ( pActiveXObject->pUnknown_ ? pActiveXObject->pUnknown_ : pActiveXObject->pDispatch_ ) );
}

bool JsEnumerator::AtEnd()
{
    return isAtEnd_;
}

JS::Value JsEnumerator::Item()
{
    if ( isAtEnd_ )
    {
        return JS::UndefinedValue();
    }

    JS::RootedValue jsValue( pJsCtx_ );
    convert::com::VariantToJs( pJsCtx_, curElem_, &jsValue );

    return jsValue;
}

void JsEnumerator::MoveFirst()
{
    HRESULT hr = pEnum_->Reset();
    qwr::error::CheckHR( hr, "Reset" );

    LoadCurrentElement();
}

void JsEnumerator::MoveNext()
{
    LoadCurrentElement();
}

void JsEnumerator::LoadCurrentElement()
{
    HRESULT hr = pEnum_->Next( 1, curElem_.GetAddress(), nullptr );
    if ( S_FALSE == hr )
    { // means that we've reached the end
        curElem_.Clear();
        isAtEnd_ = true;
    }
    else
    {
        qwr::error::CheckHR( hr, "Next" );
        isAtEnd_ = false;
    }
}

} // namespace mozjs
