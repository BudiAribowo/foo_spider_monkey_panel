#include <stdafx.h>

#include "fb_file_info.h"

#include <js_engine/js_to_native_invoker.h>
#include <js_utils/js_error_helper.h>
#include <js_utils/js_object_helper.h>

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
    JsFbFileInfo::FinalizeJsObject,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

JSClass jsClass = {
    "FbFileInfo",
    DefaultClassFlags(),
    &jsOps
};

MJS_DEFINE_JS_FN_FROM_NATIVE( InfoFind, JsFbFileInfo::InfoFind );
MJS_DEFINE_JS_FN_FROM_NATIVE( InfoName, JsFbFileInfo::InfoName );
MJS_DEFINE_JS_FN_FROM_NATIVE( InfoValue, JsFbFileInfo::InfoValue );
MJS_DEFINE_JS_FN_FROM_NATIVE( MetaFind, JsFbFileInfo::MetaFind );
MJS_DEFINE_JS_FN_FROM_NATIVE( MetaName, JsFbFileInfo::MetaName );
MJS_DEFINE_JS_FN_FROM_NATIVE( MetaValue, JsFbFileInfo::MetaValue );
MJS_DEFINE_JS_FN_FROM_NATIVE( MetaValueCount, JsFbFileInfo::MetaValueCount );

const JSFunctionSpec jsFunctions[] = {
    JS_FN( "InfoFind", InfoFind, 1, DefaultPropsFlags() ),
    JS_FN( "InfoName", InfoName, 1, DefaultPropsFlags() ),
    JS_FN( "InfoValue", InfoValue, 1, DefaultPropsFlags() ),
    JS_FN( "MetaFind", MetaFind, 1, DefaultPropsFlags() ),
    JS_FN( "MetaName", MetaName, 1, DefaultPropsFlags() ),
    JS_FN( "MetaValue", MetaValue, 2, DefaultPropsFlags() ),
    JS_FN( "MetaValueCount", MetaValueCount, 1, DefaultPropsFlags() ),
    JS_FS_END
};

MJS_DEFINE_JS_FN_FROM_NATIVE( get_InfoCount, JsFbFileInfo::get_InfoCount );
MJS_DEFINE_JS_FN_FROM_NATIVE( get_MetaCount, JsFbFileInfo::get_MetaCount );

const JSPropertySpec jsProperties[] = {

    JS_PSG( "InfoCount", get_InfoCount, DefaultPropsFlags() ),
    JS_PSG( "MetaCount", get_MetaCount, DefaultPropsFlags() ),
    JS_PS_END
};

} // namespace

namespace mozjs
{

const JSClass JsFbFileInfo::JsClass = jsClass;
const JSFunctionSpec* JsFbFileInfo::JsFunctions = jsFunctions;
const JSPropertySpec* JsFbFileInfo::JsProperties = jsProperties;
const JsPrototypeId JsFbFileInfo::PrototypeId = JsPrototypeId::FbFileInfo;

JsFbFileInfo::JsFbFileInfo( JSContext* cx, std::unique_ptr<file_info_impl> fileInfo )
    : pJsCtx_( cx )
    , fileInfo_( std::move( fileInfo ) )
{
}

JsFbFileInfo::~JsFbFileInfo()
{
}

std::unique_ptr<mozjs::JsFbFileInfo>
JsFbFileInfo::CreateNative( JSContext* cx, std::unique_ptr<file_info_impl> fileInfo )
{
    if ( !fileInfo )
    {
        throw smp::SmpException( "Internal error: file_info object is null" );
    }

    return std::unique_ptr<JsFbFileInfo>( new JsFbFileInfo( cx, std::move( fileInfo ) ) );
}

size_t JsFbFileInfo::GetInternalSize( const std::unique_ptr<file_info_impl>& /*fileInfo*/ )
{
    return sizeof( file_info_impl );
}

int32_t JsFbFileInfo::InfoFind( const pfc::string8_fast& name )
{
    return fileInfo_->info_find_ex( name.c_str(), name.length() );
}

pfc::string8_fast JsFbFileInfo::InfoName( uint32_t index )
{
    if ( index >= fileInfo_->info_get_count() )
    {
        throw smp::SmpException( "Index is out of bounds" );
    }

    return fileInfo_->info_enum_name( index );
}

pfc::string8_fast JsFbFileInfo::InfoValue( uint32_t index )
{
    if ( index >= fileInfo_->info_get_count() )
    {
        throw smp::SmpException( "Index is out of bounds" );
    }

    return fileInfo_->info_enum_value( index );
}

int32_t JsFbFileInfo::MetaFind( const pfc::string8_fast& name )
{
    t_size idx = fileInfo_->meta_find_ex( name.c_str(), name.length() );
    if ( idx == pfc_infinite )
    {
        return -1;
    }

    return static_cast<int32_t>( idx );
}

pfc::string8_fast JsFbFileInfo::MetaName( uint32_t index )
{
    if ( index >= fileInfo_->meta_get_count() )
    {
        throw smp::SmpException( "Index is out of bounds" );
    }

    return fileInfo_->meta_enum_name( index );
}

pfc::string8_fast JsFbFileInfo::MetaValue( uint32_t infoIndex, uint32_t valueIndex )
{
    if ( infoIndex >= fileInfo_->meta_get_count()
         || valueIndex >= fileInfo_->meta_enum_value_count( infoIndex ) )
    {
        throw smp::SmpException( "Index is out of bounds" );
    }

    return fileInfo_->meta_enum_value( infoIndex, valueIndex );
}

uint32_t JsFbFileInfo::MetaValueCount( uint32_t index )
{
    if ( index >= fileInfo_->meta_get_count() )
    {
        throw smp::SmpException( "Index is out of bounds" );
    }

    return fileInfo_->meta_enum_value_count( index );
}

uint32_t JsFbFileInfo::get_InfoCount()
{
    return fileInfo_->info_get_count();
}

uint32_t JsFbFileInfo::get_MetaCount()
{
    return fileInfo_->meta_get_count();
}

} // namespace mozjs
