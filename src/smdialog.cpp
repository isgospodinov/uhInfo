/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "smdialog.h"
#include "sysens.h"
#include "ud2mon.h"
#include "mwnd.h"
using uhiutil::cpu::UhiDownCast;

CSmDialog::CSmDialog(Gtk::Window *const p_wnd,CSysens &pS, Ud2mon &pUd2, const Glib::RefPtr<Gtk::CssProvider> *const cp,fp_DlgResp fp) : UhiDlgWnd(p_wnd),
                     pSensors(&pS),pUd2mon(&pUd2)
{
	set_transient_for(*p_wnd);
	set_title("Settings");

    uhiutil::GetDesktopSize((unsigned int*) &dh,(unsigned int*) &dw);

    if(!dh || !dw) {
    	dw = uhiutil::def_desktop_width;
        dh = uhiutil::def_desktop_height;
   }

   scrollWindow.set_child(treeView);
   set_child(scrollWindow);
   treeView.set_model(pRefTreeModel);

   scrollWindow.set_margin(7);
   scrollWindow.set_expand();

   treeView.append_column_editable("+/-", vColumns->col_tcheck);
   treeView.append_column("Type", vColumns->description);
   treeView.append_column("Node", vColumns->tsensor_node);
   treeView.append_column("Sensor", vColumns->tsensor_name);


   Gtk::CellRendererToggle* pRenderer = nullptr;
   Gtk::TreeViewColumn* pColumn = treeView.get_column(0);

   if(pColumn)
       pRenderer = UhiDownCast<Gtk::CellRenderer,Gtk::CellRendererToggle>(pColumn->get_first_cell());

   if(pColumn && pRenderer)
       pRenderer->signal_toggled().connect(sigc::mem_fun(*this,&CSmDialog::OnToggled));

   signal_hide().connect(sigc::bind(sigc::mem_fun((CHWindow&)*p_wnd, fp),false));
   signal_close_request().connect([&]()->bool{hide();return true;},false);

   uhiutil::set_css_style(get_style_context(),*cp);
}

void CSmDialog::InitVision()
{
	pRefTreeModel->clear();
   if(pSensors) {
       bool stat = PTSMNG(pfDlg)->GetAllInputStat();
       for(Chip_node n : pSensors->monitoring)  {
           for(Sensor_node sn : n.sensors) {
                if(!stat && sn.sntype == SENSORS_FEATURE_IN && !sn.is_Vcore) continue;
                Gtk::TreeModel::Row row = *(pRefTreeModel->append());
                row[vColumns->col_tcheck] = sn.visible;
                row[vColumns->tsensor_node] = n.chip_name.cnip_prefix;
                row[vColumns->tsensor_name] = sn.label;
                row[vColumns->tnode_id] = n.chip_id;
                row[vColumns->tsensor_id] = sn.feature_number;
                row[vColumns->description] = (sn.sntype == SENSORS_FEATURE_IN ? "   V" : (sn.sntype == SENSORS_FEATURE_TEMP ? "  °t" : ( sn.sntype == SENSORS_FEATURE_POWER ? "  W" : ( sn.sntype ==  SENSORS_FEATURE_FAN ? "  ☼" : "   -"))));
           }
       }
   }

   if(pUd2mon) {
       for(Ud2_sens_node it : pUd2mon->monitoring)  {
           Gtk::TreeModel::Row row = *(pRefTreeModel->append());
           row[vColumns->col_tcheck] = it.visible;
           row[vColumns->tsensor_node] = sensors::nud2;
           row[vColumns->tsensor_name] = it.ud2_model_name;
           row[vColumns->tnode_id] = it.ud2_drv_id;
           row[vColumns->tsensor_id] = it.index;
           row[vColumns->description] = "  °t" ;
       }
   }
}

