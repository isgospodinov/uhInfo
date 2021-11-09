/*
 *    uhInfo
 *    Copyright (C) 2021
 */

#include "aboutdlg.h"

CAboutDlg::CAboutDlg(Gtk::Window *const p_mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp) : Gtk::MessageDialog(*p_mWnd,"",false, Gtk::MessageType::INFO, Gtk::ButtonsType::NONE, true),
                          nB("Close"),lB("http://www.uhinfo.free.bg/","Go to uhInfo website"),lbGH("https://github.com/isgospodinov/uhInfo","uhInfo on GitHub"),
						  m_copyright("Copyright© Ivailo Gospodinov"),m_label("     ")
{
	get_style_context()->add_provider(*cp, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	PreInitVision();
}

void CAboutDlg::PreInitVision()
{
     set_title("About...");
     set_secondary_text("Application version : 0.17\n");
     m_Image.set_from_icon_name("info");
     m_Image.set_icon_size(Gtk::IconSize::LARGE);

     m_CGrid.attach(m_Image,0, 1, 1, 1);
     m_CGrid.attach(m_label, 1, 1, 1, 1);
     m_CGrid.attach(m_copyright, 2, 1, 1, 1);
     m_CGrid.attach(lbGH, 2, 2, 1, 1);
     m_CGrid.set_halign(Gtk::Align::CENTER);

     get_message_area()->append(m_CGrid);

     nB.set_image_from_icon_name("window-close");

     m_AGrid.attach(lB, 0, 1, 1, 1);
     m_AGrid.attach(nB, 1, 1, 1, 1);
     m_AGrid.set_halign(Gtk::Align::CENTER);

     get_content_area()->append(m_AGrid);

     nB.signal_clicked().connect(sigc::mem_fun(*this, &CAboutDlg::bt_quit_clicked));

     bt_quit_clicked();
}
