/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "hInfapp.h"

ChInfApp::ChInfApp(const std::string &name) : Gtk::Application(name)
{
     Glib::set_application_name(name);
}

Glib::RefPtr<ChInfApp> ChInfApp::create(const std::string &name)
{
           return Glib::RefPtr<ChInfApp>  (new ChInfApp(name)) ;
}

void ChInfApp::on_startup()
{
  Gtk::Application::on_startup();
}

void ChInfApp::on_activate()
{
}

