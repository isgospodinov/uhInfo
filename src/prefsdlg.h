/*
 *    uhInfo
 *    Copyright (C) 2019
 */

#ifndef _PREFSDLG_H_
#define _PREFSDLG_H_

#include "uhirc.h"

class CPrefsDlg : public Gtk::Dialog
{
public:
   CPrefsDlg(Gtk::Window *const mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp);
   virtual ~CPrefsDlg() = default;

   const bool GetInTmpMonStat() const {return ch_InTmpMon.get_active();}
   const bool GetAllInputStat() const {return ch_AllInput.get_active();}
   const bool GetSaveImpStat() const {return ch_SaveImp.get_active();}
   const bool GetShowCPUfq() const {return ch_ShowCPUfq.get_active();}
   const int GetMaxTmpStat() const {return cb_MaxTmp.get_active_row_number();}
   
   void SetFqState(bool nfs) {uhiutil::cpu::native_fq_state = nfs; ch_NativeFq.set_active(nfs);}
private:
   Gtk::Window *const pmWnd = nullptr;
   
   Gtk::ComboBoxText cb_MaxTmp;
   Gtk::CheckButton ch_InTmpMon,ch_AllInput,ch_SaveImp,ch_NativeFq,ch_ShowCPUfq;
   Gtk::Label l_MaxTemp;
   Gtk::Box box_all,box_MaxTmp,box_InTmpMon,box_AllInput,box_SaveImp,box_NativeFq,box_ShowCPUfq;
   Gtk::Frame fr_All;

   void on_MaxTmp_changed();
   void on_NativeFq_changed() {uhiutil::cpu::native_fq_state = ch_NativeFq.get_active();}
   void InitVision();
   void InitData();
   virtual void on_show() override {Gtk::Dialog::on_show();}
   bool Wnd_close_handler() {Gtk::Dialog::hide();return true;}

};

#endif // _PREFSDLG_H_
