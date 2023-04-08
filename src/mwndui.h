/*
 *    uhInfo
 *    Copyright (C) 2020
 */

#ifndef _MWNDUI_H_
#define _MWNDUI_H_

#include <mutex>
#include "uhirc.h"

using unit_calc_el = struct _unit_calc_el;

class UIHWindow : public Gtk::ApplicationWindow
{
public:
  UIHWindow();

  const uhimc_unique_ptr tColumns = uhimc_unique_ptr(new CModelUhiColumns()),dColumns = uhimc_unique_ptr(new CModelUhiColumns(ModelBaseUhiColumns::UhiModelType::DISK));
  const Glib::RefPtr<Gtk::ListStore> m_refTreeModel = Gtk::ListStore::create(*dColumns),ptRefTreeModel = Gtk::ListStore::create(*tColumns);
    
  bool temperature_monitoring_enabled = false;
  Gtk::Label m_Label_MOBO,m_Label_Memory,m_Label_CPU,m_Label_Monitors,m_Label_Audio,m_Label_Network,m_Label_OS;
  static post_init_sig signal_set_param() {return sig_postinit_param;}
protected:
  static post_init_sig sig_postinit_param;

  const Glib::RefPtr<Gio::SimpleAction> item_infomode{add_action_bool("extinfo",sigc::mem_fun(*this,&UIHWindow::enhanced_system_info))},
                                       item_cpu{add_action_bool("cpuactivity",sigc::mem_fun(*this,&UIHWindow::show_cpu_activity_all))},
                                       item_temperature{add_action_bool("tmpmonit",sigc::mem_fun(*this,&UIHWindow::monitor_temperature), false)},
                                       item_manage{add_action("sensorset",sigc::mem_fun(*this,&UIHWindow::manage_sensors))},
                                       item_options{add_action("prefs",sigc::mem_fun(*this,&UIHWindow::get_preferences))};


  Gtk::Frame m_Frame_MOBO,m_Frame_Memory,m_Frame_CPU,m_Frame_VGA,m_Frame_Monitors,m_Frame_Audio,m_Frame_Network,m_Frame_Disks,m_Frame_CPUFrecq,m_Frame_VGA_pcie,m_Frame_OS,m_Frame_Sensors; // Data.
  Gtk::Paned m_HPaned, m_VPanedTrmpetature;
  Gtk::Box m_VBoxAll,m_Revealer,m_VBoxVLeft,m_VBoxVRight,m_VBoxCPU,m_VBoxCPU_Freq_Use,m_VBoxVGA,m_VBox_Audio,m_VBox_Network,m_VBoxCPUActivityAll,m_Box_TmpControls;
  Gtk::Grid m_gridVGA_cond,m_StatusBar_Grid_condit;
  Gtk::Box m_ButtonBox,mT_All,mDA_ToolBar,m_Box_CPUActivityAll,m_Box_GRridAndButt;
  Gtk::Button m_ButtonQuit,m_ButtCPUOverall;
  Gtk::ScrolledWindow m_ScrolledWindow,m_ScrolledWindowTreeView,m_ScrolledWindowCPUActivityAll,m_ScrolledWindowTemperatures;
  Gtk::Label m_Label_Sensors,m_ToolbarChoice{"  >  "};
  Gtk::Box hbox_operation_status_cpu,hbox_operation_status_pcie,hbox_operation_status_sensors;
  Gtk::Image operation_status_image_cpu,operation_status_image_pcie,operation_status_image_sensors;

  //CPU activity all
  Gtk::Grid m_BlinkGrid,m_CPUModeGrid;
  Gtk::Label m_BlinkLabel, m_CPUModeLabel, m_CPUCompareLabel, m_CPUOverallLabel, m_CPUNativeFqLabel;
  Gtk::Switch m_CPUModeSwitch, m_CPUCompareSwitch, m_CPUNativeFqSwitch, m_CPUOverallSwitch, m_BlinkSwitch;

  Gtk::FlowBox m_Fbox_CPUActivityAll;
  Gtk::Frame m_Frame_CPUActivityAll,m_DAFrame_Temperature,m_TbFrame,mFr_GRrid;

  Gtk::ProgressBar m_pbFreq,m_pbUse;
  Gtk::Separator m_separator;
  Gtk::Label m_sb_labeltext,m_sb_status,m_sb_cpu_labeltext,m_sb_cpu_status,m_status_bar;

  bool smDlg_shown = false,sensors_printing_enable = false,temperature_mode_status = false;

  Gtk::TextView m_TextView;
  Gtk::Label m_Label_VGA,m_Label_VGA_cond,m_Label_VGA_cond_status;
  Gtk::TreeView m_TreeView,m_temperatureTreeView;
  Gtk::ComboBoxText m_Gpus;
  
  std::string Ud2printcache = "";

  std::mutex mutex_print;
  std::unique_ptr<sigc::connection> c_Timer{nullptr};

  CDrArTempr m_DAtemperature;

  // ------------------ ToolBar functionality ------------------
  Gtk::PopoverMenu m_ToolBarMenuPopup;
  Glib::RefPtr<Gtk::GestureClick> m_refToolBarChoice = Gtk::GestureClick::create();
  void InitToolBar();
  virtual void on_tbt_clicked(bool param) = 0;
  // ------------------ ToolBar functionality ------------------

  void InitUI();
  void InitUI_activity_vision(const std::list<unit_calc_el> *unclel,std::list<cpu_chain_el> &cpu_units_monit_chain);
  void StatusbarCpuText();
  
  virtual void about_dialog_info() = 0;
  virtual void enhanced_system_info() = 0;
  virtual void show_cpu_activity_all() = 0;
  virtual void manage_sensors() = 0;
  virtual void monitor_temperature() = 0;
  virtual void get_preferences() const  = 0;
  
  virtual void Wnd_show_handler() = 0;
  virtual bool Wnd_close_handler() = 0;
  virtual void on_quit_button_clicked() = 0;
  virtual void on_overall_button_clicked() = 0;
  virtual void on_gpus_selection_changed() = 0;
  virtual void On_CPUActivityAll_switch_changed() = 0;
  virtual void On_Compare_mode_switch_changed() = 0;
  virtual void On_NativeFq_changed() = 0;
  virtual void On_CPUOverall_changed() = 0;
  virtual void on_DA_button_press_event(int npress, double x, double y) = 0;

  virtual ~UIHWindow() {m_ToolBarMenuPopup.unparent();}
};

#endif // _MWNDUI_H_
