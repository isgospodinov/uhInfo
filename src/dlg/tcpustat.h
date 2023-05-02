/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _CPUSTAT_H_
#define _CPUSTAT_H_

#include "../dar/hdwaretc.h"
#include "../dar/hdwarev.h"
#include <glibmm/i18n.h>

class CpuStatDlg : public UhiDlgWnd
{
public:
	CpuStatDlg(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv,const std::unique_ptr<CProcUnits> *const pCpu);
	virtual ~CpuStatDlg() = default;

	using WrcMode = enum class WmDlg{CPUOVLDLG,TEMPRTDLG};

	bool Wnd_close_handler();
	virtual void on_show() override;
	void OnShowDlg(WrcMode wm) {lcWrMode = wm;show();}
	void stop_cpustat_timer();
	const int get_CpuFqWrnLevel() const {return cb_WrnLevel.get_active_row_number();}
	virtual void on_set_after_init_param(const int w,const int h) override {set_default_size(w / 3, (h - (h / 4)));cpufquattent = *fqmax * .9;}
	void CtrlStatMng(const bool st) const;

private:
   Gtk::Frame fr_AllWnd,lfr_Tb,lcpuDrAr,lfr_VCativ;
   Gtk::Box box_allWnd,mCPU_Stat_ToolBar,l_CPULoad,lb_VCstat;
   Gtk::ScrolledWindow scrollWindow;
   Gtk::TextView lc_TextView;
   Gtk::ComboBoxText cb_WrnLevel;
   Gtk::Label l_InfoLabel,l_InfoCpu{_("Overall : CPU Fq. / CPU usage")},l_VCstat{_("Vcore`s statistic")};
   const char *const tag_attention = "cpu_f_attent";
   double cpufquattent = .0;

   const std::unique_ptr<CProcUnits> *const lpCPU;
   const double *const fqmax = nullptr;
   CDrArCpuInTempr local_CpuInTempr;
   CDrArVcore local_SensVcore;

   WrcMode lcWrMode;

   std::unique_ptr<sigc::connection> l_timer{nullptr};
   bool ot_timer(int tmNo);

   virtual void InitVision() override;
   void on_WrnLewel_changed();
   void Redraw(){local_CpuInTempr.Redraw();if(lcWrMode == WmDlg::TEMPRTDLG && local_SensVcore.VCoresActivities()) local_SensVcore.Redraw();}
   void set_InfoLabel(std::string wf) {l_InfoLabel.set_text("Max : " + std::to_string((int) *fqmax) + " MHz" + "\nWrn : " + wf + " MHz");}
};

#endif // _CPUSTAT_H_
