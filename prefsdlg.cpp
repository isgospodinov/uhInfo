/*
 *    uhInfo
 *    Copyright (C) 2019
 */

#include "prefsdlg.h"
#include <iostream>

CPrefsDlg::CPrefsDlg(Gtk::Window *const mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp) : Gtk::Dialog("Preferences",*mWnd,true),pmWnd(mWnd),
                     ch_InTmpMon("HDD/SSD in t° monitor"),ch_AllInput("All input sensors"),
                     ch_SaveImp("Improve sensors behavior"),ch_NativeFq("Native CPU frequency"),l_MaxTemp("Set max. t°    ")
{
   for(int i = 90 ; i < 140; cb_MaxTmp.append(std::to_string(i+=10)));

   InitVision();
   InitData();
   cb_MaxTmp.signal_changed().connect(sigc::mem_fun(*this,&CPrefsDlg::on_MaxTmp_changed));
   ch_NativeFq.signal_clicked().connect(sigc::mem_fun(*this,&CPrefsDlg::on_NativeFq_changed));
   get_style_context()->add_provider(*cp, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void CPrefsDlg::InitVision()
{
   get_content_area()->pack_start(fr_All);
   fr_All.add(box_all);
   box_all.set_orientation(Gtk::ORIENTATION_VERTICAL);
   box_MaxTmp.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
   box_InTmpMon.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
   box_AllInput.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
   box_SaveImp.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
   box_NativeFq.set_orientation(Gtk::ORIENTATION_HORIZONTAL);   


   box_InTmpMon.pack_start(ch_InTmpMon, Gtk::PACK_SHRINK);
   box_AllInput.pack_start(ch_AllInput, Gtk::PACK_SHRINK);
   box_SaveImp.pack_start(ch_SaveImp, Gtk::PACK_SHRINK);
   box_NativeFq.pack_start(ch_NativeFq, Gtk::PACK_SHRINK);      
   box_MaxTmp.pack_start(l_MaxTemp, Gtk::PACK_SHRINK);
   box_MaxTmp.pack_start(cb_MaxTmp, Gtk::PACK_SHRINK);

   box_all.pack_start(box_InTmpMon, Gtk::PACK_SHRINK);
   box_all.pack_start(box_AllInput, Gtk::PACK_SHRINK);
   box_all.pack_start(box_SaveImp, Gtk::PACK_SHRINK);
   box_all.pack_start(box_NativeFq, Gtk::PACK_SHRINK);         
   box_all.pack_start(box_MaxTmp, Gtk::PACK_SHRINK);

   box_MaxTmp.set_border_width(4);
   box_InTmpMon.set_border_width(4);
   box_AllInput.set_border_width(4);
   box_SaveImp.set_border_width(4);
   box_NativeFq.set_border_width(4);   

   box_MaxTmp.set_halign(Gtk::ALIGN_CENTER);
   box_MaxTmp.set_valign(Gtk::ALIGN_CENTER);
   box_all.set_halign(Gtk::ALIGN_CENTER);
   box_all.set_valign(Gtk::ALIGN_CENTER);

   get_vbox()->set_border_width(8);
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
