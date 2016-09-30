/*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*   Product name: redemption, a FLOSS RDP proxy
*   Copyright (C) Wallix 2010-2016
*   Author(s): Jonathan Poelen
*/

#pragma once

#include "utils/sugar/numerics/safe_conversions.hpp"

#include <iosfwd>

#include <limits>
#include <utility>
#include <cstdint>

// (standalone version): https://github.com/edouarda/brigand
#define BRIGAND_NO_BOOST_SUPPORT
#include <brigand/brigand.hpp>

// https://github.com/jonathanpoelen/brigand/blob/ext_call/brigand/functions/lambda/call.hpp
namespace brigand
{
namespace detail
{
    template<template<class...> class F>
    struct call_
    {};

    template <template<class...> class F, class... Args>
    struct apply<call_<F>, brigand::list<Args...>>
    {
        using type = F<Args...>;
    };

    template<template<class> class F>
    bind<F, _1> get_call_impl(call_<F>, int);

    template<template<class, class> class F>
    bind<F, _1, _2> get_call_impl(call_<F>, int);

    template<template<class, class, class> class F>
    bind<F, _1, _2, _3> get_call_impl(call_<F>, int);

//     template<template<
//       class, class, class, class, class, class, class, class> class F>
//     bind<F, _1, _2, _3, _4, _5, _6, _7, _8> get_call_impl(call_<F>, int);

    template<class F>
    F get_call_impl(F, char);
}

    template<template<class...> class F>
    using call = decltype(detail::get_call_impl(detail::call_<F>{}, 1));

    template<class L, class P>
    using copy_if = remove_if<L, bind<not_, P>>;
}

// brigand::split_if
namespace brigand
{
namespace detail
{
    template<class L>
    struct splitted_list
    { using type = list<L>; };

    template<>
    struct splitted_list<list<>>
    { using type = list<>; };

    template<class Seq, class Pred, class I = index_if<Seq, Pred>>
    struct split_if_impl
    : append_impl<
        typename splitted_list<front<split_at<Seq, I>>>::type,
        list<list<front<front<pop_front<split_at<Seq, I>>>>>>,
        typename split_if_impl<pop_front<front<pop_front<split_at<Seq, I>>>>, Pred>::type
    >
    {};

    template<class Seq, class Pred>
    struct split_if_impl<Seq, Pred, no_such_type_>
    { using type = list<Seq>; };

    template<template<class...> class L, class Pred>
    struct split_if_impl<L<>, Pred, no_such_type_>
    { using type = list<>; };
}

    namespace lazy {
        template<class L, class Pred>
        using split_if = detail::split_if_impl<L, Pred>;
    }

    template<class L, class Pred>
    using split_if = typename detail::split_if_impl<L, Pred>::type;
}


#include "utils/sugar/array_view.hpp"
#include "utils/sugar/bytes_t.hpp"


