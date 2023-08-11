/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "hInfapp.h"
#include "mwnd.h"
#include <glibmm/i18n.h>

void ChInfApp::on_application_quit()
{
	CHWindow *const apwnd = (CHWindow*) get_run_window();
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
   appmenu->append_submenu(_("Manage"), sm = Gio::Menu::create());
   sm->append_item(Gio::MenuItem::create(_("Extended info"), "win.extinfo"));
   sm->append_item(Gio::MenuItem::create(_("CPU activity"), "win.cpuactivity"));
   sm->append(_("Quit"), "app.quit");
   add_action("quit",sigc::mem_fun(*this, &ChInfApp::on_application_quit));
   // - - - - Sensors menu - - - -
   appmenu->append_submenu(_("Sensors"), sm = Gio::Menu::create());
   sm->append_item(Gio::MenuItem::create(_("Sensors settings"), "win.sensorset"));
   sm->append_item(Gio::MenuItem::create(std::string("t°") + _(" monitor"), "win.tmpmonit"));
   // - - - - Application menu - - - -
   appmenu->append_submenu(_("Application"), sm = Gio::Menu::create());
   sm->append_item(Gio::MenuItem::create(_("Preferences"), "win.prefs"));
   sm->append_item(Gio::MenuItem::create(_("About..."), "win.about"));

   set_menubar(appmenu);
}
