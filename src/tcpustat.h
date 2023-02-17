/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _CPUSTAT_H_
#define _CPUSTAT_H_

class CpuStatDlg : public Gtk::Window
{
public:
	CpuStatDlg(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv,CProc *const pCpu);
	virtual ~CpuStatDlg() = default;

	void SetParam(const double mx) {if(plMw)set_default_size((plMw->get_width() / 3), (plMw->get_height() - (plMw->get_height() / 4)));fqmax = mx;cpufquattent = fqmax * .9;}
	bool Wnd_close_handler();
	virtual void on_show() override;
	void stop_cpustat_timer() {if(l_timer) {l_timer.get()->disconnect();l_timer = std::unique_ptr<sigc::connection>(nullptr);hide();}lpCPU->cpuFqAvg.clear_cpufq_average_data();}
	const int get_CpuFqWrnLevel() const {return cb_WrnLevel.get_active_row_number();}

private:
   Gtk::Frame fr_AllWnd,lfr_Tb,lcpuDrAr;
   Gtk::Box box_allWnd,mCPU_Stat_ToolBar,l_CPULoad;
   Gtk::ScrolledWindow scrollWindow;
   Gtk::TextView lc_TextView;
   Gtk::ComboBoxText cb_WrnLevel;
   Gtk::Label l_InfoLabel,l_InfoCpu{"Overall : CPU Fq. / CPU usage"};
   const Gtk::Window *const plMw = nullptr;
   const char *const tag_attention = "cpu_f_attent";
   double fqmax = .0,cpufquattent = .0;

   CProcUnits *const lpCPU = nullptr;
   CDrArCpuInTempr local_CpuInTempr;

   std::unique_ptr<sigc::connection> l_timer{nullptr};
   bool ot_timer(int tmNo);

   void InitVision();
   void on_WrnLewel_changed();
   void set_InfoLabel(std::string wf) {l_InfoLabel.set_text("Max : " + std::to_string((int) fqmax) + " MHz" + "\nWrn : " + wf + " MHz");}
};

#endif // _CPUSTAT_H_
