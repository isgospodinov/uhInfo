/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "mwnd.h"

CpuStatDlg::CpuStatDlg(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv) : lc_TextView(),plMw(pMWnd)
{
	set_transient_for(*pMWnd);
	set_title("CPU status");

	set_child(scrollWindow);
    scrollWindow.set_child(lc_TextView);
    uhiutil::set_css_style(get_style_context(),*cProv);

    InitVision();

    signal_close_request().connect(sigc::mem_fun(*this, &CpuStatDlg::Wnd_close_handler),false);
}

bool CpuStatDlg::Wnd_close_handler()
{
	CLRMNG(mDA_ToolBar).set_visible(true);
	stop_timer();
	hide();

	return true;
}


void CpuStatDlg::InitVision()
{

	Glib::RefPtr<Gtk::CssProvider> prv = Gtk::CssProvider::create();
	prv->load_from_data(style);
	uhiutil::set_css_style(lc_TextView.get_style_context(),prv,"toolbar");

    lc_TextView.property_justification() = Gtk::Justification::CENTER;
    scrollWindow.set_margin(4);
    scrollWindow.set_expand();

    (lc_TextView.get_buffer())->set_text("Initialization...");
}

void CpuStatDlg::on_show()
{
	Gtk::Window::on_show();
	CLRMNG(mDA_ToolBar).set_visible(false);
	(lc_TextView.get_buffer())->set_text("Initialization...");

	if(!l_timer) l_timer = SETIMER(uhiutil::timer_id,uhiutil::timer_interval,&CpuStatDlg::ot_timer);
}

bool CpuStatDlg::ot_timer(int tmNo)
{
	int cn = 0;
	std::string t_buff("\n"),ts_line(""),tc_line("");
    Glib::RefPtr<Gtk::TextBuffer> lc_buff = lc_TextView.get_buffer();
    std::istringstream fsinstrm{uhiutil::execmd("cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq")},
                                                fcinstrm{uhiutil::execmd("cat /proc/cpuinfo  | grep 'cpu MHz'")};

	t_buff += ("cpu :  " + std::string(bf ? "scaling frequency  /  " : "") + "cpuinfo\n\n");

	while(std::getline(fcinstrm, tc_line)) {
		  std::getline(fsinstrm, ts_line);
		  if(tc_line.rfind('\x0A') != std::string::npos) tc_line.pop_back();
		  uhiutil::newline(tc_line,":",Direction::RIGHT);

		  t_buff += ("CPU " + std::to_string(cn) + ":  " +
				(bf ? std::to_string((int)(std::stod(ts_line) / (double) 1000)) + " MHz  /  " : "" ) +
												      std::to_string(std::atoi(tc_line.c_str())) + " MHz\n");
		  cn++;
	}

    lc_buff->set_text(t_buff);

	return true;
}
