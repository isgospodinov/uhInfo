/*
 *    uhInfo
 *    Copyright (C) 2020
 */
 
#include "mwndui.h"
#include "procun.h"

UIHWindow::UIHWindow() : m_ScrolledWindow(), m_ScrolledWindowTreeView(), m_ScrolledWindowCPUActivityAll(),
                         m_TextView(), m_temperatureTreeView(ptRefTreeModel), m_TreeView(m_refTreeModel), m_Gpus()
{  

  Gtk::MenuBar *mb = Gtk::manage(new Gtk::MenuBar());
  m_VBoxMenu.pack_start(*mb, Gtk::PACK_SHRINK, 0);
  
  Gtk::MenuItem *menuitem_show = Gtk::manage(new Gtk::MenuItem("_Manage", true));
  mb->append(*menuitem_show);
  Gtk::Menu *showemenu = Gtk::manage(new Gtk::Menu());
  menuitem_show->set_submenu(*showemenu);
  item_infomode = Gtk::manage(new Gtk::CheckMenuItem("_Extended info", true));
  showemenu->append(*item_infomode);
  item_cpu = Gtk::manage(new Gtk::CheckMenuItem("_CPU activity", true));
  showemenu->append(*item_cpu);
  Gtk::MenuItem *item_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
  showemenu->append(*item_quit);

  Gtk::MenuItem *menuitem_sm = Gtk::manage(new Gtk::MenuItem("_Sensors", true));
  mb->append(*menuitem_sm);
  Gtk::Menu *smanage = Gtk::manage(new Gtk::Menu());
  menuitem_sm->set_submenu(*smanage);
  item_manage = Gtk::manage(new Gtk::MenuItem("_Settings", true));
  smanage->append(*item_manage);
  item_temperature = Gtk::manage(new Gtk::CheckMenuItem("_t° monitor", true));
  smanage->append(*item_temperature);

  Gtk::MenuItem *menuitem_ot = Gtk::manage(new Gtk::MenuItem("_Options", true));
  mb->append(*menuitem_ot);
  Gtk::Menu *options = Gtk::manage(new Gtk::Menu());
  menuitem_ot->set_submenu(*options);
  item_options = Gtk::manage(new Gtk::MenuItem("_Preferences", true));
  options->append(*item_options);

  Gtk::MenuItem *menuitem_app = Gtk::manage(new Gtk::MenuItem("_About", true));
  mb->append(*menuitem_app);
  Gtk::Menu *appemenu = Gtk::manage(new Gtk::Menu());
  menuitem_app->set_submenu(*appemenu);
  Gtk::MenuItem *item_about = Gtk::manage(new Gtk::MenuItem("_About", true));
  appemenu->append(*item_about);


  m_BlinkGrid.attach(m_BlinkLabel, 0, 1, 1, 1);
  m_BlinkGrid.attach(m_BlinkSwitch, 1, 1, 1, 1);
  
  m_CPUModeGrid.attach(m_CPUModeLabel, 0, 1, 1, 1);
  m_CPUModeGrid.attach(m_CPUModeSwitch, 1, 1, 1, 1);
  m_CPUModeGrid.attach(m_CPUNativeFqLabel, 2, 1, 1, 1);
  m_CPUModeGrid.attach(m_CPUNativeFqSwitch, 3, 1, 1, 1);   
  m_CPUModeGrid.attach(m_CPUCompareLabel, 1, 2, 1, 1);
  m_CPUModeGrid.attach(m_CPUCompareSwitch, 2, 2, 1, 1);   

  m_VBoxVLeft.pack_start(m_Frame_OS, Gtk::PACK_SHRINK);
  m_VBoxVLeft.pack_start(m_BlinkGrid, Gtk::PACK_SHRINK);
  m_VBoxVLeft.pack_start(m_Frame_Sensors);
  m_Box_TmpControls.pack_start(m_ScrolledWindowTemperatures);

  m_VPanedTrmpetature.add1(m_Box_TmpControls);
  m_VPanedTrmpetature.add2(m_DABox_Temperature);

  m_temperatureTreeView.append_column_editable("+/-", tColumns->col_tcheck);
  m_temperatureTreeView.append_column("Color", tColumns->color);
  m_temperatureTreeView.append_column("Node", tColumns->tsensor_node);
  m_temperatureTreeView.append_column("Sensor", tColumns->tsensor_name);

  m_TreeView.append_column("No", dColumns->No);
  m_TreeView.append_column("Link", dColumns->sys_link);
  m_TreeView.append_column("Name", dColumns->device_name);
  m_TreeView.append_column("Type", dColumns->type);
  m_TreeView.append_column("Specificity", dColumns->specificity);

  m_VBoxVRight.pack_start(m_Frame_MOBO, Gtk::PACK_SHRINK);
  m_VBoxVRight.pack_start(m_Frame_Memory, Gtk::PACK_SHRINK);
  m_VBoxVRight.pack_start(m_Frame_CPU, Gtk::PACK_SHRINK);
  m_VBoxVRight.pack_start(m_Frame_VGA, Gtk::PACK_SHRINK);
  m_VBoxVRight.pack_start(m_Frame_Monitors, Gtk::PACK_SHRINK);
  m_VBoxVRight.pack_start(m_Frame_Audio, Gtk::PACK_SHRINK);
  m_VBoxVRight.pack_start(m_Frame_Network, Gtk::PACK_SHRINK);
  m_VBoxVRight.pack_start(m_VPanedTrmpetature);
  m_VBoxVRight.pack_start(m_Frame_Disks);

  m_Frame_MOBO.add(m_Label_MOBO);
  m_Frame_Memory.add(m_Label_Memory);
  m_Frame_CPU.add(m_VBoxCPU);
  m_Frame_Monitors.add(m_Label_Monitors);
  m_Frame_Audio.add(m_VBox_Audio);
  m_Frame_Network.add(m_VBox_Network);

  m_VBox_Audio.pack_start(m_Label_Audio, Gtk::PACK_SHRINK); 
  m_VBox_Network.pack_start(m_Label_Network, Gtk::PACK_SHRINK);
  
  m_VBoxCPU_Freq_Use.pack_start(m_pbFreq, Gtk::PACK_SHRINK);  
  m_VBoxCPU_Freq_Use.pack_start(m_pbUse, Gtk::PACK_SHRINK);  

  m_Frame_CPUFrecq.add(m_VBoxCPU_Freq_Use);

  m_VBoxCPU.pack_start(m_Label_CPU, Gtk::PACK_SHRINK);
  m_VBoxCPU.pack_start(m_Frame_CPUFrecq, Gtk::PACK_SHRINK);
  m_gridVGA_cond.set_column_homogeneous(true);
  m_gridVGA_cond.attach(m_Label_VGA_cond ,0 ,0 ,1 ,1);
  m_gridVGA_cond.attach_next_to(m_Label_VGA_cond_status,m_Label_VGA_cond,Gtk::POS_RIGHT,1,1);
  m_Frame_VGA.add(m_VBoxVGA);
  m_Frame_VGA_pcie.add(m_gridVGA_cond);
  m_VBoxVGA.pack_start(m_Gpus, Gtk::PACK_SHRINK);
  m_VBoxVGA.pack_start(m_Label_VGA, Gtk::PACK_SHRINK);
  m_VBoxVGA.pack_start(m_Frame_VGA_pcie, Gtk::PACK_SHRINK, Gtk::POS_LEFT);

  m_Frame_OS.add(m_Label_OS);

  m_Frame_Disks.add(m_ScrolledWindowTreeView);
  m_Frame_Sensors.add(m_ScrolledWindow);

  m_ScrolledWindowTemperatures.add(m_temperatureTreeView);
  m_ScrolledWindowTemperatures.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_ScrolledWindowTreeView.add(m_TreeView);
  m_ScrolledWindowTreeView.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_ScrolledWindow.add(m_TextView);
  m_ScrolledWindowCPUActivityAll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_Frame_CPUActivityAll.add(m_Fbox_CPUActivityAll);
  m_ScrolledWindowCPUActivityAll.add(m_Frame_CPUActivityAll);

  m_Fbox_CPUActivityAll.set_max_children_per_line(5);//set default
  m_Fbox_CPUActivityAll.set_selection_mode(Gtk::SELECTION_NONE);
  m_Fbox_CPUActivityAll.set_homogeneous();

  m_ButtonBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK);
  m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);

  m_HPaned.add1(m_VBoxVRight);
  m_HPaned.add2(m_VBoxVLeft);

  m_Revealer.add(m_HPaned);

  m_VBoxAll.pack_start(m_VBoxMenu, Gtk::PACK_SHRINK);
  m_VBoxAll.pack_start(m_Revealer);
  m_VBoxCPUActivityAll.pack_start(m_CPUModeGrid, Gtk::PACK_SHRINK);
  m_VBoxCPUActivityAll.pack_start(m_ScrolledWindowCPUActivityAll);
  m_VBoxAll.pack_start(m_VBoxCPUActivityAll);
  m_status_bar.pack_end(m_ButtonBox, Gtk::PACK_SHRINK);
  m_status_bar.pack_start(m_sb_cpu_labeltext, Gtk::PACK_SHRINK);
  m_status_bar.pack_start(m_sb_cpu_status, Gtk::PACK_SHRINK);
  m_status_bar.pack_start(m_sb_labeltext, Gtk::PACK_SHRINK);
  m_status_bar.pack_start(m_sb_status, Gtk::PACK_SHRINK);
  m_VBoxAll.pack_start(m_separator,  Gtk::PACK_SHRINK);
  m_VBoxAll.pack_end(m_status_bar, Gtk::PACK_SHRINK);

  m_Revealer.set_transition_type(Gtk::REVEALER_TRANSITION_TYPE_NONE);
  m_Revealer.set_transition_duration(10);
  m_Revealer.set_reveal_child(true);
 
  hbox_operation_status_sensors.pack_start(operation_status_image_sensors);
  hbox_operation_status_sensors.pack_start(m_Label_Sensors);
  hbox_operation_status_cpu.pack_start(operation_status_image_cpu);
  hbox_operation_status_pcie.pack_start(operation_status_image_pcie);
  
  item_about->signal_activate().connect(sigc::mem_fun(*this, &UIHWindow::about_dialog_info));
  item_quit->signal_activate().connect(sigc::mem_fun(*this, &UIHWindow::on_quit_button_clicked));
  item_infomode->signal_activate().connect(sigc::mem_fun(*this, &UIHWindow::enhanced_system_info));
  item_cpu->signal_activate().connect(sigc::mem_fun(*this, &UIHWindow::show_cpu_activity_all));
  item_manage->signal_activate().connect(sigc::mem_fun(*this, &UIHWindow::manage_sensors));
  item_temperature->signal_activate().connect(sigc::mem_fun(*this, &UIHWindow::monitor_temperature));
  item_options->signal_activate().connect(sigc::mem_fun(*this, &UIHWindow::get_preferences));
  
  signal_show().connect(sigc::mem_fun(*this, &UIHWindow::Wnd_show_handler));
  m_ButtonQuit.signal_clicked().connect(sigc::mem_fun(*this, &UIHWindow::on_quit_button_clicked));
  m_Gpus.signal_changed().connect(sigc::mem_fun(*this,&UIHWindow::on_gpus_selection_changed));
  m_CPUModeSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_CPUActivityAll_switch_changed));
  m_CPUCompareSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_Compare_mode_switch_changed));
  m_CPUNativeFqSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_NativeFq_changed));  
}
 
