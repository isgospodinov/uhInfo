/*
 *    uhInfo
 *    Copyright (C) 2019
 */

#ifndef _PREFSDLG_H_
#define _PREFSDLG_H_

#include "../util/uhirc.h"

class CPrefsDlg : public Gtk::Window
{
public:
   CPrefsDlg(Gtk::Window *const mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp);
   virtual ~CPrefsDlg() = default;

   const bool GetInTmpMonStat() const {return ch_InTmpMon.get_active();}
   const bool GetSaveImpStat() const {return ch_SaveImp.get_active();}
   const int GetMaxTmpStat() const {return cb_MaxTmp.get_active_row_number();}
   const int GetMarckStressSessionStat() const {return ch_Marck_Stress_Session.get_active();}
   
   void SetFqState(bool nfs) {uhiutil::cpu::native_fq_state = nfs; ch_NativeFq.set_active(nfs);}
private:
   Gtk::ComboBoxText cb_MaxTmp;
   Gtk::CheckButton ch_InTmpMon,ch_SaveImp,ch_NativeFq,ch_lscpu_cpuinfo,ch_Marck_Stress_Session;
   Gtk::Label l_MaxTemp;
   Gtk::Box box_all,box_MaxTmp,box_InTmpMon,box_SaveImp,box_NativeFq,box_cpu_lsorinfo,box_Marck_Stress_Session;
   Gtk::Frame fr_All;

   void on_MaxTmp_changed();
   void InitVision();
   void InitData();
   virtual void on_show() override;
   bool Wnd_close_handler() {Gtk::Window::hide();return true;}
};

#endif // _PREFSDLG_H_
