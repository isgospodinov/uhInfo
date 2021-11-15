/*
 *    uhInfo
 *    Copyright (C) 2020
 */
 
#include "mwndui.h"
#include "procun.h"

UIHWindow::UIHWindow() : m_ScrolledWindow(), m_ScrolledWindowTreeView(), m_ScrolledWindowCPUActivityAll(),
                         m_TextView(), m_TreeView(m_refTreeModel), m_temperatureTreeView(ptRefTreeModel), m_Gpus(),
						 m_DAtemperature(this,&UIHWindow::on_DA_button_press_event)
{
   m_CPUModeGrid.attach(m_CPUModeLabel, 0, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUModeSwitch, 1, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUNativeFqLabel, 2, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUNativeFqSwitch, 3, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUCompareLabel, 1, 2, 1, 1);
   m_CPUModeGrid.attach(m_CPUCompareSwitch, 2, 2, 1, 1);

  m_BlinkGrid.attach(m_BlinkLabel, 0, 1, 1, 1);
  m_BlinkGrid.attach(m_BlinkSwitch, 1, 1, 1, 1);

  m_Box_TmpControls.append(m_ScrolledWindowTemperatures);

  m_VPanedTrmpetature.set_start_child(m_Box_TmpControls);
  m_VPanedTrmpetature.set_end_child(m_DAFrame_Temperature);

  m_temperatureTreeView.append_column_editable("+/-", tColumns->col_tcheck);
  m_temperatureTreeView.append_column("Color", tColumns->color);
  m_temperatureTreeView.append_column("Node", tColumns->tsensor_node);
  m_temperatureTreeView.append_column("Sensor", tColumns->tsensor_name);

  m_TreeView.append_column("No", dColumns->No);
  m_TreeView.append_column("Link", dColumns->sys_link);
  m_TreeView.append_column("Name", dColumns->device_name);
  m_TreeView.append_column("Type", dColumns->type);
  m_TreeView.append_column("Specificity", dColumns->specificity);

   m_VBoxVRight.append(m_Frame_MOBO);
   m_VBoxVRight.append(m_Frame_Memory);
   m_VBoxVRight.append(m_Frame_CPU);
   m_VBoxVRight.append(m_Frame_VGA);
   m_VBoxVRight.append(m_Frame_Monitors);
   m_VBoxVRight.append(m_Frame_Audio);
   m_VBoxVRight.append(m_Frame_Network);
   m_VBoxVRight.append(m_VPanedTrmpetature);
   m_VBoxVRight.append(m_Frame_Disks);

   m_VBoxVLeft.append(m_Frame_OS);
   m_VBoxVLeft.append(m_BlinkGrid);
   m_VBoxVLeft.append(m_Frame_Sensors);

  m_Frame_MOBO.set_child(m_Label_MOBO);
  m_Frame_Memory.set_child(m_Label_Memory);
  m_Frame_CPU.set_child(m_VBoxCPU);
  m_Frame_VGA.set_child(m_VBoxVGA);
  m_Frame_VGA_pcie.set_child(m_gridVGA_cond);
  m_Frame_Monitors.set_child(m_Label_Monitors);
  m_Frame_Audio.set_child(m_VBox_Audio);
  m_Frame_Network.set_child(m_VBox_Network);
  m_Frame_Disks.set_child(m_ScrolledWindowTreeView);

  m_Frame_Sensors.set_child(m_ScrolledWindow);

  m_Frame_CPUActivityAll.set_child(m_Fbox_CPUActivityAll);

  m_DAFrame_Temperature.set_child(m_DAtemperature);

  m_VBoxCPU.append(m_Label_CPU);
  m_VBoxCPU.append(m_Frame_CPUFrecq);

  m_Frame_CPUFrecq.set_child(m_VBoxCPU_Freq_Use);

  m_VBoxCPU_Freq_Use.append(m_pbFreq);
  m_VBoxCPU_Freq_Use.append(m_pbUse);

  m_VBoxVGA.append(m_Gpus);
  m_VBoxVGA.append(m_Label_VGA);
  m_VBoxVGA.append(m_Frame_VGA_pcie);

  m_gridVGA_cond.set_column_homogeneous(true);
  m_gridVGA_cond.attach(m_Label_VGA_cond ,0 ,0 ,1 ,1);
  m_gridVGA_cond.attach_next_to(m_Label_VGA_cond_status,m_Label_VGA_cond,Gtk::PositionType::RIGHT,1,1);

  m_VBox_Audio.append(m_Label_Audio);

  m_VBox_Network.append(m_Label_Network);

  m_ScrolledWindowTreeView.set_child(m_TreeView);
  m_ScrolledWindowTreeView.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

  m_ScrolledWindow.set_child(m_TextView);

  m_ScrolledWindowCPUActivityAll.set_child(m_Frame_CPUActivityAll);

  m_ScrolledWindowTemperatures.set_child(m_temperatureTreeView);

  m_Fbox_CPUActivityAll.set_max_children_per_line(5);//set default
  m_Fbox_CPUActivityAll.set_selection_mode(Gtk::SelectionMode::NONE);
  m_Fbox_CPUActivityAll.set_homogeneous();

  m_ScrolledWindowCPUActivityAll.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  m_ScrolledWindowTemperatures.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

  m_ButtonBox.append(m_sb_cpu_labeltext);
  m_ButtonBox.append(m_sb_cpu_status);
  m_ButtonBox.append(m_sb_labeltext);
  m_ButtonBox.append(m_sb_status);
  m_ButtonBox.append(m_ButtonQuit);

  Gtk::Box *pBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
  Gtk::Image *bqImg = Gtk::manage(new Gtk::Image);
  Gtk::Label *bqLabel = Gtk::make_managed<Gtk::Label>("Quit");
  bqImg->set_from_icon_name("application-exit");
  bqLabel->set_hexpand();
  bqLabel->set_margin_start(2);
  pBox->append(*bqImg);
  pBox->append(*bqLabel);
  m_ButtonQuit.set_child(*pBox);

  m_StatusBar_Grid_condit.set_column_homogeneous(true);
  m_StatusBar_Grid_condit.attach(m_ButtonBox ,0 ,0 ,1 ,1);
  m_StatusBar_Grid_condit.attach_next_to(m_status_bar,m_ButtonBox,Gtk::PositionType::LEFT,1,1);
  m_ButtonQuit.set_margin(4);

  m_ButtonBox.set_orientation(Gtk::Orientation::HORIZONTAL);
  m_ButtonBox.set_halign(Gtk::Align::END);

  m_Frame_OS.set_child(m_Label_OS);

  m_HPaned.set_start_child(m_VBoxVRight);
  m_HPaned.set_end_child(m_VBoxVLeft);

  m_Revealer.append(m_HPaned);

  m_VBoxAll.append(m_Revealer);
  m_VBoxAll.append(m_VBoxCPUActivityAll);
  m_VBoxAll.append(m_separator);
  m_VBoxAll.append(m_StatusBar_Grid_condit);

  m_VBoxCPUActivityAll.append(m_CPUModeGrid);
  m_VBoxCPUActivityAll.append(m_ScrolledWindowCPUActivityAll);
 
  hbox_operation_status_sensors.append(operation_status_image_sensors);
  hbox_operation_status_sensors.append(m_Label_Sensors);
  hbox_operation_status_cpu.append(operation_status_image_cpu);
  hbox_operation_status_pcie.append(operation_status_image_pcie);
  
  signal_show().connect(sigc::mem_fun(*this, &UIHWindow::Wnd_show_handler));
  signal_close_request().connect(sigc::mem_fun(*this, &UIHWindow::Wnd_close_handler),false);
  m_ButtonQuit.signal_clicked().connect(sigc::mem_fun(*this, &UIHWindow::on_quit_button_clicked));

  m_Gpus.signal_changed().connect(sigc::mem_fun(*this,&UIHWindow::on_gpus_selection_changed));
  m_CPUModeSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_CPUActivityAll_switch_changed));
  m_CPUCompareSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_Compare_mode_switch_changed));
  m_CPUNativeFqSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_NativeFq_changed));

   item_infomode = add_action_bool("extinfo",sigc::mem_fun(*this,&UIHWindow::enhanced_system_info));
   item_cpu = add_action_bool("cpuactivity",sigc::mem_fun(*this,&UIHWindow::show_cpu_activity_all));
   item_temperature = add_action_bool("tmpmonit",sigc::mem_fun(*this,&UIHWindow::monitor_temperature), false);
   item_manage = add_action("sensorset",sigc::mem_fun(*this,&UIHWindow::manage_sensors));
   item_options = add_action("prefs",sigc::mem_fun(*this,&UIHWindow::get_preferences));
   add_action("about", sigc::mem_fun(*this,&UIHWindow::about_dialog_info));
}
 
