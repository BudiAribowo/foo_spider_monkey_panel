#include <stdafx.h>

#include <component_guids.h>

#include <qwr/acfu_integration.h>

using namespace smp;

namespace
{

class SmpSource
    : public qwr::acfu::QwrSource
{
public:
    // ::acfu::source
    GUID get_guid() override;
    ::acfu::request::ptr create_request() override;

    // qwr::acfu::github_conf
    static pfc::string8 get_owner();
    static pfc::string8 get_repo();

    // qwr::acfu::QwrSource
    std::string GetComponentName() const override;
    std::string GetComponentFilename() const override;
};

} // namespace

namespace
{

GUID SmpSource::get_guid()
{
    return guid::acfu_source;
}

::acfu::request::ptr SmpSource::create_request()
{
    return fb2k::service_new<qwr::acfu::github_latest_release<SmpSource>>();
}

pfc::string8 SmpSource::get_owner()
{
    return "TheQwertiest";
}

pfc::string8 SmpSource::get_repo()
{
    return "https://github.com/TheQwertiest/" SMP_UNDERSCORE_NAME;
}

std::string SmpSource::GetComponentName() const
{
    return SMP_NAME;
}

std::string SmpSource::GetComponentFilename() const
{
    return SMP_UNDERSCORE_NAME;
}

FB2K_SERVICE_FACTORY( SmpSource );

} // namespace
