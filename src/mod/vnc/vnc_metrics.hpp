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
*   Copyright (C) Wallix 2018
*   Author(s): Clément Moroldo
*/

#pragma once

#include "mod/metrics.hpp"


class VNCMetrics
{


//-> copy-paste: up/down
//-> redrawn surface
//-> data quantity transfered per data type (codec)
//-> keyboard metrics (special characters, total characters)


private:
    enum : int {
        DATA_FROM_CLIENT,     // number of byte sent from client to server
        DATA_FROM_SERVER,     // number of byte sent from server to client
        
        CLIPBOARD_DATA_FROM_CLIENT,     // number of byte sent from client to server
        CLIPBOARD_DATA_FROM_SERVER,     // number of byte sent from server to client

        MOUSE_MOVE,
        KEY_PRESSED,
        RIGHT_CLICK,
        LEFT_CLICK,

        COUNT_FIELD
    };

    const char * vnc_metrics_name(int index) {

        switch (index) {
            case DATA_FROM_CLIENT:   return "data_from_client";
            case DATA_FROM_SERVER:   return "data_from_server";
            case CLIPBOARD_DATA_FROM_CLIENT:   return "clipboard_data_from_client";
            case CLIPBOARD_DATA_FROM_SERVER:   return "clipboard_data_from_server";
            case MOUSE_MOVE:         return "mouse_move";
            case KEY_PRESSED:       return  "key_pressed";
            case RIGHT_CLICK:        return "right_click";
            case LEFT_CLICK:         return "left_click";
            case COUNT_FIELD: break;
        }

        return "unknow_vnc_metrics_name";
    }

    const char * vnc_protocol_name = "vnc";

    Metrics metrics;


    // VNC context Info
    int last_x = -1;
    int last_y = -1;



public:

    VNCMetrics( const bool activate                         // do nothing if false
              , std::string path
              , std::string session_id
              , array_view_const_char primary_user_sig      // clear primary user account
              , array_view_const_char account_sig           // secondary account
              , array_view_const_char target_service_sig    // clear target service name + clear device name
              , array_view_const_char session_info_sig      // source_host + client info
              , const std::chrono::seconds now              // time at beginning of metrics
              , const std::chrono::hours file_interval      // daily rotation of filename (hours)
              , const std::chrono::seconds log_delay        // delay between 2 logs
              )
        : metrics(/*this->rdp_field_version*/"v1.0", this->vnc_protocol_name,
            activate, COUNT_FIELD, std::move(path), std::move(session_id),
            primary_user_sig, account_sig, target_service_sig, session_info_sig,
            now, file_interval, log_delay)
    {}

    bool active() const {
        return this->metrics.active_;
    }

    void log(timeval const& now) {
        this->metrics.log(now);
    }

    void data_from_client(long int len) {
        this->metrics.add_to_current_data(DATA_FROM_CLIENT, len);
    }

    void data_from_server(long int len) {
        this->metrics.add_to_current_data(DATA_FROM_SERVER, len);
    }

    void clipboard_data_from_client(long int len) {
        this->metrics.add_to_current_data(CLIPBOARD_DATA_FROM_CLIENT, len);
    }

    void clipboard_data_from_server(long int len) {
        this->metrics.add_to_current_data(CLIPBOARD_DATA_FROM_SERVER, len);
    }

    void mouse_move(const int x, const int y) {
        if (this->last_x >= 0 && this->last_y >= 0) {
            int x_shift = x - this->last_x;
            if (x_shift < 0) {
                x_shift *=  -1;
            }
            int y_shift = y - this->last_y;
            if (y_shift < 0) {
                y_shift *=  -1;
            }
            this->metrics.add_to_current_data(MOUSE_MOVE, x_shift + y_shift);
        }
        this->last_x = x;
        this->last_y = y;
    }

    void key_pressed() {
        this->metrics.add_to_current_data(KEY_PRESSED, 1);
    }

    void right_click() {
        this->metrics.add_to_current_data(RIGHT_CLICK, 1);
    }

    void left_click() {
        this->metrics.add_to_current_data(LEFT_CLICK, 1);
    }


};