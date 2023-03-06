/*
 *    uhInfo
 *    Copyright (C) 2021
 */

#ifndef _ABOUTDLG_H_
#define _ABOUTDLG_H_

#include "../uhirc.h"

class CAboutDlg : public Gtk::Window
{
public:
   CAboutDlg(Gtk::Window *const p_mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp);
   void set_message (std::string msg){m_Title.set_text("About...\n" + msg);}
private:
   void InitVision();
   Gtk::Button nB;
   Gtk::LinkButton lB,lbGH;
   Gtk::Grid m_CGrid,m_AGrid;
   Gtk::Label m_copyright,m_label,m_Title;

   const Glib::RefPtr<Gdk::Pixbuf> uhi_img;
   Gtk::Image m_Image;
};

#endif // _ABOUTDLG_H_
