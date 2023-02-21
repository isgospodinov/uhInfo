/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _CPUSTAT_H_
#define _CPUSTAT_H_

#include "hdwaretc.h"

class CpuStatDlg : public UhiDlgWnd
{
public:
	CpuStatDlg(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv,CProc *const pCpu);
	virtual ~CpuStatDlg() = default;

	bool Wnd_close_handler();
	virtual void on_show() override;
	void stop_cpustat_timer() {if(l_timer) {l_timer.get()->disconnect();l_timer = std::unique_ptr<sigc::connection>(nullptr);hide();}lpCPU->cpuFqAvg.clear_cpufq_average_data();}
	const int get_CpuFqWrnLevel() const {return cb_WrnLevel.get_active_row_number();}
	virtual void on_set_after_init_param(const int w,const int h,const double p) override {set_default_size(w / 3, (h - (h / 4)));fqmax = p;cpufquattent = fqmax * .9;}

private:
   Gtk::Frame fr_AllWnd,lfr_Tb,lcpuDrAr;
   Gtk::Box box_allWnd,mCPU_Stat_ToolBar,l_CPULoad;
   Gtk::ScrolledWindow scrollWindow;
   Gtk::TextView lc_TextView;
   Gtk::ComboBoxText cb_WrnLevel;
   Gtk::Label l_InfoLabel,l_InfoCpu{"Overall : CPU Fq. / CPU usage"};
   const char *const tag_attention = "cpu_f_attent";
   double fqmax = .0,cpufquattent = .0;

   CProcUnits *const lpCPU = nullptr;
   CDrArCpuInTempr local_CpuInTempr;

   std::unique_ptr<sigc::connection> l_timer{nullptr};
   bool ot_timer(int tmNo);

   virtual void InitVision() override;
   void on_WrnLewel_changed();
   void set_InfoLabel(std::string wf) {l_InfoLabel.set_text("Max : " + std::to_string((int) fqmax) + " MHz" + "\nWrn : " + wf + " MHz");}
};

#endif // _CPUSTAT_H_
