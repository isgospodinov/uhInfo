/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "mwnd.h"
using uhiutil::cpu::UhiDownCast;

CHWindow::CHWindow() : css_prov(Gtk::CssProvider::create()),pSysensors(new CSysens(m_DAtemperature.GetDAVcoreAccess())),pUd2Manager(new Ud2mon(this)),
		               pntProcessor(new CProcUnits),pGpus(new CGpus),pMonitor(new CMonitor),pfDlg(new CPrefsDlg(this,&css_prov)),
					   smDlg(new CSmDialog(this,*pSysensors,*pUd2Manager,&css_prov,&CHWindow::smDlgResponse)),abtDlg(new CAboutDlg(this,&css_prov))
{
  set_child(m_VBoxAll);

  uhiutil::set_css_style(get_style_context(),css_prov,nullptr,get_pango_context());

  Glib::RefPtr<Gtk::TextBuffer> tgBuff = m_TextView.get_buffer();
  tgBuff->create_tag(uhiutil::ui::active_tag)->property_foreground() = "red";
  tgBuff->create_tag(uhiutil::ui::max_tag)->property_weight() = Pango::Weight::BOLD;
  for(int index = 0; index < (int)SIZEOF(clrID); tgBuff->create_tag(clrID[index])->property_background() = clrID[index],index++);

  InitVision();

  tgBuff->set_text("Init application...");
  m_status_bar.push("Everything at a glance...", m_status_bar.get_context_id("Statusbar"));


  Gtk::CellRendererToggle* ptRenderer = UhiDownCast<Gtk::CellRenderer,Gtk::CellRendererToggle>((m_temperatureTreeView.get_column(0))->get_first_cell());
  if(ptRenderer)
      ptRenderer->signal_toggled().connect(sigc::mem_fun(*this,&CHWindow::OnTempToggled));

  Gtk::TreeViewColumn* ptColumn = m_temperatureTreeView.get_column(1);
  if(ptColumn) {
      Gtk::CellRenderer *ptRend = ptColumn->get_first_cell();
      if(ptRend) ptColumn->add_attribute(*ptRend,"cell-background", 2);
  }

  m_VBoxCPUActivityAll.set_visible(false);
  m_sb_cpu_labeltext.set_visible(false);
  m_sb_cpu_status.set_visible(false);
  m_BlinkGrid.set_visible(false);
  m_VPanedTrmpetature.set_visible(false);

  set_show_menubar(true);

  PrepAndMakeThread(this,&CHWindow::Posthreadnotify);
}

void CHWindow::on_DA_button_press_event(int npress, double x, double y)
{
	Glib::RefPtr<Gdk::Event> evbntpress = m_DAtemperature.msbntpress->get_last_event(m_DAtemperature.msbntpress->get_current_sequence());

	if(!evbntpress) return;
	else {
	   Gdk::Event::Type et = evbntpress->get_event_type();
	   if(!((et != Gdk::Event::Type::BUTTON_PRESS) && (et != Gdk::Event::Type::PAD_BUTTON_PRESS)) && npress != 2) return;
	}

	CDrawArea::TmpWndState state = CDrawArea::DAWndState::NORMAL;
	bool visiblity = true;

	if(m_DAtemperature.m_TmpWndCurrState == state) {
		state = CDrawArea::DAWndState::FULL;
		visiblity = false;
	}

	TEMPERATUREWNDVIEW(visiblity);
	m_DAtemperature.m_TmpWndCurrState = state;
	set_title((visiblity ? "uhInfo - Temperature monitor" : "uhInfo - Selected temperatures"));
}

