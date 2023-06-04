/*
 *    uhInfo
 *    Copyright (C) 2020
 */
 
#include "mwndui.h"
#include "procun.h"
#include <glibmm/i18n.h>

post_init_sig UIHWindow::sig_postinit_param;

UIHWindow::UIHWindow() : m_ButtCPUOverall(_("Sum") + std::string("\n") + _("Dlg")), m_ScrolledWindow(), m_ScrolledWindowTreeView(), m_ScrolledWindowCPUActivityAll(),
                         m_status_bar(_("  Everything at a glance...")), m_TextView(), m_TreeView(m_refTreeModel),
						 m_temperatureTreeView(ptRefTreeModel), m_Gpus(), m_DAtemperature(this,&UIHWindow::on_DA_button_press_event)
{
   m_CPUModeGrid.attach(m_CPUModeLabel, 0, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUModeSwitch, 1, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUNativeFqLabel, 4, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUNativeFqSwitch, 5, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUCompareLabel, 2, 1, 1, 1);
   m_CPUModeGrid.attach(m_CPUCompareSwitch, 3, 1, 1, 1);

  m_BlinkGrid.attach(m_BlinkLabel, 0, 1, 1, 1);
  m_BlinkGrid.attach(m_BlinkSwitch, 1, 1, 1, 1);
  m_BlinkGrid.attach(m_MoreInfoLabel, 2, 1, 1, 1);
  m_BlinkGrid.attach(m_MoreInfoSwitch, 3, 1, 1, 1);

  m_Box_TmpControls.append(m_ScrolledWindowTemperatures);

  m_VPanedTrmpetature.set_start_child(m_Box_TmpControls);

  mT_All.append(m_TbFrame);
  mT_All.append(m_DAFrame_Temperature);
  m_VPanedTrmpetature.set_end_child(mT_All);

  m_temperatureTreeView.append_column_editable("+/-", tColumns->col_tcheck);
  m_temperatureTreeView.append_column(_("Color"), tColumns->color);
  m_temperatureTreeView.append_column(_("Node"), tColumns->tsensor_node);
  m_temperatureTreeView.append_column(_("Sensor"), tColumns->tsensor_name);

  m_TreeView.append_column("No", dColumns->No);
  m_TreeView.append_column(_("Link"), dColumns->sys_link);
  m_TreeView.append_column(_("Name"), dColumns->device_name);
  m_TreeView.append_column(_("Type"), dColumns->type);
  m_TreeView.append_column(_("Specificity"), dColumns->specificity);

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

  m_Box_CPUActivityAll.append(m_Fbox_CPUActivityAll);
  m_Frame_CPUActivityAll.set_child(m_Box_CPUActivityAll);

  m_DAFrame_Temperature.set_child(m_DAtemperature);
  m_TbFrame.set_child(mDA_ToolBar);

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
  Gtk::Image *bqImg = Gtk::make_managed<Gtk::Image>();
  Gtk::Label *bqLabel = Gtk::make_managed<Gtk::Label>(_("Quit"));
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

  mFr_GRrid.set_child(m_CPUModeGrid);
  m_Box_GRridAndButt.append(mFr_GRrid);
  m_Box_GRridAndButt.append(m_ButtCPUOverall);

  m_VBoxCPUActivityAll.append(m_Box_GRridAndButt);
  m_VBoxCPUActivityAll.append(m_ScrolledWindowCPUActivityAll);
 
  hbox_operation_status_sensors.append(operation_status_image_sensors);
  hbox_operation_status_sensors.append(m_Label_Sensors);
  hbox_operation_status_cpu.append(operation_status_image_cpu);
  hbox_operation_status_pcie.append(operation_status_image_pcie);
  
  signal_show().connect(sigc::mem_fun(*this, &UIHWindow::Wnd_show_handler));
  signal_close_request().connect(sigc::mem_fun(*this, &UIHWindow::Wnd_close_handler),false);
  m_ButtonQuit.signal_clicked().connect(sigc::mem_fun(*this, &UIHWindow::on_quit_button_clicked));
  m_ButtCPUOverall.signal_clicked().connect(sigc::mem_fun(*this, &UIHWindow::on_overall_button_clicked));

  m_Gpus.signal_changed().connect(sigc::mem_fun(*this,&UIHWindow::on_gpus_selection_changed));
  m_CPUModeSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_CPUActivityAll_switch_changed));
  m_CPUCompareSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_Compare_mode_switch_changed));
  m_MoreInfoSwitch.property_active().signal_changed().connect([=](){uhiutil::draw::more_info = m_MoreInfoSwitch.get_active();});
  m_CPUNativeFqSwitch.property_active().signal_changed().connect(sigc::mem_fun(*this, &UIHWindow::On_NativeFq_changed));

   add_action("about", sigc::mem_fun(*this,&UIHWindow::about_dialog_info));

   InitToolBar(); // ToolBar functionality
}
 