void UIHWindow::InitUI()
{
  m_Frame_MOBO.set_label("Mainboard :");
  m_Frame_Memory.set_label("Memory :");
  m_Frame_CPU.set_label("CPU :");
  m_Frame_VGA.set_label("GPU :");
  m_Frame_Monitors.set_label("Monitor(s) :");
  m_Frame_Audio.set_label("Audio :");
  m_Frame_Network.set_label("Network :");
  m_Frame_Disks.set_label("Disks :");
  m_Frame_OS.set_label("OS :");

  m_Frame_MOBO.set_margin(4);
  m_Frame_Memory.set_margin(4);
  m_Frame_CPU.set_margin(4);
  m_Frame_VGA.set_margin(4);
  m_Frame_Monitors.set_margin(4);
  m_Frame_Audio.set_margin(4);
  m_Frame_Network.set_margin(4);
  m_Frame_Disks.set_margin(4);
  m_Frame_OS.set_margin(4);
  m_Frame_Sensors.set_margin(4);
  m_Frame_VGA_pcie.set_margin_start(14);
  m_Frame_VGA_pcie.set_margin_end(14);
  m_Frame_VGA_pcie.set_margin_bottom(2);
  m_VBoxCPU_Freq_Use.set_margin(2);
  m_VBoxCPU_Freq_Use.set_margin_bottom(4);
  m_Frame_CPUFrecq.set_margin_start(14);
  m_Frame_CPUFrecq.set_margin_end(14);
  m_Frame_CPUFrecq.set_margin_bottom(2);
  m_ScrolledWindow.set_margin_top(4);
  m_DAFrame_Temperature.set_margin(2);

  m_VBoxCPUActivityAll.set_margin(4);
  m_CPUModeGrid.set_margin(4);
  m_ScrolledWindowCPUActivityAll.set_margin(4);

  m_Label_MOBO.set_text("Vendor :\nMainboard :\nBIOS ver. :");
  m_Label_Memory.set_text("Memory size :\nAvailable :");
  m_Label_CPU.set_text("CPU name:\nCPU cores/threads:\nCPU min/max:");
  m_Label_VGA.set_text("GPU :\nResolution :");
  m_Label_VGA_cond.set_text("PCIE link current : ");
  m_Label_VGA_cond_status.set_text("- / -");
  m_Label_Audio.set_text("Audio device:");
  m_Label_Network.set_text("Ethernet controller:");
  m_Label_OS.set_text("OS :\nKernel :\nDisplay Server :\nCompiler :");

  m_CPUCompareLabel.set_text("Compare ");
  m_CPUNativeFqLabel.set_text("Relative/Native ");
  m_CPUModeLabel.set_text("Fq. source ");
  m_BlinkLabel.set_text("Indicators blinking    ");
  m_Label_Sensors.set_text(" Sensors :");
  m_VBoxCPUActivityAll.set_orientation(Gtk::Orientation::VERTICAL);

  m_sb_labeltext.set_text("sensors status : ");
  m_sb_cpu_labeltext.set_text("CPU freq. : ");
  m_sb_cpu_status.set_text("N/A     ");
  m_sb_status.set_text("OFF     ");

  m_HPaned.set_orientation(Gtk::Orientation::HORIZONTAL);
  m_VBoxVRight.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxVLeft.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxAll.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxCPU.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxCPU_Freq_Use.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxVGA.set_orientation(Gtk::Orientation::VERTICAL);
  m_separator.set_orientation(Gtk::Orientation::HORIZONTAL);
  hbox_operation_status_cpu.set_orientation(Gtk::Orientation::HORIZONTAL);
  hbox_operation_status_pcie.set_orientation(Gtk::Orientation::HORIZONTAL);
  hbox_operation_status_sensors.set_orientation(Gtk::Orientation::HORIZONTAL);
  m_VPanedTrmpetature.set_orientation(Gtk::Orientation::VERTICAL);

  m_VBoxAll.set_expand();
  m_Revealer.set_expand();
  m_HPaned.set_expand();
  m_VPanedTrmpetature.set_expand();
  m_TextView.set_expand();
  m_TreeView.set_expand();
  m_ScrolledWindowCPUActivityAll.set_expand();
  m_ScrolledWindowTemperatures.set_expand();

  m_pbFreq.set_halign(Gtk::Align::CENTER);
  m_pbFreq.set_valign(Gtk::Align::CENTER);
  m_pbFreq.set_text("Frequency");
  m_pbFreq.set_show_text();
  m_pbUse.set_halign(Gtk::Align::CENTER);
  m_pbUse.set_valign(Gtk::Align::CENTER);
  m_pbUse.set_text("Usage");
  m_pbUse.set_show_text();

  m_Label_VGA_cond.set_halign(Gtk::Align::END);
  m_Label_VGA_cond_status.set_halign(Gtk::Align::START);

  m_BlinkGrid.set_halign(Gtk::Align::CENTER);

  m_TextView.set_editable(false);

  m_sb_cpu_labeltext.set_halign(Gtk::Align::START);
  m_sb_cpu_status.set_halign(Gtk::Align::START);
  m_sb_labeltext.set_halign(Gtk::Align::START);
  m_sb_status.set_halign(Gtk::Align::START);

  m_CPUModeGrid.set_halign(Gtk::Align::CENTER);
  m_CPUModeGrid.set_column_spacing(4);
  m_CPUModeGrid.set_row_spacing(4);
  m_CPUModeGrid.set_column_homogeneous(false);
  m_CPUModeGrid.set_row_homogeneous(false);

  m_CPUCompareLabel.set_halign(Gtk::Align::END);
  m_CPUNativeFqLabel.set_halign(Gtk::Align::END);
  m_CPUModeLabel.set_halign(Gtk::Align::END);

  m_CPUModeSwitch.set_halign(Gtk::Align::START);
  m_CPUCompareSwitch.set_halign(Gtk::Align::START);
  m_CPUNativeFqSwitch.set_halign(Gtk::Align::START);
  m_BlinkLabel.set_halign(Gtk::Align::START);
  m_BlinkSwitch.set_halign(Gtk::Align::END);
  
  STATUSIMAGES_SET_INACTIVE;
  SET_CUSTOM_LABELS;
}

