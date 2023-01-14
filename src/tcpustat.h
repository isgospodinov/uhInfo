/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _CPUSTAT_H_
#define _CPUSTAT_H_

class CpuStatDlg : public Gtk::Window
{
public:
	CpuStatDlg(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv);
	virtual ~CpuStatDlg() = default;

	void SetParam(const double mx) {if(plMw)set_default_size((2 * plMw->get_width() / 3), (plMw->get_height() - (2 * plMw->get_height() / 3)));cpfquattent = mx * .9;}
	bool Wnd_close_handler();
	virtual void on_show() override;
	void stop_timer() {if(l_timer) {l_timer.get()->disconnect();l_timer = std::unique_ptr<sigc::connection>(nullptr);hide();}}

private:
   Gtk::ScrolledWindow scrollWindow;
   Gtk::TextView lc_TextView;
   const Gtk::Window *const plMw = nullptr;
   const char *const tag_attention = "cpu_f_attent";
   double cpfquattent = 0.0;

   std::unique_ptr<sigc::connection> l_timer{nullptr};
   bool ot_timer(int tmNo);

   bool bf{uhiutil::ExistenceVerification("/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq")};
   void InitVision();
};

#endif // _CPUSTAT_H_
