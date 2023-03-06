/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _DLGBASE_H_
#define _DLGBASE_H_

class UhiDlgWnd  : public Gtk::Window
{
public:
	UhiDlgWnd(const Gtk::Window *const pW);
	virtual ~UhiDlgWnd()  = default;
protected:
	virtual void on_set_after_init_param(const int ,const int ,const double ) = 0;
	virtual void InitVision() = 0;
	virtual void on_show() override = 0;

	const Gtk::Window *const plMw = nullptr;
};

#endif // _DLGBASE_H_
