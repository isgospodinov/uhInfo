/*
 *    uhInfo
 *    Copyright (C) 2022
 */

#include "mwnd.h"

ClrDialog::ClrDialog(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv) : Gtk::Dialog("Color choice",*pMWnd),plMw(pMWnd)
{
   scrollWindow.set_child(treeView);
   get_content_area()->append(scrollWindow);
   treeView.set_model(pRefTreeModel);

   scrollWindow.set_margin(7);
   scrollWindow.set_expand();

   treeView.append_column(" Color ", cvColumns->color);

   InitVision();

   treeView.signal_row_activated().connect(sigc::mem_fun(*this, &ClrDialog::OnColorChoiceToggled));

   signal_close_request().connect(sigc::mem_fun(*this, &ClrDialog::Wnd_close_handler),false);

   uhiutil::set_css_style(get_style_context(),*cProv);

   treeView.set_activate_on_single_click();
   treeView.property_enable_grid_lines() = true;
}

bool ClrDialog::Wnd_close_handler()
{
	CLRMNG(m_temperatureTreeView).get_selection()->unselect_all();
	CLRMNG(m_Box_TmpControls).set_sensitive(true);
	CLRMNG(item_temperature)->set_enabled(true);

	hide();

	if(citl) (*citl)[TCOLUMNS(color)] = "";
	cName = nullptr;
	citl  = Gtk::TreeModel::iterator(nullptr);

	return true;
}

void ClrDialog::OnColorChoiceToggled(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn *column)
{
	if(citl) {
	    Gtk::TreeModel::iterator iter = pRefTreeModel->get_iter(path);
	    Glib::ustring lstr = (*iter)[cvColumns->color_name];
	    *cName = (std::string)lstr;
	    (*citl)[TCOLUMNS(color_name)] = *cName;
	}
}

void ClrDialog::InitVision()
{
	int indexlimit =  SIZEOF(clrID);
    for(int i = 0 ; i < indexlimit ; i++) {
    	Gtk::TreeModel::Row row = *(pRefTreeModel->append());
        row[cvColumns->color_name] = clrID[i];
  }

    Gtk::TreeViewColumn* ptColumn = treeView.get_column(0);
      if(ptColumn) {
          Gtk::CellRenderer *ptRend = (ptColumn->get_first_cell());
          if(ptRend) ptColumn->add_attribute(*ptRend,"cell-background", 1);
      }
}

void ClrDialog::on_show()
{
	Gtk::Dialog::on_show();
	CLRMNG(item_temperature)->set_enabled(false);
	treeView.get_selection()->unselect_all();

	CDrawArea::DRAWVECTORPLUS ldv{nullptr,nullptr};
	citl = CLRMNG(m_temperatureTreeView).get_selection()->get_selected();

	if(citl) {
        if((*citl)[TCOLUMNS(tsensor_node)] == sensors::nud2)
        	ldv = CLRMNG(pUd2Manager)->SensorStatisticToggle((*citl)[TCOLUMNS(col_tcheck)],(*citl)[TCOLUMNS(color_name)],
    			            (*citl)[TCOLUMNS(tsensor_node)],(*citl)[TCOLUMNS(tsensor_name)],(*citl)[TCOLUMNS(tnode_id)],(*citl)[TCOLUMNS(tsensor_id)],nullptr);
        else
        	ldv = CLRMNG(pSysensors)->SensorStatisticToggle((*citl)[TCOLUMNS(col_tcheck)],(*citl)[TCOLUMNS(color_name)],
    			    (*citl)[TCOLUMNS(tsensor_node)],(*citl)[TCOLUMNS(tsensor_name)],(*citl)[TCOLUMNS(tnode_id)],(*citl)[TCOLUMNS(tsensor_id)],nullptr);

        cName = const_cast<std::string*>(ldv.dsn);
        (*citl)[TCOLUMNS(color)] = " -ed- ";
	}
}
