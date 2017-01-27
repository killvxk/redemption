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
*   Author(s): Jonathan Poelen;
*
*   Based on Konsole, an X terminal
*/

#include <array>
#include <cstdint>
#include <type_traits>
#include <memory>

namespace utils
{
    template<std::size_t N, class T>
    constexpr std::integral_constant<std::size_t, N>
    size(T (&)[N])
    { return {}; }

    template<class T>
    using underlying_type = typename std::underlying_type<T>::type;

    template<class E>
    constexpr underlying_type<E>
    underlying(E e)
    {
        static_assert(std::is_enum<E>::value, "must be a enum");
        return static_cast<underlying_type<E>>(e);
    }
}


// namespace vt100_old
// {
//     namespace charsets
//     {
//         //"""
//         //    pyte.charsets
//         //    ~~~~~~~~~~~~~
//         //
//         //    This module defines ``G0`` and ``G1`` charset mappings the same way
//         //    they are defined for linux terminal, see
//         //    ``linux/drivers/tty/consolemap.c`` @ http://git.kernel.org
//         //
//         //    .. note:: ``VT100_MAP`` and ``IBMPC_MAP`` were taken unchanged
//         //            from linux kernel source and therefore are licensed
//         //            under **GPL**.
//         //
//         //    :copyright: (c) 2011-2012 by Selectel.
//         //    :copyright: (c) 2012-2016 by pyte authors and contributors,
//         //                    see AUTHORS for details.
//         //    :license: LGPL, see LICENSE for more details.
//         //"""
//
//         constexpr std::size_t charset_map_size = 256;
//         using charset_map_type = uint16_t[charset_map_size];
//
//         // TODO char const * ? 0x1234 -> "\u1234"
//
//         // Latin1
//         constexpr uint16_t LAT1_map[]{
//             0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
//             0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
//             0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
//             0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f,
//             0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
//             0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
//             0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
//             0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
//             0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
//             0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
//             0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
//             0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
//             0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
//             0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
//             0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
//             0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x007f,
//             0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
//             0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,
//             0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
//             0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,
//             0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
//             0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
//             0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
//             0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
//             0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
//             0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
//             0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,
//             0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
//             0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
//             0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
//             0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,
//             0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff,
//         };
//         static_assert(utils::size(LAT1_map) == charset_map_size, "");
//
//         // VT100 graphic character set
//         constexpr uint16_t VT100_map[]{
//             0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
//             0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
//             0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
//             0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f,
//             0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
//             0x0028, 0x0029, 0x002a, 0x2192, 0x2190, 0x2191, 0x2193, 0x002f,
//             0x2588, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
//             0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
//             0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
//             0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
//             0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
//             0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x00a0,
//             0x25c6, 0x2592, 0x2409, 0x240c, 0x240d, 0x240a, 0x00b0, 0x00b1,
//             0x2591, 0x240b, 0x2518, 0x2510, 0x250c, 0x2514, 0x253c, 0x23ba,
//             0x23bb, 0x2500, 0x23bc, 0x23bd, 0x251c, 0x2524, 0x2534, 0x252c,
//             0x2502, 0x2264, 0x2265, 0x03c0, 0x2260, 0x00a3, 0x00b7, 0x007f,
//             0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
//             0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,
//             0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
//             0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,
//             0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,
//             0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,
//             0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,
//             0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,
//             0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
//             0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
//             0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,
//             0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,
//             0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
//             0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
//             0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,
//             0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff,
//         };
//         static_assert(utils::size(VT100_map) == charset_map_size, "");
//
//         // IBM Codepage 437
//         constexpr uint16_t IBMPC_map[]{
//             0x0000, 0x263a, 0x263b, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022,
//             0x25d8, 0x25cb, 0x25d9, 0x2642, 0x2640, 0x266a, 0x266b, 0x263c,
//             0x25b6, 0x25c0, 0x2195, 0x203c, 0x00b6, 0x00a7, 0x25ac, 0x21a8,
//             0x2191, 0x2193, 0x2192, 0x2190, 0x221f, 0x2194, 0x25b2, 0x25bc,
//             0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
//             0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
//             0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
//             0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
//             0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
//             0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
//             0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
//             0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
//             0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
//             0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
//             0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
//             0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x2302,
//             0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7,
//             0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
//             0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9,
//             0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
//             0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba,
//             0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
//             0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
//             0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
//             0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
//             0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
//             0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
//             0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
//             0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4,
//             0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
//             0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248,
//             0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0,
//         };
//         static_assert(utils::size(IBMPC_map) == charset_map_size, "");
//
//
//         // VAX42 character set.
//         constexpr uint16_t VAX42_map[]{
//             0x0000, 0x263a, 0x263b, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022,
//             0x25d8, 0x25cb, 0x25d9, 0x2642, 0x2640, 0x266a, 0x266b, 0x263c,
//             0x25b6, 0x25c0, 0x2195, 0x203c, 0x00b6, 0x00a7, 0x25ac, 0x21a8,
//             0x2191, 0x2193, 0x2192, 0x2190, 0x221f, 0x2194, 0x25b2, 0x25bc,
//             0x0020, 0x043b, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
//             0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
//             0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
//             0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x0435,
//             0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
//             0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
//             0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
//             0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
//             0x0060, 0x0441, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
//             0x0435, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x043a,
//             0x0070, 0x0071, 0x0442, 0x0073, 0x043b, 0x0435, 0x0076, 0x0077,
//             0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x2302,
//             0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7,
//             0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
//             0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9,
//             0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
//             0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba,
//             0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
//             0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
//             0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
//             0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
//             0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
//             0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
//             0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
//             0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4,
//             0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
//             0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248,
//             0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0
//         };
//         static_assert(utils::size(VAX42_map) == charset_map_size, "");
//
//
// //         constexpr charset_map_type maps[] {
// //             /*'B':*/ LAT1_map,
// //             /*'0':*/ VT100_map,
// //             /*'U':*/ IBMPC_map,
// //             /*'V':*/ VAX42_map,
// //         };
//     }
//
//
//     //"""
//     //    pyte.control
//     //    ~~~~~~~~~~~~
//     //
//     //    This module defines simple control sequences, recognized by
//     //    :class:`~pyte.streams.Stream`, the set of codes here is for
//     //    ``TERM=linux`` which is a superset of VT102.
//     //
//     //    :copyright: (c) 2011-2012 by Selectel.
//     //    :copyright: (c) 2012-2016 by pyte authors and contributors,
//     //                    see AUTHORS for details.
//     //    :license: LGPL, see LICENSE for more details.
//     //"""
//
//     enum class controls : uint8_t
//     {
//         // *Space*: Not suprisingly -- ``" "``.
//         SP = ' ',
//
//         // *Null*: Does nothing.
//         NUL = 0x00,
//
//         // *Bell*: Beeps.
//         BEL = 0x07,
//
//         // *Backspace*: Backspace one column, but not past the begining of the
//         // line.
//         BS = 0x08,
//
//         // *Horizontal tab*: Move cursor to the next tab stop, or to the end
//         // of the line if there is no earlier tab stop.
//         HT = 0x09,
//
//         // *Linefeed*: Give a line feed, and, if :data:`pyte.modes.LNM` (new
//         // line mode) is set also a carriage return.
//         LF = '\n',
//         // *Vertical tab*: Same as :data:`LF`.
//         VT = 0x0b,
//         // *Form feed*: Same as :data:`LF`.
//         FF = 0x0c,
//
//         // *Carriage return*: Move cursor to left margin on current line.
//         CR = '\r',
//
//         // *Shift out*: Activate G1 character set.
//         SO = 0x0e,
//
//         // *Shift in*: Activate G0 character set.
//         SI = 0x0f,
//
//         // *Cancel*: Interrupt escape sequence. If received during an escape or
//         // control sequence, cancels the sequence and displays substitution
//         // character.
//         CAN = 0x18,
//         // *Substitute*: Same as :data:`CAN`.
//         SUB = 0x1a,
//
//         // *Escape*: Starts an escape sequence.
//         ESC = 0x1b,
//
//         // *Delete*: Is ignored.
//         DEL = 0x7f,
//
//         // *Control sequence introducer*: An equivalent for ``ESC [``.
//         CSI = 0x9b,
//
//         // *String terminator*.
//         ST = 0x9c,
//
//         // *Operating system command*.
//         OSC = 0x9d,
//     };
//
//
//     //"""
//     //    pyte.escape
//     //    ~~~~~~~~~~~
//     //
//     //    This module defines both CSI and non-CSI escape sequences, recognized
//     //    by :class:`~pyte.streams.Stream` and subclasses.
//     //
//     //    :copyright: (c) 2011-2012 by Selectel.
//     //    :copyright: (c) 2012-2016 by pyte authors and contributors,
//     //                    see AUTHORS for details.
//     //    :license: LGPL, see LICENSE for more details.
//     //"""
//
//     enum class escapes : uint8_t
//     {
//         // *Reset*.
//         RIS = 'c',
//
//         // *Index*: Move cursor down one line in same column. If the cursor is
//         // at the bottom margin, the screen performs a scroll-up.
//         IND = 'D',
//
//         // *Next line*: Same as :data:`pyte.control.LF`.
//         NEL = 'E',
//
//         // Tabulation set: Set a horizontal tab stop at cursor position.
//         HTS = 'H',
//
//         // *Reverse index*: Move cursor up one line in same column. If the
//         // cursor is at the top margin, the screen performs a scroll-down.
//         RI = 'M',
//
//         // Save cursor: Save cursor position, character attribute (graphic
//         // rendition), character set, and origin mode selection (see
//         // :data:`DECRC`).
//         DECSC = '7',
//
//         // *Restore cursor*: Restore previously saved cursor position, character
//         // attribute (graphic rendition), character set, and origin mode
//         // selection. If none were saved, move cursor to home position.
//         DECRC = '8',
//
//         // "Sharp" escape sequences.
//         // -------------------------
//
//         // *Alignment display*: Fill screen with uppercase E's for testing
//         // screen focus and alignment.
//         DECALN = '8',
//
//
//         // ECMA-48 CSI sequences.
//         // ---------------------
//
//         // *Insert character*: Insert the indicated # of blank characters.
//         ICH = '@',
//
//         // *Cursor up*: Move cursor up the indicated # of lines in same column.
//         // Cursor stops at top margin.
//         CUU = 'A',
//
//         // *Cursor down*: Move cursor down the indicated # of lines in same
//         // column. Cursor stops at bottom margin.
//         CUD = 'B',
//
//         // *Cursor forward*: Move cursor right the indicated # of columns.
//         // Cursor stops at right margin.
//         CUF = 'C',
//
//         // *Cursor back*: Move cursor left the indicated # of columns. Cursor
//         // stops at left margin.
//         CUB = 'D',
//
//         // *Cursor next line*: Move cursor down the indicated # of lines to
//         // column 1.
//         CNL = 'E',
//
//         // *Cursor previous line*: Move cursor up the indicated # of lines to
//         // column 1.
//         CPL = 'F',
//
//         // *Cursor horizontal align*: Move cursor to the indicated column in
//         // current line.
//         CHA = 'G',
//
//         // *Cursor position*: Move cursor to the indicated line, column (origin
//         // at ``1, 1``).
//         CUP = 'H',
//
//         // *Erase data* (default: from cursor to end of line).
//         ED = 'J',
//
//         // *Erase in line* (default: from cursor to end of line).
//         EL = 'K',
//
//         // *Insert line*: Insert the indicated # of blank lines, starting from
//         // the current line. Lines displayed below cursor move down. Lines moved
//         // past the bottom margin are lost.
//         IL = 'L',
//
//         // *Delete line*: Delete the indicated # of lines, starting from the
//         // current line. As lines are deleted, lines displayed below cursor
//         // move up. Lines added to bottom of screen have spaces with same
//         // character attributes as last line move up.
//         DL = 'M',
//
//         // *Delete character*: Delete the indicated # of characters on the
//         // current line. When character is deleted, all characters to the right
//         // of cursor move left.
//         DCH = 'P',
//
//         // *Erase character*: Erase the indicated # of characters on the
//         // current line.
//         ECH = 'X',
//
//         // *Horizontal position relative*: Same as :data:`CUF`.
//         HPR = 'a',
//
//         // *Device Attributes*.
//         DA = 'c',
//
//         // *Vertical position adjust*: Move cursor to the indicated line,
//         // current column.
//         VPA = 'd',
//
//         // *Vertical position relative*: Same as :data:`CUD`.
//         VPR = 'e',
//
//         // *Horizontal / Vertical position*: Same as :data:`CUP`.
//         HVP = 'f',
//
//         // *Tabulation clear*: Clears a horizontal tab stop at cursor position.
//         TBC = 'g',
//
//         // *Set mode*.
//         SM = 'h',
//
//         // *Reset mode*.
//         RM = 'l',
//
//         // *Select graphics rendition*: The terminal can display the following
//         // character attributes that change the character display without
//         // changing the character (see :mod:`pyte.graphics`).
//         SGR = 'm',
//
//         // *Device status report*.
//         DSR = 'n',
//
//         // *Select top and bottom margins*: Selects margins, defining the
//         // scrolling region; parameters are top and bottom line. If called
//         // without any arguments, whole screen is used.
//         DECSTBM = 'r',
//
//         // *Horizontal position adjust*: Same as :data:`CHA`.
//         HPA = '\'',
//     };
//
//     namespace graphics
//     {
//         enum class text : uint8_t
//         {
//             add_bold = 1,
//             add_italics = 3,
//             add_underscore = 4,
//             add_reverse = 7,
//             add_strikethrough = 9,
//             sub_bold = 22,
//             sub_italics = 23,
//             sub_underscore = 24,
//             sub_reverse = 27,
//             sub_strikethrough = 29,
//         };
//
//         enum class fg_ansi : uint8_t
//         {
//             black = 30,
//             red = 31,
//             green = 32,
//             brown = 33,
//             blue = 34,
//             magenta = 35,
//             cyan = 36,
//             white = 37,
//             default_ = 39, // white
//         };
//
//         enum class fg_aixterm : uint8_t
//         {
//             black = 90,
//             red = 91,
//             green = 92,
//             brown = 93,
//             blue = 94,
//             magenta = 95,
//             cyan = 96,
//             white = 97,
//         };
//
//         enum class bg_ansi : uint8_t
//         {
//             black = 40,
//             red = 41,
//             green = 42,
//             brown = 43,
//             blue = 44,
//             magenta = 45,
//             cyan = 46,
//             white = 47,
//             default_ = 49, // white
//         };
//
//         enum class bg_aixterm : uint8_t
//         {
//             black = 100,
//             red = 101,
//             green = 102,
//             brown = 103,
//             blue = 104,
//             magenta = 105,
//             cyan = 106,
//             white = 107,
//         };
//
//         // SGR code for foreground in 256 or True color mode.
//         constexpr uint8_t fg_256 = 38;
//         // SGR code for background in 256 or True color mode.
//         constexpr uint8_t bg_256 = 48;
//
//         // A table of 256 foreground or background colors.
//         // The following code is part of the Pygments project (BSD licensed).
//         //constexpr uint8_t fg_bg_256[][3] {
//         //    {0x00, 0x00, 0x00},  // 0
//         //    {0xcd, 0x00, 0x00},  // 1
//         //    {0x00, 0xcd, 0x00},  // 2
//         //    {0xcd, 0xcd, 0x00},  // 3
//         //    {0x00, 0x00, 0xee},  // 4
//         //    {0xcd, 0x00, 0xcd},  // 5
//         //    {0x00, 0xcd, 0xcd},  // 6
//         //    {0xe5, 0xe5, 0xe5},  // 7
//         //    {0x7f, 0x7f, 0x7f},  // 8
//         //    {0xff, 0x00, 0x00},  // 9
//         //    {0x00, 0xff, 0x00},  // 10
//         //    {0xff, 0xff, 0x00},  // 11
//         //    {0x5c, 0x5c, 0xff},  // 12
//         //    {0xff, 0x00, 0xff},  // 13
//         //    {0x00, 0xff, 0xff},  // 14
//         //    {0xff, 0xff, 0xff},  // 15
//         //};
//         //
//         //// colors 16..232: the 6x6x6 color cube
//         //uint8_t valuerange[] = {0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff};
//         //
//         //# colors 16..232: the 6x6x6 color cube
//         //valuerange = (0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff)
//         //
//         //for i in range(217):
//         //    r = valuerange[(i // 36) % 6]
//         //    g = valuerange[(i // 6) % 6]
//         //    b = valuerange[i % 6]
//         //    FG_BG_256.append((r, g, b))
//         //
//         //# colors 233..253: grayscale
//         //for i in range(1, 22):
//         //    v = 8 + i * 10
//         //    FG_BG_256.append((v, v, v))
//         //
//         //FG_BG_256 = ["{0:02x}{1:02x}{2:02x}".format(r, g, b) for r, g, b in FG_BG_256]
//     }
//
//
//     //"""
//     //    pyte.modes
//     //    ~~~~~~~~~~
//     //
//     //    This module defines terminal mode switches, used by
//     //    :class:`~pyte.screens.Screen`. There're two types of terminal modes:
//     //
//     //    * `non-private` which should be set with ``ESC [ N h``, where ``N``
//     //    is an integer, representing mode being set; and
//     //    * `private` which should be set with ``ESC [ ? N h``.
//     //
//     //    The latter are shifted 5 times to the right, to be easily
//     //    distinguishable from the former ones; for example `Origin Mode`
//     //    -- :data:`DECOM` is ``192`` not ``6``.
//     //
//     //    >>> DECOM
//     //    192
//     //
//     //    :copyright: (c) 2011-2012 by Selectel.
//     //    :copyright: (c) 2012-2016 by pyte authors and contributors,
//     //                    see AUTHORS for details.
//     //    :license: LGPL, see LICENSE for more details.
//     //"""
//
//     enum class mode_t : uint16_t
//     {
//         // *Line Feed/New Line Mode*: When enabled, causes a received
//         // :data:`~pyte.control.LF`, :data:`pyte.control.FF`, or
//         // :data:`~pyte.control.VT` to move the cursor to the first column of
//         // the next line.
//         LNM = 20,
//
//         // *Insert/Replace Mode*: When enabled, new display characters move
//         // old display characters to the right. Characters moved past the
//         // right margin are lost. Otherwise, new display characters replace
//         // old display characters at the cursor position.
//         IRM = 4,
//
//
//         // Private modes.
//         // ..............
//
//         // *Text Cursor Enable Mode*: determines if the text cursor is
//         // visible.
//         DECTCEM = 25 << 5,
//
//         // *Screen Mode*: toggles screen-wide reverse-video mode.
//         DECSCNM = 5 << 5,
//
//         // *Origin Mode*: allows cursor addressing relative to a user-defined
//         // origin. This mode resets when the terminal is powered up or reset.
//         // It does not affect the erase in display (ED) function.
//         DECOM = 6 << 5,
//
//         // *Auto Wrap Mode*: selects where received graphic characters appear
//         // when the cursor is at the right margin.
//         DECAWM = 7 << 5,
//
//         // *Column Mode*: selects the number of columns per line (80 or 132)
//         // on the screen.
//         DECCOLM = 3 << 5,
//
//         origin = DECOM,
//     };
//
//     struct Margins
//     {
//         int top;
//         int bottom;
//     };
//
//     struct SavePoint
//     {
// //     "cursor",
// //     "g0_charset",
// //     "g1_charset",
// //     "charset",
// //     "use_utf8",
// //     "origin",
// //     "wrap"
//     };
//
//     struct Char
//     {
//         uint8_t data; // TODO u32 ?
//         uint32_t fg;
//         uint32_t bg;
//         // TODO bitset
//         bool bold;
//         bool itealics;
//         bool underscore;
//         bool strikethrough;
//         bool reverse;
//     };
//
//     struct Cursor
//     {
//         int x;
//         int y;
//         Char attrs;
//         bool hidden;
//     };
//
//     bool operator == (Cursor const & c1, Cursor const & c2)
//     { return c1.x == c2.x && c1.y == c2.y; }
//
//     struct noncopyable
//     {
//         noncopyable() = default;
//         noncopyable(noncopyable &&) = delete;
//         noncopyable(noncopyable const &) = delete;
//         noncopyable & operator = (noncopyable &&) = delete;
//         noncopyable & operator = (noncopyable const &) = delete;
//     };
//
//     struct Screen
//     {
//         struct ScreenBuffer : noncopyable
//         {
//             ScreenBuffer(int lines, int columns)
//             : lines_(lines)
//             , columns_(columns)
//             , top_line_(0)
//             , data_(new Char[lines * columns]{})
//             {}
//
//             ~ScreenBuffer()
//             { delete[] data_; }
//
//             int lines_;
//             int columns_;
//             int top_line_;
//             Char * data_; // TODO vector ?
//         } buffer_;
//         Margins margins_;
//         SavePoint save_point_;
//         Cursor cursor_;
//
//         // bitset mode
//         struct Modes
//         {
//             Modes() = default;
//
//             bool operator[](mode_t m) const
//             { return bool(m_ & utils::underlying(m)); }
//
//             int m_ = 0;
//         } modes_;
//
//         Screen(int lines, int columns)
//         : buffer_(lines, columns)
//         , margins_{0, lines - 1}
//         , cursor_{}
//         , modes_{}
//         {}
//
//         int top_margin() const { return margins_.top; }
//         int bottom_margin() const { return margins_.bottom; }
//
//         /*
//         CUU – Cursor Up – Host to VT100 and VT100 to Host
//
//         ESC [ Pn A        default value: 1
//
//         Moves the active position upward without altering the column
//         position. The number of lines moved is determined by the parameter. A
//         parameter value of zero or one moves the active position one line
//         upward. A parameter value of n moves the active position n lines
//         upward. If an attempt is made to move the cursor above the top margin,
//         the cursor stops at the top margin. Editor Function
//         */
//         void cursor_up(int n)
//         {
//             const int stop = cursor_.y < margins_.top ? 0 : margins_.top;
//             cursor_.x = std::min(buffer_.columns_ - 1, cursor_.x); // nowrap!
//             cursor_.y = std::max(stop, cursor_.y - n);
//         }
//
//         /*
//         CUD – Cursor Down – Host to VT100 and VT100 to Host
//
//         ESC [ Pn B        default value: 1
//
//         The CUD sequence moves the active position downward without altering
//         the column position. The number of lines moved is determined by the
//         parameter. If the parameter value is zero or one, the active position
//         is moved one line downward. If the parameter value is n, the active
//         position is moved n lines downward. In an attempt is made to move the
//         cursor below the bottom margin, the cursor stops at the bottom
//         margin. Editor Function
//         */
//         void cursor_down(int n)
//         {
//             const int stop = cursor_.y > margins_.bottom ? buffer_.lines_ - 1 : margins_.bottom;
//             cursor_.x = std::min(buffer_.columns_ - 1, cursor_.x); // nowrap!
//             cursor_.y = std::min(stop, cursor_.y + n);
//         }
//
//         /*
//         CUB – Cursor Backward – Host to VT100 and VT100 to Host
//
//         ESC [ Pn D        default value: 1
//
//         The CUB sequence moves the active position to the left. The distance
//         moved is determined by the parameter. If the parameter value is zero
//         or one, the active position is moved one position to the left. If the
//         parameter value is n, the active position is moved n positions to the
//         left. If an attempt is made to move the cursor to the left of the left
//         margin, the cursor stops at the left margin. Editor Function
//         */
//         void cursor_left(int n)
//         {
//             cursor_.x = std::min(buffer_.columns_ - 1, cursor_.x); // nowrap!
//             cursor_.x = std::max(0, cursor_.x - n);
//         }
//
//         /*
//         CUF – Cursor Forward – Host to VT100 and VT100 to Host
//
//         ESC [ Pn C        default value: 1
//
//         The CUF sequence moves the active position to the right. The distance
//         moved is determined by the parameter. A parameter value of zero or one
//         moves the active position one position to the right. A parameter value
//         of n moves the active position n positions to the right. If an attempt
//         is made to move the cursor to the right of the right margin, the
//         cursor stops at the right margin. Editor Function
//         */
//         void cursor_right(int n)
//         {
//             cursor_.x = std::min(buffer_.columns_ - 1, cursor_.x + n);
//         }
//
//         /*
//         DECSTBM – Set Top and Bottom Margins (DEC Private)
//
//         ESC [ Pn; Pn r
//
//         This sequence sets the top and bottom margins to define the scrolling
//         region. The first parameter is the line number of the first line in
//         the scrolling region; the second parameter is the line number of the
//         bottom line in the scrolling region. Default is the entire screen (no
//         margins). The minimum size of the scrolling region allowed is two
//         lines, i.e., the top margin must be less than the bottom margin. The
//         cursor is placed in the home position (see Origin Mode DECOM).
//
//         */
//         void set_margins(int top, int bot)
//         {
//             if (top == 0) top = 1;      // Default
//             if (bot == 0) bot = buffer_.lines_;  // Default
//             top = top - 1;              // Adjust to internal lineno
//             bot = bot - 1;              // Adjust to internal lineno
//             if (!(0 <= top && top < bot && bot < buffer_.lines_)) {
//                 //Debug()<<" setRegion("<<top<<","<<bot<<") : bad range.";
//                 return;                   // Default error action: ignore
//             }
//             margins_.top = top;
//             margins_.bottom = bot;
//             cursor_.x = 0;
//             cursor_.y = modes_[mode_t::origin] ? top : 0;
//         }
//
//
//         /*
//         IND – Index
//
//         ESC D
//
//         This sequence causes the active position to move downward one line
//         without changing the column position. If the active position is at the
//         bottom margin, a scroll up is performed. Format Effector
//         */
//         void index()
//         {
//             if (_cuY == _bottomMargin) {
//                 scroll_up(1);
//             }
//             else if (_cuY < _lines - 1) {
//                 _cuY += 1;
//             }
//         }
//
//         /*
//         RI – Reverse Index
//
//         ESC M
//
//         Move the active position to the same horizontal position on the
//         preceding line. If the active position is at the top margin, a scroll
//         down is performed. Format Effector
//         */
//         void reverse_index()
//         {
//             if (_cuY == _topMargin) {
//                 scrollDown(_topMargin, 1);
//             }
//             else if (_cuY > 0) {
//                 _cuY -= 1;
//             }
//         }
//
//         /*
//         NEL – Next Line
//
//         ESC E
//
//         This sequence causes the active position to move to the first position
//         on the next line downward. If the active position is at the bottom
//         margin, a scroll up is performed. Format Effector
//         */
//         void next_line()
//         {
//             toStartOfLine();
//             index();
//         }
//
//         void scroll_up(int from, int n)
//         {
//             if (n <= 0 || from + n > _bottomMargin) return;
//
//             _scrolledLines -= n;
//             _lastScrolledRegion = Rect(0, _topMargin, _columns - 1, (_bottomMargin - _topMargin));
//
//             //FIXME: make sure `topMargin', `bottomMargin', `from', `n' is in bounds.
//             moveImage(loc(0, from), loc(0, from + n), loc(_columns - 1, _bottomMargin));
//             clearImage(loc(0, _bottomMargin - n + 1), loc(_columns - 1, _bottomMargin), ' ');
//         }
//
//     };
// }

