/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "hInfapp.h"

ChInfApp::ChInfApp(const std::string &name) : Gtk::Application(name)
{
     Glib::set_application_name(Glib::ustring{name});
}

Glib::RefPtr<ChInfApp> ChInfApp::create(const std::string &name)
{
           return Glib::RefPtr<ChInfApp>  (new ChInfApp(name)) ;
}

void ChInfApp::on_startup()
{
   Gtk::Application::on_startup();

   Glib::RefPtr<Gio::Menu> appmenu = Gio::Menu::create();
   Glib::RefPtr<Gio::Menu> subm = Gio::Menu::create();
   Glib::RefPtr<Gio::MenuItem> item{nullptr};
   // - - - - Manage menu - - - -
   item = Gio::MenuItem::create("Extended info", "win.extinfo");
   subm->append_item(item);
   item = Gio::MenuItem::create("CPU activity", "win.cpuactivity");
   subm->append_item(item);
   subm->append("Quit", "app.quit");
   add_action("quit",sigc::mem_fun(*this, &ChInfApp::on_application_quit));
   appmenu->append_submenu("Manage", subm);
   // - - - - Sensors menu - - - -
   subm = Gio::Menu::create();
   item = Gio::MenuItem::create("Sensors settings", "win.sensorset");
   subm->append_item(item);
   item = Gio::MenuItem::create("t° monitor", "win.tmpmonit");
   subm->append_item(item);
   appmenu->append_submenu("Sensors", subm);
   // - - - - Application menu - - - -
   subm = Gio::Menu::create();
   item = Gio::MenuItem::create("Preferences", "win.prefs");
   subm->append_item(item);
   item = Gio::MenuItem::create("About..", "win.about");
   subm->append_item(item);
   appmenu->append_submenu("Application", subm);

   set_menubar(appmenu);
}
