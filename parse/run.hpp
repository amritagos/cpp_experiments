#include <fmt/format.h>
#include <fmt/ostream.h>
#include <algorithm>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>
#include <system_error>
#include <vector>

namespace Parse
{

template<typename T>
auto parse( std::string_view str )
{
    T result;
    auto [ptr, ec] = std::from_chars( str.begin(), str.end(), result );
    if( ec != std::errc() ) [[unlikely]]
        throw std::runtime_error( fmt::format( "Parsing error for word '{}'", str ) );
    return result;
}

template<typename T>
auto parse( std::string_view str, std::string_view separator )
{
    auto split_view   = std::views::split( str, separator );
    size_t n_elements = std::distance( split_view.begin(), split_view.end() );

    T result  = T( n_elements );
    int count = 0;
    for( const auto word : split_view )
    {
        auto [ptr, ec] = std::from_chars( word.begin(), word.end(), result[count++] );
        if( ec != std::errc() ) [[unlikely]]
            throw std::runtime_error(
                fmt::format( "Parsing error for word '{}'", std::string( word.begin(), word.end() ) ) );
    }
    return result;
}

void run()
{
    auto t1 = parse<std::vector<int>>( "3 -- 6", "--" );
    for( auto i : t1 )
    {
        fmt::print( "{}\n", i );
    }

    auto t2 = parse<std::vector<double>>( "1.2 2.34 3", " " );
    for( auto i : t2 )
    {
        fmt::print( "{}\n", i );
    }

    // auto t3 = parse<std::array<double, 3>>( "1.2 2.34 3", " " );
    // for( auto i : t3 )
    // {
    //     fmt::print( "{}\n", i );
    // }

    auto w = parse<double>( "2.3" );
    fmt::print( "{}\n", w );
}

} // namespace Parse