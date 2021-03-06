#include <stdafx.h>

#include "component_paths.h"

#include <qwr/fb2k_paths.h>

namespace smp::path
{

std::filesystem::path JsDocsIndex()
{
    return qwr::path::Component() / L"docs/html/index.html";
}

std::filesystem::path ScriptSamples()
{
    return qwr::path::Component() / "samples";
}

std::filesystem::path Packages_Sample()
{
    return ScriptSamples() / "packages";
}

std::filesystem::path Packages_Profile()
{
    return qwr::path::Profile() / SMP_UNDERSCORE_NAME / "packages";
}

std::filesystem::path Packages_Foobar2000()
{
    return qwr::path::Foobar2000() / SMP_UNDERSCORE_NAME / "packages";
}

std::filesystem::path TempFolder()
{
    return qwr::path::Foobar2000() / SMP_UNDERSCORE_NAME / "tmp";
}

std::filesystem::path TempFolder_PackageUnpack()
{
    return TempFolder() / "unpacked_package";
}

} // namespace smp::path
