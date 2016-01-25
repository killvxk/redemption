#ifndef _REDEMPTION_KEYBOARD_KEYLAYOUT_R_X80030409_HPP_
#define _REDEMPTION_KEYBOARD_KEYLAYOUT_R_X80030409_HPP_

#include "keylayout_r.hpp"

namespace x80030409{ 

const static int LCID = 0x30409;

const static char * const locale_name = "en-US.dvorak_left";

const Keylayout_r::KeyLayoutMap_t noMod
{
	{ 0x001b, 0x1 },
	{ 0x005b, 0x2 },
	{ 0x005d, 0x3 },
	{ 0x002f, 0x4 },
	{ 0x0070, 0x5 },
	{ 0x0066, 0x6 },
	{ 0x006d, 0x7 },
	{ 0x006c, 0x8 },
	{ 0x006a, 0x9 },
	{ 0x0034, 0xa },
	{ 0x0033, 0xb },
	{ 0x0032, 0xc },
	{ 0x0031, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x003b, 0x10 },
	{ 0x0071, 0x11 },
	{ 0x0062, 0x12 },
	{ 0x0079, 0x13 },
	{ 0x0075, 0x14 },
	{ 0x0072, 0x15 },
	{ 0x0073, 0x16 },
	{ 0x006f, 0x17 },
	{ 0x002e, 0x18 },
	{ 0x0036, 0x19 },
	{ 0x0035, 0x1a },
	{ 0x003d, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x002d, 0x1e },
	{ 0x006b, 0x1f },
	{ 0x0063, 0x20 },
	{ 0x0064, 0x21 },
	{ 0x0074, 0x22 },
	{ 0x0068, 0x23 },
	{ 0x0065, 0x24 },
	{ 0x0061, 0x25 },
	{ 0x007a, 0x26 },
	{ 0x0038, 0x27 },
	{ 0x0037, 0x28 },
	{ 0x0060, 0x29 },
	{ 0x005c, 0x2b },
	{ 0x0027, 0x2c },
	{ 0x0078, 0x2d },
	{ 0x0067, 0x2e },
	{ 0x0076, 0x2f },
	{ 0x0077, 0x30 },
	{ 0x006e, 0x31 },
	{ 0x0069, 0x32 },
	{ 0x002c, 0x33 },
	{ 0x0030, 0x34 },
	{ 0x0039, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x0037, 0x47 },
	{ 0x0038, 0x48 },
	{ 0x0039, 0x49 },
	{ 0x002d, 0x4a },
	{ 0x0034, 0x4b },
	{ 0x0035, 0x4c },
	{ 0x0036, 0x4d },
	{ 0x002b, 0x4e },
	{ 0x0031, 0x4f },
	{ 0x0032, 0x50 },
	{ 0x0033, 0x51 },
	{ 0x0030, 0x52 },
	{ 0x002e, 0x53 },
	{ 0x005c, 0x56 },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
	{ 0x002e, 0x7e },
};


const Keylayout_r::KeyLayoutMap_t shift
{
	{ 0x001b, 0x1 },
	{ 0x007b, 0x2 },
	{ 0x007d, 0x3 },
	{ 0x003f, 0x4 },
	{ 0x0050, 0x5 },
	{ 0x0046, 0x6 },
	{ 0x004d, 0x7 },
	{ 0x004c, 0x8 },
	{ 0x004a, 0x9 },
	{ 0x0024, 0xa },
	{ 0x0023, 0xb },
	{ 0x0040, 0xc },
	{ 0x0021, 0xd },
	{ 0x0008, 0xe },
	{ 0x003a, 0x10 },
	{ 0x0051, 0x11 },
	{ 0x0042, 0x12 },
	{ 0x0059, 0x13 },
	{ 0x0055, 0x14 },
	{ 0x0052, 0x15 },
	{ 0x0053, 0x16 },
	{ 0x004f, 0x17 },
	{ 0x003e, 0x18 },
	{ 0x005e, 0x19 },
	{ 0x0025, 0x1a },
	{ 0x002b, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x005f, 0x1e },
	{ 0x004b, 0x1f },
	{ 0x0043, 0x20 },
	{ 0x0044, 0x21 },
	{ 0x0054, 0x22 },
	{ 0x0048, 0x23 },
	{ 0x0045, 0x24 },
	{ 0x0041, 0x25 },
	{ 0x005a, 0x26 },
	{ 0x002a, 0x27 },
	{ 0x0026, 0x28 },
	{ 0x007e, 0x29 },
	{ 0x007c, 0x2b },
	{ 0x0022, 0x2c },
	{ 0x0058, 0x2d },
	{ 0x0047, 0x2e },
	{ 0x0056, 0x2f },
	{ 0x0057, 0x30 },
	{ 0x004e, 0x31 },
	{ 0x0049, 0x32 },
	{ 0x003c, 0x33 },
	{ 0x0029, 0x34 },
	{ 0x0028, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002e, 0x53 },
	{ 0x007c, 0x56 },
	{ 0x007f, 0x63 },
	{ 0x000d, 0x64 },
	{ 0x002f, 0x68 },
	{ 0x002e, 0x7e },
};


const Keylayout_r::KeyLayoutMap_t altGr
{
	{ 0x001b, 0x1 },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x000d, 0x1c },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t shiftAltGr
{
	{ 0x001b, 0x1 },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x000d, 0x1c },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_noMod
{
	{ 0x001b, 0x1 },
	{ 0x005b, 0x2 },
	{ 0x005d, 0x3 },
	{ 0x002f, 0x4 },
	{ 0x0050, 0x5 },
	{ 0x0046, 0x6 },
	{ 0x004d, 0x7 },
	{ 0x004c, 0x8 },
	{ 0x004a, 0x9 },
	{ 0x0034, 0xa },
	{ 0x0033, 0xb },
	{ 0x0032, 0xc },
	{ 0x0031, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x003b, 0x10 },
	{ 0x0051, 0x11 },
	{ 0x0042, 0x12 },
	{ 0x0059, 0x13 },
	{ 0x0055, 0x14 },
	{ 0x0052, 0x15 },
	{ 0x0053, 0x16 },
	{ 0x004f, 0x17 },
	{ 0x002e, 0x18 },
	{ 0x0036, 0x19 },
	{ 0x0035, 0x1a },
	{ 0x003d, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x002d, 0x1e },
	{ 0x004b, 0x1f },
	{ 0x0043, 0x20 },
	{ 0x0044, 0x21 },
	{ 0x0054, 0x22 },
	{ 0x0048, 0x23 },
	{ 0x0045, 0x24 },
	{ 0x0041, 0x25 },
	{ 0x005a, 0x26 },
	{ 0x0038, 0x27 },
	{ 0x0037, 0x28 },
	{ 0x0060, 0x29 },
	{ 0x005c, 0x2b },
	{ 0x0027, 0x2c },
	{ 0x0058, 0x2d },
	{ 0x0047, 0x2e },
	{ 0x0056, 0x2f },
	{ 0x0057, 0x30 },
	{ 0x004e, 0x31 },
	{ 0x0049, 0x32 },
	{ 0x002c, 0x33 },
	{ 0x0030, 0x34 },
	{ 0x0039, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002e, 0x53 },
	{ 0x005c, 0x56 },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
	{ 0x002e, 0x7e },
};


const Keylayout_r::KeyLayoutMap_t capslock_shift
{
	{ 0x001b, 0x1 },
	{ 0x007b, 0x2 },
	{ 0x007d, 0x3 },
	{ 0x003f, 0x4 },
	{ 0x0070, 0x5 },
	{ 0x0066, 0x6 },
	{ 0x006d, 0x7 },
	{ 0x006c, 0x8 },
	{ 0x006a, 0x9 },
	{ 0x0024, 0xa },
	{ 0x0023, 0xb },
	{ 0x0040, 0xc },
	{ 0x0021, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x003a, 0x10 },
	{ 0x0071, 0x11 },
	{ 0x0062, 0x12 },
	{ 0x0079, 0x13 },
	{ 0x0075, 0x14 },
	{ 0x0072, 0x15 },
	{ 0x0073, 0x16 },
	{ 0x006f, 0x17 },
	{ 0x003e, 0x18 },
	{ 0x005e, 0x19 },
	{ 0x0025, 0x1a },
	{ 0x002b, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x005f, 0x1e },
	{ 0x006b, 0x1f },
	{ 0x0063, 0x20 },
	{ 0x0064, 0x21 },
	{ 0x0074, 0x22 },
	{ 0x0068, 0x23 },
	{ 0x0065, 0x24 },
	{ 0x0061, 0x25 },
	{ 0x007a, 0x26 },
	{ 0x002a, 0x27 },
	{ 0x0026, 0x28 },
	{ 0x007e, 0x29 },
	{ 0x007c, 0x2b },
	{ 0x0022, 0x2c },
	{ 0x0078, 0x2d },
	{ 0x0067, 0x2e },
	{ 0x0076, 0x2f },
	{ 0x0077, 0x30 },
	{ 0x006e, 0x31 },
	{ 0x0069, 0x32 },
	{ 0x003c, 0x33 },
	{ 0x0029, 0x34 },
	{ 0x0028, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002e, 0x53 },
	{ 0x007c, 0x56 },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
	{ 0x002e, 0x7e },
};


const Keylayout_r::KeyLayoutMap_t capslock_altGr
{
	{ 0x001b, 0x1 },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x000d, 0x1c },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_shiftAltGr
{
	{ 0x001b, 0x1 },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x000d, 0x1c },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t ctrl
{
	{ 0x001b, 0x1 },
	{ 0x001b, 0x2 },
	{ 0x001d, 0x3 },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x000d, 0x1c },
	{ 0x001c, 0x2b },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x001c, 0x56 },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t deadkeys
{
};


const static uint8_t nbDeadkeys = 0;

}

static const Keylayout_r keylayout_x80030409( x80030409::LCID
                                 , x80030409::locale_name
                                 , x80030409::noMod
                                 , x80030409::shift
                                 , x80030409::altGr
                                 , x80030409::shiftAltGr
                                 , x80030409::ctrl
                                 , x80030409::capslock_noMod
                                 , x80030409::capslock_shift
                                 , x80030409::capslock_altGr
                                 , x80030409::capslock_shiftAltGr
                                 , x80030409::deadkeys
                                 , x80030409::nbDeadkeys

);

#endif
