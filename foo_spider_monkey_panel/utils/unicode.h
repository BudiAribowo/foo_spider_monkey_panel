#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <charconv>

namespace smp::unicode
{

std::wstring ToWide( const std::u8string_view& src );
std::u8string ToU8( const std::wstring_view& src );

} // namespace smp::string
