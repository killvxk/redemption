/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean, Javier Caverni
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   log file including syslog
*/


#pragma once

// These are used to help coverage chain when function length autodetection (using ctags and gcov) fails

// -Wnull-dereference and clang++
namespace { namespace compiler_aux_ {
    inline void * null_pointer()
    { return nullptr; }
} }
#define BOOM (*reinterpret_cast<int*>(compiler_aux_::null_pointer())=1)

// REDASSERT behave like assert but instaed of calling abort it triggers a segfault
// This is handy to get stacktrace while debugging.

#ifdef NDEBUG
#define REDASSERT(x)
#else
# if defined(LOGPRINT) || defined(REDASSERT_AS_ASSERT)
#  include <cassert>
#  define REDASSERT(x) assert(x)
# else
#  define REDASSERT(x) if(!(x)){BOOM;}
# endif
#endif

#include <cstdio> // std::printf family

#include <syslog.h>

#include <sys/types.h> // getpid
#include <unistd.h> // getpid

#include "cxx/diagnostic.hpp"

#include <string>
#include <type_traits>

// no enum type
template<class T, typename std::enable_if<std::is_enum<T>::value, bool>::type = 1>
typename std::underlying_type<T>::type
 log_value(T const & e) { return static_cast<typename std::underlying_type<T>::type>(e); }

// enum type
template<class T, typename std::enable_if<!std::is_enum<T>::value, bool>::type = 1>
T const & log_value(T const & x) { return x; }

inline char const * log_value(std::string const & x) { return x.c_str(); }

namespace {
    template<std::size_t n>
    struct redemption_log_s
    {
        char data[n];
    };
}

template<std::size_t n>
redemption_log_s<n*2+1>
log_array_02x_format(uint8_t (&d)[n])
{
    redemption_log_s<n*2+1> r;
    char * p = r.data;
    for (uint8_t c : d) {
        *p++ = "012345678abcdef"[c >> 4];
        *p++ = "012345678abcdef"[c & 0xf];
    }
    *p = 0;
    return r;
}

template<std::size_t n>
char const * log_value(redemption_log_s<n> && x) { return x.data; }

#if ! defined(IN_IDE_PARSER) && defined(__has_include) && __has_include(<boost/preprocessor/variadic/to_list.hpp>)

# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/comparison/greater.hpp>
# include <boost/preprocessor/array/pop_front.hpp>
# include <boost/preprocessor/array/pop_back.hpp>
# include <boost/preprocessor/array/to_list.hpp>
# include <boost/preprocessor/array/elem.hpp>
# include <boost/preprocessor/list/for_each.hpp>
# include <boost/preprocessor/variadic/elem.hpp>
# include <boost/preprocessor/variadic/to_list.hpp>

# define REDEMPTION_LOG_VALUE_PARAM_0(elem)
# define REDEMPTION_LOG_VALUE_PARAM_1(elem) , log_value(elem)

# define REDEMPTION_LOG_VALUE_PARAM(r, data, elem) \
    BOOST_PP_CAT(REDEMPTION_LOG_VALUE_PARAM_, BOOST_PP_BOOL(BOOST_PP_GREATER(r, 2)))(elem)

# define LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(...) \
    " " BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__)          \
    BOOST_PP_LIST_FOR_EACH(                             \
        REDEMPTION_LOG_VALUE_PARAM,                     \
        BOOST_PP_NIL,                                   \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)          \
    )

// checked by the compiler
# define LOG_REDEMPTION_FORMAT_CHECK(...)                      \
    void(sizeof(printf(                                        \
        LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(__VA_ARGS__) \
    )))

# define REDEMPTION_LOG_VALUE(x) (x)

#else

# ifndef REDEMPTION_DISABLE_NO_BOOST_PREPROCESSOR_WARNING
    REDEMPTION_DIAGNOSTIC_PUSH
#   ifdef __clang__
        REDEMPTION_DIAGNOSTIC_GCC_IGNORE("-Wpedantic")
#       warning Cannot checked format in "LOG" (no boost preprocessor)
#   else
        REDEMPTION_DIAGNOSTIC_GCC_WARNING("-Wunknown-pragmas")
#       pragma warning Cannot checked format in "LOG" (no boost/preprocessor)
#   endif
    REDEMPTION_DIAGNOSTIC_POP
# endif

# define LOG_REDEMPTION_FORMAT_CHECK(...) void()
# define REDEMPTION_LOG_VALUE(x) log_value(x)
# define LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(...) __VA_ARGS__

#endif


#ifdef IN_IDE_PARSER
#  define LOG LOGSYSLOG__REDEMPTION__INTERNAL

#elif defined(LOGPRINT)
#  define LOG(priority, ...)                                      \
    LOGCHECK__REDEMPTION__INTERNAL((                              \
        LOG_REDEMPTION_FORMAT_CHECK(__VA_ARGS__),                 \
        LOGPRINT__REDEMPTION__INTERNAL(priority, "%s (%d/%d) -- " \
        LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(__VA_ARGS__)),  \
        1                                                         \
    ))