const std::string CSmDialog::GetAllInused() const
{
   bool found = false;
   std::string buff(""),allinused = GetInused(),config = "/home/" + uhiutil::GetUserName() + "/.uhInfo/uhInfo.cfg";
   if(uhiutil::ExistenceVerification(config.c_str())) {
          buff = "cat " + config + " | grep -v uhi";
          buff = uhiutil::execmd(buff.c_str());
          if(buff != "") {
              config = GetInused(true);
              std::istringstream alreadysaved(buff);
              for(std::string line; std::getline(alreadysaved, line);) {
                    std::istringstream activenow(config);
                    for(std::string ln; std::getline(activenow, ln);) {
                          if(line == ln) {
                                found = true;
                                break;
                          }
                    }
              if(!found) allinused.append(line + "\n");
              found = false;      
              }
          }
   }

   return allinused;
}

const std::string CSmDialog::GetInused(bool all) const
{
   std::string inused("");

   if(pSensors) {
        for(Chip_node n : pSensors->monitoring)  {
              for(Sensor_node sn : n.sensors) {
                    if(all || !sn.visible)
                         inused.append(sn.label + ":" + n.chip_id + ":" + std::to_string(sn.feature_number) + "\n");
              }
        }
   }

   if(pUd2mon) {
        for(Ud2_sens_node sn : pUd2mon->monitoring)  {
              if(all || !sn.visible)
                    inused.append(std::string(sn.ud2_model_name) + ":" + std::string(sn.ud2_drv_id) + ":" + std::to_string(sn.index) + "\n");
        }
   }

   return inused;
}

void CSmDialog::OnToggled(const Glib::ustring &path_string)
{
	   Gtk::TreePath path(path_string);
	   Gtk::TreeModel::iterator iter = pRefTreeModel->get_iter(path);

	   if((*iter)[vColumns->tsensor_node] == sensors::nud2) {
	       if(pUd2mon) {
	    	   bool l_stat = PTSMNG(pfDlg)->GetInTmpMonStat();
	           for(std::list<Ud2_sens_node>::iterator it = pUd2mon->monitoring.begin(); it != pUd2mon->monitoring.end(); it++)  {
	                if(((Glib::ustring((*iter)[vColumns->tsensor_name])) == it->ud2_model_name) && ((Glib::ustring((*iter)[vColumns->tnode_id])) == it->ud2_drv_id)) {
	                    it->visible = (*iter)[vColumns->col_tcheck];
	                    ((!it->visible) ? (++pUd2mon->inactive_dev_number,(l_stat ? --pUd2mon->visible_tmp_sens_count : 0)) : (--pUd2mon->inactive_dev_number,(l_stat ? ++pUd2mon->visible_tmp_sens_count : 0)));
	                    pUd2mon->dataPrint_forced = true;
	                    break;
	                }
	           }
	       }
	   }
	   else {
	       if(pSensors) {
	           bool break_it = false;
	           for(std::list<Chip_node>::iterator n = pSensors->monitoring.begin(); n != pSensors->monitoring.end(); n++)  {
	                if((Glib::ustring((*iter)[vColumns->tsensor_node])) == Glib::ustring(n->chip_name.cnip_prefix) && ((Glib::ustring((*iter)[vColumns->tnode_id])) == Glib::ustring(n->chip_id))) {
	                    for(std::list<Sensor_node>::iterator sn =  n->sensors.begin(); sn != n->sensors.end(); sn++) {
	                        if((Glib::ustring((*iter)[vColumns->tsensor_name])) == Glib::ustring(sn->label) && ((*iter)[vColumns->tsensor_id] == sn->feature_number)) {
	                            sn->visible = (*iter)[vColumns->col_tcheck];

	                            if(sn->is_Vcore && !sn->visible && *pSensors->sVcore_val) {
	                            	**pSensors->sVcore_val = 0.0;
	                            }

	                            ((!sn->visible) ? (++n->inactive_sensors_number,(SNTP(SENSORS_FEATURE_TEMP) ? --pSensors->visible_tmp_sens_count : 0)) : (--n->inactive_sensors_number,(sn->sntype == SENSORS_FEATURE_TEMP ? ++pSensors->visible_tmp_sens_count : 0)));
	                            break_it = true;
	                            break;
	                        }
	                    }
	                }
	                if(break_it) break;
	           }
	       }
	   }
}