void UIHWindow::InitUI_activity_vision(const std::list<unit_calc_el> *unclel,std::list<cpu_chain_el> &cpu_units_monit_chain)
{
	      Glib::RefPtr<Gtk::CssProvider> prv = Gtk::CssProvider::create();
	      Glib::RefPtr<Gtk::StyleContext> sc{nullptr};
	      prv->load_from_data(style);

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
      
               Gtk::Box *pBox = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
               Gtk::Frame *pUnitFrame = Gtk::manage(new Gtk::Frame);
               Gtk::Frame *pDArFrame = Gtk::manage(new Gtk::Frame);
               Gtk::Box *pLabelBoxCU = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL));
               Gtk::Label *lCcpuColor = Gtk::manage(new Gtk::Label);
               Gtk::Label *lUsageColor = Gtk::manage(new Gtk::Label);
               cpu_unit.pDArea = Gtk::manage(new CDrawArea(nullptr,nullptr,Dm::CPUDRAW, ucEl->GetStor(uhiutil::calc::bfr), ucEl->GetStor(uhiutil::calc::cfr), ucEl->GetStor(uhiutil::calc::usg)));
               cpu_unit.cpuid_m_pbF = Gtk::manage(new Gtk::ProgressBar);
               cpu_unit.cpuid_m_pbCF = Gtk::manage(new Gtk::ProgressBar);
               cpu_unit.cpuid_m_pbU = Gtk::manage(new Gtk::ProgressBar);
               cpu_unit.lCompareColor = Gtk::manage(new Gtk::Label);

               lCcpuColor->set_text("  freq  ");
               cpu_unit.lCompareColor->set_text(" compr ");
               lUsageColor->set_text(" usage ");

               pLabelBoxCU->append(*lCcpuColor);
               pLabelBoxCU->append(*cpu_unit.lCompareColor);
               pLabelBoxCU->append(*lUsageColor);
               pBox->append(*cpu_unit.cpuid_m_pbF);
               pBox->append(*cpu_unit.cpuid_m_pbCF);
               pBox->append(*cpu_unit.cpuid_m_pbU);
               pDArFrame->set_child(*cpu_unit.pDArea);
               pBox->append(*pDArFrame);
               pBox->append(*pLabelBoxCU);
               pUnitFrame->set_child(*pBox);
               pUnitFrame->set_label(cpu_unit.cpuid);

               cpu_unit.cpuid_m_pbU->set_margin_bottom(8);
               pLabelBoxCU->set_margin_top(8);
               pLabelBoxCU->set_margin_bottom(8);
               pDArFrame->set_margin(2);
               pBox->set_margin(4);

               sc = pUnitFrame->get_style_context();
               sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
               sc->add_class("ls_cls");

               sc = cpu_unit.cpuid_m_pbF->get_style_context();
               sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
               sc->add_class("ls_cls");
               sc = cpu_unit.cpuid_m_pbCF->get_style_context();
               sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
               sc->add_class("ls_cls");
               sc = cpu_unit.cpuid_m_pbU->get_style_context();
               sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
               sc->add_class("ls_cls");

               sc = lCcpuColor->get_style_context();
               sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
               sc->add_class("bl_cls");

               sc = lUsageColor->get_style_context();
               sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
               sc->add_class("gn_cls");

               sc = cpu_unit.lCompareColor->get_style_context();
               sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
               sc->add_class("yw_cls");

               pLabelBoxCU->set_halign(Gtk::Align::CENTER);
               pLabelBoxCU->set_valign(Gtk::Align::CENTER);

               cpu_unit.cpuid_m_pbF->set_halign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbF->set_valign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbF->set_text("Frequency");
               cpu_unit.cpuid_m_pbF->set_show_text();

               cpu_unit.cpuid_m_pbCF->set_halign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbCF->set_valign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbCF->set_text("Compr.Freq.");
               cpu_unit.cpuid_m_pbCF->set_show_text();

               cpu_unit.cpuid_m_pbU->set_halign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbU->set_valign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbU->set_text("Usage");
               cpu_unit.cpuid_m_pbU->set_show_text();

               m_Fbox_CPUActivityAll.insert(*pUnitFrame, -1);
               cpu_units_monit_chain.push_back(cpu_unit);
          }

		  sc = m_DAFrame_Temperature.get_style_context();
		  sc->add_provider(prv, GTK_STYLE_PROVIDER_PRIORITY_USER);
		  sc->add_class("ls_cls");

          CDrawArea::l_CPUModeSwitch = &m_CPUModeSwitch;
          CDrawArea::l_CPUCompareSwitch = &m_CPUCompareSwitch;
}
