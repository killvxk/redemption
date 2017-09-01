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
 *   Copyright (C) Wallix 2010-2013
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen,
 *              Meng Tan, Jennifer Inthavong
 *
 */

#pragma once

#include "mod/internal/widget/composite.hpp"
#include "mod/internal/widget/label.hpp"
#include "mod/internal/widget/edit.hpp"
#include "mod/internal/widget/number_edit.hpp"
#include "mod/internal/widget/flat_button.hpp"
#include "mod/internal/widget/labelgrid.hpp"
#include "utils/translation.hpp"

class Theme;

struct WidgetSelectorParams
{
    uint16_t nb_columns = 0;
    const char * label[GRID_NB_COLUMNS_MAX] = {nullptr};
    uint16_t base_len[GRID_NB_COLUMNS_MAX] = {0};

    WidgetSelectorParams() = default;

    WidgetSelectorParams(const WidgetSelectorParams & params)
      : nb_columns(params.nb_columns)
    {
        for (int i = 0; i < this->nb_columns; i++) {
            this->label[i] = params.label[i];
            this->base_len[i] = params.base_len[i];
        }
    }
};



class WidgetSelector : public WidgetParent
{
public:
    struct temporary_number_of_page
    {
        char buffer[15];

        explicit temporary_number_of_page(const char * s);
    };

    CompositeArray composite_array;

    bool less_than_800;
    const uint16_t nb_columns;

    WidgetLabel device_label;

    WidgetLabel header_label[GRID_NB_COLUMNS_MAX];
    WidgetEdit  edit_filter[GRID_NB_COLUMNS_MAX];

    WidgetLabelGrid selector_lines;

    WidgetFlatButton first_page;
    WidgetFlatButton prev_page;

    WidgetNumberEdit current_page;
    WidgetLabel number_page;
    WidgetFlatButton next_page;
    WidgetFlatButton last_page;
    WidgetFlatButton logout;
    WidgetFlatButton apply;
    WidgetFlatButton connect;

    BGRColor bg_color;

    Font const & font;

    int16_t left;
    int16_t top;

public:
    enum {
        IDX_TARGETGROUP,
        IDX_TARGET,
        IDX_PROTOCOL,
        IDX_CLOSETIME
    };

    enum {
        HORIZONTAL_MARGIN = 15,
        VERTICAL_MARGIN = 10,
        TEXT_MARGIN = 20,
        FILTER_SEPARATOR = 5,
        NAV_SEPARATOR = 15
    };

    WidgetFlatButton * extra_button;

    uint16_t base_len[GRID_NB_COLUMNS_MAX] = {0};



    WidgetSelector(gdi::GraphicApi & drawable,
                   const char * device_name,
                   int16_t left, int16_t top, uint16_t width, uint16_t height,
                   Widget & parent, NotifyApi* notifier,
                   const char * current_page,
                   const char * number_of_page,
                   WidgetFlatButton * extra_button,
                   WidgetSelectorParams selector_params,
                   Font const & font, Theme const & theme, Translation::language_t lang);

    ~WidgetSelector() override;

    void move_size_widget(int16_t left, int16_t top, uint16_t width, uint16_t height);

    BGRColor get_bg_color() const override;

    void ask_for_connection();

    void notify(Widget* widget, notify_event_t event) override;

    void add_device(const char ** entries);

    void rdp_input_scancode(long int param1, long int param2, long int param3, long int param4, Keymap2* keymap) override;

    void show_tooltip(Widget * widget, const char * text, int x, int y,
                      Rect const preferred_display_rect, int iter) override;

private:
    void rearrange();
};