void UIHWindow::InitUI()
{
  m_Frame_MOBO.set_label(_("Mainboard :"));
  m_Frame_Memory.set_label(_("Memory :"));
  m_Frame_CPU.set_label(_("CPU :"));
  m_Frame_VGA.set_label(_("GPU :"));
  m_Frame_Monitors.set_label(_("Monitor(s) :"));
  m_Frame_Audio.set_label(_("Audio :"));
  m_Frame_Network.set_label(_("Network :"));
  m_Frame_Disks.set_label(_("Disks :"));
  m_Frame_OS.set_label(_("OS :"));

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
  mT_All.set_margin(2);

  m_VBoxCPUActivityAll.set_margin(4);
  m_CPUModeGrid.set_margin(4);
  m_ScrolledWindowCPUActivityAll.set_margin(4);

  m_Label_MOBO.set_text(_("Vendor :") + std::string("\n") + _("Mainboard :") + std::string("\n") + _("BIOS ver. : "));
  m_Label_Memory.set_text(_("Memory size :") + std::string("\n") + _("Available :"));
  m_Label_CPU.set_text(_("CPU name:") + std::string("\n") + _("CPU cores/threads:") + std::string("\n") + _("CPU min/max:"));
  m_Label_VGA.set_text(_("GPU :") + std::string("\n") + _("Resolution :"));
  m_Label_VGA_cond.set_text(_("PCIE link current : "));
  m_Label_VGA_cond_status.set_text("- / -");
  m_Label_Audio.set_text(_("Audio device:"));
  m_Label_Network.set_text(_("Ethernet controller:"));
  m_Label_OS.set_text(_("OS :") + std::string("\n") + _("Kernel :") + std::string("\n") + _("Display Server :") + std::string("\n") + _("Compiler :"));

  m_CPUCompareLabel.set_text(_("Compare "));
  m_CPUNativeFqLabel.set_text(_("Relative/Native "));
  m_CPUModeLabel.set_text(_("Fq. source "));
  m_BlinkLabel.set_text(_("Indicators blinking    "));
  m_MoreInfoLabel.set_text(_("  More Info    "));
  m_Label_Sensors.set_text(_(" Sensors :"));
  m_VBoxCPUActivityAll.set_orientation(Gtk::Orientation::VERTICAL);

  m_sb_labeltext.set_text(_("sensors status : "));
  m_sb_cpu_labeltext.set_text(_("CPU freq. : "));
  m_sb_cpu_status.set_text(_("N/A     "));
  m_sb_status.set_text(_("OFF     "));

  mT_All.set_orientation(Gtk::Orientation::VERTICAL);
  m_HPaned.set_orientation(Gtk::Orientation::HORIZONTAL);
  m_VBoxVRight.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxVLeft.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxAll.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxCPU.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxCPU_Freq_Use.set_orientation(Gtk::Orientation::VERTICAL);
  m_VBoxVGA.set_orientation(Gtk::Orientation::VERTICAL);
  m_Box_CPUActivityAll.set_orientation(Gtk::Orientation::VERTICAL);
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
  m_TextView.set_can_target(false);

  mDA_ToolBar.set_halign(Gtk::Align::END);
  mDA_ToolBar.set_valign(Gtk::Align::START);
  mDA_ToolBar.set_visible(false);

  m_status_bar.set_halign(Gtk::Align::START);
  m_sb_cpu_labeltext.set_halign(Gtk::Align::START);
  m_sb_cpu_status.set_halign(Gtk::Align::START);
  m_sb_labeltext.set_halign(Gtk::Align::START);
  m_sb_status.set_halign(Gtk::Align::START);

  m_Box_GRridAndButt.set_halign(Gtk::Align::CENTER);
  m_ButtCPUOverall.set_margin(6);
  m_ButtCPUOverall.set_margin_start(12);

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
  m_MoreInfoLabel.set_halign(Gtk::Align::START);
  m_BlinkSwitch.set_halign(Gtk::Align::END);
  m_MoreInfoSwitch.set_halign(Gtk::Align::END);
  
  STATUSIMAGES_SET_INACTIVE;
  SET_CUSTOM_LABELS;
}