void UIHWindow::InitUI()
{
  m_Frame_Disks.set_label("Disks :");
  m_Label_Sensors.set_text(" Sensors :");
  m_Frame_CPU.set_label("CPU :");
  m_Frame_VGA.set_label("GPU :");
  m_Frame_MOBO.set_label("Mainboard :");
  m_Frame_Memory.set_label("Memory :");
  m_Frame_OS.set_label("OS :");
  m_Frame_Monitors.set_label("Monitor(s) :");
  m_Frame_Audio.set_label("Audio :");
  m_Frame_Network.set_label("Network :");
  m_Label_MOBO.set_label("Vendor :\nMainboard :\nBIOS ver. :");
  m_Label_Memory.set_label("Memory size :");
  m_Label_CPU.set_label("CPU name:\nCPU cores/threads:\nCPU min/max:");
  m_Label_VGA.set_label("GPU :\nResolution :");
  m_Label_VGA_cond.set_label("PCIE link current : ");
  m_Label_VGA_cond_status.set_label("- / -");
  m_Label_OS.set_label("OS :\nKernel :\nDisplay Server :\nCompiler :");
  m_BlinkLabel.set_label("Indicators blinking    ");
  m_CPUModeLabel.set_label("Fq. source ");
  m_CPUCompareLabel.set_label("Compare ");
  m_CPUNativeFqLabel.set_label("Relative/Native ");  
  m_ButtonQuit.set_label("Quit");
  m_sb_labeltext.set_text("sensors status : ");
  m_sb_cpu_labeltext.set_text("CPU freq. : ");
  m_sb_cpu_status.set_text("N/A     ");
  m_sb_status.set_text("OFF     ");
  m_Label_Audio.set_text("Audio device:");
  m_Label_Network.set_text("Ethernet controller:");
  m_VBoxCPUActivityAll.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_VBoxMenu.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_HPaned.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
  m_VPanedTrmpetature.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_VBoxCPU.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_VBoxVLeft.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_VBoxVRight.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_VBoxAll.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_ButtonBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
  m_VBoxCPU_Freq_Use.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_VBoxVGA.set_orientation(Gtk::ORIENTATION_VERTICAL);
  m_separator.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
  
  m_HPaned.set_wide_handle();
  m_VPanedTrmpetature.set_wide_handle();

  m_VBoxMenu.set_border_width(4);
  m_ButtonQuit.set_image_from_icon_name("application-exit");
  m_ButtonQuit.set_always_show_image();

  m_VBoxVLeft.set_border_width(4);
  m_VBoxVRight.set_border_width(4);
  m_Frame_CPUFrecq.set_border_width(8);
  m_VBoxCPU_Freq_Use.set_border_width(5);
  m_VBoxCPU_Freq_Use.set_spacing(5);
  m_Gpus.set_border_width(5);
  m_Frame_CPUActivityAll.set_border_width(4);
  m_DABox_Temperature.set_border_width(4);
  m_Box_TmpControls.set_border_width(4);

  m_Frame_VGA_pcie.set_border_width(5);

  m_pbFreq.set_halign(Gtk::ALIGN_CENTER);
  m_pbFreq.set_valign(Gtk::ALIGN_CENTER);
  m_pbFreq.set_text("Frequency");
  m_pbFreq.set_show_text();

  m_pbUse.set_halign(Gtk::ALIGN_CENTER);
  m_pbUse.set_valign(Gtk::ALIGN_CENTER);
  m_pbUse.set_text("Usage");
  m_pbUse.set_show_text();

  m_TextView.set_editable(false);

  m_BlinkGrid.set_halign(Gtk::ALIGN_CENTER);
  m_BlinkLabel.set_halign(Gtk::ALIGN_START);
  m_BlinkSwitch.set_halign(Gtk::ALIGN_END);
  m_CPUModeGrid.set_halign(Gtk::ALIGN_CENTER);
  m_CPUModeGrid.set_column_spacing(4);
  m_CPUModeGrid.set_row_spacing(4);  
  m_CPUModeGrid.set_column_homogeneous(false);
  m_CPUModeGrid.set_row_homogeneous(false);  
  m_CPUModeLabel.set_halign(Gtk::ALIGN_END);
  m_CPUCompareLabel.set_halign(Gtk::ALIGN_END);
  m_CPUNativeFqLabel.set_halign(Gtk::ALIGN_END);  
  m_CPUModeSwitch.set_halign(Gtk::ALIGN_START);
  m_CPUCompareSwitch.set_halign(Gtk::ALIGN_START);
  m_CPUNativeFqSwitch.set_halign(Gtk::ALIGN_START);  
  m_Label_VGA_cond.set_halign(Gtk::ALIGN_END);
  m_Label_VGA_cond_status.set_halign(Gtk::ALIGN_START);
  m_DABox_Temperature.override_background_color(Gdk::RGBA(uhiutil::ui::cpunits_bckcolor));
  m_sb_cpu_status.override_color(Gdk::RGBA(uhiutil::ui::sensors_color));
  m_sb_status.override_color(Gdk::RGBA(uhiutil::ui::sensors_color));
  m_pbFreq.override_color(Gdk::RGBA(uhiutil::ui::sensors_color));
  m_pbUse.override_color(Gdk::RGBA(uhiutil::ui::sensors_color));
  m_Label_VGA_cond_status.override_color(Gdk::RGBA(uhiutil::ui::sensors_color));
  m_TextView.override_color(Gdk::RGBA(uhiutil::ui::sensors_color));
  
  STATUSIMAGES_SET_INACTIVE;
  SET_CUSTOM_LABELS;
}

