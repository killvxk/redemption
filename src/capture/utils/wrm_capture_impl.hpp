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
*   Author(s): Jonathan Poelen
*/

#ifndef REDEMPTION_CAPTURE_UTILS_WRM_CAPTURE_IMPL_HPP
#define REDEMPTION_CAPTURE_UTILS_WRM_CAPTURE_IMPL_HPP

#include "core/RDP/caches/bmpcache.hpp"
#include "core/RDP/caches/glyphcache.hpp"
#include "core/RDP/caches/pointercache.hpp"
#include "utils/dump_png24_from_rdp_drawable_adapter.hpp"
#include "transport/out_meta_sequence_transport.hpp"
#include "nativecapture.hpp"
#include "apis_register.hpp"


class WrmCaptureImpl final : private gdi::KbdInputApi, private gdi::CaptureApi
{
    BmpCache     bmp_cache;
    GlyphCache   gly_cache;
    PointerCache ptr_cache;

    DumpPng24FromRDPDrawableAdapter dump_png24_api;

    struct TransportVariant{
        union Variant
        {
            OutMetaSequenceTransportWithSum out_with_sum;
            CryptoOutMetaSequenceTransport out_crypto;
            OutMetaSequenceTransport out;

            struct {} dummy;
            Variant() : dummy() {}
            ~Variant() {}
        } variant;
        ::Transport * trans;

        template<class... Ts>
        TransportVariant(configs::TraceType trace_type, Ts && ... args)
        {
            TODO("there should only be one outmeta, not two."
                " Capture code should not really care if file is encrypted or not."
                "Here is not the right level to manage anything related to encryption.")
            TODO("Also we may wonder why we are encrypting wrm and not png"
                "(This is related to the path split between png and wrm)."
                "We should stop and consider what we should actually do")
            switch (trace_type) {
                case configs::TraceType::cryptofile:
                    this->trans = new (&this->variant.out_crypto)
                    CryptoOutMetaSequenceTransport(std::forward<Ts>(args)...);
                    break;
                case configs::TraceType::localfile_hashed:
                    this->trans = new (&this->variant.out_with_sum)
                    OutMetaSequenceTransportWithSum(std::forward<Ts>(args)...);
                    break;
                default :
                    this->trans = new (&this->variant.out)
                    OutMetaSequenceTransport(std::forward<Ts>(args)...);
                    break;
            }
        }

        ~TransportVariant() {
            this->trans->~Transport();
        }
    } trans_variant;

    struct Serializer final : GraphicToFile {
        using GraphicToFile::GraphicToFile;

        using GraphicToFile::GraphicToFile::draw;
        using GraphicToFile::GraphicToFile::capture_bpp;

        void draw(const RDPBitmapData & bitmap_data, const Bitmap & bmp) override {
            auto compress_and_draw_bitmap_update = [&bitmap_data, this](const Bitmap & bmp) {
                StaticOutStream<65535> bmp_stream;
                bmp.compress(this->capture_bpp, bmp_stream);

                RDPBitmapData target_bitmap_data = bitmap_data;

                target_bitmap_data.bits_per_pixel = bmp.bpp();
                target_bitmap_data.flags          = BITMAP_COMPRESSION | NO_BITMAP_COMPRESSION_HDR;
                target_bitmap_data.bitmap_length  = bmp_stream.get_offset();

                GraphicToFile::draw(target_bitmap_data, bmp);
            };

            if (bmp.bpp() > this->capture_bpp) {
                // reducing the color depth of image.
                Bitmap capture_bmp(this->capture_bpp, bmp);
                compress_and_draw_bitmap_update(capture_bmp);
            }
            else if (!bmp.has_data_compressed()) {
                compress_and_draw_bitmap_update(bmp);
            }
            else {
                GraphicToFile::draw(bitmap_data, bmp);
            }
        }

        WrmCaptureImpl * impl = nullptr;
        void enable_kbd_input_mask(bool enable) override {
            this->impl->enable_kbd_input_mask(enable);
        }
    } graphic_to_file;

    NativeCapture nc;

    ApiRegisterElement<gdi::KbdInputApi> kbd_element;

public:
    WrmCaptureImpl(
        const timeval & now, uint8_t capture_bpp, configs::TraceType trace_type,
        CryptoContext & cctx,
        const char * record_path, const char * hash_path, const char * basename,
        int groupid, auth_api * authentifier,
        RDPDrawable & drawable, const Inifile & ini
    )
    : bmp_cache(
        BmpCache::Recorder, capture_bpp, 3, false,
        BmpCache::CacheOption(600, 768, false),
        BmpCache::CacheOption(300, 3072, false),
        BmpCache::CacheOption(262, 12288, false))
    , ptr_cache(/*pointerCacheSize=*/0x19)
    , dump_png24_api{drawable}
    , trans_variant(
        trace_type, &cctx, record_path, hash_path, basename, now,
        drawable.width(), drawable.height(), groupid, authentifier)
    , graphic_to_file(
        now, *this->trans_variant.trans, drawable.width(), drawable.height(), capture_bpp,
        this->bmp_cache, this->gly_cache, this->ptr_cache,
        this->dump_png24_api, ini, GraphicToFile::SendInput::YES, ini.get<cfg::debug::capture>())
    , nc(this->graphic_to_file, now, ini)
    {}

    void attach_apis(ApisRegister & apis_register, const Inifile & ini) {
        apis_register.graphic_list->push_back(this->graphic_to_file);
        apis_register.capture_list.push_back(static_cast<gdi::CaptureApi&>(*this));
        apis_register.update_config_capture_list.push_back(this->nc);
        apis_register.external_capture_list.push_back(this->nc);
        apis_register.capture_probe_list.push_back(this->graphic_to_file);

        if (!bool(ini.get<cfg::video::disable_keyboard_log>() & configs::KeyboardLogFlags::wrm)) {
            this->kbd_element = {apis_register.kbd_input_list, this->graphic_to_file};
            this->graphic_to_file.impl = this;
        }
    }

    void enable_kbd_input_mask(bool enable) override {
        assert(this->kbd_element == *this || this->kbd_element == this->graphic_to_file);
        this->kbd_element = enable
            ? static_cast<gdi::KbdInputApi&>(*this)
            : static_cast<gdi::KbdInputApi&>(this->graphic_to_file);
    }

    void send_timestamp_chunk(timeval const & now, bool ignore_time_interval) {
        this->graphic_to_file.timestamp(now);
        this->graphic_to_file.send_timestamp_chunk(ignore_time_interval);
    }

    void request_full_cleaning() {
        this->trans_variant.trans->request_full_cleaning();
    }

private:
    std::chrono::microseconds do_snapshot(
        const timeval & now, int x, int y, bool ignore_frame_in_timeval
    ) override {
        return this->nc.snapshot(now, x, y, ignore_frame_in_timeval);
    }

    void do_resume_capture(const timeval& now) override {
        this->trans_variant.trans->next();
        this->send_timestamp_chunk(now, true);
    }

    // shadow text
    bool kbd_input(const timeval& now, uint32_t) override {
        return this->graphic_to_file.kbd_input(now, '*');
    }
};

#endif