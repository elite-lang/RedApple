#ifndef STRING_FORMATTER
#define STRING_FORMATTER


#include <memory>
#include <iostream>
#include <string>


using namespace std; //Don't if you're in a header-file

template<typename ... Args>
string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


#endif /* end of include guard: STRING_FORMATTER */
