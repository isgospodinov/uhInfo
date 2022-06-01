/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef HINFAPP_H
#define HINFAPP_H

#include "util.h"

class ChInfApp : public Gtk::Application
{
protected:
  ChInfApp(const std::string &name);

  virtual void on_startup() override;

  void on_application_quit();
public:
  static Glib::RefPtr<ChInfApp> create(const std::string &name);
  virtual ~ChInfApp() = default;
};

#endif // HINFAPP_H