void CHWindow::InitVision()
{
  InitUI();

  set_title("uhInfo - Summary");

  int width = 0, height = 0,posit = 0;
  std::string cmd("");
  std::string path = "/home/" + uhiutil::GetUserName() + "/.uhInfo/uhInfo.cfg";
  if(uhiutil::ExistenceVerification(path.c_str())) {
       cmd = "cat " + path + " | grep uhiwidth=";
       cmd = uhiutil::execmd(cmd.c_str());
       if(cmd != "") {
           if(uhiutil::newline(cmd,"uhiwidth=",Direction::RIGHTSKIP)) width = std::stoi(cmd); 
       }
       cmd = "cat " + path + " | grep uhiheight=";
       cmd = uhiutil::execmd(cmd.c_str());
       if(cmd != "") {
           if(uhiutil::newline(cmd,"uhiheight=",Direction::RIGHTSKIP)) height = std::stoi(cmd); 
       }
       
       cmd = "cat " + path + " | grep uhiBlink=";
       cmd = uhiutil::execmd(cmd.c_str());
       if(cmd != "") uhiutil::newline(cmd,"uhiBlink=",Direction::RIGHTSKIP);
       m_BlinkSwitch.set_active((cmd != "" ? std::stoi(cmd) == 1 : true));
       
       cmd = "cat " + path + " | grep uhiCpuAltCalc=";
       cmd = uhiutil::execmd(cmd.c_str());
       if(cmd != "") uhiutil::newline(cmd,"uhiCpuAltCalc=",Direction::RIGHTSKIP);
       m_CPUModeSwitch.set_active((cmd != "" ? std::stoi(cmd) == 1 : false));
  }
  else {
      m_BlinkSwitch.set_active(true);
      m_CPUModeSwitch.set_active(false);
  }
  
  m_CPUCompareSwitch.set_active(false);

  if(width > 0 && height > 0) {
       set_default_size(width,height);
       cmd = "cat " + path + " | grep uhiposit=";
       cmd = uhiutil::execmd(cmd.c_str());
       if(cmd != "") {
           if(uhiutil::newline(cmd,"uhiposit=",Direction::RIGHTSKIP)) {
                posit = std::stoi(cmd);
                m_HPaned.set_position(posit);
           }
       }
  }
  else {
       set_default_size((smDlg->dw / 3) + 50, (smDlg->dh / 3) + 50);
       m_HPaned.set_position((((smDlg->dw / 3) + 50)/ 2));
  }
}

void CHWindow::init_units_activity_vision() // init all cpu units activity vision
{
    if(pntProcessor) {
          unsigned int cpunits = pntProcessor->Get_cpu_units(),fbox_set = 0;

          // One core,one thread.No sense.
          if(cpunits < 2) {
                 item_cpu->set_enabled(false);
                 return;
          }

          switch(cpunits)
          {
             case 2  : fbox_set = 1;break;
             case 3  : fbox_set = 2;break;
             case 4 ... 9 : fbox_set = 3;break;
             case 10 ... 16 : fbox_set = 4;break;
             default : fbox_set = 5;break;
          }
          m_Fbox_CPUActivityAll.set_max_children_per_line(fbox_set); // reinit

          InitUI_activity_vision(UhiDownCast(pntProcessor.get())->GetCalcChain(),cpu_units_monit_chain);          
          
          if(!pntProcessor->m_CpuAltCalc) {
                 m_CPUModeSwitch.set_active(false);
                 CPUMNGBTNSTATE(m_CPUModeSwitch);
                 m_CPUCompareSwitch.set_active(false);
                 CPUMNGBTNSTATE(m_CPUCompareSwitch);
          }
   }
}

void CHWindow::QuitTasks() const
{       
      VeryFastQuitGuard(); // Async init task/thread very fast quit guard

      std::string path = "/home/" + uhiutil::GetUserName() + "/.uhInfo/";
      if(uhiutil::ExistenceVerification(path.c_str())) {
          int width = get_width(), height = get_height(),posit = m_HPaned.get_position();
          std::string wbuffer = (smDlg ? (pfDlg ? (pfDlg->GetSaveImpStat() ? smDlg->GetAllInused() : smDlg->GetInused()) : "") : "");
          wbuffer.append("uhiInTmpMon=" + (pfDlg ? std::to_string(pfDlg->GetInTmpMonStat()) : "0") + "\n");
          wbuffer.append("uhiAllInput=" + (pfDlg ? std::to_string(pfDlg->GetAllInputStat()) : "0") + "\n");
          wbuffer.append("uhiSaveImp=" + (pfDlg ? std::to_string(pfDlg->GetSaveImpStat()) : "0") + "\n");
          wbuffer.append("uhiCpuAltCalc="  + (std::to_string(m_CPUModeSwitch.get_active()) + "\n"));
          wbuffer.append("uhiNativeFqState="  + (std::to_string(uhiutil::cpu::native_fq_state) + "\n"));             
          wbuffer.append("uhiBlink=" + (std::to_string(m_BlinkSwitch.get_active()) + "\n"));           
          wbuffer.append("uhiMaxTmp=" + (pfDlg ? std::to_string(pfDlg->GetMaxTmpStat()) : "2") + "\n");//default 120°
          if(width > 0 && height > 0) {
                wbuffer.append("uhiwidth=" + std::to_string(width) + "\n");
                wbuffer.append("uhiheight=" + std::to_string(height) + "\n");
                wbuffer.append("uhiposit=" + std::to_string(posit));
                path += "uhInfo.cfg";
                std::FILE* flp = std::fopen(path.c_str(), "w");
                if(flp) {
                    std::fwrite(wbuffer.data(), sizeof(char), wbuffer.size(), flp);
                    std::fclose(flp);
                }
          }
      }
}

