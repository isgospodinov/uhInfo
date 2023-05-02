/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _UHICR_H_
#define _UHICR_H_

#include "dar/hdwarec.h"
#include "dar/hdwaret.h"
#include "dlg/uhiDlg.h"

#define KB1 1024
#define MB1 1048576
#define GB1 1073741824

#define CPUMNGBTNSTATE(btn,...) if(std::string(#__VA_ARGS__).empty())  btn.set_sensitive(false); else \
                                     if(pntProcessor->m_CpuAltCalc && !btn.get_sensitive() \
                                                     && !pntProcessor->m_ClearCalcData) btn.set_sensitive(true)
                                                     
#define CPUCALCDATACTION(field) pntProcessor->field = true

#define MONITORSTRING(vendor,name,dimens) m_monitor.append( (((vendor != "") ? (vendor + " ") : "" ) + \
                                                            ((name != "") ? (name + "  ") : "" ) + \
                                                            ((dimens != "") ? (dimens + "'") : "" )) + "\n"); \
                                                            vendor = name = dimens = ""
#define GETVENDOR(vendor,cv) std::bitset<16> nb(cv); \
                             vendor = {(char)(((nb & std::bitset<16>(0x00007C00)) >> 10).to_ulong() + 64), \
                                              (char)(((nb & std::bitset<16>(0x000003E0)) >> 5).to_ulong() + 64), \
                                              (char)((nb & std::bitset<16>(0x0000001F)).to_ulong() + 64)}

#define CONNECTIONSTATUS(connection) ((connection && connection->connected()) ? true : false)

#define CONNECTIONCLOSE(connection) if(CONNECTIONSTATUS(connection)) \
                                                connection->disconnect()

#define CLEAR_THREAD_CONDITIONS if(m_Connection) {CONNECTIONCLOSE(m_Connection);}

#define SIZEOF(obj) ((sizeof obj) / sizeof(char*))
#define LIBCLOSE(lib) if(lib) {dlclose(lib);lib = nullptr;}
#define RETURN_FALSE LIBCLOSE(libsensh) return false                       

#define SET_CUSTOM_LABELS m_Frame_CPUFrecq.set_label_widget(hbox_operation_status_cpu); \
                                m_Frame_VGA_pcie.set_label_widget(hbox_operation_status_pcie); \
                                      m_Frame_Sensors.set_label_widget(hbox_operation_status_sensors)

#define STATUSIMAGES_CLEAR operation_status_image_cpu.clear(); \
                            operation_status_image_pcie.clear(); \
                                  operation_status_image_sensors.clear()

#define STATUSIMAGES_SET_ACTIVE operation_status_image_cpu.set_from_icon_name("view-refresh"); \
                             operation_status_image_pcie.set_from_icon_name("view-refresh"); \
                                 operation_status_image_sensors.set_from_icon_name("view-refresh");

#define STATUSIMAGES_SET_INACTIVE operation_status_image_cpu.set_from_icon_name("window-close"); \
                                operation_status_image_pcie.set_from_icon_name("window-close"); \
                                     operation_status_image_sensors.set_from_icon_name("window-close")

#define ENHGITESTAUS(fsat) item_options->set_enabled(fsat); \
                           item_manage->set_enabled(fsat); \
                           item_temperature->set_enabled(fsat && temperature_monitoring_enabled)

#define MENUITESTAUS(ia) item_infomode->change_state(ia); \
                            item_infomode->set_enabled(ia); \
                            ENHGITESTAUS(ia)

#define SMDLGMISTAT(flag) item_cpu->set_enabled(flag); \
                          item_infomode->set_enabled(flag); \
                          ENHGITESTAUS(flag)

#define TMPITSTAT(flag) \
          m_Frame_MOBO.set_visible(flag);  \
          m_Frame_Memory.set_visible(flag);  \
          m_Frame_CPU.set_visible(flag);  \
          m_Frame_VGA.set_visible(flag); \
          m_Frame_Monitors.set_visible(flag); \
          m_Frame_Audio.set_visible(flag); \
          m_Frame_Network.set_visible(flag); \
          m_Frame_Disks.set_visible(flag); \
          m_Frame_OS.set_visible(flag); \
          m_BlinkGrid.set_visible(!flag); \
          item_cpu->set_enabled(flag); \
          item_infomode->set_enabled(flag); \
          item_options->set_enabled(flag); \
          item_manage->set_enabled(flag)

#define TEMPERATUREWNDVIEW(wndstat) \
		  m_StatusBar_Grid_condit.set_visible(wndstat); \
	      m_VBoxVLeft.set_visible(wndstat);  \
	      m_Box_TmpControls.set_visible(wndstat); \
	      set_show_menubar(wndstat); \
		  if(!clrDlg->is_visible()) mDA_ToolBar.set_visible((pfDlg ? (pfDlg->GetShowCPUfq() && m_DAtemperature.HasActivities()): false) && !wndstat)

#define  CHIPSENSORSNUMBER(chips,sensors) chips = pSysensors->GetSensorNodesNumb() + pUd2Manager->GetSensorNodesNumb(); \
                                          sensors = pSysensors->GetSensorsDetectedNumb() + pUd2Manager->GetSensorsDetectedNumb()

#define CONNECTIONSET *(new sigc::connection)

