/*
 *    uhInfo
 *    Copyright (C) 2025
 */

#ifndef _LEXPANDER_H_
#define _LEXPANDER_H_

#include "util.h"

class UIHWindow;

class uhiExpand : public Gtk::Expander
{
	using Action = enum class ExpAction {NOACTION,EXPAND,SHRINK};
public:
	using Expanders = enum class ExpType {INIT = 0, MOBO, MEM, MONIT, AUDIO, NETW ,OS = 11,MAIN = 101};

	uhiExpand(const Glib::ustring &label, Expanders ident, UIHWindow *const wnd = nullptr) : Gtk::Expander(label), id(ident)
	{
		  if(!p_mWnd) p_mWnd = wnd;

          property_expanded().signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &uhiExpand::on_expr_sig_changed), id));

	};

	virtual ~uhiExpand(){}

	void on_expr_state_changed(bool state, Expanders& ind);
	Action on_auto_adjust(bool mbe, bool ose, bool mme, bool mne, bool aue, bool nte, bool ve) const;
protected:
	virtual void on_expr_sig_changed(Expanders& data){on_expr_state_changed(get_expanded(), data);}
private:
	Expanders id = Expanders::INIT;
    static	UIHWindow *p_mWnd;
};

#endif /* _LEXPANDER_H_ */