void CHWindow::Posthreadnotify()
{
	Glib::RefPtr<Gtk::TextBuffer> TVBuff = m_TextView.get_buffer();
    std::string sysmess = TVBuff->get_text();

    unsigned int nchips = 0,nsensors = 0;
    CHIPSENSORSNUMBER(nchips,nsensors);

    sysmess = sysmess + "\n... done.\n";

    if(nchips)
        sysmess.append(" " + std::to_string(nchips) + " sensor nodes " + std::to_string(nsensors) + " sensors detected."); 
    else
        sysmess.append("No sensors detected.");

    TVBuff->set_text(sysmess);

    init_units_activity_vision();// Тhe number of units is already known

    if(m_Label_Monitors.get_text() == "") m_Frame_Monitors.set_visible(false);// If no monitors detected

    if(pGpus) {
          if(pGpus->check_adapters())
                  pGpus->GpuComboInit(m_Gpus);
    }

    if(pUd2Manager)
        Ud2printcache = pUd2Manager->PrintDetectedSensors(TVBuff,temperature_mode_status,true); // Init Ud2printcache

    sensors_printing_enable  = (nchips ? true : false);

    if(item_infomode) {
           item_infomode->set_enabled(sensors_printing_enable); // init again
           item_infomode->change_state(sensors_printing_enable);
           m_sb_status.set_text((sensors_printing_enable ? "ON      " : "OFF     ")); //ADD
           if(sensors_printing_enable) { //ADD
                 STATUSIMAGES_CLEAR;
                 STATUSIMAGES_SET_ACTIVE;
           }

    }

    if(item_manage) item_manage->set_enabled(sensors_printing_enable);

    if(item_temperature) item_temperature->set_enabled(temperature_monitoring_enabled);

    if(item_options) item_options->set_enabled(sensors_printing_enable);

    if(smDlg) smDlg->SetDefSize();

    m_CPUNativeFqSwitch.set_active(uhiutil::cpu::native_fq_state);
    ShowHide_compare_elements();

     if(!c_Timer)
             c_Timer = SETIMER(uhiutil::timer_id,uhiutil::timer_interval);

     FinishThreadAndClear();
}

bool CHWindow::uhI_Timer(int TmNo)
{
      static unsigned int condition = 0;

      bool state{false};
      item_cpu->get_state(state);

      // --------------- cpu units activity calc ---------------
      if(state) {
           if(pntProcessor) {
                 std::lock_guard<std::mutex> lock(mutex_print);
                 pntProcessor->CalcFrecqUsage(nullptr,nullptr,&cpu_units_monit_chain,m_CPUModeSwitch.get_active()); // all units
                 RedrawActivity();
           }
      
           CPUMNGBTNSTATE(m_CPUModeSwitch,state);
           CPUMNGBTNSTATE(m_CPUCompareSwitch,state);
           
           return true;
      }

      if(sensors_printing_enable) {
                sensors_print(((condition == 5 || pUd2Manager->dataPrint_forced) ? true : false),pfDlg->GetInTmpMonStat());
                if(pUd2Manager->dataPrint_forced)  {
                       pUd2Manager->dataPrint_forced = false;
                       if(condition != 5) condition = 5;
                }
      }

      // Calculate and dysplay CPU summary frequency and usage
      if(pntProcessor && sensors_printing_enable && !temperature_mode_status)
             pntProcessor->CalcFrecqUsage(&m_pbFreq,&m_pbUse);//summary

      if(pGpus && sensors_printing_enable && !temperature_mode_status) 
             m_Label_VGA_cond_status.set_text(pGpus->CurrentPcieLinkStatus(m_Gpus.get_active_row_number()));

      ((condition == 5) ? condition = 0 : condition++);

      return true;
}

