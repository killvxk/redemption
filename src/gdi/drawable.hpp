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
*   Copyright (C) Wallix 2010-2015
*   Author(s): Jonathan Poelen
*/

#ifndef REDEMPTION_GDI_DRAWABLE_HPP
#define REDEMPTION_GDI_DRAWABLE_HPP

#include <memory>
#include <iostream>

#include "utils/service_provier.hpp"
#include "graphic_device.hpp"
#include "proxy_gd.hpp"


namespace gdi {

namespace detail_
{
    struct DrawerTrait
    {
        template<class... Ts>
        static void delegate(GraphicDevice & gd, Ts const & ... args) {
            gd.draw(args...);
        }
    };

    using ServiceDrawable = utils::service_provider<GraphicDevice, DrawerTrait, ProxyGD, GraphicDevicePtr>;

    struct ServiceDrawableProxy : ServiceDrawable
    {
        template<class Base, class... Ts>
        void operator()(Base const &, Ts const & ... args) {
            ServiceDrawable::operator()(args...);
        }
    };
}

struct Drawable
: ProxyGD<detail_::ServiceDrawableProxy>
//: ProxyGD<ProxySkipBase<detail_::ServiceDrawable>>
{
    using TypeId = detail_::ServiceDrawable::type_id;
    using FilterPtr = detail_::ServiceDrawable::filter_pointer;

    struct NullFilter
    {
        template<class... Ts>
        void operator()(GraphicDevice & gd, Ts && ... args) {
            return gd.draw(std::forward<Ts>(args)...);
        }
    };

    TypeId add_gdi(GraphicDevicePtr && pgdi) {
        return this->get_proxy().add_class(std::move(pgdi));
    }

    template<class Filter>
    TypeId add_filter(Filter && filter) {
        return this->get_proxy().add_filter(std::forward<Filter>(filter));
    }

    GraphicDevicePtr remove_gdi(TypeId id) {
        return this->get_proxy().remove_class(id);
    }

    FilterPtr remove_filter(TypeId id) {
        return this->get_proxy().remove_filter(id);
    }
};

}

#endif
