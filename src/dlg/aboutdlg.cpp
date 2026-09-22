/*
 *    uhInfo
 *    Copyright (C) 2021
 */

#include "aboutdlg.h"
#include "../util/uhirc2.h"
#include <glibmm/i18n.h>
#include <thread>
//#include <iostream>

constexpr int UHILOGO_WIDTH = 70, UHILOGO_HEIGHT = 70;

CAboutDlg::CAboutDlg(Gtk::Window *const p_mWnd,const Glib::RefPtr<Gtk::CssProvider> *const cp) : nB(_("Close")),vB(_(LAST_VER " check")),lB("https://uhinfo.free.bg",_("Go to uhInfo website")),
                       lbGH("https://github.com/isgospodinov/uhInfo",_("uhInfo on GitHub")),m_copyright(_("Copyright") + std::string("©") + _("Ivailo Gospodinov")),m_label("     ")
{
	Gtk::Picture *m_Image = Gtk::make_managed<Gtk::Picture>();
	Glib::RefPtr<Gdk::Texture> uhi_texture = nullptr;

	try {
		Glib::RefPtr<Glib::Bytes> image_bytes = Glib::Bytes::create(uhilogo_png, sizeof(uhilogo_png));
		uhi_texture = Gdk::Texture::create_from_bytes(image_bytes);
	}
	catch (...) {
		uhi_texture = nullptr;
	}

	if(uhi_texture) {
		m_Image->set_paintable(uhi_texture);
		m_Image->set_keep_aspect_ratio(true);
		m_Image->set_size_request(uhi_texture->get_width(), uhi_texture->get_height());
		m_Image->set_halign(Gtk::Align::CENTER);
		m_Image->set_valign(Gtk::Align::CENTER);
	}
	else {
			try {
				Glib::RefPtr<Gtk::IconTheme> icon_theme = Gtk::IconTheme::get_for_display(Gdk::Display::get_default());

				Glib::RefPtr<Gdk::Paintable> icon_paintable = icon_theme->lookup_icon("dialog-information", {}, 64, 1, Gtk::TextDirection::NONE, {});

				if (icon_paintable) {
					m_Image->set_paintable(icon_paintable);
				}
			}
			catch (...) {
				// Final protective layer in case the system theme is missing
			}

			m_Image->set_keep_aspect_ratio(true);
			m_Image->set_size_request(UHILOGO_WIDTH, UHILOGO_HEIGHT);
			m_Image->set_halign(Gtk::Align::CENTER);
			m_Image->set_valign(Gtk::Align::CENTER);
	}

	uhiutil::set_css_style(get_style_context(),*cp);
	set_transient_for(*p_mWnd);

	vB.signal_clicked().connect(sigc::mem_fun(*this, &CAboutDlg::on_nv_check));

	InitVision();
	m_CGrid.attach(*m_Image,0, 2, 1, 1);
}

void CAboutDlg::InitVision()
{
     set_decorated(false);
	 set_modal(true);
	 set_resizable(false);

	 Gtk::Box *m_BoxAll = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
	 Gtk::Frame *m_FrAll = Gtk::make_managed<Gtk::Frame>();
	 lBver.set_text(_(APP_VER "\n" LAST_VER ": unchecked") + std::string("\n") + std::string("         Gtkmm : v.") + GTKMM_VERSION);

     set_child(*m_FrAll);
     m_FrAll->set_child(*m_BoxAll);

     LOCALCSSPROVWITHSTYLE;

     vB.set_margin_start(4);
	 vB.set_margin_end(4);
     m_Title.set_justify(Gtk::Justification::CENTER);
     m_BoxAll->set_expand();
     m_FrAll->set_margin(4);
     m_Title.set_margin(12);
     lBver.set_margin(4);
     lBver.set_margin_bottom(8);
     lBver.set_justify(Gtk::Justification::CENTER);
     m_CGrid.set_margin_start(4);
     m_CGrid.set_margin_end(4);
     m_AGrid.set_margin(4);
     m_AGrid.set_halign(Gtk::Align::CENTER);
     m_CGrid.set_halign(Gtk::Align::CENTER);

     nB.set_image_from_icon_name("window-close");

     m_BoxAll->append(m_Title);
     m_BoxAll->append(vB);
     m_BoxAll->append(lBver);
     m_BoxAll->append(m_CGrid);
     m_BoxAll->append(m_AGrid);

     m_CGrid.attach(m_label, 1, 2, 1, 1);
     m_CGrid.attach(m_copyright, 1, 1, 1, 1);
     m_CGrid.attach(lbGH, 1, 2, 1, 1);

     m_AGrid.attach(lB, 0, 1, 1, 1);
     m_AGrid.attach(nB, 1, 1, 1, 1);

     uhiutil::set_css_style(m_Title.get_style_context(),lprv,"als_cls");
     uhiutil::set_css_style(lBver.get_style_context(),lprv,"afu_cls");

     nB.signal_clicked().connect([&](){hide();});
}

