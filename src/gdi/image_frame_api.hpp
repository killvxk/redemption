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
   Copyright (C) Wallix 2017
   Author(s): Christophe Grosjean, Raphael ZHOU
*/

#pragma once

#include "utils/image_data_view.hpp"

namespace gdi {

struct ImageFrameApi
{
    using ImageView = MutableImageDataView;
    using ConstImageView = ConstImageDataView;

    virtual ~ImageFrameApi() = default;

    virtual ImageView get_mutable_image_view() = 0;
    virtual ConstImageView get_image_view() const = 0;

    virtual void prepare_image_frame() = 0;

    virtual unsigned int get_last_update_index() const = 0;

//    virtual void reset_to_origin() = 0;

    virtual void reset(unsigned int x, unsigned int y,
               unsigned int out_width, unsigned int out_height) = 0;
};

inline ImageFrameApi::ConstImageView get_image_view(ImageFrameApi const & image_frame)
{
    return image_frame.get_image_view();
}

inline ImageFrameApi::ImageView get_mutable_image_view(ImageFrameApi & image_frame)
{
    return image_frame.get_mutable_image_view();
}

}