/*
 *    uhInfo
 *    Copyright (C) 2022
 */

#ifndef _CLRDIALOG_H_
#define _CLRDIALOG_H_

#include "uhirc.h"

class ClrDialog : public Gtk::Dialog
{
public:
	ClrDialog(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv);
	virtual ~ClrDialog() = default;

	void SetDefSize() {if(plMw)set_default_size((plMw->get_width() / 3), (plMw->get_height() - (plMw->get_height() / 4)));}
	void OnColorChoiceToggled(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn *column);
	bool Wnd_close_handler();
	virtual void on_show() override;

private:
   const uhimc_unique_ptr cvColumns = uhimc_unique_ptr(new CModelUhiColumns(ModelBaseUhiColumns::UhiModelType::COLOR));
   const Glib::RefPtr<Gtk::ListStore> pRefTreeModel = Gtk::ListStore::create(*cvColumns);

   Gtk::ScrolledWindow scrollWindow;
   Gtk::TreeView treeView;
   const Gtk::Window *const plMw = nullptr;
   std::string* cName = nullptr;
   Gtk::TreeModel::iterator citl{nullptr};

   void InitVision();
};

#endif // _CLRDIALOG_H_
