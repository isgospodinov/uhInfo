/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "../mwnd.h"

CpuStatDlg::CpuStatDlg(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv,const std::unique_ptr<CProcUnits> *const pCpu) : UhiDlgWnd(pMWnd),lc_TextView(),
                        lpCPU(pCpu),fqmax((*lpCPU)->Get_PtrCpu_fqmax()),
						local_CpuInTempr((*lpCPU)->GetCpuFqAvg(),fqmax,pMWnd)
{
	set_transient_for(*pMWnd);
	set_title(_("CPU status"));

	set_child(fr_AllWnd);
	fr_AllWnd.set_child(box_allWnd);
	lcpuDrAr.set_child(l_CPULoad);
	l_CPULoad.append(l_InfoCpu);
	l_CPULoad.append(local_CpuInTempr);
	lfr_VCativ.set_child(lb_VCstat);
	lb_VCstat.append(l_VCstat);
	lb_VCstat.append(local_SensVcore);
	l_CPULoad.append(lfr_VCativ);
    scrollWindow.set_child(lc_TextView);
    lfr_Tb.set_child(mCPU_Stat_ToolBar);
    box_allWnd.append(lfr_Tb);
    box_allWnd.append(lcpuDrAr);
    box_allWnd.append(scrollWindow);
    uhiutil::set_css_style(get_style_context(),*cProv);
    lc_TextView.get_buffer()->create_tag(tag_attention)->property_foreground() = "red";

    mCPU_Stat_ToolBar.append(l_InfoLabel);
    mCPU_Stat_ToolBar.append(cb_WrnLevel);

    cb_WrnLevel.signal_changed().connect(sigc::mem_fun(*this,&CpuStatDlg::on_WrnLewel_changed));

    InitVision();

    signal_close_request().connect(sigc::mem_fun(*this, &CpuStatDlg::Wnd_close_handler),false);

    SETLOCALDECORATION;
}

bool CpuStatDlg::Wnd_close_handler()
{
	CtrlStatMng(true);
	stop_cpustat_timer();

	return true;
}

void CpuStatDlg::InitVision()
{
	for(const char *const el : {_("weak"),_("average"),_("strong"),_("heavy")}) cb_WrnLevel.append(el);

	LOCALCSSPROVWITHSTYLE;
	uhiutil::set_css_style(lc_TextView.get_style_context(),lprv,"toolbar");
	uhiutil::set_css_style(lfr_Tb.get_style_context(),lprv,"toolbar");
	uhiutil::set_css_style(cb_WrnLevel.get_child()->get_style_context(),lprv,"tb_cls");
	uhiutil::set_css_style(lcpuDrAr.get_style_context(),lprv,"toolbar");

	box_allWnd.set_orientation(Gtk::Orientation::VERTICAL);
	l_CPULoad.set_orientation(Gtk::Orientation::VERTICAL);
	lb_VCstat.set_orientation(Gtk::Orientation::VERTICAL);
	fr_AllWnd.set_expand();
	box_allWnd.set_expand();
	fr_AllWnd.set_margin(4);
	cb_WrnLevel.set_margin(4);
	l_InfoLabel.set_margin(4);
	lfr_Tb.set_margin_end(4);
	lfr_Tb.set_margin_start(4);
	l_InfoCpu.set_margin_top(2);
	l_InfoCpu.set_margin_bottom(2);
	lcpuDrAr.set_margin_start(4);
	lcpuDrAr.set_margin_end(4);
	lfr_VCativ.set_margin_top(4);

    lc_TextView.property_justification() = Gtk::Justification::CENTER;
    scrollWindow.set_margin(4);
    scrollWindow.set_expand();
    lc_TextView.set_can_target(false);

    mCPU_Stat_ToolBar.set_halign(Gtk::Align::END);
    mCPU_Stat_ToolBar.set_valign(Gtk::Align::START);
    mCPU_Stat_ToolBar.set_visible(true);

    std::string cmd = "/home/" + uhiutil::GetUserName() + "/.uhInfo/uhInfo.cfg";
    if(uhiutil::ExistenceVerification(cmd.c_str())) {
        cmd = "cat " + cmd + " | grep uhiCpuFqWrn=";
        cmd = uhiutil::execmd(cmd.c_str());
        if(cmd != "") {
            if(uhiutil::newline(cmd,"uhiCpuFqWrn=",Direction::RIGHTSKIP)) {
            	cb_WrnLevel.set_active(std::stoi(cmd));
            }
        }
        else cb_WrnLevel.set_active(0);//set default .9
    }
    else cb_WrnLevel.set_active(0);//set default .9
}

