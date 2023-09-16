/*
 *    uhInfo
 *    Copyright (C) 2019
 */

#include "prefsdlg.h"
#include "../proc.h"
#include <glibmm/i18n.h>

CPrefsDlg::CPrefsDlg(Gtk::Window *const mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp) : ch_InTmpMon(_("HDD/SSD in ") + std::string("t°") + _(" monitor")),
                     ch_SaveImp(_("Improve sensors behavior")),ch_NativeFq(_("Native CPU frequency")),ch_lscpu_cpuinfo(_("lscpu instead cuinfo")),
					                           ch_Marck_Stress_Session(_("Marck a stress session")),l_MaxTemp(_("Set max. ") + std::string("t°    "))
{
	set_transient_for(*mWnd);
	set_title(_("Preferences"));
	set_resizable(false);
	set_modal(true);

   for(int i = 90 ; i < 140; cb_MaxTmp.append(std::to_string(i+=10)));

   InitVision();
   InitData();

   signal_close_request().connect(sigc::mem_fun(*this, &CPrefsDlg::Wnd_close_handler),false);
   cb_MaxTmp.signal_changed().connect(sigc::mem_fun(*this,&CPrefsDlg::on_MaxTmp_changed));
   ch_NativeFq.signal_toggled().connect([=](){uhiutil::cpu::native_fq_state = ch_NativeFq.get_active();});
   ch_lscpu_cpuinfo.signal_toggled().connect([=](){uhiutil::cpu::cpu_fq_base = ch_lscpu_cpuinfo.get_active();});
   ch_Marck_Stress_Session.signal_toggled().connect([=](){uhiutil::draw::marck_strses = ch_Marck_Stress_Session.get_active();});
   uhiutil::set_css_style(get_style_context(),*cp);

   SETLOCALDECORATION;
}

void CPrefsDlg::InitVision()
{
   set_child(fr_All);
   fr_All.set_child(box_all);
   fr_All.set_margin(8);
   box_MaxTmp.set_margin(4);
   box_all.set_orientation(Gtk::Orientation::VERTICAL);
   box_MaxTmp.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_InTmpMon.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_SaveImp.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_NativeFq.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_cpu_lsorinfo.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_Marck_Stress_Session.set_orientation(Gtk::Orientation::HORIZONTAL);

   box_InTmpMon.append(ch_InTmpMon);
   box_SaveImp.append(ch_SaveImp);
   box_NativeFq.append(ch_NativeFq);
   box_MaxTmp.append(l_MaxTemp);
   box_MaxTmp.append(cb_MaxTmp);
   box_cpu_lsorinfo.append(ch_lscpu_cpuinfo);
   box_Marck_Stress_Session.append(ch_Marck_Stress_Session);

   box_all.append(box_InTmpMon);
   box_all.append(box_SaveImp);
   box_all.append(box_NativeFq);
   box_all.append(box_cpu_lsorinfo);
   box_all.append(box_Marck_Stress_Session);
   box_all.append(box_MaxTmp);

   box_MaxTmp.set_halign(Gtk::Align::CENTER);
   box_MaxTmp.set_valign(Gtk::Align::CENTER);
   box_all.set_halign(Gtk::Align::CENTER);
   box_all.set_valign(Gtk::Align::CENTER);

   fr_All.set_expand();
   box_all.set_expand();
}

void CPrefsDlg::InitData()
{
    std::string config = "/home/" + uhiutil::GetUserName() + "/.uhInfo/uhInfo.cfg",cmd("");

    if(uhiutil::ExistenceVerification(config.c_str())) {
           cmd = "cat " + config + " | grep uhiMaxTmp=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "") {
               if(uhiutil::newline(cmd,"uhiMaxTmp=",Direction::RIGHTSKIP)) cb_MaxTmp.set_active(std::stoi(cmd));
               else cb_MaxTmp.set_active(2);//set default 120°
           }
           else cb_MaxTmp.set_active(2);//set default 120°

           on_MaxTmp_changed();

           cmd = "cat " + config + " | grep uhiInTmpMon=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiInTmpMon=",Direction::RIGHTSKIP);
           ch_InTmpMon.set_active((cmd != "" ? std::stoi(cmd) == 1 : false));
           
           cmd = "cat " + config + " | grep uhiSaveImp=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiSaveImp=",Direction::RIGHTSKIP);
           ch_SaveImp.set_active((cmd != "" ? std::stoi(cmd) == 1 : false));

           cmd = "cat " + config + " | grep uhiNativeFqState=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiNativeFqState=",Direction::RIGHTSKIP);
           SetFqState((cmd != "" ? std::stoi(cmd) == 1 : false));

           cmd = "cat " + config + " | grep uhiMarckStressession=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiMarckStressession=",Direction::RIGHTSKIP);
           ch_Marck_Stress_Session.set_active((cmd != "" ? std::stoi(cmd) == 1 : false));
           uhiutil::draw::marck_strses = ch_Marck_Stress_Session.get_active();
    }
    else {
          ch_InTmpMon.set_active(false);
          ch_SaveImp.set_active(false);
          ch_Marck_Stress_Session.set_active(false);
          uhiutil::draw::marck_strses = false;
          SetFqState(false);
          cb_MaxTmp.set_active(2); //init default 120°
    }
}

inline void CPrefsDlg::on_show()
{
	Gtk::Window::on_show();

	ch_lscpu_cpuinfo.set_active(uhiutil::cpu::cpu_fq_base);
	if(!CProc::m_lsCpu)
		ch_lscpu_cpuinfo.set_visible(false);
}

void CPrefsDlg::on_MaxTmp_changed()
{
  std::string text = cb_MaxTmp.get_active_text();
  if(!(text.empty()))
              uhiutil::cpu::max_cpu_t = std::atoi(text.c_str());
  else uhiutil::cpu::max_cpu_t = 120;
}