void CHWindow::sensors_print(bool Ud2print,bool extension)
{
	   std::lock_guard<std::mutex> lock(mutex_print);
       bool state{false};
       item_temperature->get_state(state);
       const bool blink_global_stat = m_BlinkSwitch.get_active();
       Glib::RefPtr<Gtk::TextBuffer> buff = m_TextView.get_buffer();
       buff->erase(buff->begin(),buff->end());
       
       buff->insert(buff->get_iter_at_line(buff->get_line_count()),"  Detected sensors :    \n");

       pSysensors->PrintDetectedSensors(buff,temperature_mode_status,blink_global_stat,pfDlg->GetAllInputStat(),pSysensors->visible_tmp_sens_count + (pfDlg->GetInTmpMonStat() ? pUd2Manager->visible_tmp_sens_count : 0));
       if(temperature_mode_status)
                          m_DAtemperature.Redraw();

       if((Ud2print && !temperature_mode_status) || (temperature_mode_status && extension))
               Ud2printcache = pUd2Manager->PrintDetectedSensors(buff,temperature_mode_status,blink_global_stat); 

       if(!temperature_mode_status) buff->insert(buff->get_iter_at_line(buff->get_line_count()),Ud2printcache);

       if(buff->get_text() == "  Detected sensors :    \n") {
           if(item_temperature && !temperature_mode_status /*&& state*/)
                                     item_temperature->set_enabled(false);

           unsigned int nchips = 0,nsensors = 0;
           CHIPSENSORSNUMBER(nchips,nsensors);
           buff->insert(buff->get_iter_at_line(buff->get_line_count()),(" " + std::to_string(nchips) + " sensor nodes " + std::to_string(nsensors) + " sensors detected.\n " +
        		 (!temperature_mode_status ? "All" : "Temperature")  + " sensors are deselected." + (temperature_mode_status ? "\nLook at:\n  Sensors -> Sensors settings\n  Application -> Preferences" : "")));
       }
       else {
           if(item_temperature && !temperature_mode_status) {
                           if(!state)  {
                        	   item_temperature->set_enabled(!smDlg_shown /*&& extension*/);
                           }
                           else {
                               if(!extension && state) {
                                           item_temperature->set_enabled(temperature_monitoring_enabled);
                               }
                           }
           }
       }
}

void CHWindow::enhanced_system_info()
{
   if(item_infomode) {
       STATUSIMAGES_CLEAR;
	   bool infomode{false};
	   item_infomode->get_state(infomode);
	   infomode = !infomode;
	   item_infomode->change_state(infomode);
       m_sb_status.set_text((infomode ? "ON      " : "OFF     "));
       if(infomode) {
		   STATUSIMAGES_SET_ACTIVE;
		   if(!sensors_printing_enable) sensors_printing_enable = infomode;
       }
       else {
		   STATUSIMAGES_SET_INACTIVE;
		   if(sensors_printing_enable) {
			   sensors_printing_enable = infomode;
			   UIBCF((&m_pbFreq),0.0,"Frequency");
			   UIBCF((&m_pbUse),0.0,"Usage");
			   m_Label_VGA_cond_status.set_label("- / -");

			   std::string sysmess("Sensors activity turned off...\n");
			   unsigned int nchips = 0,nsensors = 0;
			   CHIPSENSORSNUMBER(nchips,nsensors);
			   sysmess.append(" " + std::to_string(nchips) + " sensor nodes " + std::to_string(nsensors) + " sensors detected.");

			   (m_TextView.get_buffer())->set_text(sysmess);
		   }
       }
       ENHGITESTAUS(infomode);
   }
}

