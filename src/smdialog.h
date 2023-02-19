/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _SMDIALOG_H_
#define _SMDIALOG_H_

#include "uhirc.h"

class CSysens;
class Ud2mon;
class CHWindow;

class CSmDialog : public Gtk::Window
{
   using fp_DlgResp = void (CHWindow::*)(bool);
public:
   CSmDialog(Gtk::Window *const p_wnd,CSysens &pS, Ud2mon &pUd2, const Glib::RefPtr<Gtk::CssProvider> *const cp,fp_DlgResp fp);
   virtual ~CSmDialog() = default;
   
   int dh = 0, dw = 0;
   const std::string GetInused(bool all = false) const;
   const std::string GetAllInused() const;
   void on_set_after_init_param(const int w,const int h,const double p) {set_default_size(w / 3, (h - (h / 4)));}
private:
   const uhimc_unique_ptr vColumns = uhimc_unique_ptr(new CModelUhiColumns(ModelBaseUhiColumns::UhiModelType::SENSORS));
   const Glib::RefPtr<Gtk::ListStore> pRefTreeModel = Gtk::ListStore::create(*vColumns);

   Gtk::ScrolledWindow scrollWindow;
   Gtk::TreeView treeView;
   
   const Gtk::Window *const plMw = nullptr;
   CSysens *const pSensors = nullptr;
   Ud2mon *const pUd2mon = nullptr;

   virtual void on_show() override {InitVision();Gtk::Window::on_show();}
   void InitVision();
   void OnToggled(const Glib::ustring &path_string);
};

#endif // _SMDIALOG_H_
