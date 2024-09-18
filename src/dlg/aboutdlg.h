/*
 *    uhInfo
 *    Copyright (C) 2021
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
   Gtk::Button nB;
   Gtk::LinkButton lB,lbGH;
   Gtk::Grid m_CGrid,m_AGrid;
   Gtk::Label m_copyright,m_label,m_Title;
};

#endif // _ABOUTDLG_H_
