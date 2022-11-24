/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _MWND_H_
#define _MWND_H_

#include "mwndui.h"
#include "hth.h"
#include "procun.h"
#include "sysens.h"
#include "ud2mon.h"
#include "hgpu.h"
#include "hmonit.h"
#include "prefsdlg.h"
#include "aboutdlg.h"
#include "clrdialog.h"

class CHWindow : public CInitThread, public UIHWindow
{
  const Glib::RefPtr<Gtk::CssProvider> css_prov;
public:
  CHWindow();

  void smDlgResponse(int id) {smDlg_shown = false;SMDLGMISTAT(true);}
  void QuitTasks() const;

  const std::shared_ptr<CSysens> pSysensors;
  const std::shared_ptr<Ud2mon> pUd2Manager;
  const std::unique_ptr<CProc> pntProcessor;
  const std::unique_ptr<CGpus> pGpus;
  const std::unique_ptr<CMonitor> pMonitor;
  const std::unique_ptr<CPrefsDlg> pfDlg;
private:
  const std::unique_ptr<CSmDialog> smDlg;
  const std::unique_ptr<CAboutDlg> abtDlg;
  const std::unique_ptr<ClrDialog> clrDlg;

  friend void ClrDialog::OnColorChoiceToggled(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
  friend bool ClrDialog::Wnd_close_handler();
  friend void ClrDialog::on_show();

  void On_Temperature_Row_Activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn *column);

  void sensors_print(bool Ud2print,bool extension);
  void Posthreadnotify();// Dispatcher handler.
  void init_units_activity_vision();
  void InitVision();
  bool uhI_Timer(int TmNo);
  void RedrawActivity(){for(std::list<cpu_chain_el>::iterator chnel = cpu_units_monit_chain.begin(); chnel != cpu_units_monit_chain.end(); chnel++) (*chnel).pDArea->Redraw();}
  void OnTempToggled(const Glib::ustring &path_string);
  void ShowHide_compare_elements(bool sv = false) const {
         std::for_each(cpu_units_monit_chain.begin(), cpu_units_monit_chain.end(), [sv](const cpu_chain_el &ce){ 
              if(ce.lCompareColor) ce.lCompareColor->set_visible(sv);
              if(ce.cpuid_m_pbCF) ce.cpuid_m_pbCF->set_visible(sv);});
       }
  virtual void enhanced_system_info() override;
  virtual void show_cpu_activity_all() override;
  virtual void manage_sensors() override {if(smDlg){SMDLGMISTAT(false);smDlg_shown = true;smDlg->present();}}
  virtual void monitor_temperature() override;
  virtual void get_preferences() const override {if(pfDlg){pfDlg->present();}}
  virtual void Wnd_show_handler() override {m_VPanedTrmpetature.set_position((get_height() - 110) / 2);}
  virtual void on_quit_button_clicked() override {QuitTasks();hide();}
  virtual void On_CPUActivityAll_switch_changed() override;
  virtual void On_Compare_mode_switch_changed() override;
  virtual void On_NativeFq_changed() override {pfDlg->SetFqState(m_CPUNativeFqSwitch.get_active());}
  virtual void on_gpus_selection_changed() override {if(pGpus)m_Label_VGA.set_text(pGpus->GpuStatus(m_Gpus.get_active_row_number()));}
  bool Wnd_close_handler() override {QuitTasks();return false;}
  virtual void about_dialog_info() override {abtDlg->set_message(get_title());abtDlg->show();}
  virtual void on_DA_button_press_event(int npress, double x, double y) override;

  std::list<cpu_chain_el> cpu_units_monit_chain; // cpu units activity vision elements 
};

#endif // _MWND_H_