#elif defined(LOGNULL)
#  define LOG(priority, ...)                               \
    LOGCHECK__REDEMPTION__INTERNAL((                       \
        LOG_REDEMPTION_FORMAT_CHECK(__VA_ARGS__), priority \
    ))

#elif defined(LOGASMJS) && defined(EM_ASM)
#  define LOG(priority, ...) LOGCHECK__REDEMPTION__INTERNAL((    \
        LOG_REDEMPTION_FORMAT_CHECK(__VA_ARGS__),                \
        LOGPRINT__REDEMPTION__ASMJS(priority, "%s (%d/%d) -- "   \
        LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(__VA_ARGS__)), \
        1                                                        \
    ))

#else
#  define LOG(priority, ...)                                       \
    LOGCHECK__REDEMPTION__INTERNAL((                               \
        LOG_REDEMPTION_FORMAT_CHECK(__VA_ARGS__),                  \
        LOGSYSLOG__REDEMPTION__INTERNAL(priority, "%s (%d/%d) -- " \
        LOG_REDEMPTION_VARIADIC_TO_LOG_PARAMETERS(__VA_ARGS__)),   \
        1                                                          \
    ))
#endif

namespace {
    // LOG_EMERG      system is unusable
    // LOG_ALERT      action must be taken immediately
    // LOG_CRIT       critical conditions
    // LOG_ERR        error conditions
    // LOG_WARNING    warning conditions
    // LOG_NOTICE     normal, but significant, condition
    // LOG_INFO       informational message
    // LOG_DEBUG      debug-level message

    constexpr const char * const prioritynames[] =
    {
        "EMERG"/*, LOG_EMERG*/,
        "ALERT"/*, LOG_ALERT*/,
        "CRIT"/*, LOG_CRIT*/,
        "ERR"/*, LOG_ERR*/,
        "WARNING"/*, LOG_WARNING*/,
        "NOTICE"/*, LOG_NOTICE*/,
        "INFO"/*, LOG_INFO*/,
        "DEBUG"/*, LOG_DEBUG*/,
        //{ nullptr/*, -1*/ }
    };

    inline void LOGCHECK__REDEMPTION__INTERNAL(int)
    {}

    REDEMPTION_DIAGNOSTIC_PUSH
    REDEMPTION_DIAGNOSTIC_GCC_IGNORE("-Wformat-nonliteral")

    template<class... Ts>
    void LOGPRINT__REDEMPTION__INTERNAL(int priority, char const * format, Ts const & ... args)
    {
        int const pid = getpid();
        std::printf(format, prioritynames[priority], pid, pid, REDEMPTION_LOG_VALUE(args)...);
        std::puts("");
    }

#if defined(LOGASMJS) && defined(EM_ASM)
    template<class... Ts>
    void LOGPRINT__REDEMPTION__ASMJS(int priority, char const * format, Ts const & ... args)
    {
        int const pid = getpid();
        char buffer[4096];
        int len = snprintf(
            buffer, sizeof(buffer)-2, format,
            prioritynames[priority], pid, pid, REDEMPTION_LOG_VALUE(args)...);
        buffer[len] = '\n';
        buffer[len+1] = 0;
        EM_ASM_({console.log(Pointer_stringify($0));}, buffer);
    }
#endif  // #if defined(LOGASMJS) && defined(EM_ASM)

    template<class... Ts>
    void LOGSYSLOG__REDEMPTION__INTERNAL(int priority, char const * format, Ts const & ... args)
    {
        int const pid = getpid();
        syslog(priority, format, prioritynames[priority], pid, pid, REDEMPTION_LOG_VALUE(args)...);
    }

    REDEMPTION_DIAGNOSTIC_POP
}

