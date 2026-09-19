/*
 *    uhInfo
 *    Copyright (C) 2021
 *    Implement latest version check in the About dialog - 17-9-2026
 */

#ifndef _ABOUTDLG_H_
#define _ABOUTDLG_H_

#include "../util/uhirc.h"
#include <glibmm/i18n.h>

class CAboutDlg : public Gtk::Window
{
public:
   CAboutDlg(Gtk::Window *const p_mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp);
   void set_message (std::string msg){m_Title.set_text(_("About...") + std::string("\n") + msg);}

private:
   void InitVision();
   void on_nv_check();

   Gtk::Button nB,vB;
   Gtk::LinkButton lB,lbGH;
   Gtk::Grid m_CGrid,m_AGrid;
   Gtk::Label m_copyright,m_label,m_Title;

   Glib::RefPtr<Gio::Cancellable> m_cancellable;
   void on_show() override;
   void on_hide() override;
   Gtk::Label lBver;
};

#endif // _ABOUTDLG_H_
