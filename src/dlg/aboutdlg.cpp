/*
 *    uhInfo
 *    Copyright (C) 2021
 */

#include "aboutdlg.h"
#include "../uhirc2.h"
#include <glibmm/i18n.h>

CAboutDlg::CAboutDlg(Gtk::Window *const p_mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp) : nB(_("Close")),lB("http://www.uhinfo.free.bg/",_("Go to uhInfo website")),
                                   lbGH("https://github.com/isgospodinov/uhInfo",_("uhInfo on GitHub")),m_copyright(_("Copyright") + std::string("©") + _("Ivailo Gospodinov")),
						           m_label("     "),uhi_img{Gdk::Pixbuf::create_from_xpm_data(uhilogo)},m_Image(uhi_img)
{
	uhiutil::set_css_style(get_style_context(),*cp);
	set_transient_for(*p_mWnd);
	InitVision();
}

void CAboutDlg::InitVision()
{
     set_decorated(false);
	 set_modal(true);
	 set_resizable(false);

	 Gtk::Box *m_BoxAll = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
	 Gtk::Frame *m_FrAll = Gtk::make_managed<Gtk::Frame>();
	 Gtk::Label *m_Ver = Gtk::make_managed<Gtk::Label>(_("Application version : 0.24") + std::string("\n") + std::string("         Gtkmm : v.") + GTKMM_VERSION);

     set_child(*m_FrAll);
     m_FrAll->set_child(*m_BoxAll);

     LOCALCSSPROVWITHSTYLE;

     m_Title.set_justify(Gtk::Justification::CENTER);
     m_BoxAll->set_expand();
     m_FrAll->set_margin(4);
     m_Title.set_margin(12);
     m_Ver->set_margin(4);
     m_Ver->set_margin_bottom(8);
     m_CGrid.set_margin_start(4);
     m_CGrid.set_margin_end(4);
     m_AGrid.set_margin(4);
     m_AGrid.set_halign(Gtk::Align::CENTER);
     m_CGrid.set_halign(Gtk::Align::CENTER);

     m_Image.set_size_request(uhi_img->get_height(),uhi_img->get_width());
     nB.set_image_from_icon_name("window-close");

     m_BoxAll->append(m_Title);
     m_BoxAll->append(*m_Ver);
     m_BoxAll->append(m_CGrid);
     m_BoxAll->append(m_AGrid);

     m_CGrid.attach(m_Image,0, 2, 1, 1);
     m_CGrid.attach(m_label, 1, 2, 1, 1);
     m_CGrid.attach(m_copyright, 1, 1, 1, 1);
     m_CGrid.attach(lbGH, 1, 2, 1, 1);

     m_AGrid.attach(lB, 0, 1, 1, 1);
     m_AGrid.attach(nB, 1, 1, 1, 1);

     uhiutil::set_css_style(m_Title.get_style_context(),lprv,"als_cls");
     uhiutil::set_css_style(m_Ver->get_style_context(),lprv,"afu_cls");

     nB.signal_clicked().connect([&](){hide();});
}