void CAboutDlg::on_show()
{
  Gtk::Window::on_show();

  if(!vB.get_sensitive())
  {
    vB.set_sensitive(true);
  }
}

void CAboutDlg::on_hide()
{
  Gtk::Window::on_hide();

  lBver.set_text(_(APP_VER "\n" LAST_VER ": unchecked") + std::string("\n") + std::string("         Gtkmm : v.") + GTKMM_VERSION);

  if(m_cancellable) {
      m_cancellable->cancel();
  }
}

void CAboutDlg::on_nv_check()
{
	  vB.set_sensitive(false);

	  lBver.set_text(_(APP_VER "\n checking... ") + std::string("\n") + std::string("         Gtkmm : v.") + GTKMM_VERSION);

	  m_cancellable = Gio::Cancellable::create();
	  Glib::RefPtr<Gio::Cancellable> cancellable = m_cancellable;

	  std::thread worker([this, cancellable]() {
	    try {
	      Glib::RefPtr<Gio::SocketClient> client = Gio::SocketClient::create();
	      Glib::RefPtr<Gio::SocketConnection> conn = client->connect_to_host(GITHUB_HOST, HTTPS_PORT, cancellable);

	      Glib::RefPtr<Gio::NetworkAddress> server_identity = Gio::NetworkAddress::create(GITHUB_HOST, HTTPS_PORT);
	      Glib::RefPtr<Gio::IOStream> tls_conn = Gio::TlsClientConnection::create(conn, server_identity);

	      std::string http_request =
	        "GET /repos/isgospodinov/uhInfo/releases/latest HTTP/1.1\r\n"
	        "Host: api.github.com\r\n"
	        "User-Agent: uhInfo-Update-Checker\r\n"
	        "Accept: application/json\r\n"
	        "Connection: close\r\n"
	        "\r\n";

	      Glib::RefPtr<Gio::OutputStream> out = tls_conn->get_output_stream();
	      out->write(http_request, cancellable);

	      Glib::RefPtr<Gio::InputStream> in = tls_conn->get_input_stream();
	      std::string response = "";
	      char read_buf[GH_R_BUFF_SZ];
	      long int bytes;

	      while ((bytes = in->read(read_buf, sizeof(read_buf) - 1, cancellable)) > 0) {
	        read_buf[bytes] = '\0';
	        response.append(read_buf, bytes);
	      }

	      size_t body_pos = response.find("\r\n\r\n");
	      if (body_pos != std::string::npos) {
	        std::string json_body = response.substr(body_pos + 4);

	        size_t tag_pos = json_body.find("\"tag_name\":");
	        if (tag_pos != std::string::npos) {
	          size_t size_start = json_body.find("\"", tag_pos + 11);
	          size_t size_end = json_body.find("\"", size_start + 1);

	          if (size_start != std::string::npos && size_end != std::string::npos) {
	            std::string version = json_body.substr(size_start + 1, size_end - size_start - 1);

	            if (!cancellable->is_cancelled()) {

	              Glib::signal_idle().connect([this, cancellable, version]() {
	                if (!cancellable->is_cancelled()) {

	                  this->lBver.set_text(_(APP_VER "\n" LAST_VER ": ") + version + std::string("\n         Gtkmm : v.") + GTKMM_VERSION);
	                  this->vB.set_sensitive(true);
	                }
	                return false;
	              });
	            }
	            return;
	          }
	        }
	      }

	      if (!cancellable->is_cancelled()) {

	        Glib::signal_idle().connect([this, cancellable]() {
	          if (!cancellable->is_cancelled()) {
	        	this->lBver.set_text(_(APP_VER "\nError parsing JSON!") + std::string("\n         Gtkmm : v.") + GTKMM_VERSION);
	            this->vB.set_sensitive(true);
	          }
	          return false;
	        });
	      }
	    }
	    catch (const Glib::Error& ex) {
	    	  if (!cancellable->is_cancelled()) {
	    	    //std::cerr << "Net error across HTTPS: " << ex.what() << std::endl;
	    	    Glib::signal_idle().connect([this, cancellable]() {
	    	      if (!cancellable->is_cancelled()) {
	    	    	this->lBver.set_text(_(APP_VER "\nNo internet connection!") + std::string("\n         Gtkmm : v.") + GTKMM_VERSION);
	    	        this->vB.set_sensitive(true);
	    	      }
	    	      return false;
	    	    });
	    	  }
	    }
	  });

	  worker.detach();
}