#include <vector>
#include <string>

#include <cassert>
#include <cstring> // memset

#include "utils/rect.hpp"
#include "utils/sugar/make_unique.hpp"
#include "cxx/diagnostic.hpp"

#define CXX_UNUSED(x) (void)x // [[maybe_unused]]

#define REDEMPTION_DIAGNOSTIC_IGNORE_CONVERSION REDEMPTION_DIAGNOSTIC_GCC_IGNORE("-Wconversion")

namespace vt100
{

// std::clamp (c++17)
template<class T>
constexpr inline const T &
clamp(const T & min, const T & val, const T & max)
{ return std::max(min, std::min(max, val)); }

struct Color
{
    constexpr Color() noexcept = default;

    constexpr Color(uint8_t r, uint8_t g, uint8_t b) noexcept
    : r(r)
    , g(g)
    , b(b)
    {}

    constexpr uint8_t red() const noexcept { return r; }
    constexpr uint8_t green() const noexcept { return g; }
    constexpr uint8_t blue() const noexcept { return b; }

private:
    // TODO uin32_t ?
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

constexpr bool operator == (Color const & c1, Color const & c2) noexcept
{
    return c1.red() == c2.red()
        && c1.green() == c2.green()
        && c1.blue() == c2.blue();
}

constexpr bool operator != (Color const & c1, Color const & c2) noexcept
{ return !operator==(c1, c2); }

/**
 * An entry in a terminal display's color palette.
 *
 * A color palette is an array of 16 ColorEntry instances which map
 * system color indexes (from 0 to 15) into actual colors.
 *
 * Each entry can be set as bold, in which case any text
 * drawn using the color should be drawn in bold.
 */
class ColorEntry
{
public:
    /** Specifies the weight to use when drawing text with this color. */
    enum FontWeight {
        /** Always draw text in this color with a bold weight. */
        Bold,
        /** Always draw text in this color with a normal weight. */
        Normal,
        /**
         * Use the current font weight set by the terminal application.
         * This is the default behavior.
         */
        UseCurrentFormat
    };

    /**
     * Constructs a new color palette entry.
     *
     * @param c The color value for this entry.
     * @param weight Specifies the font weight to use when drawing text with this color.
     */
    explicit ColorEntry(Color c,  FontWeight weight = UseCurrentFormat)
        : color(c), fontWeight(weight) {}

    /**
     * Constructs a new color palette entry using black color and font weight set to
     * use current format.
     */
    ColorEntry() : color(Color(0x00, 0x00, 0x00)), fontWeight(UseCurrentFormat) {}

    /**
     * Sets the color and boldness of this color to those of @p rhs.
     */
    void operator=(const ColorEntry& rhs) {
        color = rhs.color;
        fontWeight = rhs.fontWeight;
    }

    /** The color value of this entry for display. */
    Color color;

    /**
     * Specifies the font weight to use when drawing text with this color.
     * This is not applicable when the color is used to draw a character's background.
     */
    FontWeight fontWeight;

    /**
     * Compares two color entries and returns true if they represent the same
     * color and font weight.
     */
    friend bool operator == (const ColorEntry& a, const ColorEntry& b);
    /**
     * Compares two color entries and returns true if they represent different
     * color and font weight.
     */
    friend bool operator != (const ColorEntry& a, const ColorEntry& b);
};

inline bool operator == (const ColorEntry& a, const ColorEntry& b)
{
    return  a.color == b.color &&
            a.fontWeight == b.fontWeight;
}

inline bool operator != (const ColorEntry& a, const ColorEntry& b)
{
    return !operator==(a, b);
}

// Attributed Character Representations ///////////////////////////////

// Colors

constexpr int BASE_COLORS = 2+8;
constexpr int INTENSITIES = 2;
constexpr int TABLE_COLORS = INTENSITIES * BASE_COLORS;

constexpr int DEFAULT_FORE_COLOR = 0;
constexpr int DEFAULT_BACK_COLOR = 1;

/* CharacterColor is a union of the various color spaces.

   Assignment is as follows:

   Type  - Space        - Values

   0     - Undefined   - u:  0,      v:0        w:0
   1     - Default     - u:  0..1    v:intense  w:0
   2     - System      - u:  0..7    v:intense  w:0
   3     - Index(256)  - u: 16..255  v:0        w:0
   4     - RGB         - u:  0..255  v:0..255   w:0..255

   Default color space has two separate colors, namely
   default foreground and default background color.
*/

enum class ColorSpace : uint8_t
{
    Undefined   = 0,
    Default     = 1,
    System      = 2,
    Index256    = 3,
    RGB         = 4,
};

/**
 * Describes the color of a single character in the terminal.
 */
class CharacterColor
{
    friend class Character;

public:
    /** Constructs a new CharacterColor whose color and color space are undefined. */
    CharacterColor()
    : _colorSpace(ColorSpace::Undefined)
    {}

    /**
     * Constructs a new CharacterColor using the specified @p colorSpace and with
     * color value @p co
     *
     * The meaning of @p co depends on the @p colorSpace used.
     *
     * TODO : Document how @p co relates to @p colorSpace
     *
     * TODO : Add documentation about available color spaces.
     */
    CharacterColor(ColorSpace colorSpace, int co)
    : _colorSpace(colorSpace)
    {
        REDEMPTION_DIAGNOSTIC_PUSH
        REDEMPTION_DIAGNOSTIC_IGNORE_CONVERSION
        switch (colorSpace) {
        case ColorSpace::Default:
            _u = co & 1;
            break;
        case ColorSpace::System:
            _u = co & 7;
            _v = (co >> 3) & 1;
            break;
        case ColorSpace::Index256:
            _u = co & 255;
            break;
        case ColorSpace::RGB:
            _u = co >> 16;
            _v = co >> 8;
            _w = co;
            break;
        case ColorSpace::Undefined:
            break;
        }
        REDEMPTION_DIAGNOSTIC_POP
    }

    /**
     * Returns true if this character color entry is valid.
     */
    bool isValid() const {
        return _colorSpace != ColorSpace::Undefined;
    }

    /**
     * Set this color as an intensive system color.
     *
     * This is only applicable if the color is using the ColorSpace::Default or ColorSpace::System
     * color spaces.
     */
    void setIntensive();

    /**
     * Returns the color within the specified color @p palette
     *
     * The @p palette is only used if this color is one of the 16 system colors, otherwise
     * it is ignored.
     */
    Color color(const ColorEntry* palette) const;

    /**
     * Compares two colors and returns true if they represent the same color value and
     * use the same color space.
     */
    friend bool operator == (const CharacterColor& a, const CharacterColor& b);
    /**
     * Compares two colors and returns true if they represent different color values
     * or use different color spaces.
     */
    friend bool operator != (const CharacterColor& a, const CharacterColor& b);

private:
    ColorSpace _colorSpace;

    // bytes storing the character color
    uint8_t _u = 0;
    uint8_t _v = 0;
    uint8_t _w = 0;
};

inline bool operator == (const CharacterColor& a, const CharacterColor& b)
{
    return     a._colorSpace == b._colorSpace &&
               a._u == b._u &&
               a._v == b._v &&
               a._w == b._w;
}
inline bool operator != (const CharacterColor& a, const CharacterColor& b)
{
    return !operator==(a, b);
}

inline const Color color256(uint8_t u, const ColorEntry* base)
{
    REDEMPTION_DIAGNOSTIC_PUSH
    REDEMPTION_DIAGNOSTIC_IGNORE_CONVERSION
    //   0.. 16: system colors
    if (u < 8) {
        return base[u + 2].color;
    }
    u -= 8;
    if (u < 8) {
        return base[u + 2 + BASE_COLORS].color;
    }
    u -= 8;

    //  16..231: 6x6x6 rgb color cube
    if (u < 216) {
        return Color(((u / 36) % 6) ? (40 * ((u / 36) % 6) + 55) : 0,
                     ((u / 6) % 6) ? (40 * ((u / 6) % 6) + 55) : 0,
                     ((u / 1) % 6) ? (40 * ((u / 1) % 6) + 55) : 0);
    }
    u -= 216;

    // 232..255: gray, leaving out black and white
    uint8_t gray = u * 10 + 8;
    REDEMPTION_DIAGNOSTIC_POP

    return Color(gray, gray, gray);
}

inline Color CharacterColor::color(const ColorEntry* base) const
{
    switch (_colorSpace) {
    case ColorSpace::Default:
        return base[_u + 0 + (_v ? BASE_COLORS : 0)].color;
    case ColorSpace::System:
        return base[_u + 2 + (_v ? BASE_COLORS : 0)].color;
    case ColorSpace::Index256:
        return color256(_u, base);
    case ColorSpace::RGB:
        return Color(_u, _v, _w);
    case ColorSpace::Undefined:
        return Color();
    }

    assert(false); // invalid color space

    return Color();
}

inline void CharacterColor::setIntensive()
{
    if (_colorSpace == ColorSpace::System || _colorSpace == ColorSpace::Default) {
        _v = 1;
    }
}

namespace detail_
{
    template<bool>
    struct enable_if_impl
    {
        template<class T> using type = T;
    };

    template<>
    struct enable_if_impl<false>;
}

template<bool v, class R = void>
using enable_if = typename detail_::enable_if_impl<v>::template type<R>;

template<class V, class R = void>
using enable_if_c = enable_if<V::value, R>;

template<class T>
struct is_enum_flags : std::false_type
{};

template<class T>
using enable_if_enum_flags = enable_if_c<is_enum_flags<T>, T>;

template<class T>
using underlying_type = typename std::underlying_type<T>::type;

template<class T>
constexpr
underlying_type<T>
underlying_cast(T x) noexcept
{ return static_cast<underlying_type<T>>(x); }

template<class T>
constexpr
enable_if_enum_flags<T>
operator ~ (T x) noexcept
{ return T(~underlying_cast(x)); }

template<class T>
constexpr
enable_if_enum_flags<T>
operator & (T x, T y) noexcept
{ return T(underlying_cast(x) & underlying_cast(y)); }

template<class T>
constexpr
enable_if_enum_flags<T>
operator | (T x, T y) noexcept
{ return T(underlying_cast(x) | underlying_cast(y)); }

template<class T>
// constexpr c++14
enable_if_enum_flags<T> &
operator &= (T & x, T y) noexcept
{ return x = T(underlying_cast(x) & underlying_cast(y)); }

template<class T>
// constexpr c++14
enable_if_enum_flags<T> &
operator |= (T & x, T y) noexcept
{ return x = T(underlying_cast(x) | underlying_cast(y)); }


enum class LineProperty : uint8_t
{
    Default      = 0,
    Wrapped      = (1 << 0),
    DoubleWidth  = (1 << 1),
    DoubleHeight = (1 << 2),
};

template<> struct is_enum_flags<LineProperty> : std::true_type {};

enum class Rendition : uint8_t
{
    Default       = 0,
    Bold          = (1 << 0),
    Blink         = (1 << 1),
    Underline     = (1 << 2),
    Reverse       = (1 << 3), // Screen only
    //Intensive     = (1 << 3), // Widget only
    Italic        = (1 << 4),
    Cursor        = (1 << 5),
    ExtendedChar  = (1 << 6),
};

template<> struct is_enum_flags<Rendition> : std::true_type {};


/**
 * Unicode character in the range of U+2500 ~ U+257F are known as line
 * characters, or box-drawing characters. Currently, konsole draws those
 * characters itself, instead of using the glyph provided by the font.
 * Unfortunately, the triple and quadruple dash lines (┄┅┆┇┈┉┊┋) are too
 * detailed too be drawn cleanly at normal font scales without anti
 * -aliasing, so those are drawn as regular characters.
 */
inline bool isSupportedLineChar(uint16_t codePoint)
{
    return (codePoint & 0xFF80) == 0x2500 // Unicode block: Mathematical Symbols - Box Drawing
           && !(0x2504 <= codePoint && codePoint <= 0x250B); // Triple and quadruple dash range
}

/**
 * A single character in the terminal which consists of a unicode character
 * value, foreground and background colors and a set of rendition attributes
 * which specify how it should be drawn.
 */
class Character
{
public:
    /**
     * Constructs a new character.
     *
     * @param _c The unicode character value of this character.
     * @param _f The foreground color used to draw the character.
     * @param _b The color used to draw the character's background.
     * @param _r A set of rendition flags which specify how this character
     *           is to be drawn.
     * @param _real Indicate whether this character really exists, or exists
     *              simply as place holder.
     */
    explicit inline Character(uint16_t _c = ' ',
                              CharacterColor  _f = CharacterColor(ColorSpace::Default, DEFAULT_FORE_COLOR),
                              CharacterColor  _b = CharacterColor(ColorSpace::Default, DEFAULT_BACK_COLOR),
                              Rendition  _r = Rendition::Default,
                              bool _real = true)
        : character(_c)
        , rendition(_r)
        , foregroundColor(_f)
        , backgroundColor(_b)
        , isRealCharacter(_real) { }

    /** The unicode character value for this character.
     *
     * if Rendition::ExtendedChar is set, character is a hash code which can be used to
     * look up the unicode character sequence in the ExtendedCharTable used to
     * create the sequence.
     */
    uint16_t character;

    /** A combination of RENDITION flags which specify options for drawing the character. */
    Rendition rendition;

    /** The foreground color used to draw this character. */
    CharacterColor  foregroundColor;