#define SETIMER(id,interval,func) std::unique_ptr<sigc::connection>(&(CONNECTIONSET = Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this,func), id), interval)))

#define DADRAWTEXT(crt, ly, dw, dh) crt->move_to(dw,dh); \
                                     ly->show_in_cairo_context(crt)

#define FULLAPPWND(w,h) w > draw::HWmax && h > draw::HWmax
#define FULLAPPWNDMODE(w,h) m_TmpWndCurrState == DAWndState::FULL && FULLAPPWND(w,h)
#define STRCOND(vl,gv,sv) (vl >= 100 ? gv : sv)

#define CHECKBCF (m_CpuAltCalc && bCpuAltCalc)
#define UIBCF(uiel,fraction,text) uiel->set_fraction(fraction); \
	                              uiel->set_text(text)

//TestPoint
#define VCORECHECK (((int)lbl.find("Vcore") != -1) || ((int)lbl.find("SVI2_Core") != -1))

#define SNTP(type) (sn->sntype == type)

#define PTSMNG(v) ((CHWindow*)plMw)->v
#define TCOLUMNS(v) PTSMNG(tColumns)->v

#define CONDITCPUSUMMARY(stat) \
		PTSMNG(m_Fbox_CPUActivityAll).set_visible(stat); \
		PTSMNG(m_Box_GRridAndButt).set_visible(stat); \
		PTSMNG(da_CpuOverall).set_visible(!stat);

#define LOCALCSSPROVWITHSTYLE Glib::RefPtr<Gtk::CssProvider> lprv = Gtk::CssProvider::create();lprv->load_from_data(style)

#define SETLOCALDECORATION  LOCALCSSPROVWITHSTYLE; \
             Gtk::HeaderBar *pHB = Gtk::make_managed<Gtk::HeaderBar>(); \
             pHB->set_decoration_layout(":close"); \
             uhiutil::set_css_style(pHB->get_style_context(),lprv,"hb_cls"); \
             set_titlebar(*pHB)

#define LSCPUSE (CProc::m_lsCpu && uhiutil::cpu::cpu_fq_base)

#define GTKMM_VERSION (std::to_string(GTKMM_MAJOR_VERSION) + "." + \
                       std::to_string(GTKMM_MINOR_VERSION) + "." + \
                       std::to_string(GTKMM_MICRO_VERSION))

// true if Gtkmm is new enough
/*#define GTKMM_CHECK_VERSION(major,minor,micro)                             \
     (GTKMM_MAJOR_VERSION > (major)  ||                                    \
     (GTKMM_MAJOR_VERSION == (major) && GTKMM_MINOR_VERSION > (minor)) ||  \
     (GTKMM_MAJOR_VERSION == (major) && GTKMM_MINOR_VERSION == (minor) && GTKMM_MICRO_VERSION >= (micro)))*/                                

using  post_init_sig = sigc::signal<void(const int,const int)>;

using cpu_chain_el = struct _cpu_chain {
	  std::string cpuid = "";
      int processor = -1,physid = -1,coreid = -1,cpuidn = -1;
	  Gtk::ProgressBar *cpuid_m_pbF = nullptr, *cpuid_m_pbCF = nullptr, *cpuid_m_pbU = nullptr;
	  CDrArCpu *pDArea = nullptr;
      Gtk::Label *lCompareColor = nullptr;
};

using CModelBaseUhiColumns = struct ModelBaseUhiColumns : public Gtk::TreeModel::ColumnRecord {
      using UhiModelType = enum class LocalTypes {TEMPERATURE,DISK,SENSORS,COLOR};

      ModelBaseUhiColumns(UhiModelType setcolumns = UhiModelType::DISK) {if(setcolumns == UhiModelType::DISK)add_uhi_columns();}

      Gtk::TreeModelColumn<unsigned int> No;
      Gtk::TreeModelColumn<Glib::ustring> sys_link,device_name,type,color,color_name,tsensor_node,tsensor_name,tnode_id,specificity,description;
      Gtk::TreeModelColumn<bool> col_tcheck;
      Gtk::TreeModelColumn<int> tsensor_id;
protected:
      void add_uhi_columns() {add(No); add(sys_link); add(device_name); add(type); add(specificity);}
};

using CModelUhiColumns = struct ModelUhiColumns : public CModelBaseUhiColumns {
      ModelUhiColumns(UhiModelType setcolumns = UhiModelType::TEMPERATURE) : ModelBaseUhiColumns(setcolumns) {
    	  if(setcolumns == UhiModelType::TEMPERATURE || setcolumns == UhiModelType::SENSORS || setcolumns == UhiModelType::COLOR)add_uhi_columns(setcolumns);}
private:
      void add_uhi_columns(UhiModelType setcolumns) {if(setcolumns != UhiModelType::COLOR) add(col_tcheck); if(setcolumns == UhiModelType::TEMPERATURE || setcolumns == UhiModelType::COLOR) {add(color); add(color_name);}
                                                                                                       if(setcolumns != UhiModelType::COLOR) {add(tsensor_node); add(tsensor_name); add(description); add(tnode_id); add(tsensor_id);}}
};

using uhimc_unique_ptr = std::unique_ptr<CModelBaseUhiColumns>;

#endif // _UHICR_H_
