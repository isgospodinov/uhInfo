/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <gtkmm.h>

class CProcUnits;

using DeviceType = enum class DVTP {AUDIO,NETWORKRESOURSE};
using Dm = enum class DrawMode {CPUDRAW,TEMPERATUREDRAW};
using Direction = enum class GetDirection {LEFT,RIGHT,LEFTSKIP,RIGHTSKIP};

namespace uhiutil {
   void end_intervals_remove(std::string& str);
   std::string execmd(const char* command);
   std::string start_intervals_remove(std::string in);
   void GetDesktopSize(unsigned int *Wh,  unsigned int *Ww);
   bool ExistenceVerification(const char *path);
   bool newline(std::string &in_line,const char* in_find,Direction drct);
   std::string GetUserName();
   
   const unsigned int timer_interval = 1500;
   const unsigned int timer_id = 0;
   const unsigned int max_size = 1024;
   const int def_desktop_width = 620, def_desktop_height = 460;

   namespace calc {
       enum {bfr = 0,cfr,usg,dimension};
       const unsigned int draw_cpu_statistic = 10;
       const unsigned int t_statistic_len = 421;
       const int resetime = 4271;      
   }   

   namespace ui {     
       const char *const active_tag = "sensor_in_monitor";
       const char *const max_tag = "sensor_max";
       const char *const sensors_color = "midnight blue";
       const char *const cpunits_color = "White";
       const char *const stat_color_cmpr = "DarkKhaki";
       const char *const cpunits_bckcolor = "Light Slate Grey";
       const char *const stat_color_blue = "Deep Sky Blue";
       const char *const stat_color_green = "Medium Aquamarine";       
       const unsigned int app_font_size = 10;
   }

   namespace cpu {
       template <class TBase, class TCastDown = CProcUnits> TCastDown *const UhiDownCast(TBase *const base, const TCastDown *const = nullptr) {return dynamic_cast<TCastDown *const>(base);}   
       extern bool native_fq_state;    
       extern int max_cpu_t;
   }
   
   namespace draw {
       const char *const text_font_family = "Arial";
       const unsigned int dofset = 4,xoffset = 40;
       const unsigned int dtxtmax = 15;
       const unsigned int dtxthin = 8;
       const unsigned int dwaw = 100;
       const unsigned int dwah = 50;
       const unsigned int uhi_draw_yscale = 5,uhi_draw_xscale = 15;
       const int HWmax = 510;
    }

   namespace monitor {
       const char *const edid_wl_path = "/sys/class/drm/";
   }
   
   namespace sdef {
       #include <sensors/sensors.h>
       namespace sysensors {
           const char *const nud2 = "UDisks2";
           const char *const idd_path = "/org/freedesktop/UDisks2/block_devices/";
       
           using fp_sInit      = int (*) (FILE *);
           using fp_sClean     = void (*) ();
           using fp_sChips     = const sensors_chip_name* (*) (const sensors_chip_name*, int*);
           using fp_sFeatures  = const sensors_feature* (*) (const sensors_chip_name*, int*);
           using fp_sLabel     = char* (*) (const sensors_chip_name*, const sensors_feature*);
           using fp_subFeature = const sensors_subfeature* (*) (const sensors_chip_name *, const sensors_feature *, sensors_subfeature_type);
           using fp_sValue     = int (*) (const sensors_chip_name*, int,double *);
       }
   }
}

namespace sensors = uhiutil::sdef::sysensors;

const char *const Libs[] = {
  "libsensors.so",
  "libsensors.so.4",
  "libsensors.so.5"
};

const char *const clrID[] = { 
//Alice Blue",
  "Aqua",
  "Yellow",
  "Blue",
  "Green",
  "Red",
  "Cadet Blue",
  "Dark Sea Green",
  "Gold",
  "Indigo",
  "Dodger Blue",
  "Purple",
  "Cyan",
  "Black",
  "Light Blue",
//  "White",
  "Light Goldenrod Yellow",
  "Lawn Green",
  "Dark Blue",
  "Light Sky Blue",
  "Dark Goldenrod",
  "Red4",
  "Dark Gray",
  "Aquamarine",
  "Dark Green",
  "Dark Khaki",
  "Dark Magenta",
  "Dark Olive Green",
  "Azure",
  "Beige",
  "Bisque",
  "Blanched Almond",
  "Blue Violet",
  "Hot Pink",
  "Brown",
  "Burlywood",
  "Coral",
  "Plum",
  "Blue4",
  "Dark Orange",
  "Dark Orchid",
  "Dark Salmon",
  "Blue1",
  "Yellow Green"
};

const std::string style = {".bl_cls {background: #00BFFF; color: #FFFFFF; margin: 1px; border-style: solid; border-color: black; border-width: 1px;} " \
	      ".gn_cls {background: #66CCAA; color: #FFFFFF; margin: 1px; border-style: solid; border-color: black; border-width: 1px;} " \
	      ".yw_cls {background: #E5CC66; color: #FFFFFF; margin: 1px; border-style: solid; border-color: black; border-width: 1px;} " \
	      ".ls_cls {background: #778899; color: #000000; } " };

//Untrue menubar
const char* const res_uhIfo_menubar = {
  "<interface>"
  "  <menu id='uhIfomenubar'>"
  "    <submenu>"
  "      <attribute name='label'>Manage</attribute>"
  "        <item>"
  "          <attribute name='label'>Extended info</attribute>"
  "          <attribute name='action'>win.extinfo</attribute>"
  "        </item>"
  "        <item>"
  "          <attribute name='label'>CPU activity</attribute>"
  "          <attribute name='action'>win.cpuactivity</attribute>"
  "        </item>"
  "        <item>"
  "          <attribute name='label'>Quit</attribute>"
  "          <attribute name='action'>win.quitapp</attribute>"
  "        </item>"
  "    </submenu>"
  "    <submenu>"
  "      <attribute name='label'>Sensors</attribute>"
  "      <item>"
  "        <attribute name='label'>Sensors settings</attribute>"
  "        <attribute name='action'>win.sensorset</attribute>"
  "      </item>"
  "      <item>"
  "        <attribute name='label'>t° monitor</attribute>"
  "        <attribute name='action'>win.tmpmonit</attribute>"
  "      </item>"
  "    </submenu>"
  "    <submenu>"
  "      <attribute name='label'>Application</attribute>"
  "      <item>"
  "        <attribute name='label'>Preferences</attribute>"
  "        <attribute name='action'>win.prefs</attribute>"
  "      </item>"
  "      <item>"
  "        <attribute name='label'>About...</attribute>"
  "        <attribute name='action'>win.about</attribute>"
  "      </item>"
  "    </submenu>"
  "  </menu>"
  "</interface>"};

#endif // _UTIL_H_