    /** The color used to draw this character's background. */
    CharacterColor  backgroundColor;

    /** Indicate whether this character really exists, or exists simply as place holder.
     *
     *  TODO: this boolean filed can be further improved to become a enum filed, which
     *  indicates different roles:
     *
     *    RealCharacter: a character which really exists
     *    PlaceHolderCharacter: a character which exists as place holder
     *    TabStopCharacter: a special place holder for HT("\t")
     */
    bool isRealCharacter;

    /**
     * Returns true if this character should always be drawn in bold when
     * it is drawn with the specified @p palette, independent of whether
     * or not the character has the Rendition::Bold rendition flag.
     */
    ColorEntry::FontWeight fontWeight(const ColorEntry* base) const;

    /**
     * returns true if the format (color, rendition flag) of the compared characters is equal
     */
    bool equalsFormat(const Character& other) const;

    /**
     * Compares two characters and returns true if they have the same unicode character value,
     * rendition and colors.
     */
    friend bool operator == (const Character& a, const Character& b);

    /**
     * Compares two characters and returns true if they have different unicode character values,
     * renditions or colors.
     */
    friend bool operator != (const Character& a, const Character& b);

    inline bool isLineChar() const {
        if (bool(rendition & Rendition::ExtendedChar)) {
            return false;
        } else {
            return isSupportedLineChar(character);
        }
    }
};

inline bool operator == (const Character& a, const Character& b)
{
    return a.character == b.character && a.equalsFormat(b);
}

inline bool operator != (const Character& a, const Character& b)
{
    return !operator==(a, b);
}

inline bool Character::equalsFormat(const Character& other) const
{
    return backgroundColor == other.backgroundColor &&
           foregroundColor == other.foregroundColor &&
           rendition == other.rendition;
}

inline ColorEntry::FontWeight Character::fontWeight(const ColorEntry* base) const
{
    if (foregroundColor._colorSpace == ColorSpace::Default)
        return base[foregroundColor._u + 0 + (foregroundColor._v ? BASE_COLORS : 0)].fontWeight;
    else if (foregroundColor._colorSpace == ColorSpace::System)
        return base[foregroundColor._u + 2 + (foregroundColor._v ? BASE_COLORS : 0)].fontWeight;
    else
        return ColorEntry::UseCurrentFormat;
}

struct Mode {
    enum enum_t /*: uint8_t*/ {
        Origin,
        Wrap,
        Insert,
        Screen,
        Cursor,
        NewLine,
        COUNT_
    };
};

/**
    \brief An image of characters with associated attributes.

    The terminal emulation ( Emulation ) receives a serial stream of
    characters from the program currently running in the terminal.
    From this stream it creates an image of characters which is ultimately
    rendered by the display widget ( TerminalDisplay ).  Some types of emulation
    may have more than one screen image.

    getImage() is used to retrieve the currently visible image
    which is then used by the display widget to draw the output from the
    terminal.

    The number of lines of output history which are kept in addition to the current
    screen image depends on the history scroll being used to store the output.
    The scroll is specified using setScroll()
    The output history can be retrieved using writeToStream()

    The screen image has a selection associated with it, specified using
    setSelectionStart() and setSelectionEnd().  The selected text can be retrieved
    using selectedText().  When getImage() is used to retrieve the visible image,
    characters which are part of the selection have their colors inverted.
*/
class Screen
{
public:
    /** Construct a new screen image of size @p lines by @p columns. */
    Screen(int lines, int columns);
    ~Screen();

    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

    // VT100/2 Operations
    // Cursor Movement

    /**
     * Move the cursor up by @p n lines.  The cursor will stop at the
     * top margin.
     */
    void cursorUp(int n);
    /**
     * Move the cursor down by @p n lines.  The cursor will stop at the
     * bottom margin.
     */
    void cursorDown(int n);
    /**
     * Move the cursor to the left by @p n columns.
     * The cursor will stop at the first column.
     */
    void cursorLeft(int n);
    /**
     * Move the cursor to the right by @p n columns.
     * The cursor will stop at the right-most column.
     */
    void cursorRight(int n);
    /** Position the cursor on line @p y. */
    void setCursorY(int y);
    /** Position the cursor at column @p x. */
    void setCursorX(int x);
    /** Position the cursor at line @p y, column @p x. */
    void setCursorYX(int y, int x);
    /**
     * Sets the margins for scrolling the screen.
     *
     * @param topLine The top line of the new scrolling margin.
     * @param bottomLine The bottom line of the new scrolling margin.
     */
    void setMargins(int topLine , int bottomLine);
    /** Returns the top line of the scrolling region. */
    int topMargin() const;
    /** Returns the bottom line of the scrolling region. */
    int bottomMargin() const;

    /**
     * Resets the scrolling margins back to the top and bottom lines
     * of the screen.
     */
    void setDefaultMargins();

    /**
     * Moves the cursor down one line, if the Mode::NewLine mode
     * flag is enabled then the cursor is returned to the leftmost
     * column first.
     *
     * Equivalent to NextLine() if the Mode::NewLine flag is set
     * or index() otherwise.
     */
    void newLine();
    /**
     * Moves the cursor down one line and positions it at the beginning
     * of the line.  Equivalent to calling Return() followed by index()
     */
    void nextLine();

    /**
     * Move the cursor down one line.  If the cursor is on the bottom
     * line of the scrolling region (as returned by bottomMargin()) the
     * scrolling region is scrolled up by one line instead.
     */
    void index();
    /**
     * Move the cursor up one line.  If the cursor is on the top line
     * of the scrolling region (as returned by topMargin()) the scrolling
     * region is scrolled down by one line instead.
     */
    void reverseIndex();

    /**
     * Scroll the scrolling region of the screen up by @p n lines.
     * The scrolling region is initially the whole screen, but can be changed
     * using setMargins()
     */
    void scrollUp(int n);
    /**
     * Scroll the scrolling region of the screen down by @p n lines.
     * The scrolling region is initially the whole screen, but can be changed
     * using setMargins()
     */
    void scrollDown(int n);
    /**
     * Moves the cursor to the beginning of the current line.
     * Equivalent to setCursorX(0)
     */
    void toStartOfLine();
    /**
     * Moves the cursor one column to the left and erases the character
     * at the new cursor position.
     */
    void backspace();
    /** Moves the cursor @p n tab-stops to the right. */
    void tab(int n = 1);
    /** Moves the cursor @p n tab-stops to the left. */
    void backtab(int n);

    // Editing

    /**
     * Erase @p n characters beginning from the current cursor position.
     * This is equivalent to over-writing @p n characters starting with the current
     * cursor position with spaces.
     * If @p n is 0 then one character is erased.
     */
    void eraseChars(int n);
    /**
     * Delete @p n characters beginning from the current cursor position.
     * If @p n is 0 then one character is deleted.
     */
    void deleteChars(int n);
    /**
     * Insert @p n blank characters beginning from the current cursor position.
     * The position of the cursor is not altered.
     * If @p n is 0 then one character is inserted.
     */
    void insertChars(int n);
    /**
     * Removes @p n lines beginning from the current cursor position.
     * The position of the cursor is not altered.
     * If @p n is 0 then one line is removed.
     */
    void deleteLines(int n);
    /**
     * Inserts @p lines beginning from the current cursor position.
     * The position of the cursor is not altered.
     * If @p n is 0 then one line is inserted.
     */
    void insertLines(int n);
    /** Clears all the tab stops. */
    void clearTabStops();
    /**  Sets or removes a tab stop at the cursor's current column. */
    void changeTabStop(bool set);

    /** Resets (clears) the specified screen @p mode. */
    void resetMode(int mode);
    /** Sets (enables) the specified screen @p mode. */
    void setMode(int mode);
    /**
     * Saves the state of the specified screen @p mode.  It can be restored
     * using restoreMode()
     */
    void saveMode(int mode);
    /** Restores the state of a screen @p mode saved by calling saveMode() */
    void restoreMode(int mode);
    /** Returns whether the specified screen @p mode is enabled or not .*/
    bool getMode(int mode) const;

    /**
     * Saves the current position and appearance (text color and style) of the cursor.
     * It can be restored by calling restoreCursor()
     */
    void saveCursor();
    /** Restores the position and appearance of the cursor.  See saveCursor() */
    void restoreCursor();

    /** Clear the whole screen, moving the current screen contents into the history first. */
    void clearEntireScreen();
    /**
     * Clear the area of the screen from the current cursor position to the end of
     * the screen.
     */
    void clearToEndOfScreen();
    /**
     * Clear the area of the screen from the current cursor position to the start
     * of the screen.
     */
    void clearToBeginOfScreen();
    /** Clears the whole of the line on which the cursor is currently positioned. */
    void clearEntireLine();
    /** Clears from the current cursor position to the end of the line. */
    void clearToEndOfLine();
    /** Clears from the current cursor position to the beginning of the line. */
    void clearToBeginOfLine();

    /** Fills the entire screen with the letter 'E' */
    void helpAlign();

    /**
     * Enables the given @p rendition flag.  Rendition flags control the appearance
     * of characters on the screen.
     *
     * @see Character::rendition
     */
    void setRendition(Rendition rendition);
    /**
     * Disables the given @p rendition flag.  Rendition flags control the appearance
     * of characters on the screen.
     *
     * @see Character::rendition
     */
    void resetRendition(Rendition rendition);

    /**
     * Sets the cursor's foreground color.
     * @param space The color space used by the @p color argument
     * @param color The new foreground color.  The meaning of this depends on
     * the color @p space used.
     *
     * @see CharacterColor
     */
    void setForeColor(ColorSpace space, int color);
    /**
     * Sets the cursor's background color.
     * @param space The color space used by the @p color argument.
     * @param color The new background color.  The meaning of this depends on
     * the color @p space used.
     *
     * @see CharacterColor
     */
    void setBackColor(ColorSpace space, int color);
    /**
     * Resets the cursor's color back to the default and sets the
     * character's rendition flags back to the default settings.
     */
    void setDefaultRendition();

    /** Returns the column which the cursor is positioned at. */
    int  getCursorX() const;
    /** Returns the line which the cursor is positioned on. */
    int  getCursorY() const;

    /** Clear the entire screen and move the cursor to the home position.
     * Equivalent to calling clearEntireScreen() followed by home().
     */
    void clear();
    /**
     * Sets the position of the cursor to the 'home' position at the top-left
     * corner of the screen (0,0)
     */
    void home();
    /**
     * Resets the state of the screen.  This resets the various screen modes
     * back to their default states.  The cursor style and colors are reset
     * (as if setDefaultRendition() had been called)
     *
     * <ul>
     * <li>Line wrapping is enabled.</li>
     * <li>Origin mode is disabled.</li>
     * <li>Insert mode is disabled.</li>
     * <li>Cursor mode is enabled.  TODO Document me</li>
     * <li>Screen mode is disabled. TODO Document me</li>
     * <li>New line mode is disabled.  TODO Document me</li>
     * </ul>
     *
     * If @p clearScreen is true then the screen contents are erased entirely,
     * otherwise they are unaltered.
     */
    void reset(bool clearScreen = true);

    /**
     * Displays a new character at the current cursor position.
     *
     * If the cursor is currently positioned at the right-edge of the screen and
     * line wrapping is enabled then the character is added at the start of a new
     * line below the current one.
     *
     * If the MODE_Insert screen mode is currently enabled then the character
     * is inserted at the current cursor position, otherwise it will replace the
     * character already at the current cursor position.
     */
    void displayCharacter(unsigned short c);

    /**
     * Resizes the image to a new fixed size of @p new_lines by @p new_columns.
     * In the case that @p new_columns is smaller than the current number of columns,
     * existing lines are not truncated.  This prevents characters from being lost
     * if the terminal display is resized smaller and then larger again.
     *
     * The top and bottom margins are reset to the top and bottom of the new
     * screen size.  Tab stops are also reset and the current selection is
     * cleared.
     */
    void resizeImage(int new_lines, int new_columns);

    /** Return the number of lines. */
    int getLines() const {
        return _lines;
    }
    /** Return the number of columns. */
    int getColumns() const {
        return _columns;
    }

    /**
     * Sets or clears an attribute of the current line.
     *
     * @param property The attribute to set or clear
     * Possible properties are:
     * LINE_WRAPPED:     Specifies that the line is wrapped.
     * LINE_DOUBLEWIDTH: Specifies that the characters in the current line
     *                   should be double the normal width.
     * LINE_DOUBLEHEIGHT:Specifies that the characters in the current line
     *                   should be double the normal height.
     *                   Double-height lines are formed of two lines containing the same characters,
     *                   with both having the LINE_DOUBLEHEIGHT attribute.
     *                   This allows other parts of the code to work on the
     *                   assumption that all lines are the same height.
     *
     * @param enable true to apply the attribute to the current line or false to remove it
     */
    void setLineProperty(LineProperty property , bool enable);

    /**
     * Returns the number of lines that the image has been scrolled up or down by,
     * since the last call to resetScrolledLines().
     *
     * a positive return value indicates that the image has been scrolled up,
     * a negative return value indicates that the image has been scrolled down.
     */
    int scrolledLines() const;

    /**
     * Returns the region of the image which was last scrolled.
     *
     * This is the area of the image from the top margin to the
     * bottom margin when the last scroll occurred.
     */
    Rect lastScrolledRegion() const;

    /**
     * Resets the count of the number of lines that the image has been scrolled up or down by,
     * see scrolledLines()
     */
    void resetScrolledLines();

    /**
      * Fills the buffer @p dest with @p count instances of the default (ie. blank)
      * Character style.
      */
    static void fillWithDefaultChar(Character* dest, int count);

    static const Character DefaultChar;

    int cursor_x() const { return _cuX; }
    int cursor_y() const { return _cuY; }

private:
    inline int loc(int x, int y) { return x * _columns + y; }

    //fills a section of the screen image with the character 'c'
    //the parameters are specified as offsets from the start of the screen image.
    //the loc(x,y) macro can be used to generate these values from a column,line pair.
    void clearImage(int loca, int loce, char c);

    //move screen image between 'sourceBegin' and 'sourceEnd' to 'dest'.
    //the parameters are specified as offsets from the start of the screen image.
    //the loc(x,y) macro can be used to generate these values from a column,line pair.
    //
    //NOTE: moveImage() can only move whole lines
    void moveImage(int dest, int sourceBegin, int sourceEnd);
    // scroll up 'i' lines in current region, clearing the bottom 'i' lines
    void scrollUp(int from, int i);
    // scroll down 'i' lines in current region, clearing the top 'i' lines
    void scrollDown(int from, int i);

    void initTabStops();

    void updateEffectiveRendition();
    void reverseRendition(Character& p) const;

    // screen image ----------------
    int _lines;
    int _columns;

    typedef std::vector<Character> ImageLine;      // [0..columns]
#ifdef IN_IDE_PARSER
    ImageLine* _screenLines;    // [lines]
#else
    std::vector<ImageLine> _screenLines;    // [lines]
# endif
    int _screenLinesSize;                // _screenLines.size()

public:    // TODO
    std::vector<ImageLine> const & getScreenLines() const { return _screenLines;  }

private:
    int _scrolledLines;
    Rect _lastScrolledRegion;

    std::vector<LineProperty> _lineProperties; // QVarLengthArray<LineProperty, 64>

    // cursor location
    int _cuX;
    int _cuY;

    // cursor color and rendition info
    CharacterColor _currentForeground;
    CharacterColor _currentBackground;
    Rendition _currentRendition;

    // margins ----------------
    int _topMargin;
    int _bottomMargin;

    // states ----------------
    int _currentModes[Mode::COUNT_];
    int _savedModes[Mode::COUNT_];

    // ----------------------------

    std::vector<bool> _tabStops; // TODO QBitArray

    // selection -------------------
    int _selBegin; // The first location selected.
    int _selTopLeft;    // TopLeft Location.
    int _selBottomRight;    // Bottom Right Location.
    bool _blockSelectionMode;  // Column selection mode

    // effective colors and rendition ------------
    CharacterColor _effectiveForeground; // These are derived from
    CharacterColor _effectiveBackground; // the cu_* variables above
    Rendition _effectiveRendition;          // to speed up operation

    class SavedState
    {
    public:
        SavedState()
        : cursorColumn(0), cursorLine(0), rendition(Rendition::Default) {}

        int cursorColumn;
        int cursorLine;
        Rendition rendition;
        CharacterColor foreground;
        CharacterColor background;
    };
    SavedState _savedState;

