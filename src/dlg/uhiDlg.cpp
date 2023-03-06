/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "../mwndui.h"

UhiDlgWnd::UhiDlgWnd(const Gtk::Window *const pW) : plMw(pW)
{
	UIHWindow::signal_set_param().connect(sigc::mem_fun(*this,&UhiDlgWnd::on_set_after_init_param));
}

void UhiDlgWnd::on_show()
{
	Gtk::Window::on_show();
}
