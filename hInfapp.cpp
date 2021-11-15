/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "hInfapp.h"
#include <iostream> //!

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
   // - - - - Manage menu - - - -
   Glib::RefPtr<Gio::Menu> sm_menage = Gio::Menu::create();
   Glib::RefPtr<Gio::MenuItem> item = Gio::MenuItem::create("Extended info", "win.extinfo");
   sm_menage->append_item(item);
   item = Gio::MenuItem::create("CPU activity", "win.cpuactivity");
   sm_menage->append_item(item);
   sm_menage->append("Quit", "app.quit");
   add_action("quit",sigc::mem_fun(*this, &ChInfApp::on_application_quit));
   appmenu->append_submenu("Manage", sm_menage);
   // - - - - Sensors menu - - - -
   Glib::RefPtr<Gio::Menu> sm_sensors = Gio::Menu::create();
   item = Gio::MenuItem::create("Sensors settings", "win.sensorset");
   sm_sensors->append_item(item);
   item = Gio::MenuItem::create("t° monitor", "win.tmpmonit");
   sm_sensors->append_item(item);
   appmenu->append_submenu("Sensors", sm_sensors);
   // - - - - Application menu - - - -
   Glib::RefPtr<Gio::Menu> sm_apphab = Gio::Menu::create();
   item = Gio::MenuItem::create("Preferences", "win.prefs");
   sm_apphab->append_item(item);
   item = Gio::MenuItem::create("About..", "win.about");
   sm_apphab->append_item(item);
   appmenu->append_submenu("Application", sm_apphab);

   set_menubar(appmenu);
}

void ChInfApp::on_activate()
{

}

void ChInfApp::on_application_quit()
{
  quit();
  std::vector<Gtk::Window*> wnds = get_windows();
  if(wnds.size() > 0)
                     wnds[0]->hide();
}