    // last position where we added a character
    int _lastPos;
};


//FIXME: this is emulation specific. Use false for xterm, true for ANSI.
//FIXME: see if we can get this from terminfo.
const bool BS_CLEARS = false;

//Macro to convert x,y position on screen to position within an image.
//
//Originally the image was stored as one large contiguous block of
//memory, so a position within the image could be represented as an
//offset from the beginning of the block.  For efficiency reasons this
//is no longer the case.
//Many internal parts of this class still use this representation for parameters and so on,
//notably moveImage() and clearImage().
//This macro converts from an X,Y position into an image offset.

const Character Screen::DefaultChar = Character(' ',
                                      CharacterColor(ColorSpace::Default, DEFAULT_FORE_COLOR),
                                      CharacterColor(ColorSpace::Default, DEFAULT_BACK_COLOR),
                                      Rendition::Default,
                                      false);

Screen::Screen(int lines, int columns):
    _lines(lines),
    _columns(columns),
    _screenLines(_lines + 1),
    _screenLinesSize(_lines),
    _scrolledLines(0),
    _cuX(0),
    _cuY(0),
    _currentRendition(Rendition::Default),
    _topMargin(0),
    _bottomMargin(0),
    _selBegin(0),
    _selTopLeft(0),
    _selBottomRight(0),
    _blockSelectionMode(false),
    _effectiveForeground(CharacterColor()),
    _effectiveBackground(CharacterColor()),
    _effectiveRendition(Rendition::Default),
    _lastPos(-1)
{
    _lineProperties.resize(_lines + 1, LineProperty::Default);

    initTabStops();
    reset();
}

Screen::~Screen() = default;

void Screen::cursorUp(int n)
//=CUU
{
    if (n == 0) n = 1; // Default
    const int stop = _cuY < _topMargin ? 0 : _topMargin;
    _cuX = std::min(_columns - 1, _cuX); // nowrap!
    _cuY = std::max(stop, _cuY - n);
}

void Screen::cursorDown(int n)
//=CUD
{
    if (n == 0) n = 1; // Default
    const int stop = _cuY > _bottomMargin ? _lines - 1 : _bottomMargin;
    _cuX = std::min(_columns - 1, _cuX); // nowrap!
    _cuY = std::min(stop, _cuY + n);
}

void Screen::cursorLeft(int n)
//=CUB
{
    if (n == 0) n = 1; // Default
    _cuX = std::min(_columns - 1, _cuX); // nowrap!
    _cuX = std::max(0, _cuX - n);
}

void Screen::cursorRight(int n)
//=CUF
{
    if (n == 0) n = 1; // Default
    _cuX = std::min(_columns - 1, _cuX + n);
}

void Screen::setMargins(int top, int bot)
//=STBM
{
    if (top == 0) top = 1;      // Default
    if (bot == 0) bot = _lines;  // Default
    top = top - 1;              // Adjust to internal lineno
    bot = bot - 1;              // Adjust to internal lineno
    if (!(0 <= top && top < bot && bot < _lines)) {
        //Debug()<<" setRegion("<<top<<","<<bot<<") : bad range.";
        return;                   // Default error action: ignore
    }
    _topMargin = top;
    _bottomMargin = bot;
    _cuX = 0;
    _cuY = getMode(Mode::Origin) ? top : 0;
}

int Screen::topMargin() const
{
    return _topMargin;
}
int Screen::bottomMargin() const
{
    return _bottomMargin;
}

void Screen::index()
//=IND
{
    if (_cuY == _bottomMargin)
        scrollUp(1);
    else if (_cuY < _lines - 1)
        _cuY += 1;
}

void Screen::reverseIndex()
//=RI
{
    if (_cuY == _topMargin)
        scrollDown(_topMargin, 1);
    else if (_cuY > 0)
        _cuY -= 1;
}

void Screen::nextLine()
//=NEL
{
    toStartOfLine();
    index();
}

void Screen::eraseChars(int n)
{
    if (n == 0) n = 1; // Default
    const int p = std::max(0, std::min(_cuX + n - 1, _columns - 1));
    clearImage(loc(_cuX, _cuY), loc(p, _cuY), ' ');
}

void Screen::deleteChars(int n)
{
    assert(n >= 0);

    // always delete at least one char
    if (n == 0)
        n = 1;

    // if cursor is beyond the end of the line there is nothing to do
    if (_cuX >= int(_screenLines[_cuY].size()))
        return;

    if (_cuX + n > int(_screenLines[_cuY].size()))
        n = int(_screenLines[_cuY].size() - _cuX);

    assert(n >= 0);
    assert(_cuX + n <= int(_screenLines[_cuY].size()));

    auto pos = _screenLines[_cuY].begin() + _cuX;
    _screenLines[_cuY].erase(pos, pos + n);

    // Append space(s) with current attributes
    Character spaceWithCurrentAttrs(' ', _effectiveForeground,
                                    _effectiveBackground,
                                    _effectiveRendition, false);

    for (int i = 0; i < n; i++)
        _screenLines[_cuY].emplace_back(spaceWithCurrentAttrs);
}

void Screen::insertChars(int n)
{
    if (n == 0) n = 1; // Default

    if (int(_screenLines[_cuY].size()) < _cuX)
        _screenLines[_cuY].resize(_cuX);

    _screenLines[_cuY].insert(_screenLines[_cuY].begin() + _cuX, n, Character(' '));

    if (int(_screenLines[_cuY].size()) > _columns)
        _screenLines[_cuY].resize(_columns);
}

void Screen::deleteLines(int n)
{
    if (n == 0) n = 1; // Default
    scrollUp(_cuY, n);
}

void Screen::insertLines(int n)
{
    if (n == 0) n = 1; // Default
    scrollDown(_cuY, n);
}

void Screen::setMode(int m)
{
    _currentModes[m] = true;
    switch (m) {
    case Mode::Origin :
        _cuX = 0;
        _cuY = _topMargin;
        break; //FIXME: home
    }
}

void Screen::resetMode(int m)
{
    _currentModes[m] = false;
    switch (m) {
    case Mode::Origin :
        _cuX = 0;
        _cuY = 0;
        break; //FIXME: home
    }
}

void Screen::saveMode(int m)
{
    _savedModes[m] = _currentModes[m];
}

void Screen::restoreMode(int m)
{
    _currentModes[m] = _savedModes[m];
}

bool Screen::getMode(int m) const
{
    return _currentModes[m];
}

void Screen::saveCursor()
{
    _savedState.cursorColumn = _cuX;
    _savedState.cursorLine  = _cuY;
    _savedState.rendition = _currentRendition;
    _savedState.foreground = _currentForeground;
    _savedState.background = _currentBackground;
}

void Screen::restoreCursor()
{
    _cuX     = std::min(_savedState.cursorColumn, _columns - 1);
    _cuY     = std::min(_savedState.cursorLine, _lines - 1);
    _currentRendition   = _savedState.rendition;
    _currentForeground   = _savedState.foreground;
    _currentBackground   = _savedState.background;
    updateEffectiveRendition();
}

void Screen::resizeImage(int new_lines, int new_columns)
{
    if ((new_lines == _lines) && (new_columns == _columns)) return;

    if (_cuY > new_lines - 1) {
        // attempt to preserve focus and _lines
        _bottomMargin = _lines - 1; //FIXME: margin lost
        for (int i = 0; i < _cuY - (new_lines - 1); i++) {
            scrollUp(0, 1);
        }
    }

    // create new screen _lines and copy from old to new

    auto newScreenLines = std::vector<ImageLine>(new_lines + 1);
    for (int i = 0; i < std::min(_lines, new_lines + 1) ; i++)
        newScreenLines[i] = _screenLines[i];
    for (int i = _lines; (i > 0) && (i < new_lines + 1); i++)
        newScreenLines[i].resize(new_columns);

    _lineProperties.resize(new_lines + 1, LineProperty::Default);

    _screenLines = std::move(newScreenLines);
    _screenLinesSize = new_lines;

    _lines = new_lines;
    _columns = new_columns;
    _cuX = std::min(_cuX, _columns - 1);
    _cuY = std::min(_cuY, _lines - 1);

    // FIXME: try to keep values, evtl.
    _topMargin = 0;
    _bottomMargin = _lines - 1;
    initTabStops();
}

void Screen::setDefaultMargins()
{
    _topMargin = 0;
    _bottomMargin = _lines - 1;
}

/*
   Clarifying rendition here and in the display.

   currently, the display's color table is
   0       1       2 .. 9    10 .. 17
   dft_fg, dft_bg, dim 0..7, intensive 0..7

   _currentForeground, _currentBackground contain values 0..8;
   - 0    = default color
   - 1..8 = ansi specified color

   re_fg, re_bg contain values 0..17
   due to the TerminalDisplay's color table

   rendition attributes are

   attr           widget screen
   -------------- ------ ------
   Rendition::Underline     XX     XX    affects foreground only
   Rendition::Blink         XX     XX    affects foreground only
   Rendition::Bold          XX     XX    affects foreground only
   Rendition::Reverse       --     XX
   RE_TRANSPARENT   XX     --    affects background only
   RE_INTENSIVE     XX     --    affects foreground only

   Note that Rendition::Bold is used in both widget
   and screen rendition. Since xterm/vt102
   is to poor to distinguish between bold
   (which is a font attribute) and intensive
   (which is a color attribute), we translate
   this and Rendition::Bold in falls eventually apart
   into Rendition::Bold and RE_INTENSIVE.
   */

void Screen::reverseRendition(Character& p) const
{
    CharacterColor f = p.foregroundColor;
    CharacterColor b = p.backgroundColor;

    p.foregroundColor = b;
    p.backgroundColor = f; //p->r &= ~RE_TRANSPARENT;
}

void Screen::updateEffectiveRendition()
{
    _effectiveRendition = _currentRendition;
    if (bool(_currentRendition & Rendition::Reverse)) {
        _effectiveForeground = _currentBackground;
        _effectiveBackground = _currentForeground;
    } else {
        _effectiveForeground = _currentForeground;
        _effectiveBackground = _currentBackground;
    }

    if (bool(_currentRendition & Rendition::Bold))
        _effectiveForeground.setIntensive();
}

void Screen::reset(bool clearScreen)
{
    setMode(Mode::Wrap);
    saveMode(Mode::Wrap);      // wrap at end of margin

    resetMode(Mode::Origin);
    saveMode(Mode::Origin);  // position refer to [1,1]

    resetMode(Mode::Insert);
    saveMode(Mode::Insert);  // overstroke

    setMode(Mode::Cursor);                         // cursor visible
    resetMode(Mode::Screen);                         // screen not inverse
    resetMode(Mode::NewLine);

    _topMargin = 0;
    _bottomMargin = _lines - 1;

    setDefaultRendition();
    saveCursor();

    if (clearScreen)
        clear();
}

void Screen::clear()
{
    clearEntireScreen();
    home();
}

void Screen::backspace()
{
    _cuX = std::min(_columns - 1, _cuX); // nowrap!
    _cuX = std::max(0, _cuX - 1);

    if (int(_screenLines[_cuY].size()) < _cuX + 1)
        _screenLines[_cuY].resize(_cuX + 1);

    if (BS_CLEARS) {
        _screenLines[_cuY][_cuX].character = ' ';
        _screenLines[_cuY][_cuX].rendition = _screenLines[_cuY][_cuX].rendition & ~Rendition::ExtendedChar;
    }
}

void Screen::tab(int n)
{
    // note that TAB is a format effector (does not write ' ');
    if (n == 0) n = 1;
    while ((n > 0) && (_cuX < _columns - 1)) {
        cursorRight(1);
        while ((_cuX < _columns - 1) && !_tabStops[_cuX])
            cursorRight(1);
        n--;
    }
}

void Screen::backtab(int n)
{
    // note that TAB is a format effector (does not write ' ');
    if (n == 0) n = 1;
    while ((n > 0) && (_cuX > 0)) {
        cursorLeft(1);
        while ((_cuX > 0) && !_tabStops[_cuX]) {
            cursorLeft(1);
        }
        n--;
    }
}

void Screen::clearTabStops()
{
    for (int i = 0; i < _columns; i++)
        _tabStops[i] = false;
}

void Screen::changeTabStop(bool set)
{
    if (_cuX >= _columns)
        return;

    _tabStops[_cuX] = set;
}

void Screen::initTabStops()
{
    _tabStops.resize(_columns);

    // The 1st tabstop has to be one longer than the other.
    // i.e. the kids start counting from 0 instead of 1.
    // Other programs might behave correctly. Be aware.
    for (int i = 0; i < _columns; i++)
        _tabStops[i] = (i % 8 == 0 && i != 0);
}

void Screen::newLine()
{
    if (getMode(Mode::NewLine))
        toStartOfLine();

    index();
}


struct interval {
    unsigned long first;
    unsigned long last;
};

/* auxiliary function for binary search in interval table */
static int bisearch(unsigned long ucs, const struct interval* table, int max)
{
    int min = 0;
    int mid;

    if (ucs < table[0].first || ucs > table[max].last)
        return 0;
    while (max >= min) {
        mid = (min + max) / 2;
        if (ucs > table[mid].last)
            min = mid + 1;
        else if (ucs < table[mid].first)
            max = mid - 1;
        else
            return 1;
    }

    return 0;
}


/* The following functions define the column width of an ISO 10646
 * character as follows:
 *
 *    - The null character (U+0000) has a column width of 0.
 *
 *    - Other C0/C1 control characters and DEL will lead to a return
 *      value of -1.
 *
 *    - Non-spacing and enclosing combining characters (general
 *      category code Mn or Me in the Unicode database) have a
 *      column width of 0.
 *
 *    - Other format characters (general category code Cf in the Unicode
 *      database) and ZERO WIDTH SPACE (U+200B) have a column width of 0.
 *
 *    - Hangul Jamo medial vowels and final consonants (U+1160-U+11FF)
 *      have a column width of 0.
 *
 *    - Spacing characters in the East Asian Wide (W) or East Asian
 *      FullWidth (F) category as defined in Unicode Technical
 *      Report #11 have a column width of 2.
 *
 *    - All remaining characters (including all printable
 *      ISO 8859-1 and WGL4 characters, Unicode control characters,
 *      etc.) have a column width of 1.
 *
 * This implementation assumes that quint16 characters are encoded
 * in ISO 10646.
 */
int konsole_wcwidth(uint16_t oucs)
{
    /* NOTE: It is not possible to compare quint16 with the new last four lines of characters,
     * therefore this cast is now necessary.
     */
    unsigned long ucs = static_cast<unsigned long>(oucs);
    /* sorted list of non-overlapping intervals of non-spacing characters */
    /* generated by "uniset +cat=Me +cat=Mn +cat=Cf -00AD +1160-11FF +200B c" */
    static const struct interval combining[] = {
        { 0x0300, 0x036F }, { 0x0483, 0x0486 }, { 0x0488, 0x0489 },
        { 0x0591, 0x05BD }, { 0x05BF, 0x05BF }, { 0x05C1, 0x05C2 },
        { 0x05C4, 0x05C5 }, { 0x05C7, 0x05C7 }, { 0x0600, 0x0603 },
        { 0x0610, 0x0615 }, { 0x064B, 0x065E }, { 0x0670, 0x0670 },
        { 0x06D6, 0x06E4 }, { 0x06E7, 0x06E8 }, { 0x06EA, 0x06ED },
        { 0x070F, 0x070F }, { 0x0711, 0x0711 }, { 0x0730, 0x074A },
        { 0x07A6, 0x07B0 }, { 0x07EB, 0x07F3 }, { 0x0901, 0x0902 },
        { 0x093C, 0x093C }, { 0x0941, 0x0948 }, { 0x094D, 0x094D },
        { 0x0951, 0x0954 }, { 0x0962, 0x0963 }, { 0x0981, 0x0981 },
        { 0x09BC, 0x09BC }, { 0x09C1, 0x09C4 }, { 0x09CD, 0x09CD },
        { 0x09E2, 0x09E3 }, { 0x0A01, 0x0A02 }, { 0x0A3C, 0x0A3C },
        { 0x0A41, 0x0A42 }, { 0x0A47, 0x0A48 }, { 0x0A4B, 0x0A4D },
        { 0x0A70, 0x0A71 }, { 0x0A81, 0x0A82 }, { 0x0ABC, 0x0ABC },
        { 0x0AC1, 0x0AC5 }, { 0x0AC7, 0x0AC8 }, { 0x0ACD, 0x0ACD },
        { 0x0AE2, 0x0AE3 }, { 0x0B01, 0x0B01 }, { 0x0B3C, 0x0B3C },
        { 0x0B3F, 0x0B3F }, { 0x0B41, 0x0B43 }, { 0x0B4D, 0x0B4D },
        { 0x0B56, 0x0B56 }, { 0x0B82, 0x0B82 }, { 0x0BC0, 0x0BC0 },
        { 0x0BCD, 0x0BCD }, { 0x0C3E, 0x0C40 }, { 0x0C46, 0x0C48 },
        { 0x0C4A, 0x0C4D }, { 0x0C55, 0x0C56 }, { 0x0CBC, 0x0CBC },
        { 0x0CBF, 0x0CBF }, { 0x0CC6, 0x0CC6 }, { 0x0CCC, 0x0CCD },
        { 0x0CE2, 0x0CE3 }, { 0x0D41, 0x0D43 }, { 0x0D4D, 0x0D4D },
        { 0x0DCA, 0x0DCA }, { 0x0DD2, 0x0DD4 }, { 0x0DD6, 0x0DD6 },
        { 0x0E31, 0x0E31 }, { 0x0E34, 0x0E3A }, { 0x0E47, 0x0E4E },
        { 0x0EB1, 0x0EB1 }, { 0x0EB4, 0x0EB9 }, { 0x0EBB, 0x0EBC },
        { 0x0EC8, 0x0ECD }, { 0x0F18, 0x0F19 }, { 0x0F35, 0x0F35 },
        { 0x0F37, 0x0F37 }, { 0x0F39, 0x0F39 }, { 0x0F71, 0x0F7E },
        { 0x0F80, 0x0F84 }, { 0x0F86, 0x0F87 }, { 0x0F90, 0x0F97 },
        { 0x0F99, 0x0FBC }, { 0x0FC6, 0x0FC6 }, { 0x102D, 0x1030 },
        { 0x1032, 0x1032 }, { 0x1036, 0x1037 }, { 0x1039, 0x1039 },
        { 0x1058, 0x1059 }, { 0x1160, 0x11FF }, { 0x135F, 0x135F },
        { 0x1712, 0x1714 }, { 0x1732, 0x1734 }, { 0x1752, 0x1753 },
        { 0x1772, 0x1773 }, { 0x17B4, 0x17B5 }, { 0x17B7, 0x17BD },
        { 0x17C6, 0x17C6 }, { 0x17C9, 0x17D3 }, { 0x17DD, 0x17DD },
        { 0x180B, 0x180D }, { 0x18A9, 0x18A9 }, { 0x1920, 0x1922 },
        { 0x1927, 0x1928 }, { 0x1932, 0x1932 }, { 0x1939, 0x193B },
        { 0x1A17, 0x1A18 }, { 0x1B00, 0x1B03 }, { 0x1B34, 0x1B34 },
        { 0x1B36, 0x1B3A }, { 0x1B3C, 0x1B3C }, { 0x1B42, 0x1B42 },
        { 0x1B6B, 0x1B73 }, { 0x1DC0, 0x1DCA }, { 0x1DFE, 0x1DFF },
        { 0x200B, 0x200F }, { 0x202A, 0x202E }, { 0x2060, 0x2063 },
        { 0x206A, 0x206F }, { 0x20D0, 0x20EF }, { 0x302A, 0x302F },
        { 0x3099, 0x309A }, { 0xA806, 0xA806 }, { 0xA80B, 0xA80B },
        { 0xA825, 0xA826 }, { 0xFB1E, 0xFB1E }, { 0xFE00, 0xFE0F },
        { 0xFE20, 0xFE23 }, { 0xFEFF, 0xFEFF }, { 0xFFF9, 0xFFFB },
        { 0x10A01, 0x10A03 }, { 0x10A05, 0x10A06 }, { 0x10A0C, 0x10A0F },
        { 0x10A38, 0x10A3A }, { 0x10A3F, 0x10A3F }, { 0x1D167, 0x1D169 },
        { 0x1D173, 0x1D182 }, { 0x1D185, 0x1D18B }, { 0x1D1AA, 0x1D1AD },
        { 0x1D242, 0x1D244 }, { 0xE0001, 0xE0001 }, { 0xE0020, 0xE007F },
        { 0xE0100, 0xE01EF }
    };

    /* test for 8-bit control characters */
    if (ucs == 0)
        return 0;
    if (ucs < 32 || (ucs >= 0x7f && ucs < 0xa0))
        return -1;

    /* binary search in table of non-spacing characters */
    if (bisearch(ucs, combining, sizeof(combining) / sizeof(struct interval) - 1))
        return 0;

    /* if we arrive here, ucs is not a combining or C0/C1 control character */

    return 1 +
           (ucs >= 0x1100 &&
            (ucs <= 0x115f ||                    /* Hangul Jamo init. consonants */
             ucs == 0x2329 || ucs == 0x232a ||
             (ucs >= 0x2e80 && ucs <= 0xa4cf &&
              ucs != 0x303f) ||                  /* CJK ... Yi */
             (ucs >= 0xac00 && ucs <= 0xd7a3) || /* Hangul Syllables */
             (ucs >= 0xf900 && ucs <= 0xfaff) || /* CJK Compatibility Ideographs */
             (ucs >= 0xfe10 && ucs <= 0xfe19) || /* Vertical forms */
             (ucs >= 0xfe30 && ucs <= 0xfe6f) || /* CJK Compatibility Forms */
             (ucs >= 0xff00 && ucs <= 0xff60) || /* Fullwidth Forms */
             (ucs >= 0xffe0 && ucs <= 0xffe6) ||
             (ucs >= 0x20000 && ucs <= 0x2fffd) ||
             (ucs >= 0x30000 && ucs <= 0x3fffd)));
}

inline int string_width(std::string const & text)
{
    int w = 0;
    for (std::size_t i = 0; i < text.length(); ++i)
        w += konsole_wcwidth(text[i]/*TODO .unicode()*/);
    return w;
}


void Screen::displayCharacter(unsigned short c)
{
    // Note that VT100 does wrapping BEFORE putting the character.
    // This has impact on the assumption of valid cursor positions.
    // We indicate the fact that a newline has to be triggered by
    // putting the cursor one right to the last column of the screen.

    int w = konsole_wcwidth(c);
    if (w < 0)
        return;
    else if (w == 0) {
        // TODO if (QChar(c).category() != QChar::Mark_NonSpacing)
        if (c != ' ' && c != '\t') // TODO NonSpacing
            return;
        int charToCombineWithX = -1;
        int charToCombineWithY = -1;
        if (_cuX == 0) {
            // We are at the beginning of a line, check
            // if previous line has a character at the end we can combine with
            if (_cuY > 0 && _columns == int(_screenLines[_cuY - 1].size())) {
                charToCombineWithX = _columns - 1;
                charToCombineWithY = _cuY - 1;
            } else {
                // There is nothing to combine with
                // TODO Seems gnome-terminal shows the characters alone
                // might be worth investigating how to do that
                return;
            }
        } else {
            charToCombineWithX = _cuX - 1;
            charToCombineWithY = _cuY;
        }

        // Prevent "cat"ing binary files from causing crashes.
        if (charToCombineWithX >= int(_screenLines[charToCombineWithY].size())) {
            return;
        }

        // TODO
        //Character& currentChar = _screenLines[charToCombineWithY][charToCombineWithX];
        //if (!bool(currentChar.rendition & Rendition::ExtendedChar)) {
        //    const ushort chars[2] = { currentChar.character, c };
        //    currentChar.rendition |= Rendition::ExtendedChar;
        //    currentChar.character = ExtendedCharTable::instance.createExtendedChar(chars, 2);
        //} else {
        //    ushort extendedCharLength;
        //    const ushort* oldChars = ExtendedCharTable::instance.lookupExtendedChar(currentChar.character, extendedCharLength);
        //    assert(oldChars);
        //    if (oldChars) {
        //        assert(extendedCharLength > 1);
        //        assert(extendedCharLength < 65535);
        //        ushort* chars = new ushort[extendedCharLength + 1];
        //        memcpy(chars, oldChars, sizeof(ushort) * extendedCharLength);
        //        chars[extendedCharLength] = c;
        //        currentChar.character = ExtendedCharTable::instance.createExtendedChar(chars, extendedCharLength + 1);
        //        delete[] chars;
        //    }
        //}
        return;
    }

    if (_cuX + w > _columns) {
        if (getMode(Mode::Wrap)) {
            _lineProperties[_cuY] = _lineProperties[_cuY] | LineProperty::Wrapped;
            nextLine();
        } else {
            _cuX = _columns - w;
        }
    }

    // ensure current line vector has enough elements
    if (int(_screenLines[_cuY].size()) < _cuX + w) {
        _screenLines[_cuY].resize(_cuX + w);
    }

    if (getMode(Mode::Insert)) insertChars(w);

    _lastPos = loc(_cuX, _cuY);

    // check if selection is still valid.
    //checkSelection(_lastPos, _lastPos);

    Character& currentChar = _screenLines[_cuY][_cuX];

    currentChar.character = c;
    currentChar.foregroundColor = _effectiveForeground;
    currentChar.backgroundColor = _effectiveBackground;
    currentChar.rendition = _effectiveRendition;
    currentChar.isRealCharacter = true;

    int i = 0;
    const int newCursorX = _cuX + w--;
    while (w) {
        i++;

        if (int(_screenLines[_cuY].size()) < _cuX + i + 1)
            _screenLines[_cuY].resize(_cuX + i + 1);

        Character& ch = _screenLines[_cuY][_cuX + i];
        ch.character = 0;
        ch.foregroundColor = _effectiveForeground;
        ch.backgroundColor = _effectiveBackground;
        ch.rendition = _effectiveRendition;
        ch.isRealCharacter = false;

        w--;
    }
    _cuX = newCursorX;
}

int Screen::scrolledLines() const
{
    return _scrolledLines;
}
void Screen::resetScrolledLines()
{
    _scrolledLines = 0;
}

void Screen::scrollUp(int n)
{
    if (n == 0) n = 1; // Default
    scrollUp(_topMargin, n);
}

Rect Screen::lastScrolledRegion() const
{
    return _lastScrolledRegion;
}

void Screen::scrollUp(int from, int n)
{
    if (n <= 0 || from + n > _bottomMargin) return;

    _scrolledLines -= n;
    _lastScrolledRegion = Rect(
        0, static_cast<int16_t>(_topMargin),
        static_cast<uint16_t>(_columns - 1), static_cast<uint16_t>(_bottomMargin - _topMargin)
    );

    //FIXME: make sure `topMargin', `bottomMargin', `from', `n' is in bounds.
    moveImage(loc(0, from), loc(0, from + n), loc(_columns - 1, _bottomMargin));
    clearImage(loc(0, _bottomMargin - n + 1), loc(_columns - 1, _bottomMargin), ' ');
}

void Screen::scrollDown(int n)
{
    if (n == 0) n = 1; // Default
    scrollDown(_topMargin, n);
}

void Screen::scrollDown(int from, int n)
{
    _scrolledLines += n;

    //FIXME: make sure `topMargin', `bottomMargin', `from', `n' is in bounds.
    if (n <= 0)
        return;
    if (from > _bottomMargin)
        return;
    if (from + n > _bottomMargin)
        n = _bottomMargin - from;
    moveImage(loc(0, from + n), loc(0, from), loc(_columns - 1, _bottomMargin - n));
    clearImage(loc(0, from), loc(_columns - 1, from + n - 1), ' ');
}

void Screen::setCursorYX(int y, int x)
{
    setCursorY(y);
    setCursorX(x);
}

void Screen::setCursorX(int x)
{
    if (x == 0) x = 1; // Default
    x -= 1; // Adjust
    _cuX = std::max(0, std::min(_columns - 1, x));
}

void Screen::setCursorY(int y)
{
    if (y == 0) y = 1; // Default
    y -= 1; // Adjust
    _cuY = std::max(0, std::min(_lines  - 1, y + (getMode(Mode::Origin) ? _topMargin : 0)));
}

void Screen::home()
{
    _cuX = 0;
    _cuY = 0;
}

void Screen::toStartOfLine()
{
    _cuX = 0;
}

int Screen::getCursorX() const
{
    return _cuX;
}

int Screen::getCursorY() const
{
    return _cuY;
}

void Screen::clearImage(int loca, int loce, char c)
{
    //FIXME: check positions

    const int topLine = loca / _columns;
    const int bottomLine = loce / _columns;

    Character clearCh(c, _currentForeground, _currentBackground, Rendition::Default, false);

    //if the character being used to clear the area is the same as the
    //default character, the affected _lines can simply be shrunk.
    const bool isDefaultCh = (clearCh == Screen::DefaultChar);

    for (int y = topLine; y <= bottomLine; y++) {
        _lineProperties[y] = LineProperty::Default;

        const int endCol = (y == bottomLine) ? loce % _columns : _columns - 1;
        const int startCol = (y == topLine) ? loca % _columns : 0;

        std::vector<Character>& line = _screenLines[y];

        if (isDefaultCh && endCol == _columns - 1) {
            line.resize(startCol);
        } else {
            if (line.size() < std::size_t(endCol + 1))
                line.resize(endCol + 1);

            Character* data = line.data();
            for (int i = startCol; i <= endCol; i++)
                data[i] = clearCh;
        }
    }
}

void Screen::moveImage(int dest, int sourceBegin, int sourceEnd)
{
    assert(sourceBegin <= sourceEnd);

    const int lines = (sourceEnd - sourceBegin) / _columns;

    //move screen image and line properties:
    //the source and destination areas of the image may overlap,
    //so it matters that we do the copy in the right order -
    //forwards if dest < sourceBegin or backwards otherwise.
    //(search the web for 'memmove implementation' for details)
    if (dest < sourceBegin) {
        for (int i = 0; i <= lines; i++) {
            _screenLines[(dest / _columns) + i ] = _screenLines[(sourceBegin / _columns) + i ];
            _lineProperties[(dest / _columns) + i] = _lineProperties[(sourceBegin / _columns) + i];
        }
    } else {
        for (int i = lines; i >= 0; i--) {
            _screenLines[(dest / _columns) + i ] = _screenLines[(sourceBegin / _columns) + i ];
            _lineProperties[(dest / _columns) + i] = _lineProperties[(sourceBegin / _columns) + i];
        }
    }

    if (_lastPos != -1) {
        const int diff = dest - sourceBegin; // Scroll by this amount
        _lastPos += diff;
        if ((_lastPos < 0) || (_lastPos >= (lines * _columns)))
            _lastPos = -1;
    }

    // Adjust selection to follow scroll.
    if (_selBegin != -1) {
        const bool beginIsTL = (_selBegin == _selTopLeft);
        const int diff = dest - sourceBegin; // Scroll by this amount
        const int scr_TL = 0;//loc(0, _history->getLines());
        const int srca = sourceBegin + scr_TL; // Translate index from screen to global
        const int srce = sourceEnd + scr_TL; // Translate index from screen to global
        const int desta = srca + diff;
        const int deste = srce + diff;

        if ((_selTopLeft >= srca) && (_selTopLeft <= srce))
            _selTopLeft += diff;
        else if ((_selTopLeft >= desta) && (_selTopLeft <= deste))
            _selBottomRight = -1; // Clear selection (see below)

        if ((_selBottomRight >= srca) && (_selBottomRight <= srce))
            _selBottomRight += diff;
        else if ((_selBottomRight >= desta) && (_selBottomRight <= deste))
            _selBottomRight = -1; // Clear selection (see below)

        if (_selBottomRight < 0) {
            //clearSelection();
        } else {
            if (_selTopLeft < 0)
                _selTopLeft = 0;
        }

        if (beginIsTL)
            _selBegin = _selTopLeft;
        else
            _selBegin = _selBottomRight;
    }
}

void Screen::clearToEndOfScreen()
{
    clearImage(loc(_cuX, _cuY), loc(_columns - 1, _lines - 1), ' ');
}

void Screen::clearToBeginOfScreen()
{
    clearImage(loc(0, 0), loc(_cuX, _cuY), ' ');
}

void Screen::clearEntireScreen()
{
    // Add entire screen to history
    for (int i = 0; i < (_lines - 1); i++) {
        scrollUp(0, 1);
    }

    clearImage(loc(0, 0), loc(_columns - 1, _lines - 1), ' ');
}

/*! fill screen with 'E'
  This is to aid screen alignment
  */

void Screen::helpAlign()
{
    clearImage(loc(0, 0), loc(_columns - 1, _lines - 1), 'E');
}

void Screen::clearToEndOfLine()
{
    clearImage(loc(_cuX, _cuY), loc(_columns - 1, _cuY), ' ');
}

void Screen::clearToBeginOfLine()
{
    clearImage(loc(0, _cuY), loc(_cuX, _cuY), ' ');
}

void Screen::clearEntireLine()
{
    clearImage(loc(0, _cuY), loc(_columns - 1, _cuY), ' ');
}

void Screen::setRendition(Rendition rendention)
{
    _currentRendition |= rendention;
    updateEffectiveRendition();
}

void Screen::resetRendition(Rendition rendention)
{
    _currentRendition &= ~rendention;
    updateEffectiveRendition();
}

void Screen::setDefaultRendition()
{
    setForeColor(ColorSpace::Default, DEFAULT_FORE_COLOR);
    setBackColor(ColorSpace::Default, DEFAULT_BACK_COLOR);
    _currentRendition   = Rendition::Default;
    updateEffectiveRendition();
}

void Screen::setForeColor(ColorSpace space, int color)
{
    _currentForeground = CharacterColor(space, color);

    if (_currentForeground.isValid())
        updateEffectiveRendition();
    else
        setForeColor(ColorSpace::Default, DEFAULT_FORE_COLOR);
}

void Screen::setBackColor(ColorSpace space, int color)
{
    _currentBackground = CharacterColor(space, color);

    if (_currentBackground.isValid())
        updateEffectiveRendition();
    else
        setBackColor(ColorSpace::Default, DEFAULT_BACK_COLOR);
}

void Screen::setLineProperty(LineProperty property , bool enable)
{
    if (enable)
        _lineProperties[_cuY] |= property;
    else
        _lineProperties[_cuY] &= ~property;
}
void Screen::fillWithDefaultChar(Character* dest, int count)
{
    for (int i = 0; i < count; i++)
        dest[i] = Screen::DefaultChar;
}


#define MODES_SCREEN static_cast<int>(Mode::COUNT_)

#define MODE_AppScreen       (MODES_SCREEN+0)   // Mode #1
#define MODE_AppCuKeys       (MODES_SCREEN+1)   // Application cursor keys (DECCKM)
#define MODE_AppKeyPad       (MODES_SCREEN+2)   //
#define MODE_Mouse1000       (MODES_SCREEN+3)   // Send mouse X,Y position on press and release
#define MODE_Mouse1001       (MODES_SCREEN+4)   // Use Hilight mouse tracking
#define MODE_Mouse1002       (MODES_SCREEN+5)   // Use cell motion mouse tracking
#define MODE_Mouse1003       (MODES_SCREEN+6)   // Use all motion mouse tracking
#define MODE_Mouse1005       (MODES_SCREEN+7)   // Xterm-style extended coordinates
#define MODE_Mouse1006       (MODES_SCREEN+8)   // 2nd Xterm-style extended coordinates
#define MODE_Mouse1015       (MODES_SCREEN+9)   // Urxvt-style extended coordinates
#define MODE_Ansi            (MODES_SCREEN+10)   // Use US Ascii for character sets G0-G3 (DECANM)
#define MODE_132Columns      (MODES_SCREEN+11)  // 80 <-> 132 column mode switch (DECCOLM)
#define MODE_Allow132Columns (MODES_SCREEN+12)  // Allow DECCOLM mode
#define MODE_BracketedPaste  (MODES_SCREEN+13)  // Xterm-style bracketed paste mode
#define MODE_total           (MODES_SCREEN+14)

extern unsigned short vt100_graphics[32];

struct CharCodes {
    // coding info
    char charset[4]; //
    int  cu_cs;      // actual charset.
    bool graphic;    // Some VT100 tricks
    bool pound;      // Some VT100 tricks
    bool sa_graphic; // saved graphic
    bool sa_pound;   // saved pound
};


/**
 * Provides an xterm compatible terminal emulation based on the DEC VT102 terminal.
 * A full description of this terminal can be found at http://vt100.net/docs/vt102-ug/
 *
 * In addition, various additional xterm escape sequences are supported to provide
 * features such as mouse input handling.
 * See http://rtfm.etla.org/xterm/ctlseq.html for a description of xterm's escape
 * sequences.
 *
 */
class Vt102Emulation
{
public:
    /** Constructs a new emulation */
    Vt102Emulation(Screen * screen);
    ~Vt102Emulation();