namespace {

inline void hexdump(const unsigned char * data, size_t size)
{
    char buffer[2048];
    for (size_t j = 0 ; j < size ; j += 16){
        char * line = buffer;
        line += std::sprintf(line, "%.4x ", static_cast<unsigned>(j));
        size_t i = 0;
        for (i = 0; i < 16; i++){
            if (j+i >= size){ break; }
            line += std::sprintf(line, "%.2x ", static_cast<unsigned>(data[j+i]));
        }
        if (i < 16){
            line += std::sprintf(line, "%*c", static_cast<int>((16-i)*3), ' ');
        }
        for (i = 0; i < 16; i++){
            if (j+i >= size){ break; }
            unsigned char tmp = data[j+i];
            if ((tmp < ' ') || (tmp > '~')  || (tmp == '\\')){
                tmp = '.';
            }
            line += std::sprintf(line, "%c", tmp);
        }

        if (line != buffer){
            line[0] = 0;
            LOG(LOG_INFO, "%s", buffer);
            buffer[0]=0;
        }
    }
}

inline void hexdump(const char * data, size_t size)
{
    hexdump(reinterpret_cast<const unsigned char*>(data), size);
}

inline void hexdump_d(const unsigned char * data, size_t size, unsigned line_length = 16)
{
    char buffer[2048];
    for (size_t j = 0 ; j < size ; j += line_length){
        char * line = buffer;
        line += std::sprintf(line, "/* %.4x */ ", static_cast<unsigned>(j));
        size_t i = 0;
        for (i = 0; i < line_length; i++){
            if (j+i >= size){ break; }
            line += std::sprintf(line, "0x%.2x, ", static_cast<unsigned>(data[j+i]));
        }
        if (i < line_length){
            line += std::sprintf(line, "%*c", static_cast<int>((line_length-i)*3), ' ');
        }

        line += std::sprintf(line, " // ");

        for (i = 0; i < line_length; i++){
            if (j+i >= size){ break; }
            unsigned char tmp = data[j+i];
            if ((tmp < ' ') || (tmp > '~') || (tmp == '\\')){
                tmp = '.';
            }
            line += std::sprintf(line, "%c", tmp);
        }

        if (line != buffer){
            line[0] = 0;
            LOG(LOG_INFO, "%s", buffer);
            buffer[0]=0;
        }
    }
}

inline void hexdump_d(const char * data, size_t size, unsigned line_length = 16)
{
    hexdump_d(reinterpret_cast<const unsigned char*>(data), size, line_length);
}

inline void hexdump_c(const unsigned char * data, size_t size)
{
    char buffer[2048];
    for (size_t j = 0 ; j < size ; j += 16){
        char * line = buffer;
        line += std::sprintf(line, "/* %.4x */ \"", static_cast<unsigned>(j));
        size_t i = 0;
        for (i = 0; i < 16; i++){
            if (j+i >= size){ break; }
            line += std::sprintf(line, "\\x%.2x", static_cast<unsigned>(data[j+i]));
        }
        line += std::sprintf(line, "\"");
        if (i < 16){
            line += std::sprintf(line, "%*c", static_cast<int>((16-i)*4), ' ');
        }
        line += std::sprintf(line, " //");
        for (i = 0; i < 16; i++){
            if (j+i >= size){ break; }
            unsigned char tmp = data[j+i];
            if ((tmp < ' ') || (tmp > '~') || (tmp == '\\')){
                tmp = '.';
            }
            line += std::sprintf(line, "%c", tmp);
        }

        if (line != buffer){
            line[0] = 0;
            LOG(LOG_INFO, "%s", buffer);
            buffer[0]=0;
        }
    }
}

inline void hexdump_c(const char * data, size_t size)
{
    hexdump_c(reinterpret_cast<const unsigned char*>(data), size);
}

inline void hexdump96_c(const unsigned char * data, size_t size)
{
    char buffer[32768];
    const unsigned line_length = 96;
    for (size_t j = 0 ; j < size ; j += line_length){
        char * line = buffer;
        line += std::sprintf(line, "/* %.4x */ \"", static_cast<unsigned>(j));
        size_t i = 0;
        for (i = 0; i < line_length; i++){
            if (j+i >= size){ break; }
            line += std::sprintf(line, "\\x%.2x", static_cast<unsigned>(data[j+i]));
        }
        line += std::sprintf(line, "\"");
        if (i < line_length){
            line += std::sprintf(line, "%*c", static_cast<int>((line_length-i)*4), ' ');
        }
        line += std::sprintf(line, " //");
        for (i = 0; i < line_length; i++){
            if (j+i >= size){ break; }
            unsigned char tmp = data[j+i];
            if ((tmp < ' ') || (tmp > '~')){
                tmp = '.';
            }
            line += std::sprintf(line, "%c", tmp);
        }

        if (line != buffer){
            line[0] = 0;
            LOG(LOG_INFO, "%s", buffer);
            buffer[0]=0;
        }
    }
}

inline void hexdump96_c(const char * data, size_t size)
{
    hexdump96_c(reinterpret_cast<const unsigned char*>(data), size);
}

inline void hexdump8_c(const unsigned char * data, size_t size)
{
    char buffer[1024];
    const unsigned line_length = 8;
    for (size_t j = 0 ; j < size ; j += line_length){
        char * line = buffer;
        line += std::sprintf(line, "/* %.4x */ \"", static_cast<unsigned>(j));
        size_t i = 0;
        for (i = 0; i < line_length; i++){
            if (j+i >= size){ break; }
            line += std::sprintf(line, "\\x%.2x", static_cast<unsigned>(data[j+i]));
        }
        line += std::sprintf(line, "\"");
        if (i < line_length){
            line += std::sprintf(line, "%*c", static_cast<int>((line_length-i)*4), ' ');
        }
        line += std::sprintf(line, " //");
        for (i = 0; i < line_length; i++){
            if (j+i >= size){ break; }
            unsigned char tmp = data[j+i];
            if ((tmp < ' ') || (tmp > '~')){
                tmp = '.';
            }
            line += std::sprintf(line, "%c", tmp);
        }

        if (line != buffer){
            line[0] = 0;
            LOG(LOG_INFO, "%s", buffer);
            buffer[0]=0;
        }
    }
}

inline void hexdump8_c(const char * data, size_t size)
{
    hexdump8_c(reinterpret_cast<const unsigned char*>(data), size);
}

} // anonymous namespace

