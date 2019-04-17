/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Product name: redemption, a FLOSS RDP proxy
Copyright (C) Wallix 2010-2018
Author(s): Jonathan Poelen
*/

#include "mod/vnc/new_mod_vnc.hpp"
#include "mod/vnc/vnc.hpp"


std::unique_ptr<mod_api> new_mod_vnc(
    Transport& t,
    SessionReactor& session_reactor,
    const char* username,
    const char* password,
    FrontAPI& front,
    uint16_t front_width,
    uint16_t front_height,
    int keylayout,
    int key_flags,
    bool clipboard_up,
    bool clipboard_down,
    const char * encodings,
    ReportMessageApi& report_message,
    bool server_is_apple,
    bool send_alt_ksym,
    ClientExecute* client_execute,
    ModVncVariables vars,
    VNCVerbose verbose,
    VNCMetrics * metrics
)
{
    return std::make_unique<mod_vnc>(
        t, session_reactor, username, password, front,
        front_width, front_height, keylayout, key_flags,
        clipboard_up, clipboard_down, encodings, mod_vnc::ClipboardEncodingType::UTF8,
        VncBogusClipboardInfiniteLoop::delayed, report_message, server_is_apple, send_alt_ksym,
        client_execute, vars, verbose, metrics);
}