    // reimplemented from Emulation
    void clearEntireScreen();
    void reset();
    char eraseChar() const;

protected:
    // reimplemented from Emulation
    void setMode(int mode);
    void resetMode(int mode);

public: // TODO protected
    void receiveChar(int cc);
    void receiveCharNG(uin32_t ucs);

private:
    unsigned short applyCharset(unsigned short c);
    unsigned short applyCharset(int c) { return applyCharset(static_cast<unsigned short>(c)); }
    void setCharset(int n, int cs);
    void useCharset(int n);
    void setAndUseCharset(int n, int cs);
    void saveCursor();
    void restoreCursor();
    void resetCharset(int scrno);

    void setMargins(int top, int bottom);
    //set margins for all screens back to their defaults
    void setDefaultMargins();

    // returns true if 'mode' is set or false otherwise
    bool getMode(int mode);
    // saves the current boolean value of 'mode'
    void saveMode(int mode);
    // restores the boolean value of 'mode'
    void restoreMode(int mode);
    // resets all modes
    // (except MODE_Allow132Columns)
    void resetModes();

    void resetTokenizer();
#define MAX_TOKEN_LENGTH 256 // Max length of tokens (e.g. window title)
    void addToCurrentToken(int cc);
    int tokenBuffer[MAX_TOKEN_LENGTH]; //FIXME: overflow?
    int tokenBufferPos;
#define MAXARGS 15
    void addDigit(int dig);
    void addArgument();
    int argv[MAXARGS];
    int argc;
    void initTokenizer();

    // Set of flags for each of the ASCII characters which indicates
    // what category they fall into (printable character, control, digit etc.)
    // for the purposes of decoding terminal output
    int charClass[256];

    void reportDecodingError();

    void processToken(int code, int p, int q);

    void reportTerminalType() { /* TODO */ }
    void reportSecondaryAttributes() { /* TODO */ }
    void reportStatus() { /* TODO */ }
    void reportAnswerBack() { /* TODO */ }
    void reportCursorPosition() { /* TODO */ }
    void reportTerminalParms(int p) { (void)p; /* TODO */ }

    // clears the screen and resizes it to the specified
    // number of columns
    void clearScreenAndSetColumns(int columnCount);

    CharCodes _charset[2];

    class TerminalState
    {
    public:
        // Initializes all modes to false
        TerminalState() {
            memset(&mode, false, MODE_total * sizeof(bool));
        }

        bool mode[MODE_total];
    };

    TerminalState _currentModes;
    TerminalState _savedModes;

    bool _reportFocusEvents;

