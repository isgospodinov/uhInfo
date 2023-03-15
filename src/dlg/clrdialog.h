/*
 *    uhInfo
 *    Copyright (C) 2022
 */

#ifndef _CLRDIALOG_H_
#define _CLRDIALOG_H_

#include "../uhirc.h"

class ClrDialog : public UhiDlgWnd
{
public:
	ClrDialog(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv);
	virtual ~ClrDialog() = default;

	void OnColorChoiceToggled(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn *column);
	bool Wnd_close_handler();
	virtual void on_show() override;
	virtual void on_set_after_init_param(const int w,const int h) override {set_default_size(w / 3, (h - (h / 4)));}

private:
   const uhimc_unique_ptr cvColumns = uhimc_unique_ptr(new CModelUhiColumns(ModelBaseUhiColumns::UhiModelType::COLOR));
   const Glib::RefPtr<Gtk::ListStore> pRefTreeModel = Gtk::ListStore::create(*cvColumns);

   Gtk::ScrolledWindow scrollWindow;
   Gtk::TreeView treeView;
   std::string* cName = nullptr;
   Gtk::TreeModel::iterator citl{nullptr};

   virtual void InitVision() override;
};

#endif // _CLRDIALOG_H_
