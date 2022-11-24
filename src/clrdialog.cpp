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
	return true;
}

void ClrDialog::OnColorChoiceToggled(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn *column)
{
	Gtk::TreeModel::iterator itl = CLRMNG(m_temperatureTreeView).get_selection()->get_selected();
	if(itl) {
	    Gtk::TreeModel::iterator iter = pRefTreeModel->get_iter(path);
	    Glib::ustring lstr = (*iter)[cvColumns->color_name];
	    (*itl)[TCOLUMNS(color_name)] = lstr;

        if((*itl)[TCOLUMNS(tsensor_node)] == sensors::nud2)
    	    CLRMNG(pUd2Manager)->SensorStatisticToggle((*itl)[TCOLUMNS(col_tcheck)],(*itl)[TCOLUMNS(color_name)],
    			            (*itl)[TCOLUMNS(tsensor_node)],(*itl)[TCOLUMNS(tsensor_name)],(*itl)[TCOLUMNS(tnode_id)],(*itl)[TCOLUMNS(tsensor_id)],nullptr);
        else
    	    CLRMNG(pSysensors)->SensorStatisticToggle((*itl)[TCOLUMNS(col_tcheck)],(*itl)[TCOLUMNS(color_name)],
    			    (*itl)[TCOLUMNS(tsensor_node)],(*itl)[TCOLUMNS(tsensor_name)],(*itl)[TCOLUMNS(tnode_id)],(*itl)[TCOLUMNS(tsensor_id)],nullptr);

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
}
