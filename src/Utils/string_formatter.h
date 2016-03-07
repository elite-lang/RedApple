#ifndef STRING_FORMATTER
#define STRING_FORMATTER


#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

// TODO: 这里有Windows和Linux下snprintf功能不同的隐藏bug
// linux下 sprintf会自动在后面加'/0'，同时copy的长度也包含了‘/0’，
// 但windows下的_snprintf并未按照该规则，就是简单的copy指定长度的字符，不自动加0，并且长度也不包含0。

#ifdef __GNUC__
#define _snprintf snprintf
#endif


template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
	size_t size = _snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] );
	_snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


#endif /* end of include guard: STRING_FORMATTER */
