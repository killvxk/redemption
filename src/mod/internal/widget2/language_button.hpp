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
 *   Copyright (C) Wallix 2010-2015
 *   Author(s): Christophe Grosjean, Xiaopeng Zhou, Jonathan Poelen,
 *              Meng Tan, Jennifer Inthavong
 */

#ifndef REDEMPTION_MOD_INTERNAL_WIDGET2_LANGUAGE_BUTTON_HPP
#define REDEMPTION_MOD_INTERNAL_WIDGET2_LANGUAGE_BUTTON_HPP


#include "splitter.hpp"
#include "flat_button.hpp"

class LanguageButton : public WidgetFlatButton
{
        static constexpr size_t locale_name_len = 5;
        struct Loc {
            char const * locale_name;
            int LCID;
        };
        std::vector<Loc> locales;
        unsigned selected_language = 0;
        FrontAPI & front;
        Widget2 & parent;

    public:
        LanguageButton(
            std::string const & enable_locales,
            Widget2 & parent,
            mod_api & drawable,
            FrontAPI & front,
            Font const & font,
            Theme const & theme
        )
        : WidgetFlatButton(drawable, 0, 0, *this, this, nullptr, true, -1,
                           theme.global.fgcolor, theme.global.bgcolor, theme.global.focus_color, font, 7, 7)
        , front(front)
        , parent(parent)
        {
            using std::begin;
            using std::end;

            auto LCID = front.get_keylayout();

            {
                auto it = std::find_if(begin(keylayouts), end(keylayouts), [&](Keylayout const * k){
                    return k->LCID == LCID;
                });
                if (it == end(keylayouts)) {
                    LCID = keylayout_x00000409.LCID;
                    this->locales.push_back({keylayout_x00000409.locale_name, keylayout_x00000409.LCID});
                }
                else {
                    this->locales.push_back({(*it)->locale_name, (*it)->LCID});
                }
            }


            for (auto && r : get_split(enable_locales, ',')) {
                auto is_blanck = [](char c) { return c == ' ' || c == '\t'; };
                char const * first = begin(r).base();
                char const * last = end(r).base();
                auto cstr = std::find_if_not(first, last, is_blanck);
                auto cend = std::find_if(cstr, last, is_blanck);

                auto it = std::find_if(begin(keylayouts), end(keylayouts), [&](Keylayout const * k){
                    return strncmp(k->locale_name, cstr, cend-cstr) == 0;
                });
                if (it != end(keylayouts)) {
                    if ((*it)->LCID != LCID) {
                        this->locales.push_back({(*it)->locale_name, (*it)->LCID});
                    }
                }
                else {
                    LOG(LOG_WARNING, "Layout \"%.*s\" not found.", static_cast<int>(cend - cstr), cstr);
                }

                cstr = cend;
            }

            this->label.set_text(this->locales[0].locale_name);
            this->set_button_cx(this->label.rect.cx);
            this->set_button_cy(this->label.rect.cy);
        }

        void notify(Widget2* widget, NotifyApi::notify_event_t event) override {
            if (event == NOTIFY_SUBMIT || event == MOUSE_FLAG_BUTTON1) {
                auto rect = this->rect;

                this->selected_language = (this->selected_language + 1) % this->locales.size();
                this->label.set_text(this->locales[this->selected_language].locale_name);

                this->set_button_cx(this->label.rect.cx);
                this->set_button_cy(this->label.rect.cy);
                rect.cx = std::max(rect.cx, this->rect.cx);
                rect.cy = std::max(rect.cy, this->rect.cy);
                this->parent.draw(rect);

                front.set_keylayout(this->locales[this->selected_language].LCID);
            }
        }
    };

#endif