void CpuStatDlg::on_show()
{
	UhiDlgWnd::on_show();
	CtrlStatMng(false);

	(lc_TextView.get_buffer())->set_text(_("Initialization..."));

	on_WrnLewel_changed();

	if(!l_timer) l_timer = SETIMER(uhiutil::timer_id,uhiutil::timer_interval,&CpuStatDlg::ot_timer);

	if(lcWrMode == WmDlg::TEMPRTDLG && local_SensVcore.VCoresActivities() && uhiutil::draw::more_info){
		lfr_VCativ.set_visible(true);
		local_SensVcore.ClearOrActivateVCStatistic(true);
	}
	else {
		lfr_VCativ.set_visible(false);
	}

}

bool CpuStatDlg::ot_timer(int tmNo)
{
	int cn = 0;
	double sv = .0,cv = .0,sum = .0;
	std::string fq_line("");
    Glib::RefPtr<Gtk::TextBuffer> lc_buff = lc_TextView.get_buffer();
    lc_buff->erase(lc_buff->begin(),lc_buff->end());
    Gtk::TextBuffer::iterator bfit = lc_buff->get_iter_at_line(lc_buff->get_line_count());
    std::istringstream fsinstrm{(CProc::m_CpuAltCalc ? uhiutil::execmd("cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq") : "")},
                       fcinstrm{LSCPUSE ? uhiutil::execmd(std::string("lscpu -e=mhz | head -n" + std::to_string((*lpCPU)->Get_cpu_units() + 1) + " | tail -" + std::to_string((*lpCPU)->Get_cpu_units())).c_str()) :
                                                		uhiutil::execmd("grep 'cpu MHz' /proc/cpuinfo | awk -F ': ' '{print $2}'")};

	bfit = lc_buff->insert(bfit,"cpu :  " + std::string(CProc::m_CpuAltCalc ? "scaling frequency  /  " : "") + (LSCPUSE ? "lscpu"  : "cpuinfo") + "\n\n");

	while(std::getline(fcinstrm, fq_line)) {
		      if(fq_line.rfind('\x0A') != std::string::npos) fq_line.pop_back();
		      cv = std::stod(fq_line);
			  if(lcWrMode == WmDlg::TEMPRTDLG && !CProc::m_CpuAltCalc) {
			           sum += (*lpCPU)->FreqCalc(fq_line,false,true);
			  }

			  if(CProc::m_CpuAltCalc && std::getline(fsinstrm, fq_line)) {
				   if(fq_line.rfind('\x0A') != std::string::npos) fq_line.pop_back();
				   fq_line = std::to_string((std::stod(fq_line) / (double) 1000));
				   sv = std::stod(fq_line);
				   if(lcWrMode == WmDlg::TEMPRTDLG) {
			              sum += (*lpCPU)->FreqCalc(fq_line,false,true);
				   }
			  }

	          bfit = lc_buff->insert(bfit,"CPU " + std::to_string(cn) + ":  ");

	          if(sv) {
	              if(cpufquattent < sv) bfit = lc_buff->insert_with_tag(bfit,std::to_string((int)sv),tag_attention);
	              else bfit = lc_buff->insert(bfit,std::to_string((int)sv));

	              bfit = lc_buff->insert(bfit," MHz  /  ");
	          }

	          if(cpufquattent < cv) bfit = lc_buff->insert_with_tag(bfit,std::to_string((int)cv),tag_attention);
	          else bfit = lc_buff->insert(bfit,std::to_string((int)cv));

	          bfit = lc_buff->insert(bfit," MHz\n");

			  cn++;
			  sv = cv = .0;
		}

	if(lcWrMode == WmDlg::TEMPRTDLG) {
	    std::string res = uhiutil::execmd("head -n1 /proc/stat");
	    (*lpCPU)->SetAverageData(sum / (double) (*lpCPU)->Get_cpu_units(),(*lpCPU)->UsageCalc(res));
	}

	Redraw();

	return true;
}

void CpuStatDlg::on_WrnLewel_changed()
{
	  double cwf = .9;
	  switch(cb_WrnLevel.get_active_row_number()) {
		   case 1: cwf = .92;break;
		   case 2: cwf = .94;break;
		   case 3: cwf = .96;break;
	  }
	  cpufquattent = *fqmax * cwf;
      set_InfoLabel(std::to_string((int) cpufquattent));
}

void CpuStatDlg::stop_cpustat_timer()
{
	if(l_timer) {
		l_timer->disconnect();
		l_timer = std::unique_ptr<sigc::connection>(nullptr);
		hide();
	}

	if(lcWrMode == WmDlg::TEMPRTDLG) {
	    (*lpCPU)->ClearAverageData();

	    if(local_SensVcore.VCoresActivities())local_SensVcore.ClearOrActivateVCStatistic();
	}
}

inline void CpuStatDlg::CtrlStatMng(const bool st) const
{
	if(lcWrMode == WmDlg::TEMPRTDLG) {
	       PTSMNG(m_DAtemperature).triangle.set_visibility(st);
	}
	else {
		PTSMNG(set_show_menubar(st));
		PTSMNG(m_ButtCPUOverall).set_visible(st);
	}
}
