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
   Copyright (C) Wallix 2012-2013
   Author(s): Christophe Grosjean, Raphael Zhou
*/

#ifndef _REDEMPTION_CORE_RDP_CACHES_POINTERCACHE_HPP_
#define _REDEMPTION_CORE_RDP_CACHES_POINTERCACHE_HPP_

#include <math.h>

#include "pointer.hpp"

#include "drawable.hpp"
#include "client_info.hpp"


enum {
    POINTER_TO_SEND         = 0,
    POINTER_ALLREADY_SENT
};

/* difference caches */
struct PointerCache {
    int pointer_cache_entries;

    /* pointer */
    int pointer_stamp;
    struct pointer_item pointer_items[32];
    int stamps[32];

    PointerCache() {
        this->pointer_cache_entries = 0;
        this->pointer_stamp = 0;
    }

    ~PointerCache() {}

    TODO(" much duplicated code with constructor and destructor  create some intermediate functions or object")
    int reset(struct ClientInfo & client_info) {
        memset(this, 0, sizeof(struct PointerCache));
        this->pointer_cache_entries = client_info.pointer_cache_entries;
        return 0;
    }

    int add_pointer(const uint8_t * data, const uint8_t * mask, int x, int y, int & cache_idx) {
        TODO(" see code below to avoid useless copy")
        struct pointer_item pointer_item;

        pointer_item.x = x;
        pointer_item.y = y;
        memcpy(pointer_item.data, data, 32 * 32 * 3);
        memcpy(pointer_item.mask, mask, 32 * 32 / 8);
        return this->add_pointer(&pointer_item, cache_idx);
    }

    /* check if the pointer is in the cache or not and if it should be sent      */
    int add_pointer(struct pointer_item * pointer_item, int & cache_idx)
    {
        int i;
        int oldest;
        int index;

        this->pointer_stamp++;
        /* look for match */
        for (i = 2; i < this->pointer_cache_entries; i++) {
            if (this->pointer_items[i].x == pointer_item->x &&
                this->pointer_items[i].y == pointer_item->y &&
                (memcmp(this->pointer_items[i].data,
                        pointer_item->data, 32 * 32 * 3) == 0) &&
                (memcmp(this->pointer_items[i].mask,
                        pointer_item->mask, 32 * 32 / 8) == 0)) {
                this->stamps[i] = this->pointer_stamp;
                cache_idx = i;
                return POINTER_ALLREADY_SENT;
            }
        }
        /* look for oldest */
        index  = 2;
        oldest = 0x7fffffff;
        for (i = 2; i < this->pointer_cache_entries; i++) {
            if (this->stamps[i] < oldest) {
                oldest = this->stamps[i];
                index  = i;
            }
        }
        this->pointer_items[index].x = pointer_item->x;
        this->pointer_items[index].y = pointer_item->y;
        memcpy(this->pointer_items[index].data, pointer_item->data, 32 * 32 * 3);
        memcpy(this->pointer_items[index].mask, pointer_item->mask, 32 * 32 / 8);
        this->stamps[index] = this->pointer_stamp;
        cache_idx = index;
        return POINTER_TO_SEND;
    }

    void add_pointer_static(struct pointer_item * pointer_item, int index) {
        this->pointer_items[index].x = pointer_item->x;
        this->pointer_items[index].y = pointer_item->y;
        memcpy(this->pointer_items[index].data, pointer_item->data, 32 * 32 * 3);
        memcpy(this->pointer_items[index].mask, pointer_item->mask, 32 * 32 / 8);
        this->stamps[index] = this->pointer_stamp;
    }

    void add_pointer_static_2(int hotspot_x, int hotspot_y,
            const uint8_t * data, const uint8_t * mask, int index) {
        this->pointer_items[index].x = hotspot_x;
        this->pointer_items[index].y = hotspot_y;
        memcpy(this->pointer_items[index].data, data, 32 * 32 * 3);
        memcpy(this->pointer_items[index].mask, mask, 32 * 32 / 8);
        this->stamps[index] = this->pointer_stamp;
    }
};  // struct PointerCache

struct drawable_pointer_item {
    Drawable::Mouse_t mouse_cursor[16 * 32];
    int               contiguous_mouse_pixels;
    uint8_t           data[32 * 32 * 3];

    int x; /* hotspot */
    int y;
};

struct DrawablePointerCache {
    struct drawable_pointer_item pointer_items[32];

    DrawablePointerCache() {
        for (int i = 0,
                 c = sizeof(this->pointer_items) / sizeof(drawable_pointer_item);
             i < c; i++) {
            memset(&this->pointer_items[i], 0, sizeof(this->pointer_items[i]));
        }
    }

    void add_pointer_static(struct pointer_item * pointer_item, int index) {
        this->add_pointer_static_2(pointer_item->x, pointer_item->y,
            pointer_item->data, pointer_item->mask, index);
    }

    void add_pointer_static_2(int hotspot_x, int hotspot_y,
            const uint8_t * data, const uint8_t * mask, int index) {
        drawable_pointer_item & pointer_item = this->pointer_items[index];

        this->make_drawable_mouse_cursor(data, mask, pointer_item);

        pointer_item.x = hotspot_x;
        pointer_item.y = hotspot_y;
    }

protected:
    void make_drawable_mouse_cursor(const uint8_t * data,
        const uint8_t * mask, drawable_pointer_item & pointer_item) {
        memset(&pointer_item, 0, sizeof(pointer_item));

        bool                draw_pixel;
        int                 current_contiguous_mouse_pixels = 0;
        Drawable::Mouse_t * mouse_cursor                    = &pointer_item.mouse_cursor[-1];
        bool                in_contiguous_mouse_pixels      = false;
        uint8_t           * line_data                       = pointer_item.data;

        for (unsigned line = 0; line < 32; line++) {
            in_contiguous_mouse_pixels = false;

            for (unsigned column = 0; column < 32; column++) {
                div_t res = div(column, 8);
                unsigned rem = 7 - res.rem;

                draw_pixel = !(((*(mask + 128 - (line + 1) * 32 / 8 + res.quot)) & (1 << rem)) >> rem);
//                printf("%c", (draw_pixel ? 'X' : '.'));

                const uint8_t * pixel = data + 32 * 32 * 3 - (line + 1) * 32 * 3 + column * 3;
//                printf("%02X%02X%02X", *pixel, *(pixel + 1), *(pixel+2));

                if (draw_pixel && !in_contiguous_mouse_pixels) {
                    current_contiguous_mouse_pixels++;
                    mouse_cursor++;

                    mouse_cursor->x    = column;
                    mouse_cursor->y    = line;
                    mouse_cursor->lg   = 0;
                    mouse_cursor->line = reinterpret_cast<const char *>(line_data);

                    in_contiguous_mouse_pixels = true;
                }
                else if (!draw_pixel && in_contiguous_mouse_pixels) {
                    in_contiguous_mouse_pixels = false;
                }

                if (in_contiguous_mouse_pixels) {
                    memcpy(line_data, pixel, 3);
                    line_data        += 3;
                    mouse_cursor->lg += 3;
                }
            }
//            printf("\n");
        }

        pointer_item.contiguous_mouse_pixels = current_contiguous_mouse_pixels;
    }
};

#endif  // #ifndef _REDEMPTION_CORE_RDP_CACHES_POINTERCACHE_HPP_
