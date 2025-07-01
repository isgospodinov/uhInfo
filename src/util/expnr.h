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

	uhiExpand(const Glib::ustring &label, const Expanders ident, UIHWindow *const wnd = nullptr, const bool catch_sig_chg = true, const bool mnemonic = false) : Gtk::Expander(label, mnemonic), id(ident)
	{
		  if(!p_mWnd && wnd) p_mWnd = wnd;

          if(catch_sig_chg) property_expanded().signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &uhiExpand::on_expr_sig_changed), id));

	};

	virtual ~uhiExpand(){}

	void on_expr_state_changed(const bool state, const Expanders ind);
	const Action on_auto_adjust(const bool mbe, const bool ose, const bool mme, const bool mne, const bool aue, const bool nte, const bool ve) const;
protected:
	virtual void on_expr_sig_changed(const Expanders recogn) const {(const_cast<uhiExpand *const>(this))->on_expr_state_changed(get_expanded(), recogn);}
private:
	const Expanders id = Expanders::INIT;
    static	UIHWindow *p_mWnd;
};

#endif /* _LEXPANDER_H_ */
