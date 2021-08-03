/*
 *    uhInfo
 *    Copyright (C) 2021
 */

#ifndef _ABOUTDLG_H_
#define _ABOUTDLG_H_

#include "uhirc.h"

class CAboutDlg : public Gtk::MessageDialog
{
public:
   CAboutDlg(Gtk::Window *const p_mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp);
private:
   void PreInitVision();
   Gtk::Button nB;
   Gtk::LinkButton lB;

   void bt_quit_clicked(){hide();}
};

#endif // _ABOUTDLG_H_