void CHWindow::show_cpu_activity_all()
{
   static bool remeber_activity = sensors_printing_enable;
     
   m_CPUNativeFqSwitch.set_active(uhiutil::cpu::native_fq_state);
   if(item_cpu) {
	   bool cpumode{false};
	   item_cpu->get_state(cpumode);
	   cpumode = !cpumode;
	   item_cpu->change_state(cpumode);
       if(cpumode) {
           remeber_activity = sensors_printing_enable;
           StatusbarCpuText();
       }
       else {
           if(pntProcessor) {
               for(cpu_chain_el el : cpu_units_monit_chain)  {
                   UIBCF(el.cpuid_m_pbF,0.0,"Frequency");
                   UIBCF(el.cpuid_m_pbCF,0.0,"Copr.Freq.");
                   UIBCF(el.cpuid_m_pbU,0.0,"Usage");
               }
               CPUCALCDATACTION(m_ClearCalcData);
           }
       }

       MENUITESTAUS((cpumode ? !cpumode : remeber_activity));
       if(!cpumode && item_infomode && (pSysensors->GetSensorNodesNumb() +
    		                 pUd2Manager->GetSensorNodesNumb())) item_infomode->set_enabled(!cpumode);

       m_Revealer.set_visible(!cpumode);
       m_VBoxCPUActivityAll.set_visible(cpumode);
       m_sb_cpu_labeltext.set_visible(cpumode);
       m_sb_cpu_status.set_visible(cpumode);
       set_title((cpumode ? "uhInfo - CPU units" : "uhInfo - Summary"));
       m_sb_status.set_text((!cpumode ? (sensors_printing_enable ? "ON      " : "OFF     ") : "OFF     ")); //ADD
   }
}

void CHWindow::monitor_temperature()
{
	if(!pSysensors) return;

   if(item_temperature) {
       pSysensors->EraseStatisticsAll();
       pUd2Manager->EraseStatisticsAll();

 	   bool tmode{false};
 	   item_temperature->get_state(tmode);
 	   tmode = !tmode;
 	   item_temperature->change_state(tmode);

	   TMPITSTAT(!tmode);

		if(tmode) {
			pSysensors->PopulateTemperatureSelection(this);
			if(pfDlg && pfDlg->GetInTmpMonStat())
			    pUd2Manager->PopulateTemperatureSelection(this);
			Ud2printcache = "";
		}
		else {
			  ptRefTreeModel->clear();
			  m_DAtemperature.EraseAll();
			  if(pUd2Manager) pUd2Manager->PrintForceExternal();
		}

	   m_VPanedTrmpetature.set_visible(tmode);
	   temperature_mode_status = tmode;
	   set_title((tmode ? "uhInfo - Temperature monitor" : "uhInfo - Summary"));
   }
}

void CHWindow::OnTempToggled(const Glib::ustring &path_string)
{
	Gtk::TreePath path(path_string);
    Gtk::TreeModel::iterator iter = ptRefTreeModel->get_iter(path);
    CDrawArea::DRAWVECTOR dv = nullptr;
    double *ps_max = nullptr;

    if(pSysensors && (*iter)[tColumns->tsensor_id] != -1)
          dv = pSysensors->SensorStatisticToggle((*iter)[tColumns->col_tcheck],(*iter)[tColumns->color_name],(*iter)[tColumns->tsensor_node],(*iter)[tColumns->tsensor_name],(*iter)[tColumns->tnode_id],(*iter)[tColumns->tsensor_id],&ps_max);
    else
          dv = pUd2Manager->SensorStatisticToggle((*iter)[tColumns->col_tcheck],(*iter)[tColumns->color_name],(*iter)[tColumns->tsensor_node],(*iter)[tColumns->tsensor_name],(*iter)[tColumns->tnode_id],(*iter)[tColumns->tsensor_id],&ps_max);

    if(dv)
        m_DAtemperature.SetUnsetDrawItem(dv,ps_max,(*iter)[tColumns->color_name],(*iter)[tColumns->tsensor_node] + " : " + (*iter)[tColumns->tsensor_name],(*iter)[tColumns->col_tcheck]);

}

void CHWindow::On_CPUActivityAll_switch_changed()
{
	 CPUMNGBTNSTATE(m_CPUModeSwitch);
     CPUMNGBTNSTATE(m_CPUCompareSwitch);
        
    StatusbarCpuText();
}

void CHWindow::On_Compare_mode_switch_changed()
{
	bool compare_mode = m_CPUCompareSwitch.get_active();
    ShowHide_compare_elements(compare_mode);
    
    if(compare_mode) {
          CPUMNGBTNSTATE(m_CPUModeSwitch);
          CPUMNGBTNSTATE(m_CPUCompareSwitch);
    }
}