void UIHWindow::InitUI_activity_vision(const std::list<unit_calc_el> *unclel,std::list<cpu_chain_el> &cpu_units_monit_chain)
{
	      LOCALCSSPROVWITHSTYLE;

          std::string line("");
          std::istringstream instrproc(uhiutil::execmd("cat /proc/cpuinfo | grep -E 'processor|physical id|core id'"));
          cpu_chain_el cpu_unit;

          for(std::list<unit_calc_el>::const_iterator ucEl = unclel->begin(); ucEl != unclel->end(); ucEl++)
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

               Gtk::Box *pBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
			   Gtk::Frame *pUnitFrame = Gtk::make_managed<Gtk::Frame>();
			   Gtk::Frame *pDArFrame = Gtk::make_managed<Gtk::Frame>();
			   Gtk::Box *pLabelBoxCU = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
			   Gtk::Label *lCcpuColor = Gtk::make_managed<Gtk::Label>();
			   Gtk::Label *lUsageColor = Gtk::make_managed<Gtk::Label>();

               cpu_unit.pDArea = Gtk::make_managed<CDrArCpu>(ucEl->GetStor(uhiutil::calc::bfr), ucEl->GetStor(uhiutil::calc::cfr), ucEl->GetStor(uhiutil::calc::usg),this);

               cpu_unit.cpuid_m_pbF = Gtk::make_managed<Gtk::ProgressBar>();
               cpu_unit.cpuid_m_pbCF = Gtk::make_managed<Gtk::ProgressBar>();
               cpu_unit.cpuid_m_pbU = Gtk::make_managed<Gtk::ProgressBar>();
               cpu_unit.lCompareColor = Gtk::make_managed<Gtk::Label>();

               lCcpuColor->set_text(_("  freq  "));
               cpu_unit.lCompareColor->set_text(_(" compr "));
               lUsageColor->set_text(_(" usage "));

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

               uhiutil::set_css_style(pUnitFrame->get_style_context(),lprv,"ls_cls");
               uhiutil::set_css_style(cpu_unit.cpuid_m_pbF->get_style_context(),lprv,"ls_cls");
               uhiutil::set_css_style(cpu_unit.cpuid_m_pbCF->get_style_context(),lprv,"ls_cls");
               uhiutil::set_css_style(cpu_unit.cpuid_m_pbU->get_style_context(),lprv,"ls_cls");
               uhiutil::set_css_style(lCcpuColor->get_style_context(),lprv,"bl_cls");
               uhiutil::set_css_style(lUsageColor->get_style_context(),lprv,"gn_cls");
               uhiutil::set_css_style(cpu_unit.lCompareColor->get_style_context(),lprv,"yw_cls");

               pLabelBoxCU->set_halign(Gtk::Align::CENTER);
               pLabelBoxCU->set_valign(Gtk::Align::CENTER);

               cpu_unit.cpuid_m_pbF->set_halign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbF->set_valign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbF->set_text(_("Frequency"));
               cpu_unit.cpuid_m_pbF->set_show_text();

               cpu_unit.cpuid_m_pbCF->set_halign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbCF->set_valign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbCF->set_text(_("Compr.Freq."));
               cpu_unit.cpuid_m_pbCF->set_show_text();

               cpu_unit.cpuid_m_pbU->set_halign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbU->set_valign(Gtk::Align::CENTER);
               cpu_unit.cpuid_m_pbU->set_text(_("Usage"));
               cpu_unit.cpuid_m_pbU->set_show_text();

               m_Fbox_CPUActivityAll.insert(*pUnitFrame, -1);
               cpu_units_monit_chain.push_back(cpu_unit);
          }

          Gtk::Frame *ptFrame = Gtk::make_managed<Gtk::Frame>();
          ptFrame->set_child(m_ToolbarChoice);
          mDA_ToolBar.append(*Gtk::make_managed<Gtk::Label>(_("Utility :")));
          mDA_ToolBar.append(*ptFrame);
          ptFrame->set_margin_start(12);
          ptFrame->set_margin_end(4);

          uhiutil::set_css_style(mDA_ToolBar.get_style_context(),lprv,"toolbar");
          uhiutil::set_css_style(ptFrame->get_style_context(),lprv,"tbext_cls");
          uhiutil::set_css_style(m_TbFrame.get_style_context(),lprv,"ls_cls");
          uhiutil::set_css_style(m_DAFrame_Temperature.get_style_context(),lprv,"ls_cls");
          uhiutil::set_css_style(m_pbUse.get_style_context(),lprv,"fu_cls");
          uhiutil::set_css_style(m_pbFreq.get_style_context(),lprv,"fu_cls");
          uhiutil::set_css_style(m_Box_CPUActivityAll.get_style_context(),lprv,"toolbar");
          uhiutil::set_css_style(m_ButtCPUOverall.get_style_context(),lprv,"toolbar");
          uhiutil::set_css_style(mFr_GRrid.get_style_context(),lprv,"toolbar");

          CDrArCpu::l_CPUModeSwitch = &m_CPUModeSwitch;
          CDrArCpu::l_CPUCompareSwitch = &m_CPUCompareSwitch;
}