    Screen* _currentScreen;
};

unsigned short vt100_graphics[32] = {
    // 0/8     1/9    2/10    3/11    4/12    5/13    6/14    7/15
    0x0020, 0x25C6, 0x2592, 0x2409, 0x240c, 0x240d, 0x240a, 0x00b0,
    0x00b1, 0x2424, 0x240b, 0x2518, 0x2510, 0x250c, 0x2514, 0x253c,
    0xF800, 0xF801, 0x2500, 0xF803, 0xF804, 0x251c, 0x2524, 0x2534,
    0x252c, 0x2502, 0x2264, 0x2265, 0x03C0, 0x2260, 0x00A3, 0x00b7
};

Vt102Emulation::Vt102Emulation(Screen * screen)
: _reportFocusEvents(false)
, _currentScreen(screen)
{
    initTokenizer();
    reset();
}

Vt102Emulation::~Vt102Emulation() = default;

void Vt102Emulation::clearEntireScreen()
{
    _currentScreen->clearEntireScreen();
    // bufferedUpdate();
}

void Vt102Emulation::reset()
{
    // Save the current codec so we can set it later.
    // Ideally we would want to use the profile setting
//     const QTextCodec* currentCodec = codec();

    resetTokenizer();
    resetModes();
    resetCharset(0);
//     _screen[0]->reset();
    resetCharset(1);
//     _screen[1]->reset();
_currentScreen->reset();

//     if (currentCodec)
//         setCodec(currentCodec);
//     else
//         setCodec(LocaleCodec);
//
//     bufferedUpdate();
}

/* ------------------------------------------------------------------------- */
/*                                                                           */
/*                     Processing the incoming byte stream                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/* Incoming Bytes Event pipeline

   This section deals with decoding the incoming character stream.
   Decoding means here, that the stream is first separated into `tokens'
   which are then mapped to a `meaning' provided as operations by the
   `Screen' class or by the emulation class itself.

   The pipeline proceeds as follows:

   - Tokenizing the ESC codes (onReceiveChar)
   - VT100 code page translation of plain characters (applyCharset)
   - Interpretation of ESC codes (processToken)

   The escape codes and their meaning are described in the
   technical reference of this program.
*/

// Tokens ------------------------------------------------------------------ --

/*
   Since the tokens are the central notion if this section, we've put them
   in front. They provide the syntactical elements used to represent the
   terminals operations as byte sequences.

   They are encodes here into a single machine word, so that we can later
   switch over them easily. Depending on the token itself, additional
   argument variables are filled with parameter values.

   The tokens are defined below:

   - CHR        - Printable characters     (32..255 but DEL (=127))
   - CTL        - Control characters       (0..31 but ESC (= 27), DEL)
   - ESC        - Escape codes of the form <ESC><CHR but `[]()+*#'>
   - ESC_DE     - Escape codes of the form <ESC><any of `()+*#%'> C
   - CSI_PN     - Escape codes of the form <ESC>'['     {Pn} ';' {Pn} C
   - CSI_PS     - Escape codes of the form <ESC>'['     {Pn} ';' ...  C
   - CSI_PR     - Escape codes of the form <ESC>'[' '?' {Pn} ';' ...  C
   - CSI_PE     - Escape codes of the form <ESC>'[' '!' {Pn} ';' ...  C
   - VT52       - VT52 escape codes
                  - <ESC><Chr>
                  - <ESC>'Y'{Pc}{Pc}
   - XTE_HA     - Xterm window/terminal attribute commands
                  of the form <ESC>`]' {Pn} `;' {Text} <BEL>
                  (Note that these are handled differently to the other formats)

   The last two forms allow list of arguments. Since the elements of
   the lists are treated individually the same way, they are passed
   as individual tokens to the interpretation. Further, because the
   meaning of the parameters are names (although represented as numbers),
   they are includes within the token ('N').

*/

#define TY_CONSTRUCT(T,A,N) ( \
    ((static_cast<int>(N) & 0xffff) << 16) | \
    ((static_cast<int>(A) & 0xff) << 8) | \
    ( static_cast<int>(T) & 0xff) )

#define TY_CHR(   )     TY_CONSTRUCT(0,0,0)
#define TY_CTL(A  )     TY_CONSTRUCT(1,A,0)
#define TY_ESC(A  )     TY_CONSTRUCT(2,A,0)
#define TY_ESC_CS(A,B)  TY_CONSTRUCT(3,A,B)
#define TY_ESC_DE(A  )  TY_CONSTRUCT(4,A,0)
#define TY_CSI_PS(A,N)  TY_CONSTRUCT(5,A,N)
#define TY_CSI_PN(A  )  TY_CONSTRUCT(6,A,0)
#define TY_CSI_PR(A,N)  TY_CONSTRUCT(7,A,N)

#define TY_VT52(A)    TY_CONSTRUCT(8,A,0)
#define TY_CSI_PG(A)  TY_CONSTRUCT(9,A,0)
#define TY_CSI_PE(A)  TY_CONSTRUCT(10,A,0)

const int MAX_ARGUMENT = 4096;

// Tokenizer --------------------------------------------------------------- --

/* The tokenizer's state

   The state is represented by the buffer (tokenBuffer, tokenBufferPos),
   and accompanied by decoded arguments kept in (argv,argc).
   Note that they are kept internal in the tokenizer.
*/

void Vt102Emulation::resetTokenizer()
{
    tokenBufferPos = 0;
    argc = 0;
    argv[0] = 0;
    argv[1] = 0;
}

void Vt102Emulation::addDigit(int digit)
{
    if (argv[argc] < MAX_ARGUMENT)
        argv[argc] = 10 * argv[argc] + digit;
}

void Vt102Emulation::addArgument()
{
    argc = std::min(argc + 1, MAXARGS - 1);
    argv[argc] = 0;
}

void Vt102Emulation::addToCurrentToken(int cc)
{
    tokenBuffer[tokenBufferPos] = cc;
    tokenBufferPos = std::min(tokenBufferPos + 1, MAX_TOKEN_LENGTH - 1);
}

// Character Class flags used while decoding
const int CTL =  1;  // Control character
const int CHR =  2;  // Printable character
const int CPN =  4;  // TODO: Document me
const int DIG =  8;  // Digit
const int SCS = 16;  // Select Character Set
const int GRP = 32;  // TODO: Document me
const int CPS = 64;  // Character which indicates end of window resize

void Vt102Emulation::initTokenizer()
{
    auto as_bytes = [](char const * s) { return reinterpret_cast<uint8_t const *>(s); };

    for (int i = 0; i < 256; ++i)
        charClass[i] = 0;
    for (int i = 0; i < 32; ++i)
        charClass[i] |= CTL;
    for (int i = 32; i < 256; ++i)
        charClass[i] |= CHR;
    for (auto s = as_bytes("@ABCDGHILMPSTXZcdfry"); *s; ++s)
        charClass[*s] |= CPN;
    // resize = \e[8;<row>;<col>t
    for (auto s = as_bytes("t"); *s; ++s)
        charClass[*s] |= CPS;
    for (auto s = as_bytes("0123456789"); *s; ++s)
        charClass[*s] |= DIG;
    for (auto s = as_bytes("()+*%"); *s; ++s)
        charClass[*s] |= SCS;
    for (auto s = as_bytes("()+*#[]%"); *s; ++s)
        charClass[*s] |= GRP;

    resetTokenizer();
}

/* Ok, here comes the nasty part of the decoder.

   Instead of keeping an explicit state, we deduce it from the
   token scanned so far. It is then immediately combined with
   the current character to form a scanning decision.

   This is done by the following defines.

   - P is the length of the token scanned so far.
   - L (often P-1) is the position on which contents we base a decision.
   - C is a character or a group of characters (taken from 'charClass').

   - 'cc' is the current character
   - 's' is a pointer to the start of the token buffer
   - 'p' is the current position within the token buffer

   Note that they need to applied in proper order.
*/

#define lec(P,L,C) (p == (P) && s[(L)] == (C))
#define lun(     ) (p ==  1  && cc >= 32 )
#define les(P,L,C) (p == (P) && s[L] < 256 && (charClass[s[(L)]] & (C)) == (C))
#define eec(C)     (p >=  3  && cc == (C))
#define ees(C)     (p >=  3  && cc < 256 && (charClass[cc] & (C)) == (C))
#define eps(C)     (p >=  3  && s[2] != '?' && s[2] != '!' && s[2] != '>' && cc < 256 && (charClass[cc] & (C)) == (C))
#define epp( )     (p >=  3  && s[2] == '?')
#define epe( )     (p >=  3  && s[2] == '!')
#define egt( )     (p >=  3  && s[2] == '>')
#define Xpe        (tokenBufferPos >= 2 && tokenBuffer[1] == ']')
#define Xte        (Xpe      && cc ==  7 )
#define ces(C)     (cc < 256 && (charClass[cc] & (C)) == (C) && !Xte)

#define CNTL(c) ((c)-'@')
const int ESC = 27;
const int DEL = 127;

// process an incoming unicode character
void Vt102Emulation::receiveChar(int cc)
{
  if (cc == DEL)
    return; //VT100: ignore.

  if (ces(CTL))
  {
    // DEC HACK ALERT! Control Characters are allowed *within* esc sequences in VT100
    // This means, they do neither a resetTokenizer() nor a pushToToken(). Some of them, do
    // of course. Guess this originates from a weakly layered handling of the X-on
    // X-off protocol, which comes really below this level.
    if (cc == CNTL('X') || cc == CNTL('Z') || cc == ESC)
        resetTokenizer(); //VT100: CAN or SUB
    if (cc != ESC)
    {
        processToken(TY_CTL(cc+'@' ),0,0);
        return;
    }
  }
  // advance the state
  addToCurrentToken(cc);

  int* s = tokenBuffer;
  const int  p = tokenBufferPos;

  if (getMode(MODE_Ansi))
  {
    if (lec(1,0,ESC)) { return; }
    if (lec(1,0,ESC+128)) { s[0] = ESC; receiveChar('['); return; }
    if (les(2,1,GRP)) { return; }
    if (Xte         ) { /* TODO processWindowAttributeRequest();*/ resetTokenizer(); return; }
    if (Xpe         ) { return; }
    if (lec(3,2,'?')) { return; }
    if (lec(3,2,'>')) { return; }
    if (lec(3,2,'!')) { return; }
    if (lun(       )) { processToken( TY_CHR(), applyCharset(cc), 0);   resetTokenizer(); return; }
    if (lec(2,0,ESC)) { processToken( TY_ESC(s[1]), 0, 0);              resetTokenizer(); return; }
    if (les(3,1,SCS)) { processToken( TY_ESC_CS(s[1],s[2]), 0, 0);      resetTokenizer(); return; }
    if (lec(3,1,'#')) { processToken( TY_ESC_DE(s[2]), 0, 0);           resetTokenizer(); return; }
    if (eps(    CPN)) { processToken( TY_CSI_PN(cc), argv[0],argv[1]);  resetTokenizer(); return; }

    // resize = \e[8;<row>;<col>t
    if (eps(CPS))
    {
        processToken( TY_CSI_PS(cc, argv[0]), argv[1], argv[2]);
        resetTokenizer();
        return;
    }

    if (epe(   )) { processToken( TY_CSI_PE(cc), 0, 0); resetTokenizer(); return; }
    if (ees(DIG)) { addDigit(cc-'0'); return; }
    if (eec(';')) { addArgument();    return; }
    for (int i = 0; i <= argc; i++)
    {
        if (epp())
            processToken(TY_CSI_PR(cc,argv[i]), 0, 0);
        else if (egt())
            processToken(TY_CSI_PG(cc), 0, 0); // spec. case for ESC]>0c or ESC]>c
        else if (cc == 'm' && argc - i >= 4 && (argv[i] == 38 || argv[i] == 48) && argv[i+1] == 2)
        {
            // ESC[ ... 48;2;<red>;<green>;<blue> ... m -or- ESC[ ... 38;2;<red>;<green>;<blue> ... m
            i += 2;
            processToken(TY_CSI_PS(cc, argv[i-2]), static_cast<int>(ColorSpace::RGB), (argv[i] << 16) | (argv[i+1] << 8) | argv[i+2]);
            i += 2;
        }
        else if (cc == 'm' && argc - i >= 2 && (argv[i] == 38 || argv[i] == 48) && argv[i+1] == 5)
        {
            // ESC[ ... 48;5;<index> ... m -or- ESC[ ... 38;5;<index> ... m
            i += 2;
            processToken(TY_CSI_PS(cc, argv[i-2]), static_cast<int>(ColorSpace::Index256), argv[i]);
        }
        else
            processToken(TY_CSI_PS(cc,argv[i]), 0, 0);
    }
    resetTokenizer();
  }
  else
  {
    // VT52 Mode
    if (lec(1,0,ESC))
        return;
    if (les(1,0,CHR))
    {
        processToken( TY_CHR(), s[0], 0);
        resetTokenizer();
        return;
    }
    if (lec(2,1,'Y'))
        return;
    if (lec(3,1,'Y'))
        return;
    if (p < 4)
    {
        processToken(TY_VT52(s[1] ), 0, 0);
        resetTokenizer();
        return;
    }
    processToken(TY_VT52(s[1]), s[2], s[3]);
    resetTokenizer();
    return;
  }
}

void Vt102Emulation::receiveCharNG(uint32_t ucs)
{
    struct ParserState
    {
        enum State {
            None,
            ESC,

            CSI, // ESC [
            OSI, // ESC ]
        };

        enum Param {
            P1,
            P2,
            P3,
            P4,
        };
        int state;
    } parser;

    auto mk_parser = []{};
    auto match = []{};
    auto ch = []{};
    auto other = []{};
    auto on_digit = []{};
    auto Pn = []{};
    auto noop = []{};
    auto nochange = []{};
    auto root_parser = []{};
    struct P p {};

#define CH(c) ch(c) = [](P & p)
#define DIGITS() digits() = [](P & p)
#define IN(s) in(s) = [](P & p)
#define RNG(a, b) rng(a,b) = [](P & p)

    auto osi_parser = mk_parser( // ESC ]
      noop
    , p(in("012"))
    , ch(';', mk_parser(
        noop
      , ch('\a', root_parser, set_title_or_icon)
      , pstr()
    ), nochange)
    , ch('\a', root_parser, noop)
    , other(mk_parser(
        noop
      , ch('\a', root_parser, noop)
      , other(nochange, noop)
    ), noop)
    );

    auto osi_parser_ng =
        *(
          p(in("012")) >> ch(';') >> +(ch('\a')[set_title_or_icon] | p<std::string>())
        | ch('\a') >> root_parser
        | ch >> *ch >> root_parser
        )
    ;

    auto line_size_parser = mk_parser(
      noop
    , ch('3', root_parser, double_height_line_top /* DECDHL */)
    , ch('4', root_parser, double_height_line_bottom /* DECDHL */)
    , ch('5', root_parser, single_width_line/* DECSWL */)
    , ch('6', root_parser, double_width_line/* DECDWL */)
    , ch('8', root_parser, screen_alignement_diaplay /* DECALN */)
    , other(root_parser, noop)
    );

    auto charset_set_parser = mk_parser(
      noop
    , ch('A', root_parser, set_charset /* United Kingdom (UK) */)
    , ch('B', root_parser, set_charset /* United States (USASCII) */)
    , ch('0', root_parser, set_charset /* Special graphics characters and line drawing set */)
    , ch('1', root_parser, set_charset /* Alternate character ROM */)
    , ch('2', root_parser, set_charset /* Alternate character ROM special graphics characters */)
    );

    auto csi_parser = mk_parser(
      noop
    , ch('K', root_parser, set_charset /* United Kingdom (UK) */)
    , ch('J', root_parser, set_charset /* United States (USASCII) */)
    , ch('J', root_parser, set_charset /* Special graphics characters and line drawing set */)
    , ch('1', root_parser, set_charset /* Alternate character ROM */)
    , ch('2', root_parser, set_charset /* Alternate character ROM special graphics characters */)
    );

    mk_parser(
        ch(ESC, mk_parser(
            noop
          , ch(']', osi_parser, noop)
          , ch('#', line_size_parser, noop)
          , ch('(', charset_set_parser, select_g0_parser)
          , ch(')', charset_set_parser, select_g1_parser)
          , ch('[', csi_parser, noop)
          , ch('Z', root_parser, identity_terminal /* DECID */)
          , ch('=', root_parser, keypad_application_mode /* DECKPAM */)
          , ch('>', root_parser, keypad_numeric_mode /* DECKPMM */)
          , ch('<', root_parser, ansi_mode /* */)
          , ch('7', root_parser, save_cursor /* DECCS */)
          , ch('8', root_parser, restore_cursor /* DECRS */)
          , ch('H', root_parser, horizontal_tabulation_set /* HTS */)
          , ch('D', root_parser, index /* IND */)
          , ch('E', root_parser, next_line /* NEL */)
          , ch('M', root_parser, reverse_index /* RI */)
          , ch('c', root_parser, reset_to_initial_state /* RIS */)
          , ch('A', root_parser, cursor_up /* CU */)
          , ch('B', root_parser, cursor_down /*  */)
          , ch('C', root_parser, cursor_right /* C */)
          , ch('D', root_parser, cursor_left /* C */)
          , ch('F', root_parser, enter_graphics_node /*  */)
          , ch('G', root_parser, exit_graphics_mode /* */)
          , ch('H', root_parser, cursor_to_home /* */)
          , ch('I', root_parser, reverse_line_feed /* */)
          , ch('J', root_parser, erase_to_end_of_screen /* */)
          , ch('K', root_parser, erase_to_end_of_line /* */)
          , ch('Y', p(p(root_parser, direct_cursor_address), noop), noop)
          , other(root_parser, add_char)
        ))
    );

    switch (parser.state) {
        case ParserState::None:
        switch (ucs) {
            case ESC: parser.state = ParserState::ESC; break;

            case DEL:
            case '\a': break;

            case '\b': _currentScreen->cursorLeft(); break;
            case '\t': _currentScreen->tab(); break;
            case '\n': _currentScreen->newLine(); break;
            case '\v':
            case '\f': _currentScreen->cursorDown(); break;
            case '\r': _currentScreen->setCursorX(0); break;
            //case SO : selectG0(); break;
            //case SI : selectG1(); break;
        }
        break;

        case ParserState::ESC:
        switch (ucs) {
            case '[': parser.state = ParserState::CSI; break;
            case ']': parser.state = ParserState::OSI; break;

            case DEL:
            case '\a': break;

            case '\b': _currentScreen->cursorLeft(); break;
            case '\t': _currentScreen->tab(); break;
            case '\n': _currentScreen->newLine(); break;
            case '\v':
            case '\f': _currentScreen->cursorDown(); break;
            case '\r': _currentScreen->setCursorX(0); break;
            //case SO : selectG0(); break;
            //case SI : selectG1(); break;
        }
        break;

        // ESC ']' [0|1|2] ';' Str '\a'
        case ParserState::OSI:
        switch (ucs) {
            case '0':
            case '1':
            case '2': /* TODO */ break;
            case '\x9c': // String Terminator
            case '\a': /* TODO end sequence */ break;
            case ';': parser.state |= ParserState::P1; break;
        }
        case ParserState::OSI | ParserState::P1:
        switch (ucs) {
            case '0':
            case '1':
            case '2': /* TODO */ break;
            case '\x9c': // String Terminator
            case '\a': /* TODO end sequence */ break;
            case ';': parser.state |= ParserState::P1; break;
        }
        break;
    }

    // no mode
    switch (ucs) {

        case ESC: parser.state |= ParserState::ESC; break;
    }

    // ESC state
    switch (ucs) {
        case '@': _currentScreen->insertChars      (p    ); break; //Ansix364 : TODO ESC [ Pn @

        case 'A': _currentScreen->cursorUp             (         1); break; //VT52 CUU
        case 'B': _currentScreen->cursorDown           (         1); break; //VT52 CUD
        case 'C': _currentScreen->cursorRight          (         1); break; //VT52 CUR
        case 'D': _currentScreen->cursorLeft           (         1); break; //VT52 CUL

        case 'E': _currentScreen->cursorDown           (         p); break; //VT100 CNL
        case 'F': _currentScreen->cursorUp           (         p); break; //VT100 CPL
        case 'G': _currentScreen->setCursorX           (         p); break; //VT100 CHA TODO LINUX ?
        case 'H': _currentScreen->setCursorYX(         p, q); break; //VT100 CUP
    case TY_CSI_PN('I'      ) : _currentScreen->tab                  (p         ); break;
        case 'J': _currentScreen->tab(         ); break; //VT100 EL TODO erase in display
        case 'K': _currentScreen->cursorDown           (         ); break; //VT100 IL TODO erase in lines
        case 'L': _currentScreen->insertLines           (p         ); break; //VT100 DL
        case 'M': _currentScreen->deleteLines           (p         ); break; //VT100 DCH
        case 'P': _currentScreen->deleteChars           (p         ); break; //VT100 DCH
    case TY_CSI_PN('S'      ) : _currentScreen->scrollUp             (p         ); break;
    case TY_CSI_PN('T'      ) : _currentScreen->scrollDown           (p         ); break;
        case 'X': _currentScreen->eraseChars           (p         ); break; //VT100 ECH
    case TY_CSI_PN('Z'      ) : _currentScreen->backtab              (p         ); break;
        case 'a': _currentScreen->cursorRight           (         ); break; //VT100 HPR
        case 'c': reportTerminalType(); break; //VT100 DA TODO Reports terminal identity
        case 'd': _currentScreen->setCursorY           (p         ); break; //VT100 VPA
        case 'e': _currentScreen->cursorDown           (p         ); break; //VT100 VPR
        case 'f': _currentScreen->setCursorYX           (p, q        ); break; //VT100 HVP
        case 'g': break; //VT100 TBC TODO clear_tab_stop
        case 'h': setMode      (MODE_AppCuKeys); break; //VT100 SM
        case 'l': resetMode      (MODE_AppCuKeys); break; //VT100 RM
        case 'm':  break; //VT100 SGR TODO select_graphic_rendition
        case 'n': _currentScreen->cursorDown           (p         ); break; //VT100 DSR TODO report_device_status
        case 'r': _currentScreen->cursorDown           (p         ); break; //VT100 DECSTBM TODO set_margins
        case '\'': _currentScreen->setCursorX           (p         ); break; //VT100 HPA

        case 'D': _currentScreen->index             (    ); break; //VT100
        case 'E': _currentScreen->nextLine          (    ); break; //VT100
        case 'H': _currentScreen->changeTabStop     (true); break; //VT100
        case 'M': _currentScreen->reverseIndex      (    ); break; //VT100
    }
}

// Interpreting Codes ---------------------------------------------------------

/*
   Now that the incoming character stream is properly tokenized,
   meaning is assigned to them. These are either operations of
   the current _screen, or of the emulation class itself.

   The token to be interpreted comes in as a machine word
   possibly accompanied by two parameters.

   Likewise, the operations assigned to, come with up to two
   arguments. One could consider to make up a proper table
   from the function below.

   The technical reference manual provides more information
   about this mapping.
*/

void Vt102Emulation::processToken(int token, int p, int q)
{
  switch (token)
  {
    case TY_CHR(         ) : _currentScreen->displayCharacter     (static_cast<unsigned short>(p) /* TODO */ ); break; //UTF16

    //             127 DEL    : ignored on input

    case TY_CTL('@'      ) : /* NUL: ignored                      */ break;
    case TY_CTL('A'      ) : /* SOH: ignored                      */ break;
    case TY_CTL('B'      ) : /* STX: ignored                      */ break;
    case TY_CTL('C'      ) : /* ETX: ignored                      */ break;
    case TY_CTL('D'      ) : /* EOT: ignored                      */ break;
    case TY_CTL('E'      ) :      reportAnswerBack     (          ); break; //VT100
    case TY_CTL('F'      ) : /* ACK: ignored                      */ break;
    case TY_CTL('G'      ) : /* TODO emit stateSet(NOTIFYBELL);*/         break; //VT100
    case TY_CTL('H'      ) : _currentScreen->backspace            (          ); break; //VT100
    case TY_CTL('I'      ) : _currentScreen->tab                  (          ); break; //VT100
    case TY_CTL('J'      ) : _currentScreen->newLine              (          ); break; //VT100
    case TY_CTL('K'      ) : _currentScreen->newLine              (          ); break; //VT100
    case TY_CTL('L'      ) : _currentScreen->newLine              (          ); break; //VT100
    case TY_CTL('M'      ) : _currentScreen->toStartOfLine        (          ); break; //VT100

    case TY_CTL('N'      ) :      useCharset           (         1); break; //VT100
    case TY_CTL('O'      ) :      useCharset           (         0); break; //VT100

    case TY_CTL('P'      ) : /* DLE: ignored                      */ break;
    case TY_CTL('Q'      ) : /* DC1: XON continue                 */ break; //VT100
    case TY_CTL('R'      ) : /* DC2: ignored                      */ break;
    case TY_CTL('S'      ) : /* DC3: XOFF halt                    */ break; //VT100
    case TY_CTL('T'      ) : /* DC4: ignored                      */ break;
    case TY_CTL('U'      ) : /* NAK: ignored                      */ break;
    case TY_CTL('V'      ) : /* SYN: ignored                      */ break;
    case TY_CTL('W'      ) : /* ETB: ignored                      */ break;
    case TY_CTL('X'      ) : _currentScreen->displayCharacter     (    0x2592); break; //VT100
    case TY_CTL('Y'      ) : /* EM : ignored                      */ break;
    case TY_CTL('Z'      ) : _currentScreen->displayCharacter     (    0x2592); break; //VT100
    case TY_CTL('['      ) : /* ESC: cannot be seen here.         */ break;
    case TY_CTL('\\'     ) : /* FS : ignored                      */ break;
    case TY_CTL(']'      ) : /* GS : ignored                      */ break;
    case TY_CTL('^'      ) : /* RS : ignored                      */ break;
    case TY_CTL('_'      ) : /* US : ignored                      */ break;

    case TY_ESC('D'      ) : _currentScreen->index                (          ); break; //VT100
    case TY_ESC('E'      ) : _currentScreen->nextLine             (          ); break; //VT100
    case TY_ESC('H'      ) : _currentScreen->changeTabStop        (true      ); break; //VT100
    case TY_ESC('M'      ) : _currentScreen->reverseIndex         (          ); break; //VT100
    case TY_ESC('Z'      ) :      reportTerminalType   (          ); break;
    case TY_ESC('c'      ) :      reset                (          ); break;

    case TY_ESC('n'      ) :      useCharset           (         2); break;
    case TY_ESC('o'      ) :      useCharset           (         3); break;
    case TY_ESC('7'      ) :      saveCursor           (          ); break;
    case TY_ESC('8'      ) :      restoreCursor        (          ); break;

    case TY_ESC('='      ) :          setMode      (MODE_AppKeyPad); break;
    case TY_ESC('>'      ) :        resetMode      (MODE_AppKeyPad); break;
    case TY_ESC('<'      ) :          setMode      (MODE_Ansi     ); break; //VT100

    case TY_ESC_CS('(', '0') :      setCharset           (0,    '0'); break; //VT100
    case TY_ESC_CS('(', 'A') :      setCharset           (0,    'A'); break; //VT100
    case TY_ESC_CS('(', 'B') :      setCharset           (0,    'B'); break; //VT100

    case TY_ESC_CS(')', '0') :      setCharset           (1,    '0'); break; //VT100
    case TY_ESC_CS(')', 'A') :      setCharset           (1,    'A'); break; //VT100
    case TY_ESC_CS(')', 'B') :      setCharset           (1,    'B'); break; //VT100

    case TY_ESC_CS('*', '0') :      setCharset           (2,    '0'); break; //VT100
    case TY_ESC_CS('*', 'A') :      setCharset           (2,    'A'); break; //VT100
    case TY_ESC_CS('*', 'B') :      setCharset           (2,    'B'); break; //VT100

    case TY_ESC_CS('+', '0') :      setCharset           (3,    '0'); break; //VT100
    case TY_ESC_CS('+', 'A') :      setCharset           (3,    'A'); break; //VT100
    case TY_ESC_CS('+', 'B') :      setCharset           (3,    'B'); break; //VT100

    case TY_ESC_CS('%', 'G') :      /* TODO setCodec             (Utf8Codec   );*/ break; //LINUX
    case TY_ESC_CS('%', '@') :      /* TODO setCodec             (LocaleCodec );*/ break; //LINUX

    case TY_ESC_DE('3'      ) : /* Double height line, top half    */
                                _currentScreen->setLineProperty( LineProperty::DoubleWidth , true );
                                _currentScreen->setLineProperty( LineProperty::DoubleHeight , true );
                                    break;
    case TY_ESC_DE('4'      ) : /* Double height line, bottom half */
                                _currentScreen->setLineProperty( LineProperty::DoubleWidth , true );
                                _currentScreen->setLineProperty( LineProperty::DoubleHeight , true );
                                    break;
    case TY_ESC_DE('5'      ) : /* Single width, single height line*/
                                _currentScreen->setLineProperty( LineProperty::DoubleWidth , false);
                                _currentScreen->setLineProperty( LineProperty::DoubleHeight , false);
                                break;
    case TY_ESC_DE('6'      ) : /* Double width, single height line*/
                                _currentScreen->setLineProperty( LineProperty::DoubleWidth , true);
                                _currentScreen->setLineProperty( LineProperty::DoubleHeight , false);
                                break;
    case TY_ESC_DE('8'      ) : _currentScreen->helpAlign            (          ); break;

// resize = \e[8;<row>;<col>t
    case TY_CSI_PS('t',   8) : // TODO setImageSize( p /*lines */, q /* columns */ );
                               // TODO emit imageResizeRequest(QSize(q, p));
                               break;

// change tab text color : \e[28;<color>t  color: 0-16,777,215
    case TY_CSI_PS('t',   28) : /* TODO emit changeTabTextColorRequest      ( p        );*/          break;

    case TY_CSI_PS('K',   0) : _currentScreen->clearToEndOfLine     (          ); break;
    case TY_CSI_PS('K',   1) : _currentScreen->clearToBeginOfLine   (          ); break;
    case TY_CSI_PS('K',   2) : _currentScreen->clearEntireLine      (          ); break;
    case TY_CSI_PS('J',   0) : _currentScreen->clearToEndOfScreen   (          ); break;
    case TY_CSI_PS('J',   1) : _currentScreen->clearToBeginOfScreen (          ); break;
    case TY_CSI_PS('J',   2) : _currentScreen->clearEntireScreen    (          ); break;
    case TY_CSI_PS('J',      3) : /* TODO clearHistory();*/                            break;
    case TY_CSI_PS('g',   0) : _currentScreen->changeTabStop        (false     ); break; //VT100
    case TY_CSI_PS('g',   3) : _currentScreen->clearTabStops        (          ); break; //VT100
    case TY_CSI_PS('h',   4) : _currentScreen->    setMode      (Mode::Insert   ); break;
    case TY_CSI_PS('h',  20) :          setMode      (Mode::NewLine  ); break;
    case TY_CSI_PS('i',   0) : /* IGNORE: attached printer          */ break; //VT100
    case TY_CSI_PS('l',   4) : _currentScreen->  resetMode      (Mode::Insert   ); break;
    case TY_CSI_PS('l',  20) :        resetMode      (Mode::NewLine  ); break;
    case TY_CSI_PS('s',   0) :      saveCursor           (          ); break;
    case TY_CSI_PS('u',   0) :      restoreCursor        (          ); break;

    case TY_CSI_PS('m',   0) : _currentScreen->setDefaultRendition  (          ); break;
    case TY_CSI_PS('m',   1) : _currentScreen->  setRendition     (Rendition::Bold     ); break; //VT100
    case TY_CSI_PS('m',   3) : _currentScreen->  setRendition     (Rendition::Italic   ); break; //VT100
    case TY_CSI_PS('m',   4) : _currentScreen->  setRendition     (Rendition::Underline); break; //VT100
    case TY_CSI_PS('m',   5) : _currentScreen->  setRendition     (Rendition::Blink    ); break; //VT100
    case TY_CSI_PS('m',   7) : _currentScreen->  setRendition     (Rendition::Reverse  ); break;
    case TY_CSI_PS('m',  10) : /* IGNORED: mapping related          */ break; //LINUX
    case TY_CSI_PS('m',  11) : /* IGNORED: mapping related          */ break; //LINUX
    case TY_CSI_PS('m',  12) : /* IGNORED: mapping related          */ break; //LINUX
    case TY_CSI_PS('m',  22) : _currentScreen->resetRendition     (Rendition::Bold     ); break;
    case TY_CSI_PS('m',  23) : _currentScreen->resetRendition     (Rendition::Italic   ); break; //VT100
    case TY_CSI_PS('m',  24) : _currentScreen->resetRendition     (Rendition::Underline); break;
    case TY_CSI_PS('m',  25) : _currentScreen->resetRendition     (Rendition::Blink    ); break;
    case TY_CSI_PS('m',  27) : _currentScreen->resetRendition     (Rendition::Reverse  ); break;

    case TY_CSI_PS('m',   30) : _currentScreen->setForeColor         (ColorSpace::System,  0); break;
    case TY_CSI_PS('m',   31) : _currentScreen->setForeColor         (ColorSpace::System,  1); break;
    case TY_CSI_PS('m',   32) : _currentScreen->setForeColor         (ColorSpace::System,  2); break;
    case TY_CSI_PS('m',   33) : _currentScreen->setForeColor         (ColorSpace::System,  3); break;
    case TY_CSI_PS('m',   34) : _currentScreen->setForeColor         (ColorSpace::System,  4); break;
    case TY_CSI_PS('m',   35) : _currentScreen->setForeColor         (ColorSpace::System,  5); break;
    case TY_CSI_PS('m',   36) : _currentScreen->setForeColor         (ColorSpace::System,  6); break;
    case TY_CSI_PS('m',   37) : _currentScreen->setForeColor         (ColorSpace::System,  7); break;

    case TY_CSI_PS('m',   38) : _currentScreen->setForeColor         (ColorSpace(p),       q); break;

    case TY_CSI_PS('m',   39) : _currentScreen->setForeColor         (ColorSpace::Default,  0); break;

    case TY_CSI_PS('m',   40) : _currentScreen->setBackColor         (ColorSpace::System,  0); break;
    case TY_CSI_PS('m',   41) : _currentScreen->setBackColor         (ColorSpace::System,  1); break;
    case TY_CSI_PS('m',   42) : _currentScreen->setBackColor         (ColorSpace::System,  2); break;
    case TY_CSI_PS('m',   43) : _currentScreen->setBackColor         (ColorSpace::System,  3); break;
    case TY_CSI_PS('m',   44) : _currentScreen->setBackColor         (ColorSpace::System,  4); break;
    case TY_CSI_PS('m',   45) : _currentScreen->setBackColor         (ColorSpace::System,  5); break;
    case TY_CSI_PS('m',   46) : _currentScreen->setBackColor         (ColorSpace::System,  6); break;
    case TY_CSI_PS('m',   47) : _currentScreen->setBackColor         (ColorSpace::System,  7); break;

    case TY_CSI_PS('m',   48) : _currentScreen->setBackColor         (ColorSpace(p),       q); break;

    case TY_CSI_PS('m',   49) : _currentScreen->setBackColor         (ColorSpace::Default,  1); break;

    case TY_CSI_PS('m',   90) : _currentScreen->setForeColor         (ColorSpace::System,  8); break;
    case TY_CSI_PS('m',   91) : _currentScreen->setForeColor         (ColorSpace::System,  9); break;
    case TY_CSI_PS('m',   92) : _currentScreen->setForeColor         (ColorSpace::System, 10); break;
    case TY_CSI_PS('m',   93) : _currentScreen->setForeColor         (ColorSpace::System, 11); break;
    case TY_CSI_PS('m',   94) : _currentScreen->setForeColor         (ColorSpace::System, 12); break;
    case TY_CSI_PS('m',   95) : _currentScreen->setForeColor         (ColorSpace::System, 13); break;
    case TY_CSI_PS('m',   96) : _currentScreen->setForeColor         (ColorSpace::System, 14); break;
    case TY_CSI_PS('m',   97) : _currentScreen->setForeColor         (ColorSpace::System, 15); break;

    case TY_CSI_PS('m',  100) : _currentScreen->setBackColor         (ColorSpace::System,  8); break;
    case TY_CSI_PS('m',  101) : _currentScreen->setBackColor         (ColorSpace::System,  9); break;
    case TY_CSI_PS('m',  102) : _currentScreen->setBackColor         (ColorSpace::System, 10); break;
    case TY_CSI_PS('m',  103) : _currentScreen->setBackColor         (ColorSpace::System, 11); break;
    case TY_CSI_PS('m',  104) : _currentScreen->setBackColor         (ColorSpace::System, 12); break;
    case TY_CSI_PS('m',  105) : _currentScreen->setBackColor         (ColorSpace::System, 13); break;
    case TY_CSI_PS('m',  106) : _currentScreen->setBackColor         (ColorSpace::System, 14); break;
    case TY_CSI_PS('m',  107) : _currentScreen->setBackColor         (ColorSpace::System, 15); break;

    case TY_CSI_PS('n',   5) :      reportStatus         (          ); break;
    case TY_CSI_PS('n',   6) :      reportCursorPosition (          ); break;
    case TY_CSI_PS('q',   0) : /* IGNORED: LEDs off                 */ break; //VT100
    case TY_CSI_PS('q',   1) : /* IGNORED: LED1 on                  */ break; //VT100
    case TY_CSI_PS('q',   2) : /* IGNORED: LED2 on                  */ break; //VT100
    case TY_CSI_PS('q',   3) : /* IGNORED: LED3 on                  */ break; //VT100
    case TY_CSI_PS('q',   4) : /* IGNORED: LED4 on                  */ break; //VT100
    case TY_CSI_PS('x',   0) :      reportTerminalParms  (         2); break; //VT100
    case TY_CSI_PS('x',   1) :      reportTerminalParms  (         3); break; //VT100

    case TY_CSI_PN('@'      ) : _currentScreen->insertChars          (p         ); break;
    case TY_CSI_PN('A'      ) : _currentScreen->cursorUp             (p         ); break; //VT100
    case TY_CSI_PN('B'      ) : _currentScreen->cursorDown           (p         ); break; //VT100
    case TY_CSI_PN('C'      ) : _currentScreen->cursorRight          (p         ); break; //VT100
    case TY_CSI_PN('D'      ) : _currentScreen->cursorLeft           (p         ); break; //VT100
    case TY_CSI_PN('E'      ) : /* Not implemented: cursor next p lines */         break; //VT100
    case TY_CSI_PN('F'      ) : /* Not implemented: cursor preceding p lines */    break; //VT100
    case TY_CSI_PN('G'      ) : _currentScreen->setCursorX           (p         ); break; //LINUX
    case TY_CSI_PN('H'      ) : _currentScreen->setCursorYX          (p,      q); break; //VT100
    case TY_CSI_PN('I'      ) : _currentScreen->tab                  (p         ); break;
    case TY_CSI_PN('L'      ) : _currentScreen->insertLines          (p         ); break;
    case TY_CSI_PN('M'      ) : _currentScreen->deleteLines          (p         ); break;
    case TY_CSI_PN('P'      ) : _currentScreen->deleteChars          (p         ); break;
    case TY_CSI_PN('S'      ) : _currentScreen->scrollUp             (p         ); break;
    case TY_CSI_PN('T'      ) : _currentScreen->scrollDown           (p         ); break;
    case TY_CSI_PN('X'      ) : _currentScreen->eraseChars           (p         ); break;
    case TY_CSI_PN('Z'      ) : _currentScreen->backtab              (p         ); break;
    case TY_CSI_PN('c'      ) :      reportTerminalType   (          ); break; //VT100
    case TY_CSI_PN('d'      ) : _currentScreen->setCursorY           (p         ); break; //LINUX
    case TY_CSI_PN('f'      ) : _currentScreen->setCursorYX          (p,      q); break; //VT100
    case TY_CSI_PN('r'      ) :      setMargins           (p,      q); break; //VT100
    case TY_CSI_PN('y'      ) : /* IGNORED: Confidence test          */ break; //VT100

    case TY_CSI_PR('h',   1) :          setMode      (MODE_AppCuKeys); break; //VT100
    case TY_CSI_PR('l',   1) :        resetMode      (MODE_AppCuKeys); break; //VT100
    case TY_CSI_PR('s',   1) :         saveMode      (MODE_AppCuKeys); break; //FIXME
    case TY_CSI_PR('r',   1) :      restoreMode      (MODE_AppCuKeys); break; //FIXME

    case TY_CSI_PR('l',   2) :        resetMode      (MODE_Ansi     ); break; //VT100

    case TY_CSI_PR('h',   3) :          setMode      (MODE_132Columns); break; //VT100
    case TY_CSI_PR('l',   3) :        resetMode      (MODE_132Columns); break; //VT100

    case TY_CSI_PR('h',   4) : /* IGNORED: soft scrolling           */ break; //VT100
    case TY_CSI_PR('l',   4) : /* IGNORED: soft scrolling           */ break; //VT100

    case TY_CSI_PR('h',   5) : _currentScreen->    setMode      (Mode::Screen   ); break; //VT100
    case TY_CSI_PR('l',   5) : _currentScreen->  resetMode      (Mode::Screen   ); break; //VT100

    case TY_CSI_PR('h',   6) : _currentScreen->    setMode      (Mode::Origin   ); break; //VT100
    case TY_CSI_PR('l',   6) : _currentScreen->  resetMode      (Mode::Origin   ); break; //VT100
    case TY_CSI_PR('s',   6) : _currentScreen->   saveMode      (Mode::Origin   ); break; //FIXME
    case TY_CSI_PR('r',   6) : _currentScreen->restoreMode      (Mode::Origin   ); break; //FIXME

    case TY_CSI_PR('h',   7) : _currentScreen->    setMode      (Mode::Wrap     ); break; //VT100
    case TY_CSI_PR('l',   7) : _currentScreen->  resetMode      (Mode::Wrap     ); break; //VT100
    case TY_CSI_PR('s',   7) : _currentScreen->   saveMode      (Mode::Wrap     ); break; //FIXME
    case TY_CSI_PR('r',   7) : _currentScreen->restoreMode      (Mode::Wrap     ); break; //FIXME

    case TY_CSI_PR('h',   8) : /* IGNORED: autorepeat on            */ break; //VT100
    case TY_CSI_PR('l',   8) : /* IGNORED: autorepeat off           */ break; //VT100
    case TY_CSI_PR('s',   8) : /* IGNORED: autorepeat on            */ break; //VT100
    case TY_CSI_PR('r',   8) : /* IGNORED: autorepeat off           */ break; //VT100

    case TY_CSI_PR('h',   9) : /* IGNORED: interlace                */ break; //VT100
    case TY_CSI_PR('l',   9) : /* IGNORED: interlace                */ break; //VT100
    case TY_CSI_PR('s',   9) : /* IGNORED: interlace                */ break; //VT100
    case TY_CSI_PR('r',   9) : /* IGNORED: interlace                */ break; //VT100

    case TY_CSI_PR('h',  12) : /* IGNORED: Cursor blink             */ break; //att610
    case TY_CSI_PR('l',  12) : /* IGNORED: Cursor blink             */ break; //att610
    case TY_CSI_PR('s',  12) : /* IGNORED: Cursor blink             */ break; //att610
    case TY_CSI_PR('r',  12) : /* IGNORED: Cursor blink             */ break; //att610

    case TY_CSI_PR('h',  25) :          setMode      (Mode::Cursor   ); break; //VT100
    case TY_CSI_PR('l',  25) :        resetMode      (Mode::Cursor   ); break; //VT100
    case TY_CSI_PR('s',  25) :         saveMode      (Mode::Cursor   ); break; //VT100
    case TY_CSI_PR('r',  25) :      restoreMode      (Mode::Cursor   ); break; //VT100

    case TY_CSI_PR('h',  40) :         setMode(MODE_Allow132Columns ); break; // XTERM
    case TY_CSI_PR('l',  40) :       resetMode(MODE_Allow132Columns ); break; // XTERM

    case TY_CSI_PR('h',  41) : /* IGNORED: obsolete more(1) fix     */ break; //XTERM
    case TY_CSI_PR('l',  41) : /* IGNORED: obsolete more(1) fix     */ break; //XTERM
    case TY_CSI_PR('s',  41) : /* IGNORED: obsolete more(1) fix     */ break; //XTERM
    case TY_CSI_PR('r',  41) : /* IGNORED: obsolete more(1) fix     */ break; //XTERM

    case TY_CSI_PR('h',  47) :          setMode      (MODE_AppScreen); break; //VT100
    case TY_CSI_PR('l',  47) :        resetMode      (MODE_AppScreen); break; //VT100
    case TY_CSI_PR('s',  47) :         saveMode      (MODE_AppScreen); break; //XTERM
    case TY_CSI_PR('r',  47) :      restoreMode      (MODE_AppScreen); break; //XTERM

    case TY_CSI_PR('h',  67) : /* IGNORED: DECBKM                   */ break; //XTERM
    case TY_CSI_PR('l',  67) : /* IGNORED: DECBKM                   */ break; //XTERM
    case TY_CSI_PR('s',  67) : /* IGNORED: DECBKM                   */ break; //XTERM
    case TY_CSI_PR('r',  67) : /* IGNORED: DECBKM                   */ break; //XTERM

    // XTerm defines the following modes:
    // SET_VT200_MOUSE             1000
    // SET_VT200_HIGHLIGHT_MOUSE   1001
    // SET_BTN_EVENT_MOUSE         1002
    // SET_ANY_EVENT_MOUSE         1003
    //

    //Note about mouse modes:
    //There are four mouse modes which xterm-compatible terminals can support - 1000,1001,1002,1003
    //Konsole currently supports mode 1000 (basic mouse press and release) and mode 1002 (dragging the mouse).
    //TODO:  Implementation of mouse modes 1001 (something called hilight tracking) and
    //1003 (a slight variation on dragging the mouse)
    //

    case TY_CSI_PR('h', 1000) :          setMode      (MODE_Mouse1000); break; //XTERM
    case TY_CSI_PR('l', 1000) :        resetMode      (MODE_Mouse1000); break; //XTERM
    case TY_CSI_PR('s', 1000) :         saveMode      (MODE_Mouse1000); break; //XTERM
    case TY_CSI_PR('r', 1000) :      restoreMode      (MODE_Mouse1000); break; //XTERM

    case TY_CSI_PR('h', 1001) : /* IGNORED: hilite mouse tracking    */ break; //XTERM
    case TY_CSI_PR('l', 1001) :        resetMode      (MODE_Mouse1001); break; //XTERM
    case TY_CSI_PR('s', 1001) : /* IGNORED: hilite mouse tracking    */ break; //XTERM
    case TY_CSI_PR('r', 1001) : /* IGNORED: hilite mouse tracking    */ break; //XTERM

    case TY_CSI_PR('h', 1002) :          setMode      (MODE_Mouse1002); break; //XTERM
    case TY_CSI_PR('l', 1002) :        resetMode      (MODE_Mouse1002); break; //XTERM
    case TY_CSI_PR('s', 1002) :         saveMode      (MODE_Mouse1002); break; //XTERM
    case TY_CSI_PR('r', 1002) :      restoreMode      (MODE_Mouse1002); break; //XTERM

    case TY_CSI_PR('h', 1003) :          setMode      (MODE_Mouse1003); break; //XTERM
    case TY_CSI_PR('l', 1003) :        resetMode      (MODE_Mouse1003); break; //XTERM
    case TY_CSI_PR('s', 1003) :         saveMode      (MODE_Mouse1003); break; //XTERM
    case TY_CSI_PR('r', 1003) :      restoreMode      (MODE_Mouse1003); break; //XTERM

    case TY_CSI_PR('h',  1004) : _reportFocusEvents = true; break;
    case TY_CSI_PR('l',  1004) : _reportFocusEvents = false; break;

    case TY_CSI_PR('h', 1005) :          setMode      (MODE_Mouse1005); break; //XTERM
    case TY_CSI_PR('l', 1005) :        resetMode      (MODE_Mouse1005); break; //XTERM
    case TY_CSI_PR('s', 1005) :         saveMode      (MODE_Mouse1005); break; //XTERM
    case TY_CSI_PR('r', 1005) :      restoreMode      (MODE_Mouse1005); break; //XTERM

    case TY_CSI_PR('h', 1006) :          setMode      (MODE_Mouse1006); break; //XTERM
    case TY_CSI_PR('l', 1006) :        resetMode      (MODE_Mouse1006); break; //XTERM
    case TY_CSI_PR('s', 1006) :         saveMode      (MODE_Mouse1006); break; //XTERM
    case TY_CSI_PR('r', 1006) :      restoreMode      (MODE_Mouse1006); break; //XTERM

    case TY_CSI_PR('h', 1015) :          setMode      (MODE_Mouse1015); break; //URXVT
    case TY_CSI_PR('l', 1015) :        resetMode      (MODE_Mouse1015); break; //URXVT
    case TY_CSI_PR('s', 1015) :         saveMode      (MODE_Mouse1015); break; //URXVT
    case TY_CSI_PR('r', 1015) :      restoreMode      (MODE_Mouse1015); break; //URXVT

    case TY_CSI_PR('h', 1034) : /* IGNORED: 8bitinput activation     */ break; //XTERM

    case TY_CSI_PR('h', 1047) :          setMode      (MODE_AppScreen); break; //XTERM
    case TY_CSI_PR('l', 1047) : /*TODO _screen[1]*/_currentScreen->clearEntireScreen(); resetMode(MODE_AppScreen); break; //XTERM
    case TY_CSI_PR('s', 1047) :         saveMode      (MODE_AppScreen); break; //XTERM
    case TY_CSI_PR('r', 1047) :      restoreMode      (MODE_AppScreen); break; //XTERM

    //FIXME: Unitoken: save translations
    case TY_CSI_PR('h', 1048) :      saveCursor           (          ); break; //XTERM
    case TY_CSI_PR('l', 1048) :      restoreCursor        (          ); break; //XTERM
    case TY_CSI_PR('s', 1048) :      saveCursor           (          ); break; //XTERM
    case TY_CSI_PR('r', 1048) :      restoreCursor        (          ); break; //XTERM

    //FIXME: every once new sequences like this pop up in xterm.
    //       Here's a guess of what they could mean.
    case TY_CSI_PR('h', 1049) : saveCursor(); /*TODO _screen[1]*/_currentScreen->clearEntireScreen(); setMode(MODE_AppScreen); break; //XTERM
    case TY_CSI_PR('l', 1049) : resetMode(MODE_AppScreen); restoreCursor(); break; //XTERM

    case TY_CSI_PR('h', 2004) :          setMode      (MODE_BracketedPaste); break; //XTERM
    case TY_CSI_PR('l', 2004) :        resetMode      (MODE_BracketedPaste); break; //XTERM
    case TY_CSI_PR('s', 2004) :         saveMode      (MODE_BracketedPaste); break; //XTERM
    case TY_CSI_PR('r', 2004) :      restoreMode      (MODE_BracketedPaste); break; //XTERM

    //FIXME: weird DEC reset sequence
    case TY_CSI_PE('p'      ) : /* IGNORED: reset         (        ) */ break;

    //FIXME: when changing between vt52 and ansi mode evtl do some resetting.
    case TY_VT52('A'      ) : _currentScreen->cursorUp             (         1); break; //VT52
    case TY_VT52('B'      ) : _currentScreen->cursorDown           (         1); break; //VT52
    case TY_VT52('C'      ) : _currentScreen->cursorRight          (         1); break; //VT52
    case TY_VT52('D'      ) : _currentScreen->cursorLeft           (         1); break; //VT52

    case TY_VT52('F'      ) :      setAndUseCharset     (0,    '0'); break; //VT52
    case TY_VT52('G'      ) :      setAndUseCharset     (0,    'B'); break; //VT52

    case TY_VT52('H'      ) : _currentScreen->setCursorYX          (1,1       ); break; //VT52
    case TY_VT52('I'      ) : _currentScreen->reverseIndex         (          ); break; //VT52
    case TY_VT52('J'      ) : _currentScreen->clearToEndOfScreen   (          ); break; //VT52
    case TY_VT52('K'      ) : _currentScreen->clearToEndOfLine     (          ); break; //VT52
    case TY_VT52('Y'      ) : _currentScreen->setCursorYX          (p-31,q-31 ); break; //VT52
    case TY_VT52('Z'      ) :      reportTerminalType   (           ); break; //VT52
    case TY_VT52('<'      ) :          setMode      (MODE_Ansi     ); break; //VT52
    case TY_VT52('='      ) :          setMode      (MODE_AppKeyPad); break; //VT52
    case TY_VT52('>'      ) :        resetMode      (MODE_AppKeyPad); break; //VT52

    case TY_CSI_PG('c'      ) :  reportSecondaryAttributes(          ); break; //VT100

    default:
        reportDecodingError();
        break;
  };
}

void Vt102Emulation::clearScreenAndSetColumns(int /*columnCount*/)
{
    // TODO setImageSize(_currentScreen->getLines(),columnCount);
    clearEntireScreen();
    setDefaultMargins();
    _currentScreen->setCursorYX(0,0);
}

/* ------------------------------------------------------------------------- */
/*                                                                           */
/*                                VT100 Charsets                             */
/*                                                                           */
/* ------------------------------------------------------------------------- */

// Character Set Conversion ------------------------------------------------ --

/*
   The processing contains a VT100 specific code translation layer.
   It's still in use and mainly responsible for the line drawing graphics.

   These and some other glyphs are assigned to codes (0x5f-0xfe)
   normally occupied by the latin letters. Since this codes also
   appear within control sequences, the extra code conversion
   does not permute with the tokenizer and is placed behind it
   in the pipeline. It only applies to tokens, which represent
   plain characters.

   This conversion it eventually continued in TerminalDisplay.C, since
   it might involve VT100 enhanced fonts, which have these
   particular glyphs allocated in (0x00-0x1f) in their code page.
*/

#define CHARSET _charset[_currentScreen==_screen[1]]

// Apply current character map.

unsigned short Vt102Emulation::applyCharset(unsigned short c)
{
    return c;
    // TODO
    //if (CHARSET.graphic && 0x5f <= c && c <= 0x7e) return vt100_graphics[c - 0x5f];
    //if (CHARSET.pound && c == '#') return 0xa3;  //This mode is obsolete
    return c;
}

/*
   "Charset" related part of the emulation state.
   This configures the VT100 charset filter.

   While most operation work on the current _screen,
   the following two are different.
*/

void Vt102Emulation::resetCharset(int scrno)
{
    _charset[scrno].cu_cs = 0;
    strncpy(_charset[scrno].charset, "BBBB", 4);
    _charset[scrno].sa_graphic = false;
    _charset[scrno].sa_pound = false;
    _charset[scrno].graphic = false;
    _charset[scrno].pound = false;
}

void Vt102Emulation::setCharset(int n, int cs) // on both screens.
{
    _charset[0].charset[n & 3] = static_cast<char>(cs); useCharset(_charset[0].cu_cs);
    _charset[1].charset[n & 3] = static_cast<char>(cs); useCharset(_charset[1].cu_cs);
}

void Vt102Emulation::setAndUseCharset(int n, int /*cs*/)
{
    // TODO CHARSET.charset[n & 3] = cs;
    useCharset(n & 3);
}

void Vt102Emulation::useCharset(int /*n*/)
{
    // TODO CHARSET.cu_cs   = n & 3;
    // TODO CHARSET.graphic = (CHARSET.charset[n & 3] == '0');
    // TODO CHARSET.pound   = (CHARSET.charset[n & 3] == 'A'); //This mode is obsolete
}

void Vt102Emulation::setDefaultMargins()
{
    // TODO _screen[0]->setDefaultMargins();
    // TODO _screen[1]->setDefaultMargins();
    _currentScreen->setDefaultMargins();
}

void Vt102Emulation::setMargins(int t, int b)
{
    // TODO _screen[0]->setMargins(t, b);
    // TODO _screen[1]->setMargins(t, b);
    _currentScreen->setMargins(t, b);
}

void Vt102Emulation::saveCursor()
{
    // TODO CHARSET.sa_graphic = CHARSET.graphic;
    // TODO CHARSET.sa_pound   = CHARSET.pound; //This mode is obsolete
    // we are not clear about these
    //sa_charset = charsets[cScreen->_charset];
    //sa_charset_num = cScreen->_charset;
    _currentScreen->saveCursor();
}

void Vt102Emulation::restoreCursor()
{
    // TODO CHARSET.graphic = CHARSET.sa_graphic;
    // TODO CHARSET.pound   = CHARSET.sa_pound; //This mode is obsolete
    _currentScreen->restoreCursor();
}

/* ------------------------------------------------------------------------- */
/*                                                                           */
/*                                Mode Operations                            */
/*                                                                           */
/* ------------------------------------------------------------------------- */

/*
   Some of the emulations state is either added to the state of the screens.

   This causes some scoping problems, since different emulations choose to
   located the mode either to the current _screen or to both.

   For strange reasons, the extend of the rendition attributes ranges over
   all screens and not over the actual _screen.

   We decided on the precise precise extend, somehow.
*/

// "Mode" related part of the state. These are all booleans.

void Vt102Emulation::resetModes()
{
    // MODE_Allow132Columns is not reset here
    // to match Xterm's behavior (see Xterm's VTReset() function)

    resetMode(MODE_132Columns); saveMode(MODE_132Columns);
    resetMode(MODE_Mouse1000);  saveMode(MODE_Mouse1000);
    resetMode(MODE_Mouse1001);  saveMode(MODE_Mouse1001);
    resetMode(MODE_Mouse1002);  saveMode(MODE_Mouse1002);
    resetMode(MODE_Mouse1003);  saveMode(MODE_Mouse1003);
    resetMode(MODE_Mouse1005);  saveMode(MODE_Mouse1005);
    resetMode(MODE_Mouse1006);  saveMode(MODE_Mouse1006);
    resetMode(MODE_Mouse1015);  saveMode(MODE_Mouse1015);
    resetMode(MODE_BracketedPaste);  saveMode(MODE_BracketedPaste);

    resetMode(MODE_AppScreen);  saveMode(MODE_AppScreen);
    resetMode(MODE_AppCuKeys);  saveMode(MODE_AppCuKeys);
    resetMode(MODE_AppKeyPad);  saveMode(MODE_AppKeyPad);
    resetMode(Mode::NewLine);
    setMode(MODE_Ansi);
}

void Vt102Emulation::setMode(int m)
{
    _currentModes.mode[m] = true;
    switch (m) {
    case MODE_132Columns:
        if (getMode(MODE_Allow132Columns))
            clearScreenAndSetColumns(132);
        else
            _currentModes.mode[m] = false;
        break;
    case MODE_Mouse1000:
    case MODE_Mouse1001:
    case MODE_Mouse1002:
    case MODE_Mouse1003:
        // TODO emit programUsesMouseChanged(false);
        break;

    case MODE_BracketedPaste:
        // TODO emit programBracketedPasteModeChanged(true);
        break;

    case MODE_AppScreen :
        // TODO _screen[1]->clearSelection();
        // TODO setScreen(1);
        break;
    }
    // FIXME: Currently this has a redundant condition as MODES_SCREEN is 6
    // and MODE_NewLine is 5
    if (m < MODES_SCREEN || static_cast<Mode::enum_t>(m) == Mode::NewLine) {
        // TODO _screen[0]->setMode(m);
        // TODO _screen[1]->setMode(m);
        _currentScreen->setMode(m);
    }
}

void Vt102Emulation::resetMode(int m)
{
    _currentModes.mode[m] = false;
    switch (m) {
    case MODE_132Columns:
        if (getMode(MODE_Allow132Columns))
            clearScreenAndSetColumns(80);
        break;
    case MODE_Mouse1000 :
    case MODE_Mouse1001 :
    case MODE_Mouse1002 :
    case MODE_Mouse1003 :
        // TODO emit programUsesMouseChanged(true);
        break;

    case MODE_BracketedPaste:
        // TODO emit programBracketedPasteModeChanged(false);
        break;

    case MODE_AppScreen :
        // TODO _screen[0]->clearSelection();
        // TODO setScreen(0);
        break;
    }
    // FIXME: Currently this has a redundant condition as MODES_SCREEN is 6
    // and MODE_NewLine is 5
    if (m < MODES_SCREEN || static_cast<Mode::enum_t>(m) == Mode::NewLine) {
        // TODO _screen[0]->resetMode(m);
        // TODO _screen[1]->resetMode(m);
        _currentScreen->resetMode(m);
    }
}

void Vt102Emulation::saveMode(int m)
{
    _savedModes.mode[m] = _currentModes.mode[m];
}

void Vt102Emulation::restoreMode(int m)
{
    if (_savedModes.mode[m])
        setMode(m);
    else
        resetMode(m);
}

bool Vt102Emulation::getMode(int m)
{
    return _currentModes.mode[m];
}

char Vt102Emulation::eraseChar() const
{
    // TODO
//     KeyboardTranslator::Entry entry = _keyTranslator->findEntry(
//                                           Qt::Key_Backspace,
//                                           0,
//                                           0);
//     if (entry.text().count() > 0)
//         return entry.text()[0];
//     else
//         return '\b';
    return '\0';
}

#if 0
// print contents of the scan buffer
static void hexdump(int* s, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        if (s[i] == '\\')
            printf("\\\\");
        else if ((s[i]) > 32 && s[i] < 127)
            printf("%c", s[i]);
        else
            printf("\\%04x(hex)", s[i]);
    }
}
#endif