void UIHWindow::InitUI_activity_vision(const std::list<unit_calc_el> *unclel,std::list<cpu_chain_el> &cpu_units_monit_chain) 
{

          std::string line("");
          std::istringstream instrproc(uhiutil::execmd("cat /proc/cpuinfo | egrep 'processor|physical id|core id'"));
          cpu_chain_el cpu_unit;

          for(std::list<unit_calc_el>::const_iterator ucEl = unclel->begin() ; ucEl != unclel->end() ; ucEl++)          
          {
	           cpu_unit.cpuid = "cpu" + std::to_string(ucEl->cpunit) +  "  :  ";
               cpu_unit.cpuidn = ucEl->cpunit;
               std::getline(instrproc, line);
               if(uhiutil::newline(line,"processor\t: ",Direction::RIGHTSKIP)) {
                   cpu_unit.processor = std::atoi(line.c_str());
                   for(unsigned int it = 0 ; it < 2 ; it++) {
                         std::getline(instrproc, line);
                         if(uhiutil::newline(line,"core id\t\t: ",Direction::RIGHTSKIP)) {
                                  cpu_unit.cpuid += ("coreid:" + line + (it ? "" : " "));
                                  cpu_unit.coreid = std::atoi(line.c_str());
                         }
                         else
                            if(uhiutil::newline(line,"physical id\t: ",Direction::RIGHTSKIP)) {
                                        cpu_unit.cpuid += ("physid:" + line + (it ? "" : " "));
                                        cpu_unit.physid = std::atoi(line.c_str());
                            }
                   }
               }
      
               Gtk::Box *pBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
               Gtk::Box *pLabelBox = Gtk::manage(new Gtk::Box);
               Gtk::Box *pLabelBoxCU = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
               Gtk::Box *pDAreaBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
               Gtk::Label *pLabel = Gtk::manage(new Gtk::Label);
               Gtk::Label *lCcpuColor = Gtk::manage(new Gtk::Label);
               Gtk::Label *lUsageColor = Gtk::manage(new Gtk::Label);
               Gtk::Label *lSpace = Gtk::manage(new Gtk::Label);
               cpu_unit.pDArea = Gtk::manage(new CDrawArea(nullptr,nullptr,Dm::CPUDRAW, ucEl->GetStor(uhiutil::calc::bfr), ucEl->GetStor(uhiutil::calc::cfr), ucEl->GetStor(uhiutil::calc::usg)));
               cpu_unit.cpuid_m_pbF = Gtk::manage(new Gtk::ProgressBar);
               cpu_unit.cpuid_m_pbCF = Gtk::manage(new Gtk::ProgressBar);
               cpu_unit.cpuid_m_pbU = Gtk::manage(new Gtk::ProgressBar);
               cpu_unit.lCompareColor = Gtk::manage(new Gtk::Label);
               cpu_unit.lFirstSpace = Gtk::manage(new Gtk::Label);

               pLabel->set_text(cpu_unit.cpuid);
               pLabel->set_justify(Gtk::JUSTIFY_CENTER);
               lCcpuColor->set_text(" freq. ");
               cpu_unit.lCompareColor->set_text("compr"); 
               lUsageColor->set_text("usage");
               lSpace->set_text(" ");
               cpu_unit.lFirstSpace->set_text(" ");

               pDAreaBox->pack_start(*cpu_unit.pDArea);
               pLabelBoxCU->pack_start(*lCcpuColor, Gtk::PACK_SHRINK);
               pLabelBoxCU->pack_start(*cpu_unit.lFirstSpace, Gtk::PACK_SHRINK);
               pLabelBoxCU->pack_start(*cpu_unit.lCompareColor, Gtk::PACK_SHRINK);
               pLabelBoxCU->pack_start(*lSpace, Gtk::PACK_SHRINK);
               pLabelBoxCU->pack_start(*lUsageColor, Gtk::PACK_SHRINK);
               pLabelBox->pack_start(*pLabel, Gtk::PACK_SHRINK);
               pBox->pack_start(*pLabelBox, Gtk::PACK_SHRINK);
               pBox->pack_start(*cpu_unit.cpuid_m_pbF, Gtk::PACK_SHRINK);
               pBox->pack_start(*cpu_unit.cpuid_m_pbCF, Gtk::PACK_SHRINK);
               pBox->pack_start(*cpu_unit.cpuid_m_pbU, Gtk::PACK_SHRINK);
               pBox->pack_start(*pDAreaBox);
               pBox->pack_start(*pLabelBoxCU, Gtk::PACK_SHRINK);

               pLabelBoxCU->set_border_width(8);
               pDAreaBox->set_border_width(15);

               pBox->override_background_color(Gdk::RGBA(uhiutil::ui::cpunits_bckcolor));
               pLabel->override_color(Gdk::RGBA(uhiutil::ui::cpunits_color));
               cpu_unit.cpuid_m_pbF->override_color(Gdk::RGBA(uhiutil::ui::cpunits_color));
               cpu_unit.cpuid_m_pbCF->override_color(Gdk::RGBA(uhiutil::ui::stat_color_cmpr));
               cpu_unit.cpuid_m_pbU->override_color(Gdk::RGBA(uhiutil::ui::cpunits_color));
               lCcpuColor->override_background_color(Gdk::RGBA(uhiutil::ui::stat_color_blue));
               lCcpuColor->override_color(Gdk::RGBA(uhiutil::ui::cpunits_color));
               cpu_unit.lCompareColor->override_background_color(Gdk::RGBA(uhiutil::ui::stat_color_cmpr));
               cpu_unit.lCompareColor->override_color(Gdk::RGBA(uhiutil::ui::cpunits_color));
               lUsageColor->override_background_color(Gdk::RGBA(uhiutil::ui::stat_color_green));
               lUsageColor->override_color(Gdk::RGBA(uhiutil::ui::cpunits_color));

               pLabelBox->set_halign(Gtk::ALIGN_CENTER);
               pLabelBox->set_valign(Gtk::ALIGN_CENTER);
               pLabelBox->set_border_width(8);

               pLabelBoxCU->set_halign(Gtk::ALIGN_CENTER);
               pLabelBoxCU->set_valign(Gtk::ALIGN_CENTER);

               cpu_unit.cpuid_m_pbF->set_halign(Gtk::ALIGN_CENTER);
               cpu_unit.cpuid_m_pbF->set_valign(Gtk::ALIGN_CENTER);
               cpu_unit.cpuid_m_pbF->set_text("Frequency");
               cpu_unit.cpuid_m_pbF->set_show_text();

               cpu_unit.cpuid_m_pbCF->set_halign(Gtk::ALIGN_CENTER);
               cpu_unit.cpuid_m_pbCF->set_valign(Gtk::ALIGN_CENTER);
               cpu_unit.cpuid_m_pbCF->set_text("Compr.Freq.");
               cpu_unit.cpuid_m_pbCF->set_show_text();

               cpu_unit.cpuid_m_pbU->set_halign(Gtk::ALIGN_CENTER);
               cpu_unit.cpuid_m_pbU->set_valign(Gtk::ALIGN_CENTER);
               cpu_unit.cpuid_m_pbU->set_text("Usage");
               cpu_unit.cpuid_m_pbU->set_show_text();

               m_Fbox_CPUActivityAll.add(*pBox);
               cpu_units_monit_chain.push_back(cpu_unit);
          }

          CDrawArea::l_CPUModeSwitch = &m_CPUModeSwitch;
          CDrawArea::l_CPUCompareSwitch = &m_CPUCompareSwitch;

          m_Fbox_CPUActivityAll.show_all_children();
}
