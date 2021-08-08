/*
 *    uhInfo
 *    Copyright (C) 2021
 */

#include "aboutdlg.h"

CAboutDlg::CAboutDlg(Gtk::Window *const p_mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp) : Gtk::MessageDialog(*p_mWnd,"",false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_NONE, true),
                          nB(Gtk::Stock::CLOSE),lB("http://www.uhinfo.free.bg/","Go to uhInfo website"),lbGH("https://github.com/isgospodinov/uhInfo","uhInfo on GitHub")
{
	get_style_context()->add_provider(*cp, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	PreInitVision();
}

void CAboutDlg::PreInitVision()
{
     set_title("About...");
     set_secondary_text("Application version :\n          0.16");
     set_image(*(Gtk::manage(new Gtk::Image(Gtk::Stock::DIALOG_INFO,Gtk::IconSize(Gtk::BuiltinIconSize::ICON_SIZE_DIALOG)))));
     get_content_area()->pack_start(*Gtk::manage(new Gtk::Label("Copyright© Ivailo Gospodinov")));
     get_content_area()->pack_start(*(Gtk::manage(&lbGH)));

     nB.set_image_from_icon_name("window-close");
     nB.set_always_show_image();

     Gtk::ButtonBox *pB = get_action_area();
     pB->set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_END);
     pB->set_border_width(2);
     pB->pack_start(*(Gtk::manage(&lB)));
     pB->pack_start(*(Gtk::manage(&nB)));

     nB.signal_clicked().connect(sigc::mem_fun(*this, &CAboutDlg::bt_quit_clicked));

     show_all();
     bt_quit_clicked();
}