#define PROTO_VAR(t, v)                        \
    constexpr struct v                         \
    : ::proto::var<t, v>                       \
    {                                          \
        using ::proto::var<t, v>::operator = ; \
                                               \
        static constexpr char const *          \
        name() noexcept { return #v; }         \
    } v {}                                     \

namespace proto
{
    template<std::size_t N>
    using size_ = std::integral_constant<std::size_t, N>;


    struct dyn_size {};
    template<std::size_t n> struct limited_size { static const std::size_t value = n; };

    namespace tags
    {
        class static_buffer {};
        class dynamic_buffer {};
        class view_buffer {};
        class limited_buffer {};
        class overrided_buffer {};
    }

    template<class T> struct sizeof_impl { using type = typename T::sizeof_; };

    template<class T> using sizeof_ = typename sizeof_impl<T>::type;

    template<class...> using void_t = void;

    namespace detail
    {
        template<class T> struct sizeof_to_buffer_cat { using type = tags::dynamic_buffer; };
        template<std::size_t n> struct sizeof_to_buffer_cat<size_<n>> { using type = tags::static_buffer; };
        template<std::size_t n> struct sizeof_to_buffer_cat<limited_size<n>> { using type = tags::limited_buffer; };

        template<class T, class = void>
        struct buffer_category_impl : sizeof_to_buffer_cat<sizeof_<T>> {};

        template<class T>
        struct buffer_category_impl<T, void_t<typename T::buffer_category>>
        { using type = typename T::buffer_category; };
    }

    template<class T> struct buffer_category_impl : detail::buffer_category_impl<T> {};
    template<class T> using buffer_category = typename buffer_category_impl<T>::type;

    // TODO has_*
    template<class T> using is_static_buffer
      = typename std::is_same<tags::static_buffer, buffer_category<T>>::type;
    template<class T> using is_limited_buffer
      = typename std::is_same<tags::limited_buffer, buffer_category<T>>::type;
    template<class T> using is_view_buffer
      = typename std::is_same<tags::view_buffer, buffer_category<T>>::type;
    template<class T> using is_dynamic_buffer
      = typename std::is_same<tags::dynamic_buffer, buffer_category<T>>::type;
    template<class T> using is_overrided_buffer
      = typename std::is_same<tags::overrided_buffer, buffer_category<T>>::type;

    namespace detail
    {
        template<class T, class U> struct common_size_impl;
        template<class T, class U> struct common_buffer_impl;
    }

    template<class T, class U> using common_size = typename detail::common_size_impl<T, U>::type;
    template<class T, class U> using common_buffer = typename detail::common_buffer_impl<T, U>::type;

    template<class T>
    using t_ = typename T::type;

    template<class... Ts, class F>
    void for_each(brigand::list<Ts...>, F && f) {
        (void)std::initializer_list<int>{
            (void(f(Ts{})), 1)...
        };
    }


    // clang narrowing checker with std::intgral_constant... (cf: safe_int<T> = T)
    template<class T>
    struct safe_int
    {
        T val;

        template<class U> constexpr safe_int(U x) noexcept : val{x} {}

        operator T () const { return val; }
        operator T & () { return val; }
    };

    namespace types {
        class le_tag {};
        class be_tag {};

        /**
        * fixed width integer types
        * @{
        */
        template<class T, class Endianess>
        struct integer
        {
            using type = T;
            using sizeof_ = size_<sizeof(T)>;

            static_assert(std::is_integral<T>::value, "");

            safe_int<type> val;

            sizeof_ static_serialize(uint8_t * p) const
            {
                /**///std::cout << " [static_buffer] [sizeof_: " << sizeof(T) << "] {" << static_cast<void*>(p) << "}";
                using rng = brigand::range<std::size_t, 0, sizeof(T)>;
                using is_little_endian = t_<std::is_same<Endianess, le_tag>>;
                for_each(rng{}, [&p, this](auto i) {
                    // TODO std::make_unsigned
                    /**///std::cout << " { *p++ }";
                    *p++ = this->val >> ((is_little_endian{} ? i : sizeof(T)-1-i) * 8);
                });
                return sizeof_{};
            }
        };

        template<class E, class ProtoType>
        struct enum_ : ProtoType
        {
            static_assert(std::is_enum<E>::value, "");

            using type = E;

            constexpr enum_(E e) noexcept : ProtoType{typename ProtoType::type(e)} {}
        };

        using s8 = integer<int8_t, void>;
        using u8 = integer<uint8_t, void>;

        using s16_be = integer<int16_t, be_tag>;
        using s16_le = integer<int16_t, le_tag>;
        using u16_be = integer<uint16_t, be_tag>;
        using u16_le = integer<uint16_t, le_tag>;

        using s32_be = integer<int32_t, be_tag>;
        using s32_le = integer<int32_t, le_tag>;
        using u32_be = integer<uint32_t, be_tag>;
        using u32_le = integer<uint32_t, le_tag>;

        using s64_be = integer<int64_t, be_tag>;
        using s64_le = integer<int64_t, le_tag>;
        using u64_be = integer<uint64_t, be_tag>;
        using u64_le = integer<uint64_t, le_tag>;

        template<class E> using enum_s8 = enum_<E, s8>;
        template<class E> using enum_u8 = enum_<E, u8>;

        template<class E> using enum_s16_be = enum_<E, s16_be>;
        template<class E> using enum_s16_le = enum_<E, s16_le>;
        template<class E> using enum_u16_be = enum_<E, u16_be>;
        template<class E> using enum_u16_le = enum_<E, u16_le>;

        template<class E> using enum_s32_be = enum_<E, s32_be>;
        template<class E> using enum_s32_le = enum_<E, s32_le>;
        template<class E> using enum_u32_be = enum_<E, u32_be>;
        template<class E> using enum_u32_le = enum_<E, u32_le>;

        template<class E> using enum_s64_be = enum_<E, s64_be>;
        template<class E> using enum_s64_le = enum_<E, s64_le>;
        template<class E> using enum_u64_be = enum_<E, u64_be>;
        template<class E> using enum_u64_le = enum_<E, u64_le>;
        /** @} */

        /**
        * @{
        */
        struct u16_encoding
        {
            using type = uint16_t;
            using sizeof_ = limited_size<sizeof(type)>;

            safe_int<type> val;

            std::size_t limited_serialize(uint8_t * p) const
            {
                assert(!(val & 0x8000));
                return (val <= 127)
                    ? u8{uint8_t(val)}.static_serialize(p)
                    : u16_be{val}.static_serialize(p);
            }
        };
        template<class E> using enum_u16_encoding = enum_<E, u16_encoding>;

        struct u32_encoding
        {
            using type = uint32_t;
            using sizeof_ = limited_size<sizeof(type)>;

            safe_int<type> val;

            std::size_t limited_serialize(uint8_t * p) const
            {
                assert(!(val & 0xC0000000));
                auto serial = [&p](uint8_t v) { return u8{v}.static_serialize(p++); };
                return (val <= 0x3FFF)
                ?
                    (val <= 0x3F)
                    ?
                        serial(        val       )
                    :
                        serial(0x40 | (val >> 8 )) +
                        serial(        val       )
                :
                    (val <= 0x3FFFFF)
                    ?
                        serial(0x80 | (val >> 16)) +
                        serial(        val >> 8  ) +
                        serial(        val       )
                    :
                        serial(0xC0 | (val >> 24)) +
                        serial(       (val >> 16)) +
                        serial(        val >> 8  ) +
                        serial(        val       )
                ;
            }
        };
        template<class E> using enum_u32_encoding = enum_<E, u32_encoding>;
        /** @} */

        template<class Obj, class T>
        using enable_if_not_default_ctor_argument_t
            = std::enable_if_t<(!std::is_same<Obj, std::remove_reference_t<T>>::value)>;

        struct bytes
        {
            using type = array_view_const_u8;
            using sizeof_ = dyn_size;
            using buffer_category = tags::view_buffer;

            type av;

            template<class T, class = enable_if_not_default_ctor_argument_t<bytes, T>>
            constexpr bytes(T && av) noexcept
            : av(av)
            {}

            constexpr bytes(array_view_const_u8 av) noexcept
            : av(av)
            {}

            bytes(array_view_const_char av) noexcept
            : av(reinterpret_cast<uint8_t const*>(av.data()), av.size())
            {}

            array_view_const_u8 get_view_buffer() const
            {
                /**///std::cout << " [view_buffer] [size: " << av.size() << "]";
                return av;
            }
        };

        struct mutable_bytes
        {
            using type = array_view_u8;
            using sizeof_ = dyn_size;
            using buffer_category = tags::view_buffer;

            type av;

            template<class T, class = enable_if_not_default_ctor_argument_t<mutable_bytes, T>>
            constexpr mutable_bytes(T && av) noexcept
            : av(av)
            {}

            constexpr mutable_bytes(array_view_u8 av) noexcept
            : av(av)
            {}

            mutable_bytes(array_view_char av) noexcept
            : av(reinterpret_cast<uint8_t *>(av.data()), av.size())
            {}

            array_view_const_u8 get_view_buffer() const
            {
                /**///std::cout << " [view_buffer] [size: " << av.size() << "]";
                return av;
            }
        };

        struct str8_to_str16
        {
            using type = const_bytes_array;
            using sizeof_ = dyn_size;

            type str;

            constexpr str8_to_str16(const_bytes_array av) noexcept
            : str(av)
            {}

            template<class F>
            void dynamic_serialize(F && f) const
            {
                /**///std::cout << " [dynamic_buffer]";
                f(this->str);
            }
        };

        template<class Desc, typename Desc::type val>
        struct static_value
        {
            using type = typename Desc::type;
            using sizeof_ = proto::sizeof_<Desc>;
            using buffer_category = proto::buffer_category<Desc>;

            std::size_t static_serialize(uint8_t * p) const
            {
                return Desc{val}.static_serialize(p);
            }

            std::size_t limited_serialize(uint8_t * p) const
            {
                return Desc{val}.limited_serialize(p);
            }
        };

        template<class Desc>
        struct pkt_sz
        {
            using type = Desc;
            using sizeof_ = proto::sizeof_<Desc>;
        };

        template<class Desc>
        struct pkt_sz_with_self
        {
            using type = Desc;
            using sizeof_ = proto::sizeof_<Desc>;
        };

        template<class Desc>
        struct pkt_data
        {
            using type = Desc;
            using sizeof_ = proto::sizeof_<Desc>;
        };

        template<class T>
        struct value
        {
            T val;
        };
    }

    template<class...> using void_ = void;


    template<class... Ts>
    struct inherits : Ts...
    {
        template<class... Us>
        constexpr inherits(Us && ... v)
        : Ts{std::forward<Us>(v)}...
        {}
    };


    namespace detail
    {
        template<std::size_t n>
        struct common_size_impl<size_<n>, size_<n>> { using type = size_<n>; };

        template<std::size_t n1, std::size_t n2>
        struct common_size_impl<size_<n1>, size_<n2>> { using type = limited_size<std::max(n1, n2)>; };

        template<std::size_t n1, std::size_t n2>
        struct common_size_impl<limited_size<n1>, limited_size<n2>> { using type = limited_size<std::max(n1, n2)>; };
        template<std::size_t n1, std::size_t n2>
        struct common_size_impl<size_<n1>, limited_size<n2>> { using type = limited_size<std::max(n1, n2)>; };
        template<std::size_t n1, std::size_t n2>
        struct common_size_impl<limited_size<n1>, size_<n2>> { using type = limited_size<std::max(n1, n2)>; };

        template<class T> struct common_size_impl<T, dyn_size> { using type = dyn_size; };
        template<class U> struct common_size_impl<dyn_size, U> { using type = dyn_size; };
        template<> struct common_size_impl<dyn_size, dyn_size> { using type = dyn_size; };


        template<class T, class U> struct common_buffer_impl { using type = tags::dynamic_buffer; };
        template<class T> struct common_buffer_impl<T, T> { using type = T; };
        template<> struct common_buffer_impl<tags::static_buffer, tags::limited_buffer>
        { using type = tags::limited_buffer ; };
        template<> struct common_buffer_impl<tags::limited_buffer, tags::static_buffer>
        { using type = tags::limited_buffer ; };


        template<class T> struct is_pkt_sz : std::false_type {};
        template<class T> struct is_pkt_sz<types::pkt_sz<T>> : std::true_type {};

        template<class T> struct is_pkt_sz_with_self : std::false_type {};
        template<class T> struct is_pkt_sz_with_self<types::pkt_sz_with_self<T>> : std::true_type {};

        template<class T> struct is_pkt_data : std::false_type {};
        template<class T> struct is_pkt_data<types::pkt_data<T>> : std::true_type {};
    }
    template<class T> using is_pkt_sz = typename detail::is_pkt_sz<T>::type;
    template<class T> using is_pkt_sz_with_self = typename detail::is_pkt_sz_with_self<T>::type;
    template<class T> using is_pkt_sz_category = brigand::bool_<is_pkt_sz<T>{} or is_pkt_sz_with_self<T>{}>;

    template<class T> using is_pkt_data = typename detail::is_pkt_data<T>::type;


    template<class Var, class T>
    struct val
    {
        using var_type = Var;
        using value_type = T;

        Var var;
        T x;

        template<class Params>
        constexpr val
        to_proto_value(Params) const
        { return *this; }

        decltype(auto) name() const noexcept
        { return this->var.name(); }
    };

    template<class Derived, class Desc, class T>
    constexpr auto make_val(T && x)
    { return Desc{std::forward<T>(x)}; }


    template<class Desc, class Derived>
    struct var
    {
        using desc_type = Desc;
        using var_type = Derived;
        using arguments = brigand::list<Derived>;

        template<class U>
        constexpr auto operator = (U && x) const
        // Clang bug
        // {
        //     return val<Derived, decltype(trace_adapt<Derived>(Desc{}, std::forward<U>(x)))>{
        //         trace_adapt<Derived>(Desc{}, std::forward<U>(x))
        //     };
        // }
        { return impl(std::forward<U>(x)); }

        template<class Params>
        decltype(auto) to_proto_value(Params params) const noexcept
        { return params.template get_proto_value<Derived>(); }

    private:
        template<class U>
        constexpr auto impl(U && x) const
        -> val<Derived, decltype(make_val<Desc, Desc>(std::forward<U>(x)))>
        { return {Derived{}, make_val<Desc, Desc>(std::forward<U>(x))}; }
    };

    template<class Desc, class Derived>
    struct var<types::pkt_sz<Desc>, Derived>
    {
        using desc_type = types::pkt_sz<Desc>;
        using var_type = Derived;

        template<class Params>
        Derived to_proto_value(Params) const noexcept
        { return static_cast<Derived const &>(*this); }
    };

    template<class Desc, class Derived>
    struct var<types::pkt_sz_with_self<Desc>, Derived>
    {
        using desc_type = types::pkt_sz_with_self<Desc>;
        using var_type = Derived;

        template<class Params>
        Derived to_proto_value(Params) const noexcept
        { return static_cast<Derived const &>(*this); }
    };

    template<class Desc, class Derived>
    struct var<types::pkt_data<Desc>, Derived>
    {
        using desc_type = types::pkt_data<Desc>;
        using var_type = Derived;

        template<class Params>
        Derived to_proto_value(Params) const noexcept
        { return static_cast<Derived const &>(*this); }
    };

    template<class T>
    using desc_type_t = typename T::desc_type;

    template<class T, class = void> struct check;
    template<class T> struct check<T, std::enable_if_t<T::value>> { constexpr operator bool () { return 0; } };


    template<class T> using var_type_t = typename T::var_type;

    namespace cexp
    {
        constexpr std::size_t strlen(char const * s)
        {
            std::size_t n = 0;
            while (*s) {
                ++n;
                s++;
            }
            return n;
        }

        constexpr std::size_t strcpy(char * dst, char const * src)
        {
            char * p = dst;
            while (*src) {
                *p++ = *src++;
            }
            return p - dst;
        }

        template<class T, class... Ts>
        constexpr T fold(T a, Ts... as)
        {
            (void)std::initializer_list<int>{
                (void(a += as), 1)...
            };
            return a;
        }
    }

    template<class Desc, class CtxName>
    struct named_var
    : var<Desc, named_var<Desc, CtxName>>
    {
        using ::proto::var<Desc, named_var>::operator = ;

        constexpr named_var(CtxName ctx_name) noexcept : ctx_name(ctx_name) {}

        constexpr auto name() const noexcept
        { return this->ctx_name.name(); }

        CtxName ctx_name;
    };

    struct ctx_unamed
    {
        constexpr char const * name() const noexcept { return "<unamed>"; }
    };

    struct ctx_c_str_name
    {
        char const * s;

        constexpr char const * name() const noexcept { return s; }
    };

    template<class... Vars>
    struct ctx_vars_name
    {
        char s[cexp::fold(cexp::strlen(Vars::name())...) + 4 + sizeof...(Vars)];

        constexpr ctx_vars_name()
        : s{}
        {
            char * p = s;
            *p++ = '{';
            (void)std::initializer_list<int>{
                (void(p += cexp::strcpy(&(p[0] = ' ') + 1, Vars::name()) + 1), 1)...
            };
            *p++ = ' ';
            *p++ = '}';
            //*p = 0;
        }

        constexpr char const * name() const noexcept { return s; }
    };


    template<class T, class = void>
    struct get_arguments
    { using type = brigand::list<>; };

    template<class T>
    struct get_arguments<T, void_t<typename T::arguments>>
    { using type = typename T::arguments; };

    template<class T>
    using get_arguments_t = typename get_arguments<T>::type;


    namespace detail
    {
        template<class T, class = void> struct is_reserializer_impl : std::false_type {};
        template<class T> struct is_reserializer_impl<T, proto::void_<typename T::is_reserializer>>
        : brigand::bool_<T::is_reserializer::value> {};
    }
    template<class T>
    using is_reserializer = typename detail::is_reserializer_impl<T>::type;

    namespace detail
    {
        template<class T> struct is_special_value_impl : std::false_type {};
    }

    template<class T>
    using is_special_value = brigand::bool_<is_reserializer<T>::value or (
        brigand::any<
            get_arguments_t<T>,
            brigand::call<detail::is_special_value_impl>
        >::value
    )>;

    // TODO deprecated
    template<class T>
    struct value_wrapper
    {
        T x;
    };

    template<class Var, class T>
    constexpr T get_value(val<Var, T> v)
    { return v.x; }

    template<class T>
    constexpr T get_value(T v)
    { return v; }


    template<class T, class CtxName, class... Vars>
    struct lazy_creator
    {
        using var_type = lazy_creator;
        using desc_type = T;
        using arguments = brigand::append<get_arguments_t<Vars>...>;
        using is_reserializer = proto::is_reserializer<T>;

        CtxName ctx_name;
        // TODO tuple
        inherits<Vars...> values;

        template<class Params>
        constexpr T
        to_proto_value(Params params) const
        {
            return T{get_value(static_cast<Vars const &>(this->values).to_proto_value(params))...};
        }

        constexpr decltype(auto) name() const noexcept
        { return this->ctx_name.name(); }
    };

    template<class T, class CtxName, class... Vars>
    struct creator
    {
        using arguments = brigand::append<get_arguments_t<Vars>...>;

        CtxName ctx_name;
        // TODO tuple
        inherits<Vars...> values;

        template<class Params>
        constexpr auto
        to_proto_value(Params params) const
        {
            return this->to_proto_value_(
                static_cast<Vars const &>(this->values).to_proto_value(params)...
            );
        }

    private:
        template<class... V>
        constexpr
        std::enable_if_t<
            !is_reserializer<T>::value and !brigand::any<brigand::list<is_special_value<V>...>>::value,
            val<named_var<T, CtxName>, T>
        >
        to_proto_value_(V && ... values) const
        {
            return {
                named_var<T, CtxName>{this->ctx_name},
                T{std::move(values.x)...}
            };
        }

        template<class... V>
        constexpr
        std::enable_if_t<
            is_reserializer<T>::value or brigand::any<brigand::list<is_special_value<V>...>>::value,
            lazy_creator<T, CtxName, V...>
        >
        to_proto_value_(V && ... values) const
        {
            return {
                this->ctx_name,
                {std::move(values)...}
            };
        }
    };


    template<class T>
    std::enable_if_t<is_static_buffer<T>::value, std::size_t>
    static_or_limited_serialize(uint8_t * p, T const & x)
    { return x.static_serialize(p); }

    template<class T>
    std::enable_if_t<!is_static_buffer<T>::value, std::size_t>
    static_or_limited_serialize(uint8_t * p, T const & x)
    { return x.limited_serialize(p); }


    template<class T, class AV>
    std::enable_if_t<is_static_buffer<T>::value, std::size_t>
    static_or_limited_reserialize(uint8_t * p, T const & x, AV av)
    { return x.static_reserialize(p, av); }

    template<class T, class AV>
    std::enable_if_t<!is_static_buffer<T>::value, std::size_t>
    static_or_limited_reserialize(uint8_t * p, T const & x, AV av)
    { return x.limited_reserialize(p, av); }


    namespace detail
    {
        template<class Ints, class... Ts>
        struct compose_impl;

        template<std::size_t, class T>
        struct indexed_value
        { T x; };


        template<class, class>
        struct add_size_impl;

        template<std::size_t n1, std::size_t n2>
        struct add_size_impl<size_<n1>, size_<n2>> { using type = limited_size<n1 + n2>; };

        template<std::size_t n1, std::size_t n2>
        struct add_size_impl<limited_size<n1>, limited_size<n2>> { using type = limited_size<n1 + n2>; };
        template<std::size_t n1, std::size_t n2>
        struct add_size_impl<size_<n1>, limited_size<n2>> { using type = limited_size<n1 + n2>; };
        template<std::size_t n1, std::size_t n2>
        struct add_size_impl<limited_size<n1>, size_<n2>> { using type = limited_size<n1 + n2>; };

        template<class Sz1, class Sz2>
        using add_size = typename add_size_impl<Sz1, Sz2>::type;

        template<std::size_t... Ints, class... Ts>
        struct compose_impl<std::integer_sequence<std::size_t, Ints...>, Ts...>
        {
            using sizeof_ = brigand::fold<
                brigand::list<proto::sizeof_<Ts>...>,
                size_<0>,
                brigand::call<add_size>
            >;

            constexpr compose_impl(Ts... v) : values{v...} {}

            std::size_t limited_serialize(uint8_t * p) const { return serialize(p); }

            std::size_t static_serialize(uint8_t * p) const { return serialize(p); }

        private:
            inherits<indexed_value<Ints, Ts>...> values;

            std::size_t serialize(uint8_t * p) const
            {
                std::size_t sz = 0;
                (void)std::initializer_list<int>{
                    ((sz += static_or_limited_serialize(
                        p + sz,
                        static_cast<indexed_value<Ints, Ts> const &>(this->values).x
                    )), 1)...
                };
                return sz;
            }
        };

        template<class Ints, class... Ts>
        std::ostream & operator <<(std::ostream & os, compose_impl<Ints, Ts...> const &)
        { return os << "compose"; }

    }

    template<class... Ts>
    using compose_t = detail::compose_impl<std::index_sequence_for<Ts...>, Ts...>;

#ifdef IN_IDE_PARSER
# define PROTO_IDE_CPP(...)
#else
# define PROTO_IDE_CPP(...) __VA_ARGS__
#endif

    namespace utils
    {
        namespace detail
        {
            template<class T>
            struct ref
            { T & x; };

            template<class Var, class T>
            constexpr val<Var, T>
            ref_to_val(ref<val<Var, T>> r)
            { return r.x; }
        }

        template<class... Ts>
        struct parameters
        {
            parameters(Ts & ... x) : refs{x...}
            {}

            template<class T>
            constexpr decltype(auto) operator[](T const &) const noexcept
            { return detail::ref_to_val<T>(refs).x; }

            template<class T>
            constexpr decltype(auto) get_proto_value(T const &) const noexcept
            { return detail::ref_to_val<T>(refs); }

            template<class T>
            constexpr decltype(auto) get_proto_value() const noexcept
            { return detail::ref_to_val<T>(refs); }

            proto::inherits<detail::ref<Ts>...> refs;
        };

        template<class... Ts>
        parameters<Ts...>
        make_parameters(Ts & ... x)
        { return {x...}; }
    }

    template<class... Ts>
    struct packet
    {
        using type_list = brigand::list<Ts...>;

        inherits<Ts...> values;

        template<class F>
        void apply_for_each(F f) const
        {
            (void)std::initializer_list<int>{
                (void(f(static_cast<Ts const &>(this->values))), 1)...
            };
        }

        template<class F>
        decltype(auto) apply(F f) const
        {
            return f(static_cast<Ts const &>(this->values)...);
        }
    };

    class dummy_ {};

    /// \brief make a proto::packet
    template<class Desc>
    constexpr auto
    value(Desc const & value)
    {
        using value_type = val<named_var<Desc, ctx_unamed>, Desc>;
        return packet<value_type>{value_type{{ctx_unamed{}}, value}};
    }

    /// \brief make a proto::packet
    template<class Desc>
    constexpr auto
    value(char const * s, Desc const & value)
    {
        using value_type = val<named_var<Desc, ctx_c_str_name>, Desc>;
        return packet<value_type>{value_type{{{s}}, value}};
    }

    // TODO values


    template<class... Ts>
    struct packet_description
    {
        using arguments = brigand::append<get_arguments_t<Ts>...>;
        // TODO deprecated
        using lazy_arguments = brigand::list<>;

        inherits<Ts...> values;

        template<class Val>
        using check_param = std::enable_if_t<
            brigand::any<
                arguments,
                brigand::bind<
                    std::is_same,
                    brigand::_1,
                    brigand::pin<var_type_t<Val>>
                >
            >::value,
            Val
        >;

        constexpr packet_description(Ts... a)
        : packet_description{lazy_arguments{}, a...}
        {}

        template<class... Val>
        constexpr auto
        operator()(Val... values) const &
        {
            return ordering_parameter<check_param<Val>...>({values...});
        }

    private:
        template<class... Lazy>
        constexpr packet_description(brigand::list<Lazy...>, Ts... a)
        : values{a...}
        , lazy_arguments{static_cast<Lazy>(this->values)...}
        {}

        constexpr packet_description(brigand::list<>, Ts... a)
        : values{a...}
        {}

        template<class... Val>
        constexpr auto
        ordering_parameter(utils::parameters<Val...> params) const
        {
            return packet<
                decltype(static_cast<Ts const &>(this->values).to_proto_value(params))...
            >{{(static_cast<Ts const &>(this->values).to_proto_value(params))...}};
        }
    };


    template<class T, class... Desc>
    constexpr auto
    creater(Desc... d)
    {
        return creator<T, ctx_vars_name<Desc...>, Desc...>{{}, {d...}};
    }

    template<class T, class... Desc>
    constexpr auto
    creater(char const * name, Desc... d)
    {
        return creator<T, ctx_c_str_name, Desc...>{{name}, {d...}};
    }

    template<class... Desc>
    constexpr auto
    composer(Desc... d)
    {
        using subtype = compose_t<desc_type_t<var_type_t<Desc>>...>;
        return creator<subtype, ctx_vars_name<Desc...>, Desc...>{{}, {d...}};
    }

    template<class... Desc>
    constexpr auto
    composer(char const * name, Desc... d)
    {
        using subtype = compose_t<desc_type_t<var_type_t<Desc>>...>;
        return creator<subtype, ctx_c_str_name, Desc...>{{name}, {d...}};
    }

    template<class... Desc>
    constexpr auto
    desc(Desc... d)
    {
        return packet_description<Desc...>{d...};
    }

    template<class T> struct is_proto_packet : std::false_type {};
    template<class... Ts>
    struct is_proto_packet<packet<Ts...>> : std::true_type {};

    namespace detail
    {
        template<class F, class... Pkts>
        void apply_impl(F & f, Pkts const & ... pkts)
        { f(pkts...); }

        template<class Pkt>
        std::enable_if_t<is_proto_packet<Pkt>::value, Pkt const &>
        get_if_packet(Pkt const & pkt)
        { return pkt; }
    }

    template<class F, class... Pkts>
    void apply(F f, Pkts const & ... pkts)
    { f(detail::get_if_packet(pkts)...); }


    namespace dsl
    {
        template<class T>
        struct value
        {
            template<class Params>
            constexpr value to_proto_value(Params) const
            {
                return *this;
            }

            T x;
        };

#define PROTO_LAZY_BINARY_OP(op)                                  \
        template<class T, class U>                                \
        struct op##_                                              \
        {                                                         \
            using arguments = brigand::append<                    \
                get_arguments_t<T>,                               \
                get_arguments_t<U>                                \
            >;                                                    \
                                                                  \
            template<class Params>                                \
            constexpr auto                                        \
            to_proto_value(Params p) const                        \
            {                                                     \
                auto val =                                        \
                    x.to_proto_value(p).x.val                     \
                    op y.to_proto_value(p).x.val;                 \
                return value<types::value<decltype(val)>>{{val}}; \
            }                                                     \
                                                                  \
            T x;                                                  \
            U y;                                                  \
        }

        PROTO_LAZY_BINARY_OP(bitand);

#undef PROTO_LAZY_BINARY_OP

#define PROTO_LAZY_BINARY_OP(op)                        \
        template<class T, class U>                      \
        struct op##_                                    \
        {                                               \
            using arguments = brigand::append<          \
                get_arguments_t<T>,                     \
                get_arguments_t<U>                      \
            >;                                          \
                                                        \
            template<class Params>                      \
            constexpr auto                              \
            to_proto_value(Params p) const              \
            {                                           \
                auto ret = x.to_proto_value(p);         \
                ret.x.val op y.to_proto_value(p).x.val; \
                return ret;                             \
            }                                           \
                                                        \
            T x;                                        \
            U y;                                        \
        }

        PROTO_LAZY_BINARY_OP(and_eq);

#undef PROTO_LAZY_BINARY_OP

        template<class Var>
        struct param
        {
            using arguments = brigand::list<Var>;

            template<class Params>
            constexpr decltype(auto) to_proto_value(Params p) const
            {
                return p.get_proto_value(Var{});
            }
        };
    }

    template<class T, class U>
    dsl::bitand_<dsl::param<T>, dsl::value<types::value<U>>>
    constexpr operator & (dsl::param<T>, U && x)
    { return {{}, {{x}}}; }

    template<class T, class U>
    dsl::and_eq_<dsl::param<T>, dsl::value<types::value<U>>>
    constexpr operator &= (dsl::param<T>, U && x)
    { return {{}, {{x}}}; }

    template<class Sp, class Desc>
    struct special
    {
        using arguments = brigand::list<>;

        constexpr static char const * name() { return "proto::sz"; }

        struct lazy
        {
            using var_type = lazy;
            using desc_type = Desc;
            using arguments = brigand::list<Sp>;

            template<class Params>
            constexpr Desc to_proto_value(Params p) const
            {
                return Desc{checked_cast<typename Desc::type>(p.get_proto_value(Sp{}).x())};
            }

            static char const * name() noexcept { return "pkt_sz"; }
        };

        template<class Params>
        constexpr lazy to_proto_value(Params) const
        {
            return {};
        }
    };

    namespace dsl
    {
        struct pkt_sz {};
        struct pkt_sz_with_self {};
        struct pkt_data {};
    }

    template<class Desc>
    using sz = special<dsl::pkt_sz, Desc>;

    template<class Desc>
    using sz_with_self = special<dsl::pkt_sz_with_self, Desc>;

    template<class Desc>
    using data = special<dsl::pkt_data, Desc>;


    namespace types {
        using ::proto::sz;
        using ::proto::sz_with_self;
        using ::proto::data;
    }

    namespace detail
    {
        template<> struct is_special_value_impl<dsl::pkt_sz> : std::true_type {};
        template<> struct is_special_value_impl<dsl::pkt_sz_with_self> : std::true_type {};
        template<> struct is_special_value_impl<dsl::pkt_data> : std::true_type {};

        template<class T> struct is_special_sz_impl : std::false_type {};
        template<> struct is_special_sz_impl<dsl::pkt_sz> : std::true_type {};
        template<> struct is_special_sz_impl<dsl::pkt_sz_with_self> : std::true_type {};

        template<class T> struct is_special_data_impl : std::false_type {};
        template<> struct is_special_data_impl<dsl::pkt_data> : std::true_type {};
    }

    template<class T>
    using has_special_sz = brigand::any<
        get_arguments_t<T>,
        brigand::call<detail::is_special_sz_impl>
    >;


    constexpr struct params_
    {
        constexpr params_() noexcept {}

        template<class Desc, class Derived>
        constexpr dsl::param<Derived> operator[](var<Desc, Derived>) const noexcept
        {
            return {};
        }
    } params;

    namespace detail
    {
        template<class Val, class ValElse>
        struct if_else
        {
            using sizeof_ = proto::common_size<proto::sizeof_<Val>, proto::sizeof_<ValElse>>;
            using buffer_category = proto::common_buffer<
                proto::buffer_category<Val>,
                proto::buffer_category<ValElse>
            >;

            std::size_t static_serialize(uint8_t * p) const
            {
                return this->is_ok
                    ? this->val_ok.static_serialize(p)
                    : this->val_fail.static_serialize(p);
            }

            std::size_t limited_serialize(uint8_t * p) const
            {
                return this->is_ok
                    ? static_or_limited_serialize(p, this->val_ok)
                    : static_or_limited_serialize(p, this->val_fail);
            }

            array_view_const_u8 get_view_buffer() const
            {
                return this->is_ok
                    ? this->val_ok.get_view_buffer()
                    : this->val_fail.get_view_buffer();
            }

            template<class F>
            void dynamic_serialize(F && f) const
            {
                // TODO { static or limited | view | dynamic } => dynamic_adapter
                return this->is_ok
                    ? this->val_ok.dynamic_serialize(f)
                    : this->val_fail.dynamic_serialize(f);
            }

            bool is_ok;
            Val val_ok;
            ValElse val_fail;
        };

        template<class Cond, class Var, class VarElse>
        struct if_else_act
        {
            using arguments = brigand::append<
                get_arguments_t<Cond>,
                get_arguments_t<Var>,
                get_arguments_t<VarElse>
            >;

            template<class Params>
            constexpr auto to_proto_value(Params params) const
            {
                auto value = this->var.to_proto_value(params);
                using proto_val = decltype(value);

                auto value_else = this->var_else.to_proto_value(params);
                using proto_val_else = decltype(value_else);

                using is_same = std::is_same<
                    typename proto_val::value_type,
                    typename proto_val_else::value_type
                >;

                static_assert(!is_special_value<decltype(cond.to_proto_value(params))>::value, "unimplemented special value with if_else");
                static_assert(!is_special_value<decltype(value)>::value, "unimplemented special value with if_else");
                static_assert(!is_special_value<decltype(value_else)>::value, "unimplemented special value with if_else");

                return to_proto_value_(
                    typename is_same::type{},
                    bool(cond.to_proto_value(params).x.val),
                    value,
                    value_else
                );
            }

            Cond cond;
            Var var;
            VarElse var_else;

        private:
            template<class Value>
            constexpr auto to_proto_value_(
                std::true_type, bool test, Value & value, Value & value_else
            ) const {
                return test ? std::move(value) : std::move(value_else);
            }

            template<class Value, class ValueElse>
            constexpr auto to_proto_value_(
                std::true_type, bool test, Value & value, ValueElse & value_else
            ) const {
                auto ctx_name = get_ctx_name(value, value_else);
                using new_var_type = named_var<Value, decltype(ctx_name)>;
                return val<new_var_type, Value>{
                    ctx_name,
                    {test ? std::move(value.x) : std::move(value_else.x)}
                };
            }

            template<class Value, class ValueElse>
            constexpr auto to_proto_value_(
                std::false_type, bool test, Value & value, ValueElse & value_else
            ) const {
                using proto_val = decltype(value);
                using proto_val_else = decltype(value_else);

                using new_value_type = if_else<
                    typename proto_val::value_type,
                    typename proto_val_else::value_type
                >;

                auto ctx_name = get_ctx_name(value, value_else);
                using new_var_type = named_var<new_value_type, decltype(ctx_name)>;
                return val<new_var_type, new_value_type>{
                    ctx_name,
                    {test, std::move(value.x), std::move(value_else.x)}
                };
            }

            template<class Value, class ValueElse>
            static constexpr auto
            get_ctx_name(Value & value, ValueElse & value_else)
            {
                return get_ctx_name_(
                    typename std::is_same<typename Value::var_type, typename ValueElse::var_type>::type{},
                    value, value_else
                );
            }

            template<class Value, class ValueElse>
            static constexpr auto
            get_ctx_name_(std::true_type, Value & value, ValueElse &)
            {
                return value.var;
            }

            template<class Value, class ValueElse>
            static constexpr ctx_vars_name<typename Value::var_type, typename ValueElse::var_type>
            get_ctx_name_(std::false_type, Value &, ValueElse &)
            {
                return {};
            }
        };

        template<class Val>
        struct only_if_true
        {
            using sizeof_ = proto::common_size<proto::sizeof_<Val>, limited_size<0>>;
            using buffer_category = typename std::conditional_t<
                is_view_buffer<Val>::value,
                proto::detail::buffer_category_impl<Val>,
                proto::detail::common_buffer_impl<
                    tags::limited_buffer,
                    proto::buffer_category<Val>
                >
            >::type;
            using is_reserializer = proto::is_reserializer<Val>;

            std::size_t limited_serialize(uint8_t * p) const
            {
                return this->is_ok ? static_or_limited_serialize(p, this->val_ok) : 0u;
            }

            template<class AV>
            std::size_t limited_reserialize(uint8_t * p, AV av) const
            {
                return this->is_ok ? static_or_limited_reserialize(p, this->val_ok, av) : 0u;
            }

            array_view_const_u8 get_view_buffer() const
            {
                return this->is_ok ? this->val_ok.get_view_buffer() : array_view_const_u8{};
            }

            template<class F>
            void dynamic_serialize(F && f) const
            {
                return (this->is_ok) ? this->val_ok.dynamic_serialize(f) : f();
            }

            bool is_ok;
            Val val_ok;
        };

        template<class Var>
        struct lazy_only_if_true
        {
            using var_type = lazy_only_if_true;
            using desc_type = only_if_true<desc_type_t<Var>>;
            using arguments = get_arguments_t<Var>;
            using is_reserializer = proto::is_reserializer<desc_type_t<Var>>;

            bool is_ok;
            Var var;

            template<class Params>
            constexpr auto
            to_proto_value(Params params) const
            {
                return only_if_true<decltype(this->var.to_proto_value(params))>{
                    this->is_ok,
                    this->var.to_proto_value(params)
                };
            }

            std::size_t reserved_size() const
            {
                static_assert(proto::is_static_buffer<desc_type_t<Var>>{}, "unimplemented");
                return this->is_ok ? proto::sizeof_<desc_type_t<Var>>::value : 0u;
            }

            static constexpr char const * name() noexcept
            { return "<unimplemented name>"; }
        };

        template<class Cond, class Var>
        struct if_act
        {
            using arguments = brigand::append<get_arguments_t<Cond>, get_arguments_t<Var>>;

            template<class Params>
            constexpr auto to_proto_value(Params params) const
            {
                return this->to_proto_value_(
                    this->else_.cond.to_proto_value(params),
                    this->else_.var.to_proto_value(params),
                    1
                );
            }

            struct
            {
                template<class VarElse>
                constexpr auto operator[](VarElse var_else) const
                {
                    return if_else_act<Cond, Var, VarElse>{cond, var, var_else};
                }

                Cond cond;
                Var var;
            } else_;

        private:
            template<class ValueCond, class Value, class D = std::enable_if_t<
                (!is_special_value<Value>::value and !is_special_value<ValueCond>::value),
                char
            >>
            constexpr auto to_proto_value_(ValueCond cond, Value value, D) const
            {
                using new_value_type = only_if_true<typename Value::value_type>;
                using context_name_type = typename Value::var_type;
                using new_var_type = named_var<new_value_type, context_name_type>;
                return val<new_var_type, new_value_type>{
                    {value.var},
                    {bool(cond.x.val), value.x}
                };
            }

            template<class ValueCond, class Value>
            constexpr std::enable_if_t<
                (is_special_value<Value>::value or is_special_value<ValueCond>::value),
                lazy_only_if_true<Value>
            >
            to_proto_value_(ValueCond cond, Value value, int) const
            {
                static_assert(!is_special_value<ValueCond>::value, "unimplemented");
                return {bool(cond.x.val), value};
            }
        };

        template<class Cond>
        struct if_
        {
            template<class Var>
            constexpr auto operator[](Var var) const
            {
                return if_act<Cond, Var>{{cond, var}};
            }

            Cond cond;
        };
    }

    template<class Cond>
    constexpr auto if_(Cond cond)
    {
        return detail::if_<Cond>{cond};
    }

    template<class Var>
    constexpr auto if_true(Var v)
    {
        return if_(dsl::param<Var>{})[v];
    }
}