// return contents of the scan buffer
static std::string hexdump2(int* s, int len)
{
    int i;
    char dump[128];
    std::string returnDump;

    for (i = 0; i < len; i++) {
        if (s[i] == '\\')
            snprintf(dump, sizeof(dump), "%s", "\\\\");
        else if ((s[i]) > 32 && s[i] < 127)
            snprintf(dump, sizeof(dump), "%c", s[i]);
        else
            snprintf(dump, sizeof(dump), "\\%04x(hex)", s[i]);
        returnDump.append(std::string(dump));
    }
    return returnDump;
}

void Vt102Emulation::reportDecodingError()
{
    if (tokenBufferPos == 0 || (tokenBufferPos == 1 && (tokenBuffer[0] & 0xff) >= 32))
        return;

//    printf("Undecodable sequence: ");
//    hexdump(tokenBuffer, tokenBufferPos);
//    printf("\n");

    std::string outputError("Undecodable sequence: ");
    outputError.append(hexdump2(tokenBuffer, tokenBufferPos));
    //qDebug() << outputError;
}

}


#include <iostream>

template<class T>
auto print_value(std::ostream & os, T const & value)
-> decltype(void(os << value))
{ os << value; }

// void print_value(std::ostream & os, vt100::Cursor const & cursor)
// { os << "{" << cursor.x << " " << cursor.y << "}"; }


