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
  virtual ~ChInfApp() = default;
  //Overrides of default signal handlers:
  virtual void on_startup() override;
  virtual void on_activate() override;
public:
  static Glib::RefPtr<ChInfApp> create(const std::string &name);
};

#endif // HINFAPP_H
