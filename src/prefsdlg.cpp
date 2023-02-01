/*
 *    uhInfo
 *    Copyright (C) 2019
 */

#include "prefsdlg.h"

CPrefsDlg::CPrefsDlg(Gtk::Window *const mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp) : pmWnd(mWnd),
                     ch_InTmpMon("HDD/SSD in t° monitor"),ch_AllInput("All input sensors"),
                     ch_SaveImp("Improve sensors behavior"),ch_NativeFq("Native CPU frequency"),ch_ShowCPUfq("When t° show CPU Fq."),
					 l_MaxTemp("Set max. t°    ")
{
	set_transient_for(*mWnd);
	set_title("Preferences");
	set_resizable(false);
	set_modal(true);

   for(int i = 90 ; i < 140; cb_MaxTmp.append(std::to_string(i+=10)));

   InitVision();
   InitData();
   signal_close_request().connect(sigc::mem_fun(*this, &CPrefsDlg::Wnd_close_handler),false);
   cb_MaxTmp.signal_changed().connect(sigc::mem_fun(*this,&CPrefsDlg::on_MaxTmp_changed));
   ch_NativeFq.signal_toggled().connect(sigc::mem_fun(*this,&CPrefsDlg::on_NativeFq_changed));
   uhiutil::set_css_style(get_style_context(),*cp);
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
   box_AllInput.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_SaveImp.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_NativeFq.set_orientation(Gtk::Orientation::HORIZONTAL);
   box_ShowCPUfq.set_orientation(Gtk::Orientation::HORIZONTAL);

   box_InTmpMon.append(ch_InTmpMon);
   box_AllInput.append(ch_AllInput);
   box_SaveImp.append(ch_SaveImp);
   box_NativeFq.append(ch_NativeFq);
   box_ShowCPUfq.append(ch_ShowCPUfq);
   box_MaxTmp.append(l_MaxTemp);
   box_MaxTmp.append(cb_MaxTmp);

   box_all.append(box_InTmpMon);
   box_all.append(box_AllInput);
   box_all.append(box_SaveImp);
   box_all.append(box_NativeFq);
   box_all.append(box_ShowCPUfq);
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

           cmd = "cat " + config + " | grep uhiAllInput=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiAllInput=",Direction::RIGHTSKIP);
           ch_AllInput.set_active((cmd != "" ? std::stoi(cmd) == 1 : false));
           
           cmd = "cat " + config + " | grep uhiSaveImp=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiSaveImp=",Direction::RIGHTSKIP);
           ch_SaveImp.set_active((cmd != "" ? std::stoi(cmd) == 1 : false));

           cmd = "cat " + config + " | grep uhiShowCpuFqWrn=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiShowCpuFqWrn=",Direction::RIGHTSKIP);
           ch_ShowCPUfq.set_active((cmd != "" ? std::stoi(cmd) == 1 : false));

           cmd = "cat " + config + " | grep uhiNativeFqState=";
           cmd = uhiutil::execmd(cmd.c_str());
           if(cmd != "")
               uhiutil::newline(cmd,"uhiNativeFqState=",Direction::RIGHTSKIP);
           SetFqState((cmd != "" ? std::stoi(cmd) == 1 : false));
           
    }
    else {
          ch_InTmpMon.set_active(false);
          ch_AllInput.set_active(false);
          ch_SaveImp.set_active(false);
          ch_ShowCPUfq.set_active(false);
          SetFqState(false);
          cb_MaxTmp.set_active(2); //init default 120°
    }
}

void CPrefsDlg::on_MaxTmp_changed()
{
  std::string text = cb_MaxTmp.get_active_text();
  if(!(text.empty()))
              uhiutil::cpu::max_cpu_t = std::atoi(text.c_str());
  else uhiutil::cpu::max_cpu_t = 120;
}
