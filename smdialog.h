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

class CSmDialog : public Gtk::Dialog
{
   using fp_DlgResp = void (CHWindow::*)(int);
public:
   CSmDialog(Gtk::Window *const p_wnd,CSysens &pS, Ud2mon &pUd2, const Glib::RefPtr<Gtk::CssProvider> *const cp,fp_DlgResp fp);
   virtual ~CSmDialog() = default;
   
   int dh = 0, dw = 0;
   const std::string GetInused(bool all = false) const;
   const std::string GetAllInused() const;
   void SetDefSize() {if(pmWnd)set_default_size((pmWnd->get_width() / 3), (pmWnd->get_height() - (pmWnd->get_height() / 4)));}
private:
   const uhimc_unique_ptr vColumns = uhimc_unique_ptr(new CModelUhiColumns(ModelBaseUhiColumns::UhiModelType::SENSORS));
   const Glib::RefPtr<Gtk::ListStore> pRefTreeModel = Gtk::ListStore::create(*vColumns);

   Gtk::ScrolledWindow scrollWindow;
   Gtk::TreeView treeView;
   
   Gtk::Window *const pmWnd = nullptr;
   CSysens *const pSensors = nullptr;
   Ud2mon *const pUd2mon = nullptr;

   virtual void on_show() override;
   void InitVision();
   void OnToggled(const Glib::ustring &path_string);
};

#endif // _SMDIALOG_H_
