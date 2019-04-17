/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2013
 *   Author(s): Christophe Grosjean, Xiaopeng Zhou, Jonathan Poelen, Meng Tan,
 *              Jennifer Inthavong
 */


#pragma once

#include "configs/config_access.hpp"
#include "mod/internal/copy_paste.hpp"
#include "mod/internal/locally_integrable_mod.hpp"
#include "mod/internal/widget/flat_login.hpp"
#include "mod/internal/widget/language_button.hpp"


using FlatLoginModVariables = vcfg::variables<
    vcfg::var<cfg::context::password,                   vcfg::accessmode::set>,
    vcfg::var<cfg::globals::auth_user,                  vcfg::accessmode::set>,
    vcfg::var<cfg::context::selector,                   vcfg::accessmode::ask>,
    vcfg::var<cfg::context::target_protocol,            vcfg::accessmode::ask>,
    vcfg::var<cfg::globals::target_device,              vcfg::accessmode::ask>,
    vcfg::var<cfg::globals::target_user,                vcfg::accessmode::ask>,
    vcfg::var<cfg::translation::language,               vcfg::accessmode::get>,
    vcfg::var<cfg::context::opt_message,                vcfg::accessmode::get>,
    vcfg::var<cfg::context::login_message,              vcfg::accessmode::get>,
    vcfg::var<cfg::client::keyboard_layout_proposals,   vcfg::accessmode::get>,
    vcfg::var<cfg::globals::authentication_timeout,     vcfg::accessmode::get>,
    vcfg::var<cfg::debug::mod_internal,                 vcfg::accessmode::get>
>;


class FlatLoginMod : public LocallyIntegrableMod, public NotifyApi
{
    LanguageButton language_button;

    FlatLogin login;
    SessionReactor::TimerPtr timeout_timer;
    SessionReactor::GraphicEventPtr started_copy_past_event;

    CopyPaste copy_paste;

    FlatLoginModVariables vars;

public:
    FlatLoginMod(
        FlatLoginModVariables vars, SessionReactor& session_reactor,
        char const * username, char const * password,
        FrontAPI & front, uint16_t width, uint16_t height, Rect const widget_rect, time_t now,
        ClientExecute & client_execute, Font const& font, Theme const& theme
    );

    ~FlatLoginMod() override;

    void notify(Widget* sender, notify_event_t event) override;

    void draw_event(time_t now, gdi::GraphicApi & gapi) override;

    bool is_up_and_running() const override { return true; }

    void send_to_mod_channel(CHANNELS::ChannelNameId front_channel_name, InStream& chunk, size_t length, uint32_t flags) override;

    void move_size_widget(int16_t left, int16_t top, uint16_t width, uint16_t height) override
    {
        this->login.move_size_widget(left, top, width, height);
    }
};