void UIHWindow::InitToolBar()
{
	  m_refToolBarChoice->set_button(GDK_BUTTON_PRIMARY);
	  m_refToolBarChoice->signal_pressed().connect([&](int , double x, double y){m_ToolBarMenuPopup.set_pointing_to(Gdk::Rectangle(x,y,1,1));m_ToolBarMenuPopup.popup();});
	  m_ToolbarChoice.add_controller(m_refToolBarChoice);

	  Glib::RefPtr<Gio::SimpleActionGroup> refTBAcGr = Gio::SimpleActionGroup::create();
	  refTBAcGr->add_action("show",sigc::bind(sigc::mem_fun(*this,&UIHWindow::on_tbt_clicked),true));
	  refTBAcGr->add_action("hide",sigc::bind(sigc::mem_fun(*this,&UIHWindow::on_tbt_clicked),false));
	  insert_action_group("tbchoice",refTBAcGr);

	  Glib::RefPtr<Gio::Menu> tbmenu = Gio::Menu::create();
	  tbmenu->append_item(Gio::MenuItem::create(_("Show CPU load"),"tbchoice.show"));
	  tbmenu->append_item(Gio::MenuItem::create(_("Hide toolbar"),"tbchoice.hide"));

	  m_ToolBarMenuPopup.set_parent(m_ToolbarChoice);
	  m_ToolBarMenuPopup.set_menu_model(tbmenu);
}

void UIHWindow::StatusbarCpuText()
{
	m_sb_cpu_status.set_text(m_CPUModeSwitch.get_active() ? "scaling_cur_freq      " :
			                      (LSCPUSE ? "lscpu      " : "cpuinfo      " ));
}
