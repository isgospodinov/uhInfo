/*
 *    uhInfo
 *    Copyright (C) 2022
 */

#include "../mwnd.h"
#include <glibmm/i18n.h>

ClrDialog::ClrDialog(Gtk::Window *const pMWnd,const Glib::RefPtr<Gtk::CssProvider> *const cProv) : UhiDlgWnd(pMWnd)
{
   set_transient_for(*pMWnd);
   set_title(_("Color choice"));

   set_child(allBox);

   allBox.set_orientation(Gtk::Orientation::VERTICAL);

   allBox.append(l_chSeName);
   allBox.append(scrollWindow);

   scrollWindow.set_child(treeView);

   treeView.set_model(pRefTreeModel);

   scrollWindow.set_margin(7);
   scrollWindow.set_expand();

   treeView.append_column(_(" Color "), cvColumns->color);
   treeView.set_headers_visible(false);

   InitVision();

   treeView.signal_row_activated().connect(sigc::mem_fun(*this, &ClrDialog::OnColorChoiceToggled));

   signal_close_request().connect(sigc::mem_fun(*this, &ClrDialog::Wnd_close_handler),false);

   uhiutil::set_css_style(get_style_context(),*cProv);

   treeView.set_activate_on_single_click();
   treeView.property_enable_grid_lines() = true;

   SETLOCALDECORATION;
   uhiutil::set_css_style(l_chSeName.get_style_context(),lprv,"ctext_cls");
}

bool ClrDialog::Wnd_close_handler()
{
	PTSMNG(m_temperatureTreeView).get_selection()->unselect_all();
	PTSMNG(m_Box_TmpControls).set_sensitive(true);
	PTSMNG(item_temperature)->set_enabled(true);

	hide();

	if(PTSMNG(m_DAtemperature).m_TmpWndCurrState == CDrArTempr::DAWndState::FULL)
		PTSMNG(mDA_ToolBar).set_visible((PTSMNG(pfDlg) ? (PTSMNG(pfDlg)->GetShowCPUfq() && PTSMNG(m_DAtemperature).HasActivities()) : false) && PTSMNG(mDA_ToolBar).is_visible());

	if(citl) (*citl)[TCOLUMNS(color)] = "";
	cName = nullptr;
	citl  = Gtk::TreeModel::iterator(nullptr);
	sensID = "";

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
	UhiDlgWnd::on_show();
	PTSMNG(item_temperature)->set_enabled(false);
	treeView.get_selection()->unselect_all();

	if(PTSMNG(m_DAtemperature).m_TmpWndCurrState == CDrArTempr::DAWndState::NORMAL) {
		citl = PTSMNG(m_temperatureTreeView).get_selection()->get_selected();
	    l_chSeName.set_text((*citl)[TCOLUMNS(tsensor_node)] + " : " + (*citl)[TCOLUMNS(tsensor_name)]);
		if(citl) {
	        (*citl)[TCOLUMNS(color)] = "( ● )";
		}
	}
	else {
		Gtk::TreeModel::Children chl = PTSMNG(ptRefTreeModel)->children();
		for(Gtk::TreeIter<Gtk::TreeRow> rowit = chl.begin();rowit != chl.end(); rowit++)
		{
			std::string sid = (*rowit)[TCOLUMNS(tsensor_node)] + ":" + (*rowit)[TCOLUMNS(tsensor_name)] + (*rowit)[TCOLUMNS(tnode_id)] + std::to_string((*rowit)[TCOLUMNS(tsensor_id)]);
			if((*rowit)[TCOLUMNS(col_tcheck)] && sid == sensID) {
				citl = rowit;
				l_chSeName.set_text((*citl)[TCOLUMNS(tsensor_node)] + " : " + (*citl)[TCOLUMNS(tsensor_name)]);
			}
		}
	}

	SetChangeNameClr(citl);
	l_chSeName.set_text((*citl)[TCOLUMNS(tsensor_node)] + " : " + (*citl)[TCOLUMNS(tsensor_name)]);
}

void ClrDialog::SetChangeNameClr(Gtk::TreeModel::iterator& it)
{
	CDrArTempr::DRAWVECTORPLUS ldv{nullptr,nullptr};

	if((*it)[TCOLUMNS(tsensor_node)] == sensors::nud2)
			        	ldv = PTSMNG(pUd2Manager)->SensorStatisticToggle((*it)[TCOLUMNS(col_tcheck)],(*it)[TCOLUMNS(color_name)],
			    			            (*it)[TCOLUMNS(tsensor_node)],(*it)[TCOLUMNS(tsensor_name)],(*it)[TCOLUMNS(tnode_id)],(*it)[TCOLUMNS(tsensor_id)]);
			        else
			        	ldv = PTSMNG(pSysensors)->SensorStatisticToggle((*it)[TCOLUMNS(col_tcheck)],(*it)[TCOLUMNS(color_name)],
			    			    (*it)[TCOLUMNS(tsensor_node)],(*it)[TCOLUMNS(tsensor_name)],(*it)[TCOLUMNS(tnode_id)],(*it)[TCOLUMNS(tsensor_id)]);

	cName = const_cast<std::string*>(ldv.dsn);
}