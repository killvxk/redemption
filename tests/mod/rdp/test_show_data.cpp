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
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean, Javier Caverni
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   Unit test to writing RDP orders to file and rereading them
*/
/*
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestRdp
#include <boost/test/auto_unit_test.hpp>
*/
#undef SHARE_PATH
#define SHARE_PATH FIXTURES_PATH

#define LOGNULL
//#define LOGPRINT

#include "configs/config.hpp"
//#include "socket_transport.hpp"
#include "transport/test_transport.hpp"
#include "core/client_info.hpp"
#include "mod/rdp/rdp.hpp"

#include "../tests/front/front_show_data.hpp"



int main(int argc, char** argv) //TestModRDPWin2008Server)
{
    ClientInfo info;
    info.keylayout = 0x040C;
    info.console_session = 0;
    info.brush_cache_code = 0;
    info.bpp = 16;
    info.width = 1024;
    info.height = 768;
    info.rdp5_performanceflags =   PERF_DISABLE_WALLPAPER
                                 | PERF_DISABLE_FULLWINDOWDRAG
                                 | PERF_DISABLE_MENUANIMATIONS;
    snprintf(info.hostname,sizeof(info.hostname),"192-168-1-100");
    int verbose = 511;

    Front_Show_Data front(info, verbose);

    const char * name = "RDP W2008 TLS Target";

    // int client_sck = ip_connect("10.10.47.36", 3389, 3, 1000, verbose);
    // std::string error_message;
    // SocketTransport t( name
    //                  , client_sck
    //                  , "10.10.47.36"
    //                  , 3389
    //                  , verbose
    //                  , &error_message
    //                  );

    #include "../tests/fixtures/dump_w2008_2.hpp"

    std::ofstream fichier("projects/browser_client_JS/test_emscripten_browser_client_JS.js", std::ios::out | std::ios::trunc);



    fichier << "var inData" << " = [" <<  std::endl;

    const uint8_t * ibuffer = reinterpret_cast<const uint8_t *>(indata);
    int i;
    int val;
    std::string s("");
    for (i = 0; i < 250568; i++) {
        s = "";
        val = int(ibuffer[i]);
        if (val < 16) {
            s = "0";
        }
        fichier << "0x" <<  s << std::hex << val << ", ";
        if ((i+1) % 20 == 0) {
            fichier <<  std::endl;
        }
    }
    fichier <<  std::endl << "];";

    fichier <<  std::endl <<  std::endl <<  std::endl;

    fichier << "var outData" << " = [" <<  std::endl;

    const uint8_t * obuffer = reinterpret_cast<const uint8_t *>(outdata);
    for (i = 0; i < 250568; i++) {
        s = "";
        val = int(obuffer[i]);
        if (val < 16) {
            s = "0";
        }
        fichier << "0x" <<  s << std::hex << val << ", ";
        if ((i+1) % 20 == 0) {
            fichier <<  std::endl;
        }
    }
    fichier <<  std::endl << "];";


    TestTransport t(name, indata, sizeof(indata)-1, outdata, sizeof(outdata)-1, verbose);

    if (verbose > 2){
        LOG(LOG_INFO, "--------- CREATION OF MOD ------------------------");
    }

    Inifile ini;

    ModRDPParams mod_rdp_params( "administrateur"
                               , "S3cur3!1nux"
                               , "10.10.47.35"
                               , "192.168.1.100"
                               , 7
                               , 511
                               );
    mod_rdp_params.device_id                       = "device_id";
    mod_rdp_params.enable_tls                      = false;
    mod_rdp_params.enable_nla                      = false;
    //mod_rdp_params.enable_krb                      = false;
    //mod_rdp_params.enable_clipboard                = true;
    mod_rdp_params.enable_fastpath                 = false;
    mod_rdp_params.enable_mem3blt                  = false;
    mod_rdp_params.enable_bitmap_update            = true;
    mod_rdp_params.enable_new_pointer              = false;
    //mod_rdp_params.rdp_compression                 = 0;
    //mod_rdp_params.error_message                   = nullptr;
    //mod_rdp_params.disconnect_on_logon_user_change = false;
    //mod_rdp_params.open_session_timeout            = 0;
    //mod_rdp_params.certificate_change_action       = 0;
    //mod_rdp_params.extra_orders                    = "";
    mod_rdp_params.server_redirection_support        = true;

    // To always get the same client random, in tests
    LCGRandom gen(0);

    mod_rdp mod_(t, front, info, ini.get_ref<cfg::mod_rdp::redir_info>(), gen, mod_rdp_params);
    mod_api * mod = &mod_;

    if (verbose > 2){
        LOG(LOG_INFO, "========= CREATION OF MOD DONE ====================\n\n");
    }


    uint32_t count = 0;
    BackEvent_t res = BACK_EVENT_NONE;
    while (res == BACK_EVENT_NONE){
        LOG(LOG_INFO, "===================> count = %u", count);
        if (count++ >= 70) break;
        mod->draw_event(time(nullptr));
    }

    front.over();

    return 0;
}