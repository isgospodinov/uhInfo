/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "hInfapp.h"
#include "mwnd.h"

void ChInfApp::on_application_quit()
{
	const CHWindow *const apwnd = (CHWindow*) get_run_window();
	if(apwnd) apwnd->QuitTasks();
	quit();
}

ChInfApp::ChInfApp(const std::string &name) : Gtk::Application(name)
{
     Glib::set_application_name(Glib::ustring{name});
}

Glib::RefPtr<ChInfApp> ChInfApp::create(const std::string &name)
{
           return Glib::RefPtr<ChInfApp>  (new ChInfApp(name));
}

void ChInfApp::on_startup()
{
   Gtk::Application::on_startup();

   Glib::RefPtr<Gio::Menu> appmenu = Gio::Menu::create();
   Glib::RefPtr<Gio::Menu> sm(nullptr);
   // - - - - Manage menu - - - -
   appmenu->append_submenu("Manage", sm = Gio::Menu::create());
   sm->append_item(Gio::MenuItem::create("Extended info", "win.extinfo"));
   sm->append_item(Gio::MenuItem::create("CPU activity", "win.cpuactivity"));
   sm->append("Quit", "app.quit");
   add_action("quit",sigc::mem_fun(*this, &ChInfApp::on_application_quit));
   // - - - - Sensors menu - - - -
   appmenu->append_submenu("Sensors", sm = Gio::Menu::create());
   sm->append_item(Gio::MenuItem::create("Sensors settings", "win.sensorset"));
   sm->append_item(Gio::MenuItem::create("t° monitor", "win.tmpmonit"));
   // - - - - Application menu - - - -
   appmenu->append_submenu("Application", sm = Gio::Menu::create());
   sm->append_item(Gio::MenuItem::create("Preferences", "win.prefs"));
   sm->append_item(Gio::MenuItem::create("About..", "win.about"));

   set_menubar(appmenu);
}