struct Checker
{
    char const * file;
    int line;
    const char * expr;

    template<class T, class U>
    void check_equal(T const & a, U const & b)
    {
        if (!(a == b)) {
            std::cerr << file << ":" << line << ": " << expr << " -- [";
            print_value(std::cerr, a);
            std::cerr << " == ";
            print_value(std::cerr, b);
            std::cerr << "] failed\n";
            std::abort();
        }
    }
};

#ifndef IN_IDE_PARSER
# define BOOST_EQUAL_CHECK(a, b) Checker{__FILE__, __LINE__, #a " == " #b}.check_equal(a, b)
#else
# define BOOST_EQUAL_CHECK
#endif

#include <iomanip>

int main()
{
    vt100::Screen screen(40, 40);
    screen.cursorDown(1);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 1);
    screen.cursorDown(1);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 2);
    screen.cursorDown(2);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 4);
    screen.cursorDown(40);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 39);
    screen.cursorUp(10);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 29);
    screen.cursorUp(40);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 0);
    screen.cursorRight(2);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 2);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 0);
    screen.cursorRight(200);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 39);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 0);
    screen.cursorLeft(3);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 36);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 0);
    screen.cursorLeft(40);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 0);
    screen.cursorRight(1);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 1);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 0);
    screen.cursorLeft(2);
    BOOST_EQUAL_CHECK(screen.cursor_x(), 0);
    BOOST_EQUAL_CHECK(screen.cursor_y(), 0);

    vt100::Vt102Emulation emulator(&screen);
    for (int i = 0; i < 20; ++i) {
        emulator.receiveChar('a');
        emulator.receiveChar('b');
        emulator.receiveChar('c');
    }
    emulator.receiveChar('\033');
    emulator.receiveChar('[');
    emulator.receiveChar('0');
    emulator.receiveChar('B');
    for (int i = 0; i < 20; ++i) {
        emulator.receiveChar('a');
        emulator.receiveChar('b');
        emulator.receiveChar('c');
    }

    int i = 0;
    for (auto const & line : screen.getScreenLines()) {
        std::cout << std::setw(4) << ++i << " ";
        for (vt100::Character const & ch : line) {
            std::cout << char(ch.character ? ch.character : '?');
        }
        std::cout << '\n';
    }
}